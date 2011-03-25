#include "SrcUpdater.h"

#define IMAGECLASS IdeImg
#define IMAGEFILE <ide/ide.iml>
#include <Draw/iml_header.h>

//bool IsUbuntu(){
//	return Sys("lsb_release -si").StartsWith("Ubuntu");
//}

//String GetDebVersion(){
//	return Sys("dpkg-query --showformat ${Version} --show theide");
//}

void LoadUpdaterCfg(){
	String cfg=ConfigFile("updates.xml");
	if(FileExists(cfg)){
		LoadFromXMLFile(UpdaterCfg(),cfg);
	}else{
		UpdaterCfg().method=0;
		UpdaterCfg().sync=0;
		UpdaterCfg().period=0;
		UpdaterCfg().ignored=0;
		UpdaterCfg().localsrc=GetHomeDirFile("upp");
		UpdaterCfg().svnserver="http://upp-mirror.googlecode.com/svn/trunk/";
		UpdaterCfg().svnuser="";
		UpdaterCfg().svnpass="";
	}
	UpdaterCfg().globalsrc=FindGlobalSrc();
	UpdaterCfg().available=false;
}

String GetSrcVersion(const char* dir,String& error){
	String fn=AppendFileName(dir,"uppsrc/ide/version.h");
	String ver=LoadFile(fn);
	if(ver.IsVoid()){
		error="Can't read file "+fn;
	}else{
		ver=ver.Mid(ver.Find("IDE_VERSION")+12);
		ver=ver.Mid(ver.Find('"')+1);
		ver=ver.Left(ver.Find('"'));
	}
	return ver;
}

String GetSvnVersion(const String& server,bool verbose,String& error){
	String ver;
	LocalProcess p("svn info "+server);
	if(verbose){
		Progress pi;
		pi.SetText("Checking revision at "+server);
		while(p.IsRunning()){
			pi.Step();
			Sleep(50);
		}
		pi.Close();
	}else{
		while(p.IsRunning()){Ctrl::GuiSleep(1000);}
	}
	int exitcode=p.GetExitCode();
	if(exitcode){
		error="SVN command 'svn info "+server+"' failed with exit code "+AsString(exitcode)+"&Output:&[l150 "+DeQtf(p.Get());
		return "-1";
	}
	ver=p.Get();
	ver=ver.Mid(ver.Find("Revision: ")+10);
	return IntStr(ScanInt(ver));
}

String FindGlobalSrc(){
#ifdef PLATFORM_POSIX
	static const char* dirs[]={"/usr/share/upp","/usr/local/share/upp","/usr/local/lib/upp","/usr/lib/upp"};
	int n=4;
#else
	static const char* dirs[]={}; // Program Files?
	int n=0;
#endif
	for(int i=0;i<n;i++){
		if(DirectoryExists(dirs[i])) return dirs[i];
	}
	return "";
}

SourceUpdater::SourceUpdater(){
	CtrlLayout(*this,"Updates available");
	ignore<<THISBACK(Ignore);
	skip<<=THISBACK(Close);
	update<<=THISBACK(DoUpdate);
	update.SetFocus();
	Icon(IdeImg::Package(), IdeImg::PackageLarge());
}

bool SourceUpdater::NeedsUpdate(bool verbose){
	String where;
	switch(UpdaterCfg().method){
		case 0:{
			//copy in /home
			local=GetSrcVersion(UpdaterCfg().localsrc,error);
			global=GetSrcVersion(UpdaterCfg().globalsrc,error);
			where="in "+UpdaterCfg().globalsrc+" (updated by your package manager)";
			break;
		}
		case 2:{
			//svn repo
			switch(UpdaterCfg().sync){
				case 0:{
					//always newest
					global=GetSvnVersion(UpdaterCfg().svnserver,verbose,error);
					if(global.IsVoid()) return false;
					break;
				}
				case 1:{
					//sync with the version of theide
					global=IDE_VERSION;
					break;
				}
				default: return false;
			}
			where="in SVN repository ("+DeQtf(UpdaterCfg().svnserver)+")";
			local=GetSvnVersion(UpdaterCfg().localsrc,false,error);
			if(local.IsVoid()) return false;
			break;
		}
		default: return false;
	}
	if(ScanInt(global)<=UpdaterCfg().ignored) return false;
	text<<="[ [ [/ Newer version of U`+`+ sources is available.]&][ &]"
	       "[ {{5000:5000FNGN@N; [ [1 Curent local version:]]:: [ [1 "+local+"]]:: [ [1 Will update to:]]:: [ [1 "+global+"]]}}][ &&]"
	       "[1# If you choose to update now, your local sources (directory "+DeQtf(UpdaterCfg().localsrc)+") will be compared to the files "+where+". If you modified the files in your local copy, you will be able to choose appropriate actions for each changed file before writing anything on your hard drive.]";
	return UpdaterCfg().available=(ScanInt(local)<ScanInt(global));
}

