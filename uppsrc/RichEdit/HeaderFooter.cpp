#include "RichEdit.h"

NAMESPACE_UPP

struct RichEditHdrFtr : public RichEdit {
	ToolBar  toolbar;

	void RefreshBar();
	void TheBar(Bar& bar);
	
	void  PageNumber();
	void  PageCount();
	
	void  Init(int pageno, int pagecount);

	typedef RichEditHdrFtr CLASSNAME;

	RichEditHdrFtr();
};

void RichEditHdrFtr::TheBar(Bar& bar)
{
	EditTools(bar);
	bar.Gap();
	FontTools(bar);
	bar.Gap();
	InkTool(bar);
	PaperTool(bar);
	bar.Gap();
	LanguageTool(bar);
	SpellCheckTool(bar);
	bar.Break();
	StyleTool(bar);
	bar.Gap();
	ParaTools(bar);
	bar.Gap();
	TableTools(bar);
	bar.Gap();
	bar.Add(!IsReadOnly(), t_("Insert page number"), RichEditImg::PageNumber(), THISBACK(PageNumber));
	bar.Add(!IsReadOnly(), t_("Insert page count"), RichEditImg::PageCount(), THISBACK(PageCount));
}

void RichEditHdrFtr::PageNumber()
{
	PasteText(ParseQTF("{:VALUE:PAGENUMBER:}"));
	EvaluateFields();
}

void RichEditHdrFtr::PageCount()
{
	PasteText(ParseQTF("{:VALUE:PAGECOUNT:}"));
	EvaluateFields();
}

void RichEditHdrFtr::RefreshBar()
{
	toolbar.Set(THISBACK(TheBar));
}

RichEditHdrFtr::RichEditHdrFtr()
{
	InsertFrame(0, toolbar);
	WhenRefreshBar = callback(this, &RichEditHdrFtr::RefreshBar);
	SetVar("PAGECOUNT", "###");
	SetVar("PAGENUMBER", "#");
	SetVar("__DISPLAY_VALUE_FIELDS", 1);
}

struct HeaderFooterDlg : WithHeaderFooterLayout<TopWindow> {
	typedef HeaderFooterDlg CLASSNAME;
	
	RichEditHdrFtr header_editor, footer_editor;
	
	void Sync();
	void Load(const RichText& text);
	void Save(RichText& text);

	HeaderFooterDlg();
};

void HeaderFooterDlg::Sync()
{
	header_editor.Enable(use_header);
	footer_editor.Enable(use_footer);
}

HeaderFooterDlg::HeaderFooterDlg()
{
	CtrlLayoutOKCancel(*this, "Header / Footer");
	use_header <<= use_footer <<= THISBACK(Sync);
	Sync();
}

void HeaderFooterDlg::Load(const RichText& text)
{
	String h = text.GetHeaderQtf();
	use_header = !IsNull(h);
	if(use_header)
		header_editor.SetQTF(h);
	header_editor.EvaluateFields();
	h = text.GetFooterQtf();
	use_footer = !IsNull(h);
	if(use_footer)
		footer_editor.SetQTF(h);
	footer_editor.EvaluateFields();
	Sync();
}

void HeaderFooterDlg::Save(RichText& text)
{
	if(use_header)
		text.SetHeaderQtf(header_editor.GetQTF());
	else
		text.ClearHeader();
	if(use_footer)
		text.SetFooterQtf(footer_editor.GetQTF());
	else
		text.ClearFooter();
}

void RichEdit::HeaderFooter()
{
	HeaderFooterDlg dlg;
	dlg.Load(text);
	dlg.SetRect(0, 0, GetSize().cx, dlg.GetLayoutSize().cy);
	if(dlg.Execute() == IDOK) {
		dlg.Save(text);
		Refresh();
		Finish();
	}
}

END_UPP_NAMESPACE