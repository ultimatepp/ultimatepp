#ifndef _SourceUpdater_SourceUpdater_h
#define _SourceUpdater_SourceUpdater_h

#include "Wizard.h"
#include <usvn/usvn.h>
#include <ide/version.h>

#ifndef PLATFORM_POSIX
#error Updater is only implemented for POSIX compliant platforms
#endif

#define LAYOUTFILE <ide/SrcUpdater/SrcUpdater.lay>
#include <CtrlCore/lay.h>

struct UpdaterConfig{
	int method,sync,ignored,period;
	String localsrc,globalsrc;
	String svnserver,svnuser,svnpass;
	Time last;
	bool svnreadonly,available;
	void Xmlize(XmlIO xml){
		xml("Method",method)
		   ("Synchronization",sync)
		   ("CheckPeriod",period)
		   ("LastCheck",last)
		   ("Ignored",ignored)
		   ("LocalSource",localsrc)
		   ("GlobalSource",globalsrc)
		   ("SVN",svnserver)
		   ("SVNUser",svnuser)
		   ("SVNPassword",svnpass)
		   ("SVNReadonly",svnreadonly);
	}
};

UpdaterConfig& UpdaterCfg();

void Uninstall();
String FindGlobalSrc();
void LoadUpdaterCfg();
String GetSvnVersion(const String& server,bool verbose,String& error);
String GetSrcVersion(const char* dir,String& error);

typedef Wizard<WithTextOnlyLayout<ParentCtrl>,
               WithSourceLayout<ParentCtrl>,
               WithSvnLayout<ParentCtrl>,
               WithAssembliesLayout<ParentCtrl>,
               WithTextOnlyLayout<ParentCtrl> 
              > SrcWiz;

class InstallWizard : public SrcWiz {
	String src,out;
	void SrcChange();
	void RepoChange();
	void RepoEdit();
	void Perform();
	void AsmMenu(Bar& bar);
	void OnAsmAdd();
	void OnAsmEdit();
	void OnAsmRemove();
	void RestoreAsm();
	String ReplaceVars(String str,const VectorMap<String,String>& vars);
public:
	typedef InstallWizard CLASSNAME;
	WithCustomSvnLayout<TopWindow> svndlg;
	InstallWizard();
};

struct LocalSync : WithSvnSyncLayout<TopWindow> {
	String localsrc,globalsrc;
	Array<Switch> switches;
	Array<Button> btns;
	void DoShowFile(int ii,int state);
	void ShowFile();
public:
	enum FileState{FS_CHANGED=0,FS_DELETED=1,FS_ADDED=2};
	typedef LocalSync CLASSNAME;
	LocalSync(const VectorMap<String,int>& changed,const String& localsrc);
	void Populate(VectorMap<String,int> changed);
	void Perform();
};

class SourceUpdater : public WithUpdateLayout<TopWindow>{
	String local, global, error;
	void DoUpdate();
	void Ignore();
	void GetFiles(const String& dir,int prefix,VectorMap<String,String>& result,Progress& p);
	void GetMd5Sums(const String& dir,VectorMap<String,String>& list,Progress& p);
	void LoadMd5Sums(const String& fn,VectorMap<String,String>& list,Progress& p);
	VectorMap<String,int> CompareFiles(VectorMap<String,String>& local,VectorMap<String,String>& global,Progress& p);
	
	LocalProcess pl,pg;
	void CheckLocalSvn();
	void CheckLocalSvnFinished();
	void CheckGlobalSvn();
	void CheckGlobalSvnFinished();
public:
	typedef SourceUpdater CLASSNAME;
	SourceUpdater();
	bool NeedsUpdate(bool verbose=false);
	void CheckUpdates();
	String GetLocal()const {return local;}
	String GetError()const {return error;}
	Callback WhenUpdateAvailable;
};

#endif

