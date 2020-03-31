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
	
    for(int i = 1; i < Font::GetFaceCount(); i++)
        if(Font::GetFaceInfo(i) & Font::SCALEABLE)
            face.Add(i, Font::GetFaceName(i));

	face <<= Font::SANSSERIF;
	height.MinMax(6, 500);
	
	for(int i = 4; i < 500; i += i < 16 ? 1 : i < 32 ? 4 : i < 48 ? 8 : 16)
		height.AddList(i);
	height <<= 128;
	
	svgpath.SetReadOnly();
	preview.SetFrame(ViewFrame());

	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		if(!dynamic_cast<Button *>(q))
			*q << [=] {
				Font fnt(~face, ~height);
				fnt.Bold(~bold);
				fnt.Italic(~italic);
			
				svgpath <<= preview.svgpath = TextToSvgPath(0, 0, (String)~text, fnt, ~singleline);
				preview.Refresh();
			 };
	
	copy.SetImage(CtrlImg::copy());
	copy << [=] {
		WriteClipboardText(preview.svgpath);
	};
	
	Sizeable().Zoomable();
}

GUI_APP_MAIN
{
	TextToSvgPathDlg().Run();
}
