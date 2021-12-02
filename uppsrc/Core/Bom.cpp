#include "Core.h"

namespace Upp {

bool Utf8BOM(Stream& in)
{
	int64 pos = in.GetPos();
	if(in.Get() == 0xef && in.Get() == 0xbb && in.Get() == 0xbf)
		return true;
	in.Seek(pos);
	return false;
}

static void sLoadBom(Stream& in, String *t, WString *wt, byte def_charset) 
{
	if(in.IsOpen()) {
		String s;
		if(in.GetLeft() > 3) {
			word header = in.Get16le();
			if(header == 0xfffe || header == 0xfeff) {
				int n = (int)in.GetLeft() / 2;
				WStringBuffer ws(n);
				ws.SetLength(in.Get(~ws, 2 * n) / 2);
				if(header == 0xfffe)
					EndianSwap((word *)~ws, ws.GetCount());
				if(wt)
					*wt = ws;
				else
					*t = FromUnicode(ws);
				return;
			}
			int c = in.Get();
			if(c < 0)
				return;
			byte *h = (byte *)&header;
			if(h[0] == 0xef && h[1] == 0xbb && c == 0xbf) {
				if(wt)
					*wt = ToUtf32(LoadStream(in));
				else
					*t = ToCharset(CHARSET_DEFAULT, LoadStream(in), CHARSET_UTF8);
				return;
			}
			s.Cat(h, 2);
			s.Cat(c);
		}
		s.Cat(LoadStream(in));
		if(wt)
			*wt = ToUnicode(s, def_charset);
		else
			*t = ToCharset(CHARSET_DEFAULT, s, def_charset);
		return;
	}
	return;
}

WString LoadStreamBOMW(Stream& in, byte def_charset)
{
	WString s = WString::GetVoid();
	sLoadBom(in, NULL, &s, def_charset);
	return s;
}

WString LoadStreamBOMW(Stream& in)
{
	return LoadStreamBOMW(in, GetLNGCharset(GetSystemLNG()));
}

String LoadStreamBOM(Stream& in, byte def_charset)
{
	String s = String::GetVoid();
	sLoadBom(in, &s, NULL, def_charset);
	return s;
}

String LoadStreamBOM(Stream& in)
{
	return LoadStreamBOM(in, GetLNGCharset(GetSystemLNG()));
}

WString LoadFileBOMW(const char *path, byte def_charset)
{
	FileIn in(path);
	return LoadStreamBOMW(in, def_charset);
}

WString LoadFileBOMW(const char *path)
{
	FileIn in(path);
	return LoadStreamBOMW(in);
}

String LoadFileBOM(const char *path, byte def_charset)
{
	FileIn in(path);
	return LoadStreamBOM(in, def_charset);
}

String LoadFileBOM(const char *path)
{
	FileIn in(path);
	return LoadStreamBOM(in);
}

bool SaveStreamBOM(Stream& out, const WString& data) {
	if(!out.IsOpen() || out.IsError()) 
		return false;
	word w = 0xfeff;
	out.Put(&w, 2);
	out.Put(~data, 2 * data.GetLength());
	out.Close();
	return out.IsOK();
}

bool SaveFileBOM(const char *path, const WString& data)
{
	FileOut out(path);
	return SaveStreamBOM(out, data);
}

bool SaveStreamBOMUtf8(Stream& out, const String& data) {
	if(!out.IsOpen() || out.IsError()) 
		return false;
	static unsigned char bom[] = {0xEF, 0xBB, 0xBF};
	out.Put(bom, 3);
	out.Put(ToCharset(CHARSET_UTF8, data));
	out.Close();
	return out.IsOK();
}

bool SaveFileBOMUtf8(const char *path, const String& data)
{
	FileOut out(path);
	return SaveStreamBOMUtf8(out, data);
}

}
