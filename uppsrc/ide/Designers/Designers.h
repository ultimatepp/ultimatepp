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

INITIALIZE(Img)

struct FileHexView : public IdeDesigner, public HexView, private LRUCache<String, int64>::Maker {
	virtual bool   Key(dword key, int count)  { return HexView::Key(key, count); } // silence CLANG warning

public:
	virtual int    Byte(int64 addr);

	virtual int64  Key() const;
	virtual int    Make(String& object) const;

	virtual String GetFileName() const        { return filename; }
	virtual void   Save()                     {}
	virtual void   EditMenu(Bar& menu)        { StdMenu(menu); }
	virtual Ctrl&  DesignerCtrl()             { return *this; }
	virtual void   RestoreEditPos();
	virtual void   SaveEditPos();

	String  filename;

	FileIn  file;
	int64   blk;
	
	LRUCache<String, int64> cache;

	static VectorMap<String, Tuple2<int64, int64>> pos;
	
	enum { BLKSHIFT = 14, BLKSIZE = 1 << BLKSHIFT };
	
	void    Serialize(Stream& s)              { SerializeSettings(s); }

	void Open(const char *path);
	
	FileHexView();
	~FileHexView();
};

INITIALIZE(HexView)

struct IdeQtfDes : IdeDesigner, RichEditWithToolBar {
	String  filename;

	virtual String GetFileName() const        { return filename; }
	virtual void   Save();
	virtual void   SaveEditPos();
	virtual void   EditMenu(Bar& menu);
	virtual Ctrl&  DesignerCtrl()             { return *this; }

	virtual void   Serialize(Stream& s);

	bool   Load(const char *filename);
	void   FileProperties();
	void   CopyId(const String& n);

	typedef IdeQtfDes CLASSNAME;
	
	IdeQtfDes() { Extended(); }
};

INITIALIZE(Qtf)

bool  FileIsBinary(const char *path);

struct TreeDesPos {
	Time               filetime = Null;
	Vector<int>        openid;
	int                cursor = -1;
	Point              scroll;

	void Serialize(Stream& s) { s % filetime % openid % cursor % scroll; }
	void Save(Time filetime, TreeCtrl& tree);
	void Restore(Time filetime, TreeCtrl& tree);
};

void Save(ArrayMap<String, TreeDesPos>& pos, const String& filename, Time filetime, TreeCtrl& tree);
void Restore(ArrayMap<String, TreeDesPos>& pos, const String& filename, Time filetime, TreeCtrl& tree);
void Serialize(Stream& s, ArrayMap<String, TreeDesPos>& pos);

struct TreeViewDes : IdeDesigner, Ctrl {
	virtual String GetFileName() const        { return filename; }
	virtual void   Save()                     {}
	virtual void   EditMenu(Bar& menu);
	virtual Ctrl&  DesignerCtrl()             { return *this; }
	virtual void   RestoreEditPos();
	virtual void   SaveEditPos();

	static ArrayMap<String, TreeDesPos> pos;

	String                filename;
	Time                  filetime;
	TreeCtrl              tree;
	FrameTop<StaticRect>  errorbg;
	Label                 error;
	ParentCtrl            data;
	
	TreeViewDes();
	
	void Error(const char *e);
	bool Load(const String& txt);

	virtual String GetId() = 0;
	virtual void   CopyPath() = 0;
	virtual String Load0(const String& data) = 0;
};

INITIALIZE(TreeViewDes)

struct XmlViewDes : TreeViewDes {
	virtual String GetId() { return "XML"; }
	virtual void   CopyPath();
	virtual String Load0(const String& data);

	void   Load0(int parent, XmlParser& p);
};

INITIALIZE(XmlViewDes)

struct JsonViewDes : TreeViewDes {
	virtual String GetId() { return "JSON"; }
	virtual String Load0(const String& json);
	virtual void   CopyPath();
	
	int AddNode(int parent_id, const Value& id, const String& name, const Value& v);
	
	JsonViewDes();
};

INITIALIZE(JsonViewDes)

#endif
