#ifndef _usvn_usvn_h_
#define _usvn_usvn_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include "SlaveProcess.h"

#define LAYOUTFILE <usvn/usvn.lay>
#include <CtrlCore/lay.h>

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
	void    Add(const String& working, const String& user, const String& data);
	void    Load(const String& text);
	String  Save() const;
	
	int     GetCount() const;
	SvnWork operator[](int i) const;

	typedef SvnWorks CLASSNAME;
	
	SvnWorks();
};

struct SvnSync : WithSvnSyncLayout<TopWindow> {
	enum {
		MODIFY,
		CONFLICT,
		ADD,
		REMOVE,
		
		REPOSITORY,
		MESSAGE,
	};
	
	Array<Option>     confirm;
	Array<EditString> message;
	
	void Add(const char *working);
	
	SvnSync();
};

class SysConsole : public WithConsoleLayout<TopWindow> {
	typedef SysConsole CLASSNAME;
	
	Font font;
	bool canceled;
	void AddResult(const String& out);

public:
	int System(const char *s);
	
	void Cancel();
	void IsCanceled();
	void ClearCanceled();
	
	SysConsole();
};

#endif