void SourceUpdater::CheckUpdates(){
	if(UpdaterCfg().method==2) {
		RDUMP(UpdaterCfg().sync);
		if(UpdaterCfg().sync==1){
			global=IDE_VERSION;
			CheckLocalSvn();
		} else if(UpdaterCfg().sync==0){
			CheckGlobalSvn();
		}
		return;
	}
	//else it is fast enough to run without the callback chain
	RLOG("running needsupdate");
	if(NeedsUpdate(false))
		WhenUpdateAvailable();
}

void SourceUpdater::CheckLocalSvn(){
	if(pl.IsRunning()) return;
	RLOG("check local svn");
	pl.Start("svn info "+UpdaterCfg().localsrc);
	SetTimeCallback(-50,THISBACK(CheckLocalSvnFinished),1);
}

void SourceUpdater::CheckLocalSvnFinished(){
	if(pl.IsRunning()) return;
	RLOG("check local svn finished");
	KillTimeCallback(1);
	int exitcode=pl.GetExitCode();
	if(exitcode){
		error="SVN command 'svn info "+UpdaterCfg().localsrc+"' failed with exit code "+AsString(exitcode)+"&Output:&[l150 "+DeQtf(pl.Get());
		RLOG(error);
		return;
	}
	local=pl.Get();
	local=local.Mid(local.Find("Revision: ")+10);
	local=IntStr(ScanInt(local));
	text<<="[ [ [/ Newer version of U`+`+ sources is available.]&][ &]"
	       "[ {{5000:5000FNGN@N; [ [1 Curent local version:]]:: [ [1 "+local+"]]:: [ [1 Will update to:]]:: [ [1 "+global+"]]}}][ &&]"
	       "[1# If you choose to update now, your local sources (directory "+DeQtf(UpdaterCfg().localsrc)+") will be compared to the files in SVN repository ("+DeQtf(UpdaterCfg().svnserver)+"). If you modified the files in your local copy, you will be able to choose appropriate actions for each changed file before writing anything on your hard drive.]";
	UpdaterCfg().available=(ScanInt(local)<ScanInt(global));
	if(UpdaterCfg().available)
		WhenUpdateAvailable();
}

void SourceUpdater::CheckGlobalSvn(){
	if(pg.IsRunning()) return;
	RLOG("check global svn");
	pg.Start("svn info "+UpdaterCfg().svnserver);
	SetTimeCallback(-500,THISBACK(CheckGlobalSvnFinished),2);
}

void SourceUpdater::CheckGlobalSvnFinished(){
	if(pg.IsRunning()) return;
	RLOG("check global svn finished");
	KillTimeCallback(2);
	int exitcode=pg.GetExitCode();
	if(exitcode){
		error="SVN command 'svn info "+UpdaterCfg().svnserver+"' failed with exit code "+AsString(exitcode)+"&Output:&[l150 "+DeQtf(pg.Get());
		RLOG(error);
		return;
	}
	global=pg.Get();
	global=global.Mid(global.Find("Revision: ")+10);
	global=IntStr(ScanInt(global));
	if(ScanInt(global)>UpdaterCfg().ignored)
		CheckLocalSvn();
}

