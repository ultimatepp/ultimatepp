#include <RichEdit/RichEdit.h>
#include <PdfDraw/PdfDraw.h>
#include <art/BlueBar/BlueBar.h>

using namespace Upp;

#define IMAGECLASS UWordImg
#define IMAGEFILE  <UWord/UWord.iml>
#include <Draw/iml.h>

FileSel& UWordFs()
{
	static FileSel fs;
	return fs;
}

FileSel& PdfFs()
{
	static FileSel fs;
	return fs;
}

FileSel& RTFFs() {
	static FileSel fs;
	return fs;
}


class UWord : public TopWindow {
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
	void Save();
	void SaveAs();
	void ExportAs();
	void Import();
	void Print();
	void Pdf();
	void About();
	void Destroy();
	void SetBar();
	void FileBar(Bar& bar);
	void AboutMenu(Bar& bar);
	void MainMenu(Bar& bar);
	void MainBar(Bar& bar);

public:
	typedef UWord CLASSNAME;

	static void SerializeApp(Stream& s);

	UWord();
};

void UWord::FileBar(Bar& bar)
{
	bar.Add("New", CtrlImg::new_doc(), THISBACK(New))
	   .Key(K_CTRL_N)
	   .Help("Open new window");
	bar.Add("Open..", CtrlImg::open(), THISBACK(Open))
	   .Key(K_CTRL_O)
	   .Help("Open existing document");
	bar.Add(editor.IsModified(), "Save", CtrlImg::save(), THISBACK(Save))
	   .Key(K_CTRL_S)
	   .Help("Save current document");
	bar.Add("SaveAs", CtrlImg::save_as(), THISBACK(SaveAs))
	   .Help("Save current document with a new name");
	bar.ToolGap();
	bar.Add("Export As RTF", THISBACK(ExportAs))
		.Help("Export the file as RTF");
	bar.Add("Import RTF",THISBACK(Import))
		.Help("Import RTF File");
	bar.MenuSeparator();
	bar.Add("Print..", CtrlImg::print(), THISBACK(Print))
	   .Key(K_CTRL_P)
	   .Help("Print document");
	bar.Add("Export to PDF..", UWordImg::pdf(), THISBACK(Pdf))
	   .Help("Export document to PDF file");
	if(bar.IsMenuBar()) {
		if(lrufile().GetCount())
			lrufile()(bar, THISBACK(OpenFile));
		bar.Separator();
		bar.Add("Exit", THISBACK(Destroy));
	}
}

void UWord::AboutMenu(Bar& bar)
{
	bar.Add("About..", THISBACK(About));
	bar.Separator();
	bar.Add("What is this ?", callback(PerformDescription))
	   .Key(K_SHIFT_F1);
}

void UWord::MainMenu(Bar& bar)
{
	bar.Add("File", THISBACK(FileBar));
	bar.Add("Window", callback(WindowsMenu));
	bar.Add("Help", THISBACK(AboutMenu));
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

void UWord::Save()
{
	lrufile().NewEntry(filename);
	if(!editor.IsModified()) return;
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

void UWord::SaveAs()
{
	FileSel& fs = UWordFs();
	if(fs.ExecuteSaveAs()) {
		filename = fs;
		Title(filename);
		Save();
	}
}


void UWord::ExportAs() {
	FileSel & fs = RTFFs();
	if(fs.ExecuteSaveAs()) {
	String	filename = fs;
		if(SaveFile(filename,EncodeRTF(ParseQTF(editor.GetQTF()),CHARSET_UTF8))){
			Exclamation("File " + filename + "was saved.");
			ClearModify();
			}
	}
}

//
//IMPORT RTF
void UWord::Import() {
	FileSel& fs = RTFFs();
	if(fs.ExecuteOpen()) {
		String	filename = (String) fs;
		String fileb = LoadFile(filename);
		DUMP(fileb);
		RichText fbody = ParseRTF(fileb);
		String qbody = AsQTF(fbody);
		DUMP(qbody);
		editor.SetQTF(qbody);
		editor.ClearModify();
	}
	else
		statusbar.Temporary("Loading Aborted.");
}



void UWord::Print()
{
	editor.Print();
}

void UWord::Pdf()
{
	FileSel& fs = PdfFs();
	if(!fs.ExecuteSaveAs("Output PDF file"))
		return;
	Size page = Size(3968, 6074);
	PdfDraw pdf;
	::Print(pdf, editor.Get(), page);
	SaveFile(~fs, pdf.Finish());
}

void UWord::About()
{
	MemoryProfileInfo();
//	PromptOK("[A5 uWord]&Using [*^www://upp.sf.net^ Ultimate`+`+] technology.");
}

void UWord::Destroy()
{
	if(editor.IsModified()) {
		switch(PromptYesNoCancel("Do you want to save the changes to the document?")) {
		case 1:
			Save();
			break;
		case -1:
			return;
		}
	}
	delete this;
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
//	toolbar.ButtonKind(ToolButton::BOTTOMLABEL);
	AddFrame(toolbar);
	AddFrame(statusbar);
	Add(editor.SizePos());
	menubar.Set(THISBACK(MainMenu));
	Sizeable().Zoomable();
	WhenClose = THISBACK(Destroy);
	menubar.WhenHelp = toolbar.WhenHelp = statusbar;
	static int doc;
	Title(Format("Document%d", ++doc));
	Icon(CtrlImg::File());
	editor.ClearModify();
	editor <<= THISBACK(SetBar);
	SetBar();
	editor.WhenRefreshBar = THISBACK(SetBar);
	OpenMain();
	ActiveFocus(editor);
	Vector<int> font;
	for(int i = 0; i < 10; i++)
		font.Add(i);
	editor.FontFaces(font);
//	BackPaint();
}

void UWord::SerializeApp(Stream& s)
{
	int version = 1;
	s / version;
	s % UWordFs()
	  % PdfFs();
	if(version >= 1)
		s % lrufile();
}

GUI_APP_MAIN
{
//	DUMP(XGetSelectionOwner(Xdisplay, XAtom(String().Cat() << "_NET_WM_CM_S" << Xscreenno)) != None);
//	return;

//	DUMP((Xheight*25.4/XheightMM));
	
	SetLanguage(LNG_('D','E','D','E'));

	UWordFs().Type("QTF files", "*.qtf")
	         .AllFilesType()
	         .DefaultExt("qtf");
	PdfFs().Type("PDF files", "*.pdf")
	       .AllFilesType()
	       .DefaultExt("pdf");

//	InstallBlueBar();
//	InstallBlueBar();
//	ChClassicSkin();
//	Ctrl::ShowRepaint(50);
	LoadFromFile(callback(UWord::SerializeApp));
	new UWord;
	Ctrl::EventLoop();
	StoreToFile(callback(UWord::SerializeApp));
}
