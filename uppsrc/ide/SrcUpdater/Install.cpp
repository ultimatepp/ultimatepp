#include "SrcUpdater.h"

#define IMAGECLASS IdeImg
#define IMAGEFILE <ide/ide.iml>
#include <Draw/iml_header.h>

bool CopyFolder(const char *dst, const char *src, Progress *pi);
bool LoadVarFile(const char *name, VectorMap<String, String>& _var);

bool HasSvn(){
	String tmp;
	return Sys("svn",tmp)>=0;
}

InstallWizard::InstallWizard(){
	Title("TheIDE - Initial setting wizard");
	Sizeable();
	Icon(IdeImg::Package(), IdeImg::PackageLarge());

	s0.text<<="[ [ [/ Welcome to TheIDE !]&][ &][ [1 This short wizard dialogue will help you to set up everything you need to get a full working Integrated Development Environment. Clicking ][/1 Finish][1  at any time will save the values to your hard disk (using default values for unfilled fields), while ][/1 Cancel][1  leaves this wizard without doing anything.]]";
	s1.text<<="[ [ [/ Source code handling methods]&][ &][ [1 There is several ways how to access the U`+`+ source codes. Choose the method that best suits your needs. You can always change the settings later if necessary.]]";
	s1.src<<=THISBACK(SrcChange);
	bool usrshare=!UpdaterCfg().globalsrc.IsEmpty();
	if(usrshare){
		s1.src.EnableCase(0);
		s1.src.EnableCase(1);
		s1.src.SetLabel(0,"Local copy of sources from "+UpdaterCfg().globalsrc+" [recomended]");
		s1.src.SetLabel(1,"Read only repositories in "+UpdaterCfg().globalsrc);
	}
	bool hassvn=HasSvn();
	s1.src.EnableCase(2,hassvn);
	if((UpdaterCfg().method<2&&usrshare)
	 ||(UpdaterCfg().method==2&&hassvn)
	 ||(UpdaterCfg().method==3)){
		s1.src<<=UpdaterCfg().method;
	}else{
		s1.src<<=(usrshare?0:(hassvn?2:3));
	}
	s1.unit.Add(1,    "minute(s)")
	       .Add(60,   "hour(s)")
	       .Add(1440, "day(s)")
	       .Add(10080,"week(s)");
	s1.unit.SetIndex(1);
	int p=UpdaterCfg().period;
	if(!IsNull(p)) {
		if(p<=0)
			s1.startup = true;
		if(p!=0){
			if(p%10080==0)
				s1.unit.SetIndex(3);
			else if(p%1440==0)
				s1.unit.SetIndex(2);
			else if(p%60==0)
				s1.unit.SetIndex(1);
			else
				s1.unit.SetIndex(0);
			s1.period<<=abs(p)/int(~s1.unit);
		}
	}
	SrcChange();

	s2.text<<="[ [ [/ SVN options]&][ &][ [1 Since you have chosen to use SVN working copy for assemblies, you can now specify which server to use and what sychronization scheme you want to use. Default server is the read only mirror on Google Code, but you can also use any other server (e.g. if you are a developer with commit rights) using the custom setup.]]";
	if(UpdaterCfg().svnserver==""||UpdaterCfg().svnserver=="http://upp-mirror.googlecode.com/svn/trunk/"){
		s2.server<<=0;
	}else{
		s2.server<<=1;
		svndlg.readonly<<=UpdaterCfg().svnreadonly;
	}
	svndlg.url<<=UpdaterCfg().svnserver;
	svndlg.usr<<=UpdaterCfg().svnuser;
	svndlg.pwd<<=UpdaterCfg().svnpass;
	svndlg.pwd.Password(true);
	CtrlLayoutOK(svndlg,"Custom SVN repository");
	s2.server<<=THISBACK(RepoChange);
	s2.svnsetup<<=THISBACK(RepoEdit);
	s2.syncmethod<<=UpdaterCfg().sync;
	RepoChange();
	s3.text<<="[ [ [/ Set up assemblies]&][ &][ [1 You can choose which assemblies should be set up for you. Assembly is a set of directories (called nests) containing packages. You can find more about assemblies, nests and package managment in U`+`+ in the ][^http`:`/`/ultimatepp`.org`/app`$ide`$PackagesAssembliesAndNests`$en`-us`.html^1 manual][1 . To edit assemblies double-click or right`-click on them.]]";
	s3.srcpath<<=UpdaterCfg().localsrc;
	s3.outpath<<=GetHomeDirFile(".upp/_out");
	s3.asmbls.AddColumn("Name");
	s3.asmbls.AddColumn("Paths");
	s3.asmbls.AddColumn("Output");
	s3.asmbls.AddColumn("Description");
	s3.asmbls.ColumnWidths("10 25 10 25");
	s3.asmbls.WhenBar = THISBACK(AsmMenu);
	s3.asmbls.WhenLeftDouble = THISBACK(OnAsmEdit);
	s3.restore<<=THISBACK(RestoreAsm);
	RestoreAsm();
	s4.text<<="[ [/ Thank you ...]&&][ [1 That is all, thank you for your patience. Now just click ][/1 Finish][1  to save all the changes to your hard drive. TheIDE will now proceed to start up normaly.]&][> [/ &Happy coding with TheIDE!    ]";
	WhenFinish=THISBACK(Perform);
}

