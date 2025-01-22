#include <VirtualGui/SDL2GL/SDL2GL.h>
#include <RichEdit/RichEdit.h>
#include <PdfDraw/PdfDraw.h>

using namespace Upp;

FileSel& UWordFs()
{
	static FileSel fs;
	return fs;
}

class UWord : public TopWindow {
public:
	virtual void DragAndDrop(Point, PasteClip& d);
	virtual void FrameDragAndDrop(Point, PasteClip& d);
	
	virtual void ShutdownWindow();

protected:
	RichEdit   editor;
	MenuBar    menubar;
	ToolBar    toolbar;
	StatusBar  statusbar;
	String     filename;

	static LRUList& lrufile() { static LRUList l; return l; }

	void Load(const String& filename);
	void OpenFile(const String& fn);
	void New();
	void Open();
	void Save0();
	void Save();
	void SaveAs();
	void Print();
	void Destroy(bool shutdown);
	void SetBar();
	void FileBar(Bar& bar);
	void MainMenu(Bar& bar);
	void MainBar(Bar& bar);

public:
	typedef UWord CLASSNAME;

	static void SerializeApp(Stream& s);

	UWord();
};

void UWord::FileBar(Bar& bar)
{
	bar.Add("New", CtrlImg::new_doc(), THISFN(New))
	   .Key(K_CTRL_N)
	   .Help("Open new window");
	bar.Add("Open..", CtrlImg::open(), THISFN(Open))
	   .Key(K_CTRL_O)
	   .Help("Open existing document");
	bar.Add(editor.IsModified(), "Save", CtrlImg::save(), THISFN(Save))
	   .Key(K_CTRL_S)
	   .Help("Save current document");
	bar.Add("SaveAs", CtrlImg::save_as(), THISFN(SaveAs))
	   .Help("Save current document with a new name");
	bar.ToolGap();
	bar.MenuSeparator();
	bar.Add("Print..", CtrlImg::print(), THISFN(Print))
	   .Key(K_CTRL_P)
	   .Help("Print document");
	if(bar.IsMenuBar()) {
		if(lrufile().GetCount())
			lrufile()(bar, THISFN(OpenFile));
		bar.Separator();
		bar.Add("Exit", [=] { Destroy(false); });
	}
}

void UWord::MainMenu(Bar& bar)
{
	bar.Sub("File", THISFN(FileBar));
	bar.Sub("Window", [=](Bar& bar) { WindowsMenu(bar); });
	bar.Sub("Help", [=](Bar& bar) {
		bar.Add("About..", [=] {
			PromptOK("[A5 uWord]&Using [*^www://upp.sf.net^ Ultimate`+`+] technology.");
		});
	});
}

void UWord::New()
{
	new UWord;
}

void UWord::Load(const String& name)
{
	lrufile().NewEntry(name);
	editor.SetQTF(LoadFile(name));
	filename = name;
	editor.ClearModify();
	Title(filename);
}

void UWord::OpenFile(const String& fn)
{
	if(filename.IsEmpty() && !editor.IsModified())
		Load(fn);
	else
		(new UWord)->Load(fn);
}

void UWord::Open()
{
	FileSel& fs = UWordFs();
	if(fs.ExecuteOpen())
		OpenFile(fs);
	else
		statusbar.Temporary("Loading aborted.");
}

void UWord::DragAndDrop(Point, PasteClip& d)
{
	if(AcceptFiles(d)) {
		Vector<String> fn = GetFiles(d);
		for(int i = 0; i < fn.GetCount(); i++)
			if(FileExists(fn[i]))
				OpenFile(fn[i]);
	}
}

void UWord::FrameDragAndDrop(Point p, PasteClip& d)
{
	DragAndDrop(p, d);
}

void UWord::Save0()
{
	lrufile().NewEntry(filename);
	if(filename.IsEmpty())
		SaveAs();
	else
		if(SaveFile(filename, editor.GetQTF())) {
			statusbar.Temporary("File " + filename + " was saved.");
			ClearModify();
		}
		else
			Exclamation("Error saving the file [* " + DeQtf(filename) + "]!");
}

void UWord::Save()
{
	if(!editor.IsModified()) return;
	Save0();
}

void UWord::SaveAs()
{
	FileSel& fs = UWordFs();
	if(fs.ExecuteSaveAs()) {
		filename = fs;
		Title(filename);
		Save0();
	}
}

void UWord::Print()
{
	editor.Print();
}

void UWord::Destroy(bool shutdown)
{
	if(editor.IsModified()) {
		switch((shutdown ? PromptYesNo : PromptYesNoCancel)("Do you want to save the changes to the document?")) {
		case 1:
			Save();
			break;
		case -1:
			return;
		}
	}
	delete this;
}

void UWord::ShutdownWindow()
{
	Destroy(true);
}

void UWord::MainBar(Bar& bar)
{
	FileBar(bar);
	bar.Separator();
	editor.DefaultBar(bar);
}

void UWord::SetBar()
{
	toolbar.Set(THISBACK(MainBar));
}

UWord::UWord()
{
	AddFrame(menubar);
	AddFrame(TopSeparatorFrame());
	AddFrame(toolbar);
	AddFrame(statusbar);
	Add(editor.SizePos());
	menubar.Set(THISFN(MainMenu));
	Sizeable().Zoomable();
	WhenClose = [=] { Destroy(false); };
	menubar.WhenHelp = toolbar.WhenHelp = statusbar;
	static int doc;
	Title(Format("Document%d", ++doc));
	Icon(CtrlImg::File());
	editor.ClearModify();
	SetBar();
	editor.WhenRefreshBar = THISBACK(SetBar);
	OpenMain();
	ActiveFocus(editor);
	
	editor <<= LoadFile(GetDataFile("test.qtf"));
}

void UWord::SerializeApp(Stream& s)
{
	int version = 1;
	s / version;
	s % UWordFs();
	if(version >= 1)
		s % lrufile();
}

CONSOLE_APP_MAIN
{
	SDL2GUI gui;
	gui.Create(RectC(100, 100, 1024, 768), "SDL2GL Virtual Gui Test");

	RunVirtualGui(gui, [] {
		SetLanguage(LNG_ENGLISH);
		SetDefaultCharset(CHARSET_UTF8);
	
		UWordFs().Type("QTF files", "*.qtf")
		         .AllFilesType()
		         .DefaultExt("qtf");
	
		LoadFromFile(callback(UWord::SerializeApp));
		UWord uword;
		Ctrl::EventLoop();
		StoreToFile(callback(UWord::SerializeApp));
	});
}
