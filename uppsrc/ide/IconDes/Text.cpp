#include "IconDes.h"

IconDes::TextDlg::TextDlg()
{
	CtrlLayout(*this, "Text");
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		if(!dynamic_cast<Button *>(q))
			q->WhenAction = Proxy(WhenAction);
	
	face.Add(Font::ARIAL);
	face.Add(Font::ROMAN);
	face.Add(Font::COURIER);
	for(int i = Font::COURIER + 1; i < Font::GetFaceCount(); i++)
		if(Font::GetFaceInfo(i) & Font::SCALEABLE)
			face.Add(i);
	SetupFaceList(face);
	face <<= Font::ARIAL;
	height.MinMax(4, 2000);
	height <<= 12;
	for(int i = 4; i < 300; i += i < 16 ? 1 : i < 32 ? 4 : i < 48 ? 8 : 16)
		height.AddList(i);
}

Font IconDes::TextDlg::GetFont()
{
	Font fnt(~face, ~height);
	fnt.Bold(~bold);
	fnt.Italic(~italic);
	fnt.Underline(~underline);
	fnt.Strikeout(~strikeout);
	fnt.NonAntiAliased(~nonaa);
	return fnt;
}

void IconDes::CloseText()
{
	if(textdlg.IsOpen())
		textdlg.Close();
	SetBar();
}

void IconDes::PasteText()
{
	if(!IsCurrent() || !IsPasting() || !textdlg.IsOpen())
		return;
	WString text = ~textdlg.text;
	Font font = textdlg.GetFont();
	Size tsz = GetTextSize(text, font);
	tsz.cx += tsz.cy / 3;
	ImageDraw iw(tsz);
	iw.Alpha().DrawText(0, 0, text, font, GrayColor(CurrentColor().a));
	iw.DrawRect(tsz, CurrentColor());
#ifdef PLATFORM_WIN32 // non-antialiased works
	Current().paste_image = iw;
#else
	if(!font.IsNonAntiAliased())
		Current().paste_image = iw;
	else {
		Image h = iw;
		ImageBuffer ib(h);
		RGBA cc = CurrentColor();
		for(RGBA& c : ib)
			c = c.a < 128 ? RGBAZero() : cc;
		Current().paste_image = ib;
	}
#endif
	MakePaste();
}

void IconDes::Text()
{
	if(textdlg.IsOpen())
		textdlg.Close();
	else {
		ONCELOCK {
			Rect r = GetScreenView();
			Size sz = textdlg.GetLayoutSize();
			r.left = r.left + (r.GetWidth() - sz.cx) / 2;
			r.right = r.left + sz.cx;
			r.top = r.bottom - sz.cy;
			textdlg.NoCenter().SetRect(r);
		}
		Paste(CreateImage(Size(1, 1), RGBAZero()));
		textdlg.nonaa = Current().flags & IML_IMAGE_FLAG_S3;
		textdlg.WhenClose = THISBACK(CloseText);
		textdlg <<= THISBACK(PasteText);
		textdlg.Open();
		PasteText();
	}
	SetBar();
}
