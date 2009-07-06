#include "CtrlCore.h"

#include <plugin/bmp/bmp.h>

NAMESPACE_UPP

#ifdef PLATFORM_X11


const char *ClipFmtsImage()
{
	static const char *q;
	ONCELOCK {
		static String s(ClipFmt<Image>() + ";image/png");
		q = s;
	}
	return q;
}

bool AcceptImage(PasteClip& clip)
{
	return clip.Accept(ClipFmt<Image>()) || clip.Accept("image/png");
}

Image GetImage(PasteClip& clip)
{
	Image m;
	if(Accept<Image>(clip)) {
		LoadFromString(m, ~clip);
		if(!m.IsEmpty())
			return m;
	}
	if(clip.Accept("image/png"))
		return StreamRaster::LoadStringAny(~clip);
	return Null;
}

Image ReadClipboardImage()
{
	return GetImage(Ctrl::Clipboard());
}

static String sBmp(const Value& data)
{
	Image img = data;
	return BMPEncoder().SaveString(img);
}

static String sImg(const Value& data)
{
	Image img = data;
	return StoreAsString(const_cast<Image&>(img));
}

String GetImageClip(const Image& img, const String& fmt)
{
	if(img.IsEmpty())
		return Null;
	if(fmt == "image/bmp")
		return BMPEncoder().SaveString(img);
	if(fmt == ClipFmt<Image>())
		return StoreAsString(const_cast<Image&>(img));
	return Null;
}

void AppendClipboardImage(const Image& img)
{
	if(img.IsEmpty()) return;
	AppendClipboard(ClipFmt<Image>(), img, sImg);
	AppendClipboard("image/bmp", img, sBmp);
}

#endif

END_UPP_NAMESPACE
