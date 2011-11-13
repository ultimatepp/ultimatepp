#include "Core.h"

NAMESPACE_UPP

static void sLoadBom(Stream& in, String *t, WString *wt) 
{
	if(in.IsOpen()) {
		String s;
		if(in.GetLeft() > 3) {
			word header = in.Get16();
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
					*wt = FromUtf8(LoadStream(in));
				else
					*t = ToCharset(CHARSET_DEFAULT, LoadStream(in), CHARSET_UTF8);
				return;
			}
			s.Cat(h, 2);
			s.Cat(c);
		}
		s.Cat(LoadStream(in));
		if(wt)
			*wt = ToUnicode(s, GetLNGCharset(GetSystemLNG()));
		else
			*t = ToCharset(CHARSET_DEFAULT, s, GetLNGCharset(GetSystemLNG()));
		return;
	}
	return;
}

WString LoadStreamBOMW(Stream& in)
{
	WString s = WString::GetVoid();
	sLoadBom(in, NULL, &s);
	return s;
}

String LoadStreamBOM(Stream& in)
{
	String s = String::GetVoid();
	sLoadBom(in, &s, NULL);
	return s;
}

WString LoadFileBOMW(const char *path)
{
	FileIn in(path);
	return LoadStreamBOMW(in);
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

END_UPP_NAMESPACE
