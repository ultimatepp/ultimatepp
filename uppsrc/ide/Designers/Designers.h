#ifndef _ide_Designers_Designers_h_
#define _ide_Designers_Designers_h_

#include <HexView/HexView.h>
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

class FileHexView : public IdeDesigner, public HexView, private LRUCache<String, int64>::Maker {
public:
	virtual int Byte(int64 addr);

	virtual int64 Key() const;
	virtual int Make(String& object) const;

	virtual String GetFileName() const        { return filename; }
	virtual void   Save()                     {}
	virtual void   EditMenu(Bar& menu)        { StdMenu(menu); }
	virtual Ctrl&  DesignerCtrl()             { return *this; }

private:
	String  filename;

	FileIn  file;
	int64   blk;
	
	LRUCache<String, int64> cache;

	enum { BLKSHIFT = 14, BLKSIZE = 1 << BLKSHIFT };
	

public:
	void    Serialize(Stream& s)              { SerializeSettings(s); }

	void Open(const char *path);
	
	FileHexView();
	~FileHexView();
};

bool  FileIsBinary(const char *path);

INITIALIZE(Img)
INITIALIZE(Qtf)
INITIALIZE(HexView)

#endif
