#include "TextToSvgPath.h"

void Preview::Paint(Draw& w)
{
	DrawPainter sw(w, GetSize());
	sw.Clear(SWhite());
	sw.Path(svgpath).Fill(SBlack());
}

TextToSvgPathDlg::TextToSvgPathDlg()
{
	CtrlLayout(*this, "Text to SVG path converter");
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		if(!dynamic_cast<Button *>(q))
			q->WhenAction = [=] { Render(); };
	
	face.Add(Font::ARIAL);
	face.Add(Font::ROMAN);
	face.Add(Font::COURIER);
	for(int i = Font::COURIER + 1; i < Font::GetFaceCount(); i++)
		if(Font::GetFaceInfo(i) & Font::SCALEABLE)
			face.Add(i);
	SetupFaceList(face);
	face <<= Font::ARIAL;
	height.MinMax(6, 500);
	
	for(int i = 4; i < 500; i += i < 16 ? 1 : i < 32 ? 4 : i < 48 ? 8 : 16)
		height.AddList(i);
	height <<= 128;
	
	svgpath.SetReadOnly();
	preview.SetFrame(ViewFrame());
	
	copy.SetImage(CtrlImg::copy());
	copy << [=] {
		WriteClipboardText(preview.svgpath);
	};
}

void TextToSvgPathDlg::Render()
{
	Font fnt(~face, ~height);
	fnt.Bold(~bold);
	fnt.Italic(~italic);
	fnt.Underline(~underline);
	fnt.Strikeout(~strikeout);
	fnt.NonAntiAliased(~nonaa);

	svgpath <<= preview.svgpath = TextToSvgPath(0, 0, (String)~text, fnt, ~singleline);
	preview.Refresh();
}

GUI_APP_MAIN
{
	TextToSvgPathDlg().Run();
}
