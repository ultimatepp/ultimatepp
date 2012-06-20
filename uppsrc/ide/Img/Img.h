#ifndef _ide_IconDes_IconDes_h_
#define _ide_IconDes_IconDes_h_

#include <ide/Common/Common.h>
#include <IconDes/IconDes.h>

struct IdeImgView : IdeDesigner, Ctrl {
	Size   img_sz;
	Image  img;
	String filename;
	
	virtual void   Paint(Draw& w);

	virtual String GetFileName() const        { return filename; }
	virtual void   EditMenu(Bar& menu);
	virtual void   Save()                     {}
	virtual Ctrl&  DesignerCtrl()             { return *this; }

	typedef IdeImgView CLASSNAME;
};

#endif