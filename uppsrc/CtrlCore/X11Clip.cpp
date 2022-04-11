#include "CtrlCore.h"

#ifdef GUI_X11

namespace Upp {

#define LLOG(x)  // LOG(x)

Index<String> Ctrl::sel_formats;
Ptr<Ctrl>     Ctrl::sel_ctrl;

void Ctrl::SetSelectionSource(const char *fmts)
{
	GuiLock __;
	LLOG("SetSelectionSource " << UPP::Name(this) << ": " << fmts);
	sel_formats = Split(fmts, ';');
	sel_ctrl = this;
	XSetSelectionOwner(Xdisplay, XAtom("PRIMARY"), xclipboard().win, CurrentTime);
}

Ctrl::Xclipboard::Xclipboard()
{
	GuiLock __;
	XSetWindowAttributes swa;
	win = XCreateWindow(Xdisplay, RootWindow(Xdisplay, Xscreenno),
	                    0, 0, 10, 10, 0, CopyFromParent, InputOnly, CopyFromParent,
	                    0, &swa);
	XSelectInput(Xdisplay, win, PropertyChangeMask);
}

Ctrl::Xclipboard::~Xclipboard()
{
	GuiLock __;
	XDestroyWindow(Xdisplay, win);
}

void Ctrl::Xclipboard::Write(int fmt, const ClipData& _data)
{
	GuiLock __;
	LLOG("SetSelectionOwner " << XAtomName(fmt));
	data.GetAdd(fmt) = _data;
	XSetSelectionOwner(Xdisplay, XAtom("CLIPBOARD"), win, CurrentTime);
}

void Ctrl::Xclipboard::Request(XSelectionRequestEvent *se)
{
	GuiLock __;
	LLOG("Request " << XAtomName(se->target));
	XEvent e;
	e.xselection.type      = SelectionNotify;
	e.xselection.display   = Xdisplay;
	e.xselection.requestor = se->requestor;
	e.xselection.selection = se->selection;
	e.xselection.target    = se->target;
	e.xselection.time      = se->time;
	e.xselection.property  = se->property;
	if(se->target == XAtom("TARGETS")) {
		LLOG("Request targets:");
		if(se->selection == XAtom("PRIMARY")) {
			Buffer<Atom> x(sel_formats.GetCount());
			for(int i = 0; i < sel_formats.GetCount(); i++) {
				x[i] = XAtom(sel_formats[i]);
				LLOG('\t' << sel_formats[i]);
			}
			XChangeProperty(Xdisplay, se->requestor, se->property, XAtom("ATOM"),
			                32, 0, (unsigned char*)~x,
			                sel_formats.GetCount());
		}
		else {
			Buffer<Atom> x(data.GetCount());
			for(int i = 0; i < data.GetCount(); i++) {
				x[i] = data.GetKey(i);
				LLOG('\t' << XAtomName(x[i]));
			}
			XChangeProperty(Xdisplay, se->requestor, se->property, XAtom("ATOM"),
			                32, 0, (unsigned char*)~x,
			                data.GetCount());
		}
	}
	else {
		if(se->selection == XAtom("PRIMARY")) {
			LLOG("Request PRIMARY data " << XAtomName(se->target));
			String fmt = XAtomName(se->target);
			int i = sel_formats.Find(fmt);
			if(i >= 0 && sel_ctrl) {
				String d = sel_ctrl->GetSelectionData(fmt);
				XChangeProperty(Xdisplay, se->requestor, se->property, se->target, 8, PropModeReplace,
				                d, d.GetLength());
			}
			else
			    e.xselection.property = None;
		}
		else {
			LLOG("Request CLIPBOARD data " << XAtomName(se->target));
			int i = data.Find(se->target);
			if(i >= 0) {
				String d = data[i].Render();
				XChangeProperty(Xdisplay, se->requestor, se->property, se->target, 8, PropModeReplace,
				                d, d.GetLength());
			}
			else
			    e.xselection.property = None;
		}
	}
	XSendEvent(Xdisplay, se->requestor, XFalse, 0, &e);
}

String Ctrl::Xclipboard::Read(int fmt, int selection, int property)
{
	GuiLock __;
	if(data.GetCount() && (dword)selection == XAtom("CLIPBOARD")) {
		int q = data.Find(fmt);
		return q >= 0 ? data[q].Render() : String();
	}
	if(sel_ctrl && (dword)selection == XAtom("PRIMARY"))
		return sel_ctrl->GetSelectionData(XAtomName(fmt));
	XConvertSelection(Xdisplay, selection, fmt, property, win, CurrentTime);
	XFlush(Xdisplay);
	XEvent event;
	for(int i = 0; i < 20; i++) {
		if(XCheckTypedWindowEvent(Xdisplay, win, SelectionNotify, &event)) {
			if(event.xselection.property != None) {
				XSync(Xdisplay, false);
				return ReadPropertyData(win, event.xselection.property);
			}
			return Null;
		}
		if(XCheckTypedWindowEvent(Xdisplay, win, SelectionRequest, &event) &&
		   event.xselectionrequest.owner == win)
			Request(&event.xselectionrequest);
		if(XCheckTypedWindowEvent(Xdisplay, win, SelectionClear, &event) &&
		   event.xselectionclear.window == win) {
			if(event.xselectionclear.selection == XAtom("CLIPBOARD"))
				Clear();
			if(event.xselectionclear.selection == XAtom("PRIMARY")) {
				sel_ctrl = NULL;
				sel_formats.Clear();
			}
		}
		Sleep(10);
	}
	return Null;
}

Ctrl::Xclipboard& Ctrl::xclipboard()
{
	static Xclipboard xc;
	return xc;
}

void ClearClipboard()
{
	GuiLock __;
	Ctrl::xclipboard().Clear();
}

void AppendClipboard(const char *format, const Value& data, String (*render)(const Value& data))
{
	GuiLock __;
	Vector<String> s = Split(format, ';');
	for(int i = 0; i < s.GetCount(); i++)
		Ctrl::xclipboard().Write(XAtom(s[i]), ClipData(data, render));
}

String sRawClipData(const Value& data);

void AppendClipboard(const char *fmt, const String& data)
{
	GuiLock __;
	AppendClipboard(fmt, data, sRawClipData);
}

String ReadClipboard(const char *fmt)
{
	GuiLock __;
	return Ctrl::xclipboard().Read(XAtom(fmt), XAtom("CLIPBOARD"), XAtom("CLIPDATA"));
}

void AppendClipboardText(const String& s)
{
	GuiLock __;
	AppendClipboard("STRING", s);
}

String ReadClipboardText()
{
	GuiLock __;
	return ReadClipboard("STRING");
}

void AppendClipboardUnicodeText(const WString& s)
{
	GuiLock __;
	AppendClipboard("UTF8_STRING", ToUtf8(s));
}

WString ReadClipboardUnicodeText()
{
	GuiLock __;
	return ToUtf32(ReadClipboard("UTF8_STRING"));
}

bool Ctrl::Xclipboard::IsAvailable(int fmt, const char *type)
{
	GuiLock __;
	if(data.GetCount())
		return data.Find(fmt) >= 0;
	String formats = Read(XAtom("TARGETS"), XAtom(type), XAtom("CLIPDATA"));
	int c = formats.GetCount() / sizeof(Atom);
	const Atom *m = (Atom *) ~formats;
	for(int i = 0; i < c; i++) {
		if(m[i] == (dword)fmt)
			return true;
	}
	return false;
}

bool Ctrl::ClipHas(int type, const char *fmt)
{
	GuiLock __;
	LLOG("ClipHas " << type << ": " << fmt);
	if(strcmp(fmt, "files") == 0)
		fmt = "text/uri-list";
	if(type == 0)
		return Ctrl::xclipboard().IsAvailable(XAtom(fmt), "CLIPBOARD");
	if(type == 2) {
		if(sel_ctrl)
			return sel_formats.Find(fmt) >= 0;
		return Ctrl::xclipboard().IsAvailable(XAtom(fmt), "PRIMARY");
	}
	return drop_formats.Find(fmt) >= 0;
}

String DnDGetData(const String& f);

String Ctrl::ClipGet(int type, const char *fmt)
{
	GuiLock __;
	LLOG("ClipGet " << type << ": " << fmt);
	if(strcmp(fmt, "files") == 0)
		fmt = "text/uri-list";
	if(type && GetDragAndDropSource())
		return DnDGetData(fmt);
	return Ctrl::xclipboard().Read(
	           XAtom(fmt),
	           XAtom(type == 2 ? "PRIMARY" : type == 1 ? "XdndSelection" : "CLIPBOARD"),
	           type == 1 ? XA_SECONDARY : XAtom("CLIPDATA")
	       );
}

const char *ClipFmtsText()
{
	return "STRING;UTF8_STRING;text/plain;text/unicode";
}

String GetString(PasteClip& clip)
{
	GuiLock __;
	if(clip.Accept("STRING") || clip.Accept("text/plain"))
		return ~clip;
	if(clip.Accept("UTF8_STRING"))
		return ToUtf32(~clip).ToString();
	return Null;
}

WString GetWString(PasteClip& clip)
{
	GuiLock __;
	if(clip.Accept("UTF8_STRING"))
		return ToUtf32(~clip);
	if(clip.Accept("STRING") || clip.Accept("text/plain"))
		return ToUnicode(~clip, CHARSET_ISO8859_1);
	return Null;
}

String GetTextClip(const WString& text, const String& fmt)
{
	GuiLock __;
	if(fmt == "STRING" || fmt == "text/plain")
		return text.ToString();
	if(fmt == "UTF8_STRING")
		return ToUtf8(text);
	return Null;
}

String GetTextClip(const String& text, const String& fmt)
{
	GuiLock __;
	if(fmt == "STRING" || fmt == "text/plain")
		return text;
	if(fmt == "UTF8_STRING")
		return ToUtf8(text.ToWString());
	return Null;
}

bool AcceptText(PasteClip& clip)
{
	GuiLock __;
	return clip.Accept(ClipFmtsText());
}

void Append(VectorMap<String, ClipData>& data, const String& text) // optimize
{
	GuiLock __;
	data.GetAdd("STRING", text);
	data.GetAdd("text/plain", text);
	data.GetAdd("UTF8_STRING", ToUtf8(text.ToWString()));
}

void Append(VectorMap<String, ClipData>& data, const WString& text) // optimize
{
	GuiLock __;
	data.GetAdd("STRING", text.ToString());
	data.GetAdd("text/plain", text.ToString());
	data.GetAdd("UTF8_STRING", ToUtf8(text));
}

bool IsClipboardAvailable(const char *fmt)
{
	GuiLock __;
	return Ctrl::xclipboard().IsAvailable(XAtom(fmt), "CLIPBOARD");
}

bool IsClipboardAvailableText()
{
	GuiLock __;
	return IsClipboardAvailable("STRING") ||
	       IsClipboardAvailable("UTF8_STRING") ||
	       IsClipboardAvailable("text/plain") ||
	       IsClipboardAvailable("text/unicode");
}

bool AcceptFiles(PasteClip& clip)
{
	GuiLock __;
	return clip.Accept("text/uri-list");
}

bool IsAvailableFiles(PasteClip& clip)
{
	GuiLock __;
	return clip.IsAvailable("text/uri-list");
}

int JustLf(int c)
{
	return (byte)c >= 32 || c == '\n' ? c : 0;
}

Vector<String> GetClipFiles(const String& data)
{
	Vector<String> r;
	Vector<String> f = Split(Filter(data, JustLf), '\n');
	for(int i = 0; i < f.GetCount(); i++)
		if(f[i].StartsWith("file://"))
			r.Add(UrlDecode(f[i].Mid(7)));
	return r;
}

Vector<String> GetFiles(PasteClip& clip) {
	GuiLock __;
	Vector<String> r;
	if(clip.Accept("text/uri-list")) {
		String txt = clip;
		r = GetClipFiles(txt);
	}
	return r;
}

void AppendFiles(VectorMap<String, ClipData>& data, const Vector<String>& files)
{
	GuiLock __;
	if(files.GetCount() == 0)
		return;
	String h;
	for(int i = 0; i < files.GetCount(); i++)
		h << "file://" << UrlEncode(files[i]) << '\n';
	data.GetAdd("text/uri-list") = h;
}

bool PasteClip::IsAvailable(const char *fmt) const
{
	return Ctrl::ClipHas(type, fmt);
}

String PasteClip::Get(const char *fmt) const
{
	return Ctrl::ClipGet(type, fmt);
}

void PasteClip::GuiPlatformConstruct()
{
	type = 0;
}

}

#endif