void SourceUpdater::DoUpdate(){
	Close();Hide();
	switch(UpdaterCfg().method){
		case 0: {
			//copy in /home
			VectorMap<String,String> md5local,md5global;
			Progress p;
			p.Title("Preparing update ...");
			p.AlignText(ALIGN_LEFT);
			GetFiles(UpdaterCfg().localsrc,AppendFileName(UpdaterCfg().localsrc,"").GetLength(),md5local,p);
			GetMd5Sums(UpdaterCfg().localsrc,md5local,p);
			LoadMd5Sums(ConfigFile("md5sums"),md5global,p);
			VectorMap<String,int> changed=CompareFiles(md5local,md5global,p);
			p.Close();
			int i=changed.Find("uppsrc/ide/version.h");
			if(i>=0) changed.Remove(i);
			LocalSync ls(changed,UpdaterCfg().localsrc);
			if(ls.Execute()==IDOK){
				ls.Perform();
				DeleteFile(ConfigFile("md5sums"));
				FileCopy(AppendFileName(UpdaterCfg().globalsrc,"uppsrc/ide/version.h"),AppendFileName(UpdaterCfg().localsrc,"uppsrc/ide/version.h"));
			}
			break;
		}
		case 2: {
			//svn repository
			SaveFile(AppendFileName(UpdaterCfg().localsrc,"uppsrc/ide/version.h"),"#define IDE_VERSION    \"\"\n");
			SvnSync ss;
			ss.Dir(UpdaterCfg().localsrc,UpdaterCfg().svnreadonly);
			if(UpdaterCfg().sync==1){
				ss.FixRevision(UpdaterCfg().localsrc,ScanInt(global));
			}
			ss.DoSync();
			SaveFile(AppendFileName(UpdaterCfg().localsrc,"uppsrc/ide/version.h"),"#define IDE_VERSION    \""+global+"\"\n");
			break;
			}
		default: return;
	}
	UpdaterCfg().available=(ScanInt(global)>ScanInt(GetSrcVersion(UpdaterCfg().localsrc,error)));
}

void SourceUpdater::Ignore(){
	UpdaterCfg().ignored=ScanInt(global);
	UpdaterCfg().available=false;
	Close();
}

void SourceUpdater::GetFiles(const String& dir,int prefix,VectorMap<String,String>& result,Progress& p){
	p.SetText("Gathering files ("+dir+")...");
	FindFile ff(AppendFileName(dir,"*"));
	while(ff) {
		String path=AppendFileName(dir,ff.GetName());
		if(ff.IsFile()) {
			result.Add(path.Mid(prefix))="";
		} else if(ff.IsFolder()){
			GetFiles(path,prefix,result,p);
		}
		p.Step();
		ff.Next();
	}
	result.RemoveKey("GCC.bm");
}

void SourceUpdater::GetMd5Sums(const String& dir,VectorMap<String,String>& list,Progress& p){
	p.Reset();
	p.SetText("Calculating md5 sums ("+dir+")...");
	p.SetTotal(list.GetCount());
	for(int i = 0; i < list.GetCount(); i++){
		list[i]=MD5String(LoadFile(AppendFileName(dir,list.GetKey(i))));
		p.Step();
	}
}

void SourceUpdater::LoadMd5Sums(const String& fn,VectorMap<String,String>& list,Progress& p){
	p.Reset();
	if(!FileExists(fn)){
		GetFiles(UpdaterCfg().globalsrc,strlen(UpdaterCfg().globalsrc)+1,list,p);
		GetMd5Sums(UpdaterCfg().globalsrc,list,p);
		String out;
		for(int i = 0; i < list.GetCount(); i++)
			out+=list[i]+" "+AppendFileName(UpdaterCfg().globalsrc,list.GetKey(i))+"\n";
		SaveFile(fn,out);
		return;
	}
	p.SetText("Loading md5sums file ...");
	FileIn f(fn);
	while(!f.IsEof()){
		String ln=f.GetLine();
		list.Add(ln.Mid(34+strlen(UpdaterCfg().globalsrc)),ln.Left(32));
		p.Step();
	}
}

VectorMap<String,int> SourceUpdater::CompareFiles(VectorMap<String,String>& local,VectorMap<String,String>& global,Progress& p){
	p.Reset();
	p.SetText("Comparing ...");
	p.SetTotal(global.GetCount()+local.GetCount());
	VectorMap<String,int> result;
	for(int i = 0; i < global.GetCount(); i++){
		String k=global.GetKey(i);
		int pos=local.Find(k);
		if(pos<0){
			result.Add(k,LocalSync::FS_DELETED);
		}else{
			if(local[pos]!=global[i])
				result.Add(k,LocalSync::FS_CHANGED);
			local.Unlink(pos);
		}
		p.Step();
	}
	for(int i = 0; i < local.GetCount(); i++){
		if(!local.IsUnlinked(i))
			result.Add(local.GetKey(i),LocalSync::FS_ADDED);
		p.Step();
	}
	return result;
}
