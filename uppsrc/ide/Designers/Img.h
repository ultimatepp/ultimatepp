#ifndef _ide_ImgDes_ImgDes_h_
#define _ide_ImgDes_ImgDes_h_

#include <ide/Common/Common.h>
#include <ide/IconDes/IconDes.h>

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

struct IdePngDes : IdeIconDes {
	virtual void   Save();

	void   Load(const char *filename);
	void   Create(const char *filename);
};

#endif
