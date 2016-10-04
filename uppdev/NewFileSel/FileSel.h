#ifndef _NewFileSel_FileSel_h_
#define _NewFileSel_FileSel_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

NAMESPACE_UPP

#define FileSel NewFileSel
#define TrimDot NewTrimDot
#define DirectoryUp NewDirectoryUp
#define Catq        NewCatq
#define FolderDisplay NewFolderDisplay

class FileSel : public WithFileSelectorLayout<TopWindow> {
public:
	virtual bool Key(dword key, int count);

private:
	SizeGrip    sizegrip;

	Button      dirup, mkdir, plus, minus, toggle;

protected:
	struct Folder {
		String  name;
		Value   folder;
	};
	
	Array<Folder>  current;
	
	Vector<String> mask;
	Vector<String> path;
	Vector<String> lru;
	int            lastsby;
	FileSystemInfo *filesystem;

	String         olddir;
	String         basedir;
	int            activetype;
	String         defext;
	Vector<String> fn;

	DisplayCtrl    preview_display;
	Ctrl          *preview;
	FileList       list;
	
	enum {
		OPEN, SAVEAS, SELECTDIR
	};

	int         mode;
	bool        asking;
	bool        multi;
	bool        rdonly;
	bool        bidname;
	bool        appmodal;

	bool        PatternMatch(const char *fn);
	bool        OpenItem();
	void        OpenItem2()                                  { OpenItem(); }
	void        Open();
	void        DirUp();
	void        MkDir();
	void        Plus();
	void        Minus();
	void        Toggle();
	void        PlusMinus(const char *title, bool sel);
	void        Update();
	void        FileUpdate();
	void        Rename(const String& on, const String& nn);
	void        Choice();
	void        Load();
	String      FilePath(const String& fn);
	void        SetDir(const String& dir);
	String      GetDir();
	void        AddName(Vector<String>& fn, String& o);
	void        Finish();
	bool        Execute(int mode);
	bool        IsMulti()                                     { return multi && mode == OPEN; }
	void        SyncSplitter();
	String      GetMask();

	using       WithFileSelectorLayout<TopWindow>::Title;

public:
	typedef FileSel CLASSNAME;

	Callback3<bool, const String&, Image&> WhenIcon;

	void        Serialize(Stream& s);

	bool        ExecuteOpen(const char *title = NULL);
	bool        ExecuteSaveAs(const char *title = NULL);

	bool        ExecuteSelectDir(const char *title = NULL);

	String Get() const                           { return GetFile(0); }
	void   Set(const String& s);
	void   Set(const Vector<String>& s)          { fn <<= s; bidname = true; }

	operator String() const                      { return Get(); }
	void operator=(const String& s)              { Set(s); }

	String operator~() const                     { return Get(); }
	void operator<<=(const String& s)            { Set(s); }

	int    GetCount() const                      { return fn.GetCount(); }
	String GetFile(int i) const;
	String operator[](int i) const               { return GetFile(i); }
	void   ClearFiles()                          { fn.Clear(); }

	bool   GetReadOnly() const                   { return readonly; }
	String GetActiveDir() const                  { return dir.GetData(); }
	int    GetActiveType() const                 { return activetype; }

	void   Filesystem(FileSystemInfo& fsys)      { filesystem = &fsys;}
	FileSystemInfo& GetFilesystem() const        { return *filesystem; }

	FileSel& Type(const char *name, const char *ext);
	FileSel& AllFilesType();
	FileSel& ActiveDir(const String& d)          { dir <<= d; return *this; }
	FileSel& ActiveType(int i)                   { activetype = i; return *this;  }
	FileSel& DefaultExt(const char *ext)         { defext = ext; return *this; }
	FileSel& Multi(bool b = true)                { multi = b; return *this; }
	FileSel& ReadOnlyOption(bool b = true)       { rdonly = b; return *this; }
	FileSel& MkDirOption(bool b = true)          { mkdir.Show(b); return *this; }
	FileSel& NoMkDirOption()                     { return MkDirOption(false); }
	FileSel& BaseDir(const char *dir)            { basedir = dir; return *this; }
	FileSel& Asking(bool b = true)               { asking = b; return *this; }
	FileSel& NoAsking()                          { return Asking(false); }
	FileSel& EditFileName(bool b)                { file.SetEditable(b); return *this; }
	FileSel& NoEditFileName()                    { return EditFileName(false); }
	FileSel& AppModal(bool b = true)             { appmodal = b; return *this; }
	FileSel& NoAppModal()                        { return AppModal(false); }
	FileSel& Preview(Ctrl& ctrl);
	FileSel& Preview(const Display& d);

	FileSel();
	virtual ~FileSel();
};

END_UPP_NAMESPACE

#endif
