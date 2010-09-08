#include "ide.h"

INITBLOCK {
	RegisterGlobalConfig("QTF-designer");
	RegisterGlobalConfig("QTF-designer-editor");
}

class QtfDlg : public WithQtfLayout<TopWindow> {
	typedef QtfDlg CLASSNAME;

	void Text();
	void Clear();
	void Copy();
	void Editor();

public:
	void Serialize(Stream& s);

	QtfDlg();
};

void QtfDlg::Text()
{
	qtf <<= ~text;
}

void QtfDlg::Clear()
{
	text.Clear();
	Text();
}

void QtfDlg::Copy()
{
	WriteClipboardText(~text);
	Break();
}

struct QtfDlgEditor : TopWindow {
	RichEditWithToolBar editor;

	void Serialize(Stream& s);	
	
	QtfDlgEditor();
};

void QtfDlgEditor::Serialize(Stream& s)
{
	SerializePlacement(s);
}

QtfDlgEditor::QtfDlgEditor()
{
	Add(editor.SizePos());
	Rect r = GetWorkArea();
	SetRect(0, 0, r.GetWidth() - 100, r.GetHeight() - 100);
}

void QtfDlg::Editor()
{
	QtfDlgEditor dlg;
	LoadFromGlobal(dlg, "QTF-designer-editor");
	dlg.editor.SetQTF((String)~text);
	dlg.Run();
	if(PromptYesNo("Use the text?")) {
		text <<= AsQTF(dlg.editor.Get(),
		               CHARSET_UTF8, QTF_BODY|QTF_NOSTYLES|QTF_NOCHARSET|QTF_NOLANG);
		Text();
	}
	StoreToGlobal(dlg, "QTF-designer-editor");
}

QtfDlg::QtfDlg()
{
	CtrlLayout(*this, "QTF designer");
	text <<= THISBACK(Text);
	qtf.SetFrame(ViewFrame());
	qtf.Background(SColorPaper);
	Sizeable().Zoomable();
	Rect r = GetWorkArea();
	SetRect(0, 0, r.GetWidth() - 100, r.GetHeight() - 200);

	String path = AppendFileName(AppendFileName(PackageDirectory("RichText"), "srcdoc.tpp"), "QTF$en-us.tpp");
	if(FileExists(path))
		help.SetQTF(ReadTopic(LoadFile(path)).text);
	else
		help <<= "[= &&&QTF documentation not found";
	
	clear <<= THISBACK(Clear);
	copy <<= THISBACK(Copy);
	editor <<= THISBACK(Editor);
}

void QtfDlg::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	s % text;
	SerializePlacement(s);
	Text();
}

void Ide::Qtf()
{
	QtfDlg dlg;
	LoadFromGlobal(dlg, "QTF-designer");
	int l,h;
	bool sel=editor.GetSelection(l,h);
	if(qtfsel && sel){
		dlg.text<<=(~editor).ToString().Mid(l,h-l);
		dlg.copy.SetLabel("Apply and close");
		dlg.Run();
		editor.Remove(l,h-l);
		editor.Insert(l,(~dlg.text).ToString());
	}
	else{
		dlg.copy.SetLabel("Copy and close");
		dlg.Run();
	}
	StoreToGlobal(dlg, "QTF-designer");
}
