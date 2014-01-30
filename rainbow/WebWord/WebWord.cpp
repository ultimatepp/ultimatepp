#include "WebWord.h"

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

void UWord::FileBar(Bar& bar)
{
/*
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
		bar.Add("Exit", THISBACK1(Destroy, false));
	}
*/
}

void UWord::AboutMenu(Bar& bar)
{
	bar.Add("About..", THISBACK(About));
}

String FormatSize(int64 n)
{
	if(n < 10000)
		return Format("%d B", n);
	else
	if(n < 10000 * 1024)
		return Format("%d.%d KB", n >> 10, (n & 1023) / 103);
	else
	if(n < I64(10000000) * 1024)
		return Format("%d.%d MB", n >> 20, (n & 1023) / 103);
	else
		return Format("%d.%d GB", n >> 30, (n & 1023) / 103);
}


void UWord::ShowInfo()
{
	String s;
	s << "Mem " << MemoryUsedKb() << " KB";
	int secs = GetSysTime() - Ctrl::stat_started;
	Time tm = Time(1, 1, 1, 0, 0, 0) + secs;
	s << ", uptime " << Format("%d:%0d:%02d:%02d", tm - Date(1, 1, 1), tm.hour, tm.minute, tm.second);
	s << ", data sent " << FormatSize(Ctrl::stat_data_send);
	if(secs)
		s << ", average bandwidth " << FormatSize(Ctrl::stat_data_send / secs) << "/s";
	s << ", actual bandwidth " << FormatSize(Ctrl::stat_data_send - sent_prev);
	s << ", putimage " << Ctrl::stat_putimage;
	s << ", putrect " << Ctrl::stat_putrect;
	s << ", setimage " << Ctrl::stat_setimage << " len " << FormatSize(Ctrl::stat_setimage_len);
	sent_prev = Ctrl::stat_data_send;
	statusbar.Set(s);
}

void UWord::MainMenu(Bar& bar)
{
//	bar.Add("File", THISBACK(FileBar));
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

void UWord::Pdf()
{
	FileSel& fs = PdfFs();
	if(!fs.ExecuteSaveAs("Output PDF file"))
		return;
	Size page = Size(3968, 6074);
	PdfDraw pdf;
	UPP::Print(pdf, editor.Get(), page);
	SaveFile(~fs, pdf.Finish());
}

void UWord::About()
{
	PromptOK("[A5 uWord]&Using [*^www://upp.sf.net^ Ultimate`+`+] technology.");
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
	menubar.Set(THISBACK(MainMenu));
	Sizeable().Zoomable();
	WhenClose = THISBACK1(Destroy, false);
	menubar.WhenHelp = toolbar.WhenHelp = statusbar;
	static int doc;
	Title(Format("Document%d", ++doc));
	Icon(CtrlImg::File());
	editor.ClearModify();
	SetBar();
	editor.WhenRefreshBar = THISBACK(SetBar);
	OpenMain();
	ActiveFocus(editor);

	SetTimeCallback(-1000, THISBACK(ShowInfo));
	sent_prev = 0;
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

struct EventsWnd : TopWindow {
	Label l;
	String k;


	Image  CursorImage(Point p, dword keyflags)
	{
		return UWordImg::pdf();
	}

	void Do() {
		static int ii;
		String x;
		if(GetCtrl())
			x << "Ctrl ";
		if(GetAlt())
			x << "Alt ";
		if(GetShift())
			x << "Shift ";
		x << k << ' ' << GetMousePos();
		l = x;
	}

	bool Key(dword key, int count) {
		k = GetKeyDesc(key) + ' ' + FormatIntHex(key);
		if(key < 256)
			k << '\"' << (char)key << '\"';
		Do();
	}

	typedef EventsWnd CLASSNAME;

	EventsWnd() {
		Add(l.SizePos());
		SetTimeCallback(-1000, THISBACK(Do));
	}
};


void FinishApp()
{
	LOG("Disconnected");
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	LOG("Session Started");

#ifdef _DEBUG
	MemoryLimitKb(5000000);
	Ctrl::debugmode = true;
#endif

	Ctrl::WhenDisconnect = callback(FinishApp);

#ifndef _DEBUG
	Ctrl::host = "eventcraft.eu";
#endif

	if(Ctrl::StartSession())
	{
		SetLanguage(LNG_ENGLISH);
		SetDefaultCharset(CHARSET_UTF8);
		
		PromptOK("Session started");
	
		UWordFs().Type("QTF files", "*.qtf")
		         .AllFilesType()
		         .DefaultExt("qtf");
		PdfFs().Type("PDF files", "*.pdf")
		       .AllFilesType()
		       .DefaultExt("pdf");
	
		LoadFromFile(callback(UWord::SerializeApp));
	#ifdef _DEBUG
		(new UWord)->editor.SetQTF(LoadFile(GetDataFile("test.qtf")));
	#else
		new UWord;
	#endif
		Ctrl::EventLoop();

		Ctrl::EndSession();
	}

	LOG("Session Finished");
}