void InstallWizard::RestoreAsm(){
	if(s1.src==1)
		s3.srcpath<<=UpdaterCfg().globalsrc;
	else if(UpdaterCfg().localsrc.StartsWith("/usr"))
		s3.srcpath<<=GetHomeDirFile("upp");
	else
		s3.srcpath<<=UpdaterCfg().localsrc;
	s3.outpath<<=GetHomeDirFile(".upp/_out");
	s3.asmbls.Clear();

	// we should explain new user what is inside the assemblies
	static const char* desc[]={"",
	                           "The \"heart\" of U++",
	                           "Example applications",
	                           "Simple reference examples",
	                           "Sources of tutorial examples",
	                           "Community contributed packages",
	                           "U++ infrastructure (web, packaging)",
	                           "Place for users own packages"};
	VectorMap<String,String> map;
	map.Add(~s3.srcpath,"$(SRC)");
	map.Add(~s3.outpath,"$(OUT)");
	for(FindFile ff(ConfigFile("*.var")); ff; ff.Next())
		if(ff.IsFile()){
			VectorMap<String,String> var;
			String name(ff.GetName());
			LoadVarFile(ConfigFile(name),var);
			int upp=var.Find("UPP");
			int out=var.Find("OUTPUT");
			if(upp>=0&&out>=0){
				name=name.Left(name.GetCount()-4);
				int d=0;
				     if(name=="uppsrc")   {d=1;}
				else if(name=="examples") {d=2;}
				else if(name=="reference"){d=3;}
				else if(name=="tutorial") {d=4;}
				else if(name=="bazaar")   {d=5;}
				else if(name=="uppbox")   {d=6;}
				else if(name=="MyApps")   {d=7;}
				s3.asmbls.Add(name,ReplaceVars(var[upp],map),ReplaceVars(var[out],map),desc[d]);
			}
		}
	if(s3.asmbls.Find("uppsrc",0)<0)
		s3.asmbls.Add("uppsrc","$(SRC)/uppsrc","$(OUT)","The heart of U++");
	if(s3.asmbls.Find("examples",0)<0)
		s3.asmbls.Add("examples","$(SRC)/examples;$(SRC)/uppsrc","$(OUT)","Example applications");
	if(s3.asmbls.Find("reference",0)<0)
		s3.asmbls.Add("reference","$(SRC)/reference;$(SRC)/uppsrc","$(OUT)","Simple reference examples");
	if(s3.asmbls.Find("tutorial",0)<0)
		s3.asmbls.Add("tutorial","$(SRC)/tutorial;$(SRC)/uppsrc","$(OUT)","Sources of tutorial examples");
	if(s3.asmbls.Find("bazaar",0)<0)
		s3.asmbls.Add("bazaar","$(SRC)/bazaar;$(SRC)/uppsrc","$(OUT)","Community contributed packages");
	if(s3.asmbls.Find("uppbox",0)<0 && s1.src==2)
		s3.asmbls.Add("uppbox","$(SRC)/uppbox;$(SRC)/uppsrc","$(OUT)","U++ infrastructure (web, packaging)");
	if(s3.asmbls.Find("MyApps",0)<0)
		s3.asmbls.Add("MyApps",GetHomeDirFile("MyApps")+";$(SRC)/uppsrc","$(OUT)","Place for users own packages");
}

