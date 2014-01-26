#ifndef _WebWord_WebWord_h_
#define _WebWord_WebWord_h_

#include <RichEdit/RichEdit.h>
#include <PdfDraw/PdfDraw.h>

using namespace Upp;

struct UWord : public TopWindow {
public:
	virtual void DragAndDrop(Point, PasteClip& d);
	virtual void FrameDragAndDrop(Point, PasteClip& d);
	
	virtual void ShutdownWindow();

	RichEdit   editor;
	MenuBar    menubar;
	ToolBar    toolbar;
	StatusBar  statusbar;
	String     filename;
	
	int64      sent_prev;

	static LRUList& lrufile() { static LRUList l; return l; }

	void Load(const String& filename);
	void OpenFile(const String& fn);
	void New();
	void Open();
	void Save0();
	void Save();
	void SaveAs();
	void Print();
	void Pdf();
	void About();
	void Destroy(bool shutdown);
	void SetBar();
	void FileBar(Bar& bar);
	void AboutMenu(Bar& bar);
	void MainMenu(Bar& bar);
	void MainBar(Bar& bar);
	
	void ShowInfo();

public:
	typedef UWord CLASSNAME;

	static void SerializeApp(Stream& s);

	UWord();
};

#endif
