#ifndef _urepo_urepo_h_
#define _urepo_urepo_h_

#include <TextDiffCtrl/TextDiffCtrl.h>

using namespace Upp;

#define LAYOUTFILE <urepo/urepo.lay>
#include <CtrlCore/lay.h>

class UrepoConsole : public WithUrepoConsoleLayout<TopWindow> {
	typedef UrepoConsole CLASSNAME;
	
	Font font;
	void AddResult(const String& out);
	bool withcancel = false;
	bool canceled = false;

public:
	int  System(const char *s);
	int  CheckSystem(const char *s);
	int  Git(const char *dir, const char *command);
	void Log(const Value& s, Color ink = SColorText());
	void Perform()	                            { exit.Show(); Execute(); }
	void WithCancel(bool b = true)              { withcancel = b; }
	void Clear()                                { list.Clear(); }

	UrepoConsole();
};

String SvnCmd(const char *cmd);

enum { NOT_REPO_DIR, SVN_DIR, GIT_DIR };

int    GetRepoKind(const String& p);

String GitCmd(const char *dir, const char *command);

struct RepoSync : WithRepoSyncLayout<TopWindow> {
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
	
	struct SvnOptions : WithSvnOptionsLayout<ParentCtrl> {
		SvnOptions() { CtrlLayout(*this); }
	};
	
	struct GitOptions : WithGitOptionsLayout<ParentCtrl> {
		GitOptions() { CtrlLayout(*this); }
	};

	struct Work {
		int    kind;
		String dir;
	};
	
	Array<Work> work;
	
	bool ListGit(const String& path);
	bool ListSvn(const String& path);
	bool GitFile(UrepoConsole& sys, int action, const String& path, bool revert);
	bool SvnFile(UrepoConsole& sys, String& filelist, int action, const String& path, bool revert);
	void SyncCommits();
	void SyncList();
	void DoDiff(int ii);

	typedef RepoSync CLASSNAME;

public:
	void   SetMsgs(const String& s);
	String GetMsgs();

	void Dir(const char *dir, int kind);
	void Dir(const char *dir);
	void DoSync();
	
	RepoSync();
};


void RunRepoDiff(const String& filepath);

#endif
