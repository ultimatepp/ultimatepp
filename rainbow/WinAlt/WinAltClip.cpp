#include <CtrlCore/CtrlCore.h>
#include <plugin/bmp/bmp.h>

#ifdef GUI_WINALT

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

VectorMap<int, ClipData>& sClipMap()
{
	static VectorMap<int, ClipData> x;
	return x;
}

extern HWND utilityHWND;

int  GetClipboardFormatCode(const char *format_id)
{
	GuiLock ___;
	int x = (int)(intptr_t)format_id;
	if(x >= 0 && x < 65535)
		return x;
	String fmt = format_id;
	if(fmt == "text")
		return CF_TEXT;
	if(fmt == "wtext")
		return CF_UNICODETEXT;
	if(fmt == "dib")
		return CF_DIB;
	if(fmt == "files")
		return CF_HDROP;
	static StaticMutex m;
	Mutex::Lock __(m);
	static VectorMap<String, int> format_map;
	int f = format_map.Find(format_id);
	if(f < 0) {
		f = format_map.GetCount();
		format_map.Add(format_id,
#ifdef PLATFORM_WINCE
			::RegisterClipboardFormat(ToSystemCharset(format_id))
#else
			::RegisterClipboardFormat(format_id)
#endif
		);
	}
	return format_map[f];
}

void ClipboardLog(const char *txt)
{
#ifdef flagCHECKCLIPBOARD
	FileAppend f(GetExeDirFile("clip.log"));
	f << txt << "\n";
#endif
}

void ClipboardError(const char *txt)
{
#ifdef flagCHECKCLIPBOARD
	String s = txt;
	s << "\n" << GetLastErrorMessage();
	MessageBox(::GetActiveWindow(), s, "Clipboard error", MB_ICONSTOP | MB_OK | MB_APPLMODAL);
	ClipboardLog(String().Cat() << s << " ERROR");
#endif
}

String FromWin32CF(int cf);

void ClipboardError(const char *txt, int format)
{
#ifdef flagCHECKCLIPBOARD
	ClipboardError(String().Cat() << txt << ' ' << FromWin32CF(format));
#endif
}

bool ClipboardOpen()
{
	// Win32 has serious race condition problem with clipboard; system or other apps open it
	// right after we close it thus blocking us to send more formats
	// So the solution is to wait and retry... (mirek, 2011-01-09)
	int delay = 5;
	for(int i = 0; i < 5; i++) {
		if(OpenClipboard(utilityHWND))
			return true;
		Sleep(delay += delay);
	}
	return false;
}

void ClearClipboard()
{
	GuiLock __;
	sClipMap().Clear();
#ifdef flagCHECKCLIPBOARD
	DeleteFile(GetExeDirFile("clip.log"));
#endif
	ClipboardLog("* ClearClipboard");
	if(ClipboardOpen()) {
		if(!EmptyClipboard())
			ClipboardError("EmptyClipboard ERROR");
		if(!CloseClipboard())
			ClipboardError("CloseClipboard ERROR");
	}
#ifdef flagCHECKCLIPBOARD
	else {
		ClipboardError("OpenClipboard ERROR");
	}
#endif
}

void SetClipboardRaw(int format, const byte *data, int length)
{
	GuiLock __;
	HANDLE handle = NULL;
	ClipboardLog("* SetClipboardRaw");
	if(data) {
		handle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, length + 2);
		byte *ptr;
		if(!handle) {
			ClipboardLog("GlobalAlloc ERROR");
			return;
		}
		if(!(ptr = (byte *)GlobalLock(handle))) {
			ClipboardLog("GlobalLock ERROR");
			GlobalFree(handle);
			return;
		}
		memcpy(ptr, data, length);
		ptr[length] = 0;
		ptr[length + 1] = 0;
		GlobalUnlock(handle);
	}
	if(!SetClipboardData(format, handle)) {
		ClipboardError("SetCliboardData", format);
		LLOG("SetClipboardData error: " << GetLastErrorMessage());
		GlobalFree(handle);
	}
}

void AppendClipboard(int format, const byte *data, int length)
{
	GuiLock __;
	ClipboardLog("* AppendClipboard");
	if(ClipboardOpen()) {
		SetClipboardRaw(format, data, length);
		if(!CloseClipboard())
			ClipboardError("CloseClipboard", format);
	}
	else
		ClipboardError("OpenClipboard", format);
}

void AppendClipboard(const char *format, const byte *data, int length)
{
	GuiLock __;
	Vector<String> f = Split(format, ';');
	for(int i = 0; i < f.GetCount(); i++)
		AppendClipboard(GetClipboardFormatCode(f[i]), data, length);
}

void AppendClipboard(const char *format, const String& data)
{
	GuiLock __;
	AppendClipboard(format, data, data.GetLength());
}

void AppendClipboard(const char *format, const Value& data, String (*render)(const Value&))
{
	GuiLock __;
	Vector<String> f = Split(format, ';');
	for(int i = 0; i < f.GetCount(); i++) {
		int c = GetClipboardFormatCode(f[i]);
		sClipMap().GetAdd(c) = ClipData(data, render);
		AppendClipboard(c, NULL, 0);
	}
}

void Ctrl::RenderFormat(int format)
{
	GuiLock __;
	int q = sClipMap().Find(format);
	if(q >= 0) {
		String s = sClipMap()[q].Render();
		SetClipboardRaw(format, s, s.GetLength());
	}
}

void Ctrl::RenderAllFormats()
{
	GuiLock __;
	if(sClipMap().GetCount() && OpenClipboard(utilityHWND)) {
		for(int i = 0; i < sClipMap().GetCount(); i++)
			RenderFormat(sClipMap().GetKey(i));
		CloseClipboard();
	}
}

void Ctrl::DestroyClipboard()
{
	GuiLock __;
	sClipMap().Clear();
}

String ReadClipboard(const char *format)
{
	GuiLock __;
	if(!OpenClipboard(NULL))
		return Null;
	HGLOBAL hmem = GetClipboardData(GetClipboardFormatCode(format));
	if(hmem == 0) {
		CloseClipboard();
		return Null;
	}
	const byte *src = (const byte *)GlobalLock(hmem);
	ASSERT(src);
	int length = (int)GlobalSize(hmem);
	if(length < 0) {
		CloseClipboard();
		return Null;
	}
	String out(src, length);
	GlobalUnlock(hmem);
	CloseClipboard();
	return out;
}

void AppendClipboardText(const String& s)
{
#ifdef PLATFORM_WINCE
	AppendClipboardUnicodeText(s.ToWString());
#else
	AppendClipboard("text", ToSystemCharset(s));
#endif
}

void AppendClipboardUnicodeText(const WString& s)
{
#ifndef PLATFORM_WINCE
	AppendClipboardText(s.ToString());
#endif
	AppendClipboard("wtext", (byte *)~s, 2 * s.GetLength());
}

const char *ClipFmtsText()
{
	return "wtext;text";
}

String GetString(PasteClip& clip)
{
	GuiLock __;
	if(clip.Accept("wtext")) {
		String s = ~clip;
		return WString((const wchar *)~s, wstrlen((const wchar *)~s)).ToString();
	}
	if(clip.IsAvailable("text"))
		return ~clip;
	return Null;
}

WString GetWString(PasteClip& clip)
{
	GuiLock __;
	if(clip.Accept("wtext")) {
		String s = ~clip;
		return WString((const wchar *)~s, wstrlen((const wchar *)~s));
	}
	if(clip.IsAvailable("text"))
		return (~clip).ToWString();
	return Null;
}


bool AcceptText(PasteClip& clip)
{
	return clip.Accept(ClipFmtsText());
}

static String sText(const Value& data)
{
	return data;
}

static String sWText(const Value& data)
{
	return Unicode__(WString(data));
}

void Append(VectorMap<String, ClipData>& data, const String& text)
{
	data.GetAdd("text", ClipData(text, sText));
	data.GetAdd("wtext", ClipData(text, sWText));
}

void Append(VectorMap<String, ClipData>& data, const WString& text)
{
	data.GetAdd("text", ClipData(text, sText));
	data.GetAdd("wtext", ClipData(text, sWText));
}

String GetTextClip(const WString& text, const String& fmt)
{
	if(fmt == "text")
		return text.ToString();
	if(fmt == "wtext")
		return Unicode__(text);
	return Null;
}

String GetTextClip(const String& text, const String& fmt)
{
	if(fmt == "text")
		return text;
	if(fmt == "wtext")
		return Unicode__(text.ToWString());
	return Null;
}

String ReadClipboardText()
{
#ifdef PLATFORM_WINCE
	return ReadClipboardUnicodeText().ToString();
#else
	String s = ReadClipboard((const char *)CF_TEXT);
	return String(s, (int)strlen(~s));
#endif
}

WString ReadClipboardUnicodeText()
{
	String s = ReadClipboard((const char *)CF_UNICODETEXT);
	return WString((const wchar *)~s, wstrlen((const wchar *)~s));
}

bool IsClipboardAvailable(const char *id)
{
	return ::IsClipboardFormatAvailable(GetClipboardFormatCode(id));
}

bool IsClipboardAvailableText()
{
	return IsClipboardAvailable((const char *)CF_TEXT);
}

const char *ClipFmtsImage()
{
	static const char *q;
	ONCELOCK {
		static String s = "dib;" + ClipFmt<Image>();
		q = s;
	}
	return q;
}

