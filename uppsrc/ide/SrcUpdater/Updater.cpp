#include "SrcUpdater.h"

#define LLOG(x)  //RLOG(x)
#define LDUMP(x) //RDUMP(x)

//#define IMAGECLASS IdeImg
//#define IMAGEFILE <ide/ide.iml>
//#include <Draw/iml_header.h>

void LoadUpdaterCfg(){
	String cfg=ConfigFile("updates.xml");
	if(FileExists(cfg)){
		LoadFromXMLFile(UpdaterCfg(),cfg);
	}else{
		UpdaterCfg().method=0;
		UpdaterCfg().sync=0;
		UpdaterCfg().period=0;
		UpdaterCfg().last=Null;
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
	LLOG("GetSrcVersion("<<dir<<") = "<<ver);
	return ver;
}

String GetSvnVersion(const String& server,bool verbose,String& error){
	String ver;
	LocalProcess p("svn info "+server);
	if(verbose){
		Progress pi;
		pi.SetText("Checking revision at "+server);
		while(p.IsRunning()){
			if(pi.Canceled()){
				p.Detach(); // p.Kill() would be better, but it hangs...
				error="CANCEL";
				return String::GetVoid();
			}
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
		LDUMP(error);
		return String::GetVoid();
	}
	ver=p.Get();
	ver=ver.Mid(ver.Find("Revision: ")+10);
	LLOG("GetSvnVersion("<<server<<") = "<<IntStr(ScanInt(ver)));
	return IntStr(ScanInt(ver));
}

String FindGlobalSrc(){
#ifdef PLATFORM_POSIX
	static const char* dirs[]={"/usr/share/upp","/usr/local/share/upp","/usr/local/lib/upp","/usr/lib/upp"};
	int n=4;
#else
	static const char* dirs[]={"C:\\upp"}; // Program Files?
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
	//Icon(IdeImg::Package(), IdeImg::PackageLarge());
}

bool SourceUpdater::NeedsUpdate(bool verbose){
	String where;
	LLOG("checking for updates");
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
			break;
		}
		default: return false;
	}
	LLOG("NeedsUpdate: local="<<local<<", global="<<global);


	if(ScanInt(global)<=UpdaterCfg().ignored || global.IsVoid() || local.IsVoid())
		return false;
	text<<="[ [ [/ Newer version of U`+`+ sources is available.]&][ &]"
	       "[ {{5000:5000FNGN@N; [ [1 Curent local version:]]:: [ [1 "+local+"]]:: [ [1 Will update to:]]:: [ [1 "+global+"]]}}][ &&]"
	       "[1# If you choose to update now, your local sources (directory "+DeQtf(UpdaterCfg().localsrc)+") will be compared to the files "+where+". If you modified the files in your local copy, you will be able to choose appropriate actions for each changed file before writing anything on your hard drive.]";
	return UpdaterCfg().available=(ScanInt(local)<ScanInt(global));
}

void SourceUpdater::CheckUpdates(){
	if(UpdaterCfg().method==2) {
		if(UpdaterCfg().sync==1){
			global=IDE_VERSION;
			CheckLocalSvn();
		} else if(UpdaterCfg().sync==0){
			CheckGlobalSvn();
		}
		return;
	}
	//else it is fast enough to run without the callback chain
	UpdaterCfg().last=GetUtcTime();
	StoreAsXMLFile(UpdaterCfg(),"SourceUpdater",ConfigFile("updates.xml"));
	LLOG("running needsupdate");
	if(NeedsUpdate(false))
		WhenUpdateAvailable();
}

void SourceUpdater::CheckLocalSvn(){
	if(pl.IsRunning()) return;
	LLOG("checking local svn");
	pl.Start("svn info "+UpdaterCfg().localsrc);
	SetTimeCallback(-50,THISBACK(CheckLocalSvnFinished),1);
}

void SourceUpdater::CheckLocalSvnFinished(){
	if(pl.IsRunning()) return;
	LLOG("checking local svn finished");
	KillTimeCallback(1);
	int exitcode=pl.GetExitCode();
	if(exitcode){
		error="SVN command 'svn info "+UpdaterCfg().localsrc+"' failed with exit code "+AsString(exitcode)+"&Output:&[l150 "+DeQtf(pl.Get());
		LDUMP(error);
		return;
	}
	local=pl.Get();
	local=local.Mid(local.Find("Revision: ")+10);
	local=IntStr(ScanInt(local));
	text<<="[ [ [/ Newer version of U`+`+ sources is available.]&][ &]"
	       "[ {{5000:5000FNGN@N; [ [1 Curent local version:]]:: [ [1 "+local+"]]:: [ [1 Will update to:]]:: [ [1 "+global+"]]}}][ &&]"
	       "[1# If you choose to update now, your local sources (directory "+DeQtf(UpdaterCfg().localsrc)+") will be compared to the files in SVN repository ("+DeQtf(UpdaterCfg().svnserver)+"). If you modified the files in your local copy, you will be able to choose appropriate actions for each changed file before writing anything on your hard drive.]";
	UpdaterCfg().available=(ScanInt(local)<ScanInt(global));
	LLOG("Event<>  chain finished: local="<<local<<", global="<<global);
	UpdaterCfg().last=GetUtcTime();
	StoreAsXMLFile(UpdaterCfg(),"SourceUpdater",ConfigFile("updates.xml"));
	if(UpdaterCfg().available)
		WhenUpdateAvailable();
}

void SourceUpdater::CheckGlobalSvn(){
	if(pg.IsRunning()) return;
	LLOG("checking global svn");
	pg.Start("svn info "+UpdaterCfg().svnserver);
	SetTimeCallback(-500,THISBACK(CheckGlobalSvnFinished),2);
}

void SourceUpdater::CheckGlobalSvnFinished(){
	if(pg.IsRunning()) return;
	LLOG("checking global svn finished");
	KillTimeCallback(2);
	int exitcode=pg.GetExitCode();
	if(exitcode){
		error="SVN command 'svn info "+UpdaterCfg().svnserver+"' failed with exit code "+AsString(exitcode)+"&Output:&[l150 "+DeQtf(pg.Get());
		LLOG(error);
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
	LLOG("DoUpdate");
	switch(UpdaterCfg().method){
		case 0: {
			//copy in /home
			LocalSync ls;
			if(ls.Execute()==IDOK){
				ls.Perform();
				//DeleteFile(ConfigFile("md5sums"));
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
