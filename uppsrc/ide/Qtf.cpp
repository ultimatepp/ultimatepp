#include "ide.h"

INITBLOCK {
	RegisterGlobalConfig("QTF-designer");
	RegisterGlobalConfig("QTF-designer-editor");
}

class QtfDlg : public TopWindow {
	void Text();
	void Clear();
	void Copy();
	void Editor();
	void OnHelpLink(const String& link);

public:
	Splitter leftSplit, mainSplit;
	WithQtfLayout<ParentCtrl> qtfText;
	RichTextCtrl qtf;
	RichTextView help;

	void Serialize(Stream& s);

	QtfDlg();
};

void QtfDlg::Text()
{
	qtf <<= ~qtfText.text;
}

void QtfDlg::Clear()
{
	qtfText.text.Clear();
	Text();
}

void QtfDlg::Copy()
{
	WriteClipboardText(~qtfText.text);
	Break();
}

void QtfDlg::Editor()
{
	String text = (String)~qtfText.text;
	QTFEdit(text);
	if(PromptYesNo("Use the text?")) {
		qtfText.text <<= text;
		Text();
	}
}

void QtfDlg::OnHelpLink(const String& link)
{
	int q = link.ReverseFind("#");
	if(q >= 0)
		help.GotoLabel(link.Mid(q + 1));
}

QtfDlg::QtfDlg()
{
	Title("QTF designer");
	CtrlLayout(qtfText);
	qtfText.text << [=] { Text(); };
	qtf.SetFrame(ViewFrame());
	qtf.Background(SColorPaper);
	Sizeable().Zoomable();
	Rect r = GetWorkArea();
	SetRect(0, 0, r.GetWidth() - 100, r.GetHeight() - 200);
	SetMinSize(Size(min(640, r.GetWidth() - 100), min(480, r.GetHeight() - 200)));

	help.Margins(Rect(12, 0, 12, 0));
	String path = AppendFileName(AppendFileName(PackageDirectory("RichText"), "srcdoc.tpp"), "QTF_en-us.tpp");
	if(FileExists(path)) {
		help.SetQTF(ReadTopic(LoadFile(path)).text);
	}
	else {
		help <<= "[= &&&QTF documentation not found";
	}
	help.WhenLink = [=] (const String& link) { OnHelpLink(link); };

	qtfText.clear << [=] { Clear(); };
	qtfText.copy << [=] { Copy(); };
	qtfText.editor << [=] { Editor(); };

	leftSplit.Vert(qtfText, qtf).SetPos(4000);
	mainSplit.Horz(leftSplit, help).SetPos(3500);
	Add(mainSplit.HSizePosZ(4, 4).VSizePosZ(4, 4));
}

void QtfDlg::Serialize(Stream& s)
{
	int version = 1;
	s / version;
	s % qtfText.text;
	SerializePlacement(s);
	if(version >= 1) {
		leftSplit.Serialize(s);
		mainSplit.Serialize(s);
	}
	Text();
}

void Ide::Qtf()
{
	QtfDlg dlg;
	LoadFromGlobal(dlg, "QTF-designer");
	int l, h;
	bool sel = editor.GetSelection(l,h);
	if(qtfsel && sel) {
		dlg.qtfText.text <<= (~editor).ToString().Mid(l, h - l);
		dlg.qtfText.copy.SetLabel("Apply and close");
		dlg.Run();
		editor.Remove(l, h - l);
		editor.Insert(l, (~dlg.qtfText.text).ToString());
	}
	else {
		dlg.qtfText.copy.SetLabel("Copy and close");
		dlg.Run();
	}
	StoreToGlobal(dlg, "QTF-designer");
}