void InstallWizard::AsmMenu(Bar& bar)
{
	bar.Add("New assembly..", THISBACK(OnAsmAdd))
		.Key(K_INSERT);
	bar.Add(s3.asmbls.IsCursor(), "Edit assembly..", THISBACK(OnAsmEdit))
		.Key(K_CTRL_ENTER);
	bar.Add(s3.asmbls.IsCursor(), "Remove assembly", THISBACK(OnAsmRemove))
		.Key(K_CTRL_DELETE);
}

void InstallWizard::OnAsmAdd(){
	WithAsmSetupLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg,"Assembly setup");
	if(s3.asmbls.IsCursor()){
		dlg.base  <<=s3.asmbls.Get(0);
		dlg.upp   <<=s3.asmbls.Get(1);
		dlg.output<<=s3.asmbls.Get(2);
	}
	if(dlg.Run()==IDOK) {
		s3.asmbls.Add(~dlg.base,~dlg.upp,~dlg.output);
	}
}

void InstallWizard::OnAsmEdit(){
	if(!s3.asmbls.IsCursor())
		return;
	WithAsmSetupLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg,"Assembly setup");
	dlg.base  <<=s3.asmbls.Get(0);
	dlg.upp   <<=s3.asmbls.Get(1);
	dlg.output<<=s3.asmbls.Get(2);
	if(dlg.Run()==IDOK) {
		s3.asmbls.Set(0,~dlg.base);
		s3.asmbls.Set(1,~dlg.upp);
		s3.asmbls.Set(2,~dlg.output);
	}
}

void InstallWizard::OnAsmRemove(){
	int c = s3.asmbls.GetCursor();
	if(c < 0) return;
	s3.asmbls.Remove(c);
}

void InstallWizard::SrcChange(){
	int s=s1.src;
	
	bool check=s!=3 && s!=1;
	s1.period.Enable(check);
	s1.unit.Enable(check);
	s1.startup.Enable(check);
	
	int i=s3.asmbls.Find("uppbox",0);
	if(s==2&&i<0){
		s3.asmbls.Add("uppbox","$(SRC)/uppbox;$(SRC)/uppsrc","$(OUT)","U++ infrastructure (web, packaging)");
	}
	if(s!=2&&i>=0&&s3.asmbls.Get(i,1)=="$(SRC)/uppbox;$(SRC)/uppsrc"&&!FileExists(ConfigFile("uppbox.var"))){
		s3.asmbls.Remove(i);
	}
	
	Block(2,(s!=2)); //block 2 unless svn
	Block(3,(s==3)); //block 3 if no repositories
	
	if(s==1){
		s3.srcpath.Disable();
		s3.srcpath<<=UpdaterCfg().globalsrc;
	}else{
		s3.srcpath.Enable();
		if(UpdaterCfg().localsrc==UpdaterCfg().globalsrc)
			s3.srcpath<<=GetHomeDirFile("upp");
		else
			s3.srcpath<<=UpdaterCfg().localsrc;
	}
}

void InstallWizard::RepoChange(){
	s2.svnsetup.Enable(s2.server==1);
}

void InstallWizard::RepoEdit(){
	svndlg.ExecuteOK();
}

String InstallWizard::ReplaceVars(String str,const VectorMap<String,String>& vars){
	String tmp;
	int o,p;
	for(int i=0;i<vars.GetCount();i++){
		o=0;
		const char* var=vars.GetKey(i);
		p=str.Find(var);
		while(p>=0){
			tmp+=str.Mid(o,p-o)+vars[i];
			o=p+strlen(var);
			p=str.Find(var,o);
		}
		str=tmp+str.Mid(o);
		tmp="";
	}
	return str;
}

