#include "RichEdit.h"

namespace Upp {

bool DiaRichEdit::Key(dword key, int count)
{
	if(key == K_ENTER) {
		WhenEnter();
		return true;
	}
	if(key == K_ESCAPE) {
		WhenEsc();
		return true;
	}
	if(findarg(key, K_SHIFT|K_ENTER, K_CTRL|K_ENTER) >= 0)
		key = K_ENTER;
	
	return RichEdit::Key(key, count);
}

void DiagramEditor::SyncEditor()
{
	text_editor.AllowDarkContent(allow_dark_content);
	text_editor.DarkContent(dark_content);
	if(edit_text && cursor >= 0) {
		text_editor.SetEditable(); // to enable toolbar widgets
		text_editor.Floating(-Diagram::TextZoom().AsDouble() * GetZoom());
		Color p = CursorItem().paper;
		if(IsDarkContent())
			p = DarkThemeCached(p);
		text_editor.OverridePaper(p);
		text_editor.Show();
	}
	else {
		text_editor.Hide();
		text_editor.SetReadOnly(); // to disable toolbar widgets
		return;
	}
	SyncEditorRect();
}

void DiagramEditor::SyncEditorRect()
{
	Rectf r = CursorItem().GetTextEditRect();
	r = GetZoom() * r;
	r.Offset(-(Pointf)(Point)sb * GetZoom());
	Upp::Zoom z = Diagram::TextZoom();
	z.m = int(z.m * GetZoom());

	int cy = max(30, text_editor.Get().GetHeight(z, (int)r.GetWidth()) + DPI(10));
	r.top = r.CenterPoint().y - cy / 2;
	r.bottom = r.top + cy;
	r.left -= DPI(2);
	r.right += DPI(2) + ScrollBarSize();
	
	int q = GetSize().cx - (int)r.right;
	if(q < 0)
		r.Offset(q, 0);
	if(r.left < 0)
		r.left = 0;
	text_editor.SetRect(r);
}

void DiagramEditor::StartText()
{
	FinishText();
	edit_text = true;
	Sync();
	text_editor.SetFocus();
	text_editor.SetQTF("[= " + CursorItem().qtf);
	text_editor.Select(0, text_editor.GetLength());
	SyncEditorRect();
}

void DiagramEditor::FinishText()
{
	if(edit_text && cursor >= 0)
		CursorItem().qtf = AsQTF(text_editor.Get(), CHARSET_UTF8, QTF_BODY|QTF_NOCHARSET|QTF_NOLANG|QTF_NOSTYLES);
	edit_text = false;
	Sync();
}

}