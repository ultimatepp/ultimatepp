#include "CtrlCore.h"
#include <plugin/bmp/bmp.h>

#ifdef GUI_WIN

namespace Upp {

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

bool DebugClipboard()
{
	static bool b = GetIniKey("DebugClipboard") == "1";
	return b;
}

void ClipboardLog(const char *txt)
{
	if(!DebugClipboard())
		return;
	FileAppend f(GetExeDirFile("clip.log"));
	f << GetSysTime() << ": " << txt << "\n";
}

void ClipboardError(const char *txt)
{
	if(!DebugClipboard())
		return;
	String s = txt;
	s << "\n" << GetLastErrorMessage();
	MessageBox(::GetActiveWindow(), s, "Clipboard error", MB_ICONSTOP | MB_OK | MB_APPLMODAL);
	ClipboardLog(String().Cat() << s << " ERROR");
}

String FromWin32CF(int cf);

void ClipboardError(const char *txt, int format)
{
	if(!DebugClipboard())
		return;
	ClipboardError(String().Cat() << txt << ' ' << FromWin32CF(format));
}

bool ClipboardOpen()
{
	// Win32 has serious race condition problem with clipboard; system or other apps open it
	// right after we close it thus blocking us to send more formats
	// So the solution is to wait and retry... (mirek, 2011-01-09)
	for(int i = 0; i < 200; i++) {
		if(OpenClipboard(utilityHWND)) {
			ClipboardLog("----- ClipboardOpen OK");
			return true;
		}
		Sleep(10);
	}
	ClipboardError("ClipboardOpen has failed!");
	return false;
}

void ClearClipboard()
{
	GuiLock __;
	sClipMap().Clear();
	ClipboardLog("* ClearClipboard");
	if(ClipboardOpen()) {
		if(!EmptyClipboard())
			ClipboardError("EmptyClipboard ERROR");
		if(!CloseClipboard())
			ClipboardError("CloseClipboard ERROR");
	}
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
			ClipboardError("GlobalAlloc ERROR");
			return;
		}
		if(!(ptr = (byte *)GlobalLock(handle))) {
			ClipboardError("GlobalLock ERROR");
			GlobalFree(handle);
			return;
		}
		memcpy(ptr, data, length);
		ptr[length] = 0;
		ptr[length + 1] = 0;
		GlobalUnlock(handle);
	}
	if(SetClipboardData(format, handle) != handle) {
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
	if(!ClipboardOpen())
		return Null;
	HGLOBAL hmem = GetClipboardData(GetClipboardFormatCode(format));
	if(hmem == 0) {
		ClipboardError("GetClipboardData failed");
		CloseClipboard();
		return Null;
	}
	const byte *src = (const byte *)GlobalLock(hmem);
	ASSERT(src);
	int length = (int)GlobalSize(hmem);
	if(length < 0) {
		ClipboardError("ReadCliboard length < 0");
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
	AppendClipboardUnicodeText(s.ToWString());
}

void AppendClipboardUnicodeText(const WString& s)
{
	Vector<char16> ws = ToUtf16(s);
	AppendClipboard("wtext", (const byte *)ws.begin(), 2 * ws.GetCount());
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
		return ToUtf8((const char16 *)~s, strlen16((const char16 *)~s));
	}
	if(clip.Accept("text"))
		return ~clip;
	return Null;
}

WString GetWString(PasteClip& clip)
{
	GuiLock __;
	if(clip.Accept("wtext")) {
		String s = ~clip;
		return ToUtf32((const char16 *)~s, strlen16((const char16 *)~s));
	}
	if(clip.Accept("text"))
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
	Vector<char16> h = ToUtf16(WString(data));
	return String((char *)h.begin(), sizeof(char16) * h.GetCount());
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
		return sWText(text);
	return Null;
}

String GetTextClip(const String& text, const String& fmt)
{
	if(fmt == "text")
		return text;
	if(fmt == "wtext")
		return sWText(text.ToWString());
	return Null;
}

String ReadClipboardText()
{
#ifdef PLATFORM_WINCE
	return ReadClipboardUnicodeText().ToString();
#else
	String s = ReadClipboardUnicodeText().ToString();
	if(s.GetCount())
		return s;
	s = ReadClipboard((const char *)CF_TEXT);
	return String(s, (int)strlen(~s));
#endif
}

WString ReadClipboardUnicodeText()
{
	String s = ReadClipboard((const char *)CF_UNICODETEXT);
	return ToUtf32((const char16 *)~s, strlen16((const char16 *)~s));
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
		BITMAPINFO *lpBI = (BITMAPINFO *)~data;
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
	Size sz = img.GetSize();
	header.biSize = sizeof(header);
	header.biWidth = sz.cx;
	header.biHeight = sz.cy;
	header.biBitCount = 32;
	header.biPlanes = 1;
	header.biCompression = BI_RGB;
	StringBuffer b(int(sizeof(header) + 4 * img.GetLength()));
	byte *p = (byte *)~b;
	memcpy(p, &header, sizeof(header));
	p += sizeof(header);
	for(int i = 0; i < sz.cy; i++)
		memcpy(4 * i * sz.cx + p, img[sz.cy - 1 - i], 4 * sz.cx);
	return String(b);
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

void Append(VectorMap<String, ClipData>& data, const Image& img)
{
	data.Add(ClipFmt<Image>(), ClipData(img, sImage));
	data.Add("dib", ClipData(img, sDib));
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
    POINT pt;
    BOOL  nc;
    BOOL  unicode;
};

Vector<String> GetClipFiles(const String& data)
{
	GuiLock __;
	Vector<String> f;
	if((unsigned)data.GetCount() < sizeof(sDROPFILES) + 2)
		return f;
	const sDROPFILES *df = (const sDROPFILES *)~data;
	const char *s = ((const char *)df + df->offset);
	if(df->unicode) {
		const char16 *ws = (char16 *)s;
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

Vector<String> GetFiles(PasteClip& clip)
{
	GuiLock __;
	Vector<String> f;
	return GetClipFiles(clip.Get("files"));
}

void AppendFiles(VectorMap<String, ClipData>& clip, const Vector<String>& files)
{
	WString wfiles32;
	for(int i = 0; i < files.GetCount(); i++)
		wfiles32 << files[i].ToWString() << (wchar)0;
	Vector<char16> wfiles = ToUtf16(wfiles32);
	wfiles.Add(0);
	sDROPFILES h;
	h.unicode = true;
	h.offset = sizeof(h);
    GetCursorPos(&h.pt);
    h.nc = TRUE;
    String data;
	data.Cat((byte *)&h, sizeof(h));
	data.Cat((byte *)wfiles.begin(), sizeof(char16) * (wfiles.GetCount() + 1));
	clip.GetAdd("files") = ClipData(data);
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

}

#endif