void InstallWizard::Perform(){
	Progress p;
	p.AlignText(LEFT);
	p.Title("Installation");
	UpdaterCfg().method=s1.src;
	UpdaterCfg().sync=s2.syncmethod;
	if(!IsNull(~s1.period)){
		UpdaterCfg().period=s1.period * int(~s1.unit) * (bool(s1.startup)?-1:1);
	}else{
		UpdaterCfg().period=bool(s1.startup)?0:int(Null);
	}
	UpdaterCfg().localsrc=~s3.srcpath;
	if(s2.server==0){
		//defualt SVN server
		UpdaterCfg().svnserver="http://upp-mirror.googlecode.com/svn/trunk/";
		UpdaterCfg().svnuser="";
		UpdaterCfg().svnpass="";
		UpdaterCfg().svnreadonly=true;
	}else{
		//custom SVN server
		UpdaterCfg().svnserver=~svndlg.url;
		UpdaterCfg().svnuser=~svndlg.usr;
		UpdaterCfg().svnpass=~svndlg.pwd;
		UpdaterCfg().svnreadonly=~svndlg.readonly;
	}
	switch(UpdaterCfg().method) {
		case 0:{
			// local copy of files from /usr/share/upp
			p.SetText("Copying files ...");
			if(!CopyFolder(UpdaterCfg().localsrc,UpdaterCfg().globalsrc,&p)){
				Exclamation(DeQtf("Failed to copy "+UpdaterCfg().globalsrc+" to "+UpdaterCfg().localsrc));
			}
			FindFile ff(AppendFileName(UpdaterCfg().globalsrc, "*.bm"));
			while (ff) {
				DeleteFile(AppendFileName(UpdaterCfg().localsrc, ff.GetName()));
				ff.Next();
			}
			break;
		}
		case 1:{
			// read-only repos in /usr/share/upp
			UpdaterCfg().localsrc=UpdaterCfg().globalsrc;
			break;
		}
		case 2: {
			// SVN assemblies
			p.SetText("Downloading sources from svn (This may take a while) ...");
			String cmd="svn checkout --non-interactive --force ";
			if(!UpdaterCfg().svnuser.IsEmpty()){cmd+="--username \""+UpdaterCfg().svnuser+"\" ";}
			if(!UpdaterCfg().svnpass.IsEmpty()){cmd+="--password \""+UpdaterCfg().svnpass+"\" ";}
			cmd+=String(~svndlg.url);
			if(UpdaterCfg().sync==1){cmd+="@"+IntStr(ScanInt(IDE_VERSION));}
			cmd+=" "+String(~s3.srcpath);
			LocalProcess svn(cmd);
			int count=0;
			while(svn.IsRunning()){
				p.Step();
				Sleep(50);
				//we better assure user that something is happening, downloading all the files is rather long...
				String out=svn.Get();
				int i=out.Find('\n',0);
				while(i>0){
					i=out.Find('\n',i+1);
					count++;
				}
				p.SetText(Format("%i files downloaded",count));
			}
			String err;
			SaveFile(AppendFileName(~s3.srcpath,"uppsrc/ide/version.h"),"#define IDE_VERSION    \""+GetSvnVersion(UpdaterCfg().localsrc,false,err)+"\"\n");
			break;
		}
		case 3:
			//no assemblies
			return;
	}
	for(int i=0;i<s3.asmbls.GetCount();i++){
		VectorMap<String,String> map;
		map.Add("$(SRC)",~s3.srcpath);
		map.Add("$(OUT)",~s3.outpath);
		String cfg="UPP = " + AsCString(ReplaceVars(s3.asmbls.Get(i,1),map)) + "\n";
		cfg+="OUTPUT = " + AsCString(ReplaceVars(s3.asmbls.Get(i,2),map));
		SaveFile(ConfigFile(AsString(s3.asmbls.Get(i,0))+".var"),cfg);
	}
	
	Vector<String> deleted;
	for(FindFile ff(ConfigFile("*.var")); ff; ff.Next())
		if(ff.IsFile()){
			String name=ff.GetName();
			name=name.Left(name.GetCount()-4);
			if(s3.asmbls.Find(name)<0)
				deleted.Add(ConfigFile(name+".var"));
		}
	String list;
	for(int i = 0; i < deleted.GetCount(); i++){
		list<<'&'<<DeQtf(deleted[i]);
	}
	if(!list.IsEmpty()&&PromptYesNo("Do you really want to delete following file(s)?&"+list)){
		for(int i = 0; i < deleted.GetCount(); i++){
			DeleteFile(deleted[i]);
		}
	}
	StoreAsXMLFile(UpdaterCfg(),"SourceUpdater",ConfigFile("updates.xml"));
}

void Uninstall(){
	DeleteFile(ConfigFile("updater.cfg"));
	DeleteFile(ConfigFile("uppsrc.var"));
	DeleteFile(ConfigFile("bazaar.var"));
	DeleteFile(ConfigFile("examples.var"));
	DeleteFile(ConfigFile("tutorial.var"));
	DeleteFile(ConfigFile("reference.var"));
	DeleteFile(ConfigFile("uppbox.var"));
	DeleteFile(ConfigFile("MyApps.var"));
	if(UpdaterCfg().localsrc!=UpdaterCfg().globalsrc)
		DeleteFolderDeep(UpdaterCfg().localsrc);
}

