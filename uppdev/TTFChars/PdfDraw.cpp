#include <PdfDraw/PdfDraw.h>
#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	for(int i = 0; i < Font::GetFaceCount(); i++) {
		Font fnt(i, 0);
		FontInfo fi = fnt.Info();
		String fn = fi.GetFileName();
		String ext = ToLower(GetFileExt(fn));
		if(ext == ".ttf" || ext == ".otf") {
			String data = LoadFile(fn);
			TTFReader ttf;
			if(ttf.Open(data, false, true)) {
				LLOG(Font::GetFaceName(i));
				
			}
		}
	}
/*
			FontInfo fi = pdffont.GetKey(i).Info();
			String fontbuffer = LoadFile(fi.GetFileName());

			TTFReader ttf;
			if(!ttf.Open(fontbuffer))
				return Null;*/
}
