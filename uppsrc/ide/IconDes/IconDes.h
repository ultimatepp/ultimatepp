#ifndef _ide_IconDes_IconDes_h_
#define _ide_IconDes_IconDes_h_

#include <ide/Common/Common.h>
#include <IconDes/IconDes.h>

struct IdeIconDes : IdeDesigner, IconDes {
	virtual String GetFileName() const;
	virtual void   Save();
	virtual void   SaveEditPos();
	virtual void   EditMenu(Bar& menu);
	virtual Ctrl&  DesignerCtrl()             { return *this; }

	virtual void Serialize(Stream& s);

	int     format;
	String  filename;
	Time    filetime;

	virtual void   ToolEx(Bar& bar);

	bool   Load(const char *filename);
	void   FileProperties();

	typedef IdeIconDes CLASSNAME;

	IdeIconDes()                              { format = 0; }
};

#endif
