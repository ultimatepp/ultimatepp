#include "CtrlCore.h"

NAMESPACE_UPP

ClipData::ClipData(const Value& data, String (*render)(const Value& data))
:	data(data), render(render)
{}

String sRawClipData(const Value& data)
{
	return data;
}

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
#ifdef PLATFORM_X11
	d.fmt.Clear();
	d.type = 2;
#endif
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

#ifdef PLATFORM_WIN32
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
#endif

#ifdef PLATFORM_X11
bool PasteClip::IsAvailable(const char *fmt) const
{
	return Ctrl::ClipHas(type, fmt);
}

String PasteClip::Get(const char *fmt) const
{
	return Ctrl::ClipGet(type, fmt);
}
#endif

bool PasteClip::Accept()
{
	accepted = true;
	return paste;
}

bool   PasteClip::Accept(const char *_fmt)
{
	Vector<String> f = Split(_fmt, ';');
	for(int i = 0; i < f.GetCount(); i++) {
		if(IsAccepted() && fmt == _fmt)
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
#ifdef PLATFORM_WIN32
	dt = NULL;
#else
	type = 0;
#endif
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

String Unicode__(const WString& w)
{
	return String((const char *)~w, 2 * w.GetLength());
}

WString Unicode__(const String& s)
{
	return WString((const wchar *)~s, s.GetLength() / 2);
}

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
		Size ssz = sample.GetSize();
		Over(b, Point(2, 22), sample, sample.GetSize());
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
#ifdef PLATFORM_X11
		if(Ctrl::IsCompositedGui()) {
			Image h = Rescale(b, 64, 64);
			b = h;
		}
#endif
		Over(b, Point(0, 0), arrow, arrow.GetSize());
	}

	return b;
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
		if(ctrl->IsMouseActive())
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

END_UPP_NAMESPACE
