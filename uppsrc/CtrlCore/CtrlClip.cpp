#include "CtrlCore.h"

namespace Upp {

String sRawClipData(const Value& data)
{
	return data;
}

ClipData::ClipData(const Value& data, String (*render)(const Value& data))
:	data(data), render(render ? render : sRawClipData)
{}

ClipData::ClipData(const String& data)
:	data(data), render(sRawClipData)
{}

ClipData::ClipData()
:	render(sRawClipData)
{}

void Ctrl::DragAndDrop(Point p, PasteClip& d)       {}
void Ctrl::FrameDragAndDrop(Point p, PasteClip& d)  {}
void Ctrl::DragEnter()                              {}
void Ctrl::DragRepeat(Point p) {}
void Ctrl::DragLeave() {}

PasteClip& Ctrl::Clipboard()
{
	GuiLock __;
	static PasteClip d;
	d.fmt.Clear();
	return d;
}

PasteClip& Ctrl::Selection()
{
	GuiLock __;
	static PasteClip d;
	GuiPlatformSelection(d);
	return d;
}

String Ctrl::GetDropData(const String& fmt) const
{
	return GetSelectionData(fmt);
}

String Ctrl::GetSelectionData(const String& fmt) const
{
	return Null;
}

bool PasteClip::Accept()
{
	accepted = true;
	return paste;
}

bool PasteClip::IsAnyAvailable(const char *fmt) const
{
	for(String f : Split(fmt, ';'))
		if(IsAvailable(f))
			return true;
	return false;
}

bool   PasteClip::Accept(const char *_fmt)
{
	Vector<String> f = Split(_fmt, ';');
	for(int i = 0; i < f.GetCount(); i++) {
		if(IsAccepted() && fmt == f[i])
			return paste;
		if(IsAvailable(f[i])) {
			accepted = true;
			if(paste) {
				fmt = f[i];
				data = Get(f[i]);
				return true;
			}
			break;
		}
	}
	return false;
}

PasteClip::PasteClip()
{
	paste = true;
	accepted = false;
	GuiPlatformConstruct();
}

int Ctrl::DoDragAndDrop(const char *fmts, const Image& sample, dword actions)
{
	VectorMap<String, ClipData> dummy;
	return DoDragAndDrop(fmts, sample, actions, dummy);
}

int Ctrl::DoDragAndDrop(const VectorMap<String, ClipData>& data, const Image& sample, dword actions)
{
	return DoDragAndDrop("", sample, actions, data);
}

Uuid        sDndUuid;
const void *sInternalPtr;

String GetInternalDropId__(const char *type, const char *id)
{
	return "U++ Internal clip:" + AsString(sDndUuid) + '-' + type + '-' + id;
}

void NewInternalDrop__(const void *ptr)
{
	sDndUuid = Uuid::Create();
	sInternalPtr = ptr;
}

const void *GetInternalDropPtr__()
{
	return sInternalPtr;
}

void GuiPlatformAdjustDragImage(ImageBuffer& b);

Image MakeDragImage(const Image& arrow, Image sample)
{
	ImageBuffer b;
	if(IsNull(sample)) {
		sample = CtrlCoreImg::DndData();
		b = sample;
		Over(b, Point(0, 0), arrow, arrow.GetSize());
	}
	else {
		b.Create(128, 128);
		memset(~b, 0, sizeof(RGBA) * b.GetLength());
		Over(b, Point(2, 22), sample, sample.GetSize());
		Unmultiply(b);
		for(int y = 20; y < 96; y++) {
			RGBA *s = b[y];
			RGBA *e = s + 96;
			while(s < e)
				(s++)->a >>= 1;
			e += 32;
			int q = 128;
			while(s < e) {
				s->a = (s->a * q) >> 8;
				q -= 4;
				s++;
			}
		}
		int qq = 128;
		for(int y = 96; y < 128; y++) {
			RGBA *s = b[y];
			RGBA *e = s + 96;
			while(s < e) {
				s->a = (s->a * qq) >> 8;
				s++;
			}
			e += 32;
			int q = 255;
			while(s < e) {
				s->a = (s->a * q * qq) >> 16;
				q -= 8;
				s++;
			}
			qq -= 4;
		}
		RGBA *s = b[21] + 1;
		RGBA c1 = Blue();
		RGBA c2 = White();
		for(int a = 255; a > 0; a -= 3) {
			c1.a = c2.a = a;
			*s++ = c1;
			Swap(c1, c2);
		}
		s = b[21] + 1;
		c1 = Black();
		c2 = White();
		for(int a = 255; a > 0; a -= 8) {
			c1.a = c2.a = a;
			*s = c1;
			s += b.GetWidth();
			Swap(c1, c2);
		}
		Premultiply(b);
		GuiPlatformAdjustDragImage(b);
		Over(b, Point(0, 0), arrow, arrow.GetSize());
	}

	return Image(b);
}

Ptr<Ctrl> Ctrl::dndctrl;
Point     Ctrl::dndpos;
bool      Ctrl::dndframe;
PasteClip Ctrl::dndclip;

void Ctrl::DnDRepeat()
{
	GuiLock __;
	if(dndctrl) {
		dndctrl->DragRepeat(dndpos);
		if(dndctrl) {
			PasteClip d = dndclip;
			if(dndframe)
				dndctrl->FrameDragAndDrop(dndpos, d);
			else
				dndctrl->DragAndDrop(dndpos, d);
		}
	}
	else
		UPP::KillTimeCallback(&dndpos);
}

void Ctrl::DnD(Point p, PasteClip& clip)
{
	GuiLock __;
	UPP::KillTimeCallback(&dndpos);
	dndclip = clip;
	Point hp = p - GetScreenRect().TopLeft();
	Ptr<Ctrl> ctrl = this;
	while(ctrl && ctrl->IsEnabled()) {
		Rect view = ctrl->GetScreenView();
		if(ctrl->IsMouseActive()) {
			if(view.Contains(p)) {
				dndpos = p - view.TopLeft();
				dndframe = false;
				ctrl->DragAndDrop(dndpos, clip);
				if(clip.IsAccepted())
					break;
			}
			else {
				dndpos = p - ctrl->GetScreenRect().TopLeft();
				dndframe = true;
				ctrl->FrameDragAndDrop(dndpos, clip);
				if(clip.IsAccepted())
					break;
			}
		}
		ctrl = ctrl->ChildFromPoint(hp);
	}
	if(ctrl != dndctrl) {
		if(dndctrl)
			dndctrl->DragLeave();
		dndctrl = ctrl;
		if(dndctrl)
			dndctrl->DragEnter();
	}
	if(dndctrl)
		UPP::SetTimeCallback(-40, callback(DnDRepeat), &dndpos);
}

void Ctrl::DnDLeave()
{
	GuiLock __;
	if(dndctrl) {
		dndctrl->DragLeave();
		UPP::KillTimeCallback(&dndpos);
		dndctrl = NULL;
	}
}

Ctrl *Ctrl::GetDragAndDropTarget()
{
	GuiLock __;
	return dndctrl;
}

bool IsClipboardAvailableImage()
{
	GuiLock __;
	PasteClip d = Ctrl::Clipboard();
	return IsAvailableImage(d);
}

void AppendClipboard(const char *format, const ClipData& data)
{
	AppendClipboard(format, data.data, data.render);
}

void AppendClipboard(const VectorMap<String, ClipData>& data)
{
	for(int i = 0; i < data.GetCount(); i++)
		AppendClipboard(data.GetKey(i), data[i]);
}

void InitRichImage(String      (*fGetImageClip)(const Image& img, const String& fmt),
                   bool        (*fAcceptImage)(PasteClip& clip),
                   Image       (*fGetImage)(PasteClip& clip),
                   const char *(*fClipFmtsImage)());

INITBLOCK {
	InitRichImage(GetImageClip, AcceptImage, GetImage, ClipFmtsImage);
}

const char *sClipFmtsRTF = "Rich Text Format;text/rtf;application/rtf";

const char *ClipFmtsRTF()
{
	return sClipFmtsRTF;
}

#ifdef PLATFORM_POSIX

void WriteClipboardHTML(const String& html)
{
    WriteClipboard("text/html", html);
}

#else

void WriteClipboardHTML(const String& html)
{
	String data;
	
	data =
		"Version:0.9\n"
		"StartHTML:#000001#\n"
		"EndHTML:#000002#\n"
		"StartFragment:#000003#\n"
		"EndFragment:#000004#\n"
		"StartSelection:#000003#\n"
		"EndSelection:#000004#\n"
		"<HTML>\n"
		"<!--StartFragment -->"
	    "<!--EndFragment -->\n"
		"</HTML>\n"
	;
	
	data.Replace("#000001#", Sprintf("%08u", data.Find("<HTML>") + 6));
	int start = data.Find("<!--StartFragment -->") + 21;
	data.Replace("#000003#", Sprintf("%08u", start));
	data.Replace("#000004#", Sprintf("%08u", data.Find("<!--EndFragment -->") + html.GetCount()));
	data.Replace("#000002#", Sprintf("%08u", data.Find("</HTML>") + html.GetCount()));
	
	data.Insert(start, html);
	
	WriteClipboard("HTML Format", data);
}

#endif

}
