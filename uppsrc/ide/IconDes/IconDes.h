#ifndef _ide_IconDes_IconDes_h_
#define _ide_IconDes_IconDes_h_

#include <ide/Common/Common.h>
#include <IconDes/IconDes.h>

struct IdeIconDes : IdeDesigner, IconDes {
	virtual String GetFileName() const;
	virtual void   Save();
	virtual void   SaveEditPos();
	virtual void   RestoreEditPos();
	virtual void   EditMenu(Bar& menu);
	virtual Ctrl&  DesignerCtrl()             { return *this; }

	virtual void   Serialize(Stream& s);

	int     format;
	String  filename;
	Time    filetime;

	virtual void   ToolEx(Bar& bar);
	virtual void   ListMenuEx(Bar& bar);

	bool   Load(const char *filename);
	void   FileProperties();
	void   CopyId(const String& n);
	void   FindId(const String& id);

	typedef IdeIconDes CLASSNAME;

	IdeIconDes()                              { format = 0; }
};

struct IdeIconEditPos : IconDes::EditPos, Moveable<IdeIconEditPos> {
	Time filetime;

	IdeIconEditPos() { filetime = Null; }
};

#endif