bool AcceptImage(PasteClip& clip)
{
	GuiLock __;
	return clip.Accept(ClipFmtsImage());
}

Image GetImage(PasteClip& clip)
{
	GuiLock __;
	Image m;
	if(Accept<Image>(clip)) {
		LoadFromString(m, ~clip);
		if(!m.IsEmpty())
			return m;
	}
	if(clip.Accept("dib")) {
		String data = ~clip;
		if((unsigned)data.GetCount() < sizeof(BITMAPINFO)) return Null;
		BITMAPINFO *lpBI = 	(BITMAPINFO *)~data;
		BITMAPINFOHEADER& hdr = lpBI->bmiHeader;
		byte *bits = (byte *)lpBI + hdr.biSize;
		if(hdr.biBitCount <= 8)
			bits += (hdr.biClrUsed ? hdr.biClrUsed : 1 << hdr.biBitCount) * sizeof(RGBQUAD);
		if(hdr.biBitCount >= 16 || hdr.biBitCount == 32) {
			if(hdr.biCompression == 3)
				bits += 12;
			if(hdr.biClrUsed != 0)
				bits += hdr.biClrUsed * sizeof(RGBQUAD);
		}
		int h = abs((int)hdr.biHeight);
		ImageDraw   iw(hdr.biWidth, h);
		::StretchDIBits(iw.GetHandle(),
			0, 0, hdr.biWidth, h,
			0, 0, hdr.biWidth, h,
			bits, lpBI, DIB_RGB_COLORS, SRCCOPY);
		return iw;
	}
	return Null;
}

Image ReadClipboardImage()
{
	GuiLock __;
	PasteClip d = Ctrl::Clipboard();
	return GetImage(d);
}

String sDib(const Value& image)
{
	Image img = image;
	BITMAPINFOHEADER header;
	Zero(header);
	header.biSize = sizeof(header);
	header.biWidth = img.GetWidth();
	header.biHeight = -img.GetHeight();
	header.biBitCount = 32;
	header.biPlanes = 1;
	header.biCompression = BI_RGB;
	StringBuffer b(sizeof(header) + 4 * img.GetLength());
	byte *p = (byte *)~b;
	memcpy(p, &header, sizeof(header));
	memcpy(p + sizeof(header), ~img, 4 * img.GetLength());
	return b;
}

String sImage(const Value& image)
{
	Image img = image;
	return StoreAsString(const_cast<Image&>(img));
}

String GetImageClip(const Image& img, const String& fmt)
{
	GuiLock __;
	if(img.IsEmpty()) return Null;
	if(fmt == "dib")
		return sDib(img);
	if(fmt == ClipFmt<Image>())
		return sImage(img);
	return Null;
}

void AppendClipboardImage(const Image& img)
{
	GuiLock __;
	if(img.IsEmpty()) return;
	AppendClipboard(ClipFmt<Image>(), img, sImage);
	AppendClipboard("dib", img, sDib);
}

bool AcceptFiles(PasteClip& clip)
{
	if(clip.Accept("files")) {
		clip.SetAction(DND_COPY);
		return true;
	}
	return false;
}

bool IsAvailableFiles(PasteClip& clip)
{
	return clip.IsAvailable("files");
}

struct sDROPFILES {
    DWORD offset;
    POINT dummy;
    BOOL  dummy2;
    BOOL  unicode;
};

Vector<String> GetFiles(PasteClip& clip)
{
	GuiLock __;
	Vector<String> f;
	String data = clip;
	if((unsigned)data.GetCount() < sizeof(sDROPFILES) + 2)
		return f;
	const sDROPFILES *df = (const sDROPFILES *)~data;
	const char *s = ((const char *)df + df->offset);
	if(df->unicode) {
		const wchar *ws = (wchar *)s;
		while(*ws) {
			WString fn;
			while(*ws)
				fn.Cat(*ws++);
			f.Add(fn.ToString());
			ws++;
		}
	}
	else
		while(*s) {
			String fn;
			while(*s)
				fn.Cat(*s++);
			f.Add(fn.ToString());
			s++;
		}
	return f;
}

bool   Has(UDropTarget *dt, const char *fmt);
String Get(UDropTarget *dt, const char *fmt);

bool PasteClip::IsAvailable(const char *fmt) const
{
	if(this == &Ctrl::Selection())
		return false;
	return dt ? UPP::Has(dt, fmt) : IsClipboardAvailable(fmt);
}

String PasteClip::Get(const char *fmt) const
{
	if(this == &Ctrl::Selection())
		return Null;
	return dt ? UPP::Get(dt, fmt) : ReadClipboard(fmt);
}

void PasteClip::GuiPlatformConstruct()
{
	dt = NULL;
}

END_UPP_NAMESPACE

#endif
