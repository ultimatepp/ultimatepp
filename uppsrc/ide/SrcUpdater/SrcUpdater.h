#ifndef _SourceUpdater_SourceUpdater_h
#define _SourceUpdater_SourceUpdater_h

#include "Wizard.h"
#include <usvn/usvn.h>
#include <ide/version.h>

#if !defined(PLATFORM_POSIX) && !defined(flagWIN32)
#error Updater is only implemented for POSIX compliant platforms
#endif

#define LAYOUTFILE <ide/SrcUpdater/SrcUpdater.lay>
#include <CtrlCore/lay.h>

struct UpdaterConfig{
	int method,sync,ignored,period;
	String localsrc,globalsrc;
	String svnserver,svnuser,svnpass;
	Time last;
	Vector<String> ignores;
	bool svnreadonly,available;
	void Xmlize(XmlIO xml){
		xml("Method",method)
		   ("Synchronization",sync)
		   ("CheckPeriod",period)
		   ("LastCheck",last)
		   ("Ignored",ignored)
		   ("LocalSource",localsrc)
		   ("GlobalSource",globalsrc)
		   ("IgnoredFiles",ignores)
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

//storage and access to triplets of crc hashes (in moveable struct)
struct synchashes{
	dword a[3];
	synchashes(){a[0]=a[1]=a[2]=0;}
};
inline void AssertMoveable0(synchashes*){};

struct LocalSync : WithSvnSyncLayout<TopWindow> {
	String localsrc,globalsrc;
	Array<Switch> switches;
	Array<Button> btns;
	VectorMap<String,synchashes> files;
	int row;
	
	typedef LocalSync CLASSNAME;
	void DoShowFile(const String& l,const String& g);
	void ShowFile();
public:
	enum{
		FA_NOOP=0,
		FA_SKIP=1,
		FA_KEEP=2,
		FA_REVERT=4,
		FA_INSTALL=8,
		FA_ADD=16,
		FA_DELETE=32
	};
	struct FileState{
		int n;
		String desc;
		Color c;
		Font f;
		void operator()(int fa,const String& description,const class Color& color,bool italic){
			n=fa;
			desc=description;
			c=color;
			f=StdFont().Italic(italic);
		};
	};
	LocalSync();
	void Populate(const VectorMap<String,synchashes>& changed);
	void Perform();
	void Menu(Bar& bar);
	void Ignore(const String& str);
	void SetAction(const String& str, int action);
	void Manage();
};

class SourceUpdater : public WithUpdateLayout<TopWindow>{
	String local, global, error;
	void DoUpdate();
	void Ignore();
	
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
	void ClearError()      {error="";}
	Event<>  WhenUpdateAvailable;
};

#endif

