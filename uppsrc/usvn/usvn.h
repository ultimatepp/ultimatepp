#ifndef _usvn_usvn_h_
#define _usvn_usvn_h_

#include <TextDiffCtrl/TextDiffCtrl.h>

using namespace Upp;

#define LAYOUTFILE <usvn/usvn.lay>
#include <CtrlCore/lay.h>

class SysConsole : public WithConsoleLayout<TopWindow> {
	typedef SysConsole CLASSNAME;
	
	Font font;
	void AddResult(const String& out);

public:
	int  System(const char *s);
	int  CheckSystem(const char *s);
	void Perform()	                            { exit.Show(); Execute(); }

	SysConsole();
};

class SvnSel : public WithSvnSelLayout<TopWindow> {
	String url, usr, pwd;
	String folder;
	
	bool Load(const String& path);
	void SyncResult();
	void Go();
	void DirUp();
	bool NewUrl();
	void Url();
	bool TryLoad(const char *url);
	bool Select0();

	typedef SvnSel CLASSNAME;

public:
	bool Select();
	bool Select(const char *url, const char *user, const char *pwd);

	String GetUsername() const         { return usr; }
	String GetPassword() const         { return pwd; }
	String GetUrl() const              { return ~result; }
	
	SvnSel();
};

struct Repo {
	String repo;
	String work;
	String user;
	String pswd;

	void Save(String& s);
	void Load(CParser& p);
};

struct SvnWork {
	String working;
	String user;
	String password;
	int revision;
	bool readonly;
};

class SvnWorks : public WithSvnWorksLayout<TopWindow> {
	void New();
	void Edit();
	void Remove();
	void Checkout();
	void Sync();
	
	FrameRight<Button> dirsel;
	void DirSel(EditField& f);

public:
	void    Clear();
	void    Add(const String& working, const String& user, const String& data, bool readonly=false);
	void    Load(const String& text);
	String  Save() const;
	
	int     GetCount() const;
	SvnWork operator[](int i) const;
	void SetRevision(int i, int revision);

	typedef SvnWorks CLASSNAME;
	
	SvnWorks();
};

String SvnCmd(const char *cmd, const String& user, const String& pwd, int rev=0);
String SvnCmd(const char *cmd, const SvnWork& w);

bool   IsSvnDir(const String& p);

bool   CheckSvn();

struct SvnSync : WithSvnSyncLayout<TopWindow> {
	enum {
		MODIFY,
		CONFLICT,
		ADD,
		REMOVE,
		REPLACE,
		
		REPOSITORY,
		MESSAGE,
		
		SVN_IGNORE = -1,
		DELETEC = -2,
		SVN_ACTION = -3,
	};
	
	Array<Switch>            revert;
	Array<DocEdit>           message;
	Array<Button>            diff;
	ArrayMap<String, String> msgmap;
	
	SvnWorks works;
	
	void SyncList();
	void Setup();
	void DoDiff(int ii);
	void Diff();

	typedef SvnSync CLASSNAME;

public:
	void   SetMsgs(const String& s);
	String GetMsgs();

	void Dir(const char *dir, bool readonly=false);
	void FixRevision(const char *dir, int revision);
	void Perform();
	void DoSync();
	
	SvnSync();
};

void RunSvnDiff(String editfile);

#endif
