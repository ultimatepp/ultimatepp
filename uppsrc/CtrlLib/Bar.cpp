#include "CtrlLib.h"

namespace Upp {

#define LLOG(x) // LOG(x)

void BarPane::LeftDown(Point pt, dword keyflags)
{
	WhenLeftClick();
}

void BarPane::PaintBar(Draw& w, const SeparatorCtrl::Style& ss, const Value& pane,
                       const Value& iconbar, int iconsz)
{
	Size sz = GetSize();
	Rect r = sz;
	for(int i = 0; i < breakpos.GetCount(); i++)
		if(horz) {
			int y = breakpos[i];
			Rect rr = r;
			rr.bottom = y;
			ChPaint(w, rr, pane);
			if(!IsNull(ss.l1))
				ChPaint(w, 0, y++, sz.cx, 1, ss.l1);
			if(!IsNull(ss.l2))
				ChPaint(w, 0, y++, sz.cx, 1, ss.l2);
			r.top = y;
		}
		else {
			int x = breakpos[i];
			Rect rr = r;
			rr.right = x;
			if(!IsNull(iconbar)) {
				Rect r2 = rr;
				r2.right = r2.left + iconsz;
				ChPaint(w, r2, iconbar);
				rr.left = r2.right;
			}
			ChPaint(w, rr, pane);
			if(!IsNull(ss.l1))
				ChPaint(w, x++, 0, 1, sz.cy, ss.l1);
			if(!IsNull(ss.l2))
				ChPaint(w, x++, 0, 1, sz.cy, ss.l2);
			r.left = x;
		}
	if(!horz && !IsNull(iconbar)) {
		Rect r2 = r;
		r2.right = r2.left + iconsz;
		ChPaint(w, r2, iconbar);
		r.left = r2.right;
	}
	ChPaint(w, r, pane);
}

Size BarPane::LayOut(bool horz, int maxsize, bool repos)
{
	auto HoVe = [&](Size& sz)->int& { return horz ? sz.cx : sz.cy; };
	auto VeHo = [&](Size& sz)->int& { return horz ? sz.cy : sz.cx; };
	maxsize -= 2 * (horz ? hmargin : vmargin);
	if(repos) {
		this->horz = horz;
		breakpos.Clear();
	}
	int bmargin = horz ? vmargin : hmargin;
	Size asz(0, 0);
	VeHo(asz) = bmargin;
	auto q = item.begin();
	while(q != item.end()) {
		Size psz(0, 0);
		auto f = q;
		while(q != item.end()) {
			if(q->ctrl == NULL && IsNull(q->gapsize)) {
				q++;
				break;
			}
			int gapsize = q->gapsize;
			if(gapsize == INT_MAX && q->ctrl == NULL) {
				if(maxsize == INT_MAX) {
					q++;
					break;
				}
				int rsz = 0;
				auto w = q + 1;
				while(w < item.end()) {
					if(!w->ctrl) break;
					Size sz = w->ctrl->GetMinSize();
//					if(HoVe(psz) + gapsize + HoVe(sz) > maxsize) break;
					rsz += HoVe(sz);
					w++;
				}
				if(rsz > maxsize)
					gapsize = 0;
				else {
					gapsize = maxsize - rsz - HoVe(psz);
					if(gapsize < 0)
						gapsize = maxsize - rsz;
				}
			}
			Size sz = q->ctrl ? q->ctrl->GetMinSize()
			                  : Size(horz ? gapsize : 0, horz ? 0 : gapsize);
			if(HoVe(sz) == INT_MAX)
				HoVe(sz) = maxsize - HoVe(psz);
			if(HoVe(psz) + HoVe(sz) > maxsize && HoVe(psz)) {
				while(q < item.end() && q->ctrl == NULL && !IsNull(q->gapsize) && q->gapsize != INT_MAX)
					q++;
				break;
			}
			VeHo(psz) = max(VeHo(psz), VeHo(sz));
			if(q->ctrl) {
				if(horz)
					q->ctrl->LeftPos(min(psz.cx + hmargin, 32000), sz.cx);
				else
					q->ctrl->TopPos(min(psz.cy + vmargin, 32000), sz.cy);
			}
			HoVe(psz) += HoVe(sz);
			q++;
		}
		int& bp = VeHo(asz);
		if(bp > bmargin) {
			if(repos)
				breakpos.Add(bp + bmargin);
			bp += 2 + 2 * bmargin;
		}
		if(repos)
			while(f != q) {
				if(f->ctrl) {
					int bpm = min(bp, 32000);
					if(horz)
						f->ctrl->TopPos(bpm, psz.cy);
					else
						f->ctrl->LeftPos(bpm, psz.cx);
				}
				f++;
			}
		bp += VeHo(psz);
		HoVe(asz) = max(HoVe(asz), HoVe(psz));
	}
	VeHo(asz) += bmargin;
	HoVe(asz) += horz ? 2 * hmargin : 2 * vmargin;
	LLOG("Repos " << Name() << " " << asz);
	return asz;
}


Size BarPane::Repos(bool horz, int maxsize)
{
	return LayOut(horz, maxsize, true);
}

Size BarPane::GetPaneSize(bool _horz, int maxsize) const
{
	return const_cast<BarPane *>(this)->LayOut(horz, maxsize, false);
}

void BarPane::IClear()
{
	breakpos.Clear();
	item.Clear();
}

void BarPane::Clear()
{
	while(GetFirstChild())
		RemoveChild(GetFirstChild());
	IClear();
}

void BarPane::Add(Ctrl *ctrl, int gapsize)
{
	Item& m = item.Add();
	m.ctrl = ctrl;
	m.gapsize = gapsize;
	if(ctrl)
		AddChild(ctrl);
}

void BarPane::MouseMove(Point p, dword)
{
	Ctrl *q = Ctrl::GetVisibleChild(this, p, false);
	if(q != phelpctrl) {
		phelpctrl = q;
		BarCtrl::SendHelpLine(q);
	}
}

BarPane::BarPane()
{
	NoWantFocus();
	phelpctrl = NULL;
	hmargin = vmargin = 0;
	menu = false;
}

BarPane::~BarPane() {}

Bar::Item::Item() {}
Bar::Item::~Item() {}

Bar::Item& Bar::Item::Text(const char *text)        { return *this; }
Bar::Item& Bar::Item::Key(dword key)                { return *this; }
Bar::Item& Bar::Item::Repeat(bool b)                { return *this; }
Bar::Item& Bar::Item::Image(const UPP::Image& img)  { return *this; }
Bar::Item& Bar::Item::Bold(bool bold)               { return *this; }
Bar::Item& Bar::Item::Enable(bool _enable)          { return *this; }
Bar::Item& Bar::Item::Tip(const char *tip)          { return *this; }
Bar::Item& Bar::Item::Help(const char *help)        { return *this; }
Bar::Item& Bar::Item::Topic(const char *topic)      { return *this; }
Bar::Item& Bar::Item::Description(const char *desc) { return *this; }
Bar::Item& Bar::Item::Check(bool check)             { return *this; }
Bar::Item& Bar::Item::Radio(bool check)             { return *this; }
void       Bar::Item::FinalSync() {}

Bar::Item& Bar::Item::Label(const char *text)
{
	ToolButton *b = dynamic_cast<ToolButton *>(this);
	if(b) b->Label(text, ToolButton::BOTTOMLABEL);
	return *this;
}

Bar::Item& Bar::Item::RightLabel(const char *text)
{
	ToolButton *b = dynamic_cast<ToolButton *>(this);
	if(b) b->Label(text, ToolButton::RIGHTLABEL);
	return *this;
}

Bar::Item& Bar::Item::Key(KeyInfo& (*key)()) {
	KeyInfo& k = (*key)();
	return Key(k.key[0]).Key(k.key[1]).Key(k.key[2]).Key(k.key[3]);
}

Bar::Bar() {}
Bar::~Bar() {}

Bar::Item& Bar::NilItem()
{
	static Item m;
	return m;
}

void Bar::AddNC(Ctrl& ctrl)
{
	AddCtrl(&ctrl, (int)Null);
}

Bar::Item&  Bar::Add(bool enable, const char *text, const UPP::Image& image, const Callback& callback)
{
	return AddItem(callback).Text(text).Image(image).Enable(enable);
}

Bar::Item&  Bar::Add(bool enable, KeyInfo& (*key)(), const UPP::Image& image, const Callback& callback)
{
	const char *text = (*key)().name;
	if(*text == '\3')
		text = t_GetLngString(text + 1);
	return Add(enable, text, image, callback).Key(key);
}

Bar::Item&  Bar::Add(const char *text, const UPP::Image& image, const Callback& callback)
{
	return AddItem(callback).Text(text).Image(image);
}
/*
Bar::Item& Bar::Add(const String& text, const UPP::Image& image, const Event<> & callback)
{
	return Add(~text, image, callback);
}
*/
Bar::Item&  Bar::Add(KeyInfo& (*key)(), const UPP::Image& image, const Callback& callback)
{
	const char *text = (*key)().name;
	if(*text == '\3')
		text = t_GetLngString(text + 1);
	return Add(text, image, callback).Key(key);
}

Bar::Item&  Bar::Add(bool enable, const char *text, const Callback & callback)
{ return IsMenuBar() ? Add(enable, text, Image(), callback) : NilItem(); }

Bar::Item&  Bar::Add(const char *text, const Callback & callback)
{ return IsMenuBar() ? Add(true, text, Image(), callback) : NilItem(); }

Bar::Item&  Bar::Add(bool enable, KeyInfo& (*key)(), const Callback & callback)
{ return IsMenuBar() ? Add(enable, key, Image(), callback) : NilItem(); }

Bar::Item&  Bar::Add(KeyInfo& (*key)(), const Callback & callback)
{ return IsMenuBar() ? Add(true, key, Image(), callback) : NilItem(); }

Bar::Item& Bar::Add(bool enable, const char *text, const UPP::Image& image, const UPP::Function<void ()>& fn)
{
	return Add(enable, text, image, Callback() << fn);
}

Bar::Item& Bar::Add(bool enable, KeyInfo& (*key)(), const UPP::Image& image, const Function<void ()>& fn)
{
	return Add(enable, key, image, Callback() << fn);
}

Bar::Item& Bar::Add(const char *text, const UPP::Image& image, const Function<void ()>& fn)
{
	return Add(text, image, Callback() << fn);
}

/*
Bar::Item& Bar::Add(const String& text, const UPP::Image& image, const Function<void ()>& fn)
{
	return Add(text, image, Event<> () << fn);
}
*/

Bar::Item& Bar::Add(KeyInfo& (*key)(), const UPP::Image& image, const Function<void ()>& fn)
{
	return Add(key, image, Callback() << fn);
}

Bar::Item& Bar::Add(bool enable, const char *text, const Function<void ()>& fn)
{
	return Add(enable, text, Callback() << fn);
}

Bar::Item& Bar::Add(bool enable, KeyInfo& (*key)(), const Function<void ()>& fn)
{
	return Add(enable, key, Callback() << fn);
}

Bar::Item& Bar::Add(const char *text, const Function<void ()>& fn)
{
	return Add(text, Callback() << fn);
}

Bar::Item& Bar::Add(KeyInfo& (*key)(), const Function<void ()>& fn)
{
	return Add(key, Callback() << fn);
}

void Bar::Break()
{
	AddCtrl(NULL, (int)Null);
}

void Bar::Gap(int size)
{
	AddCtrl(NULL, size);
}

void   Bar::MenuSeparator()              { if(IsMenuBar()) Separator(); }
void   Bar::MenuBreak()                  { if(IsMenuBar()) Break(); }
void   Bar::MenuGap(int size)            { if(IsMenuBar()) Gap(size); }
void   Bar::AddMenu(Ctrl& ctrl)          { if(IsMenuBar()) Add(ctrl); }
void   Bar::AddMenu(Ctrl& ctrl, Size sz) { if(IsMenuBar()) Add(ctrl, sz); }

Bar::Item&  Bar::AddMenu(bool enable, KeyInfo& (*key)(), const UPP::Image& image, const Callback & callback)
{ return IsMenuBar() ? Add(enable, key, image, callback) : NilItem(); }

Bar::Item&  Bar::AddMenu(KeyInfo& (*key)(), const UPP::Image& image, const Callback & callback)
{ return IsMenuBar() ? Add(key, image, callback) : NilItem(); }

Bar::Item&  Bar::AddMenu(bool enable, const char *text, const UPP::Image& image, const Callback & callback)
{ return IsMenuBar() ? Add(enable, text, image, callback) : NilItem(); }

Bar::Item&  Bar::AddMenu(const char *text, const UPP::Image& image, const Callback & callback)
{ return IsMenuBar() ? Add(text, image, callback) : NilItem(); }


Bar::Item& Bar::AddMenu(bool enable, const char *text, const UPP::Image& image, const Function<void ()>& fn)
{
	return AddMenu(enable, text, image, Callback() << fn);
}

Bar::Item& Bar::AddMenu(bool enable, KeyInfo& (*key)(), const UPP::Image& image, const Function<void ()>& fn)
{
	return AddMenu(enable, key, image, Callback () << fn);
}

Bar::Item& Bar::AddMenu(const char *text, const UPP::Image& image, const Function<void ()>& fn)
{
	return AddMenu(text, image, Callback () << fn);
}

Bar::Item& Bar::AddMenu(KeyInfo& (*key)(), const UPP::Image& m, const Function<void ()>& fn)
{
	return AddMenu(key, m, Callback () << fn);
}

Bar::Item& Bar::Add(bool enable, const char *text, const Callback1<Bar&>& proc)
{ return AddSubMenu(proc).Text(text).Enable(enable); }

Bar::Item& Bar::Add(const char *text, const Callback1<Bar&>& proc)
{ return Add(true, text, proc); }

Bar::Item& Bar::Add(bool enable, const char *text, const UPP::Image& image, const Callback1<Bar&>& proc)
{ return Add(enable, text, proc).Image(image); }

Bar::Item& Bar::Add(const char *text, const UPP::Image& image, const Callback1<Bar&>& proc)
{ return Add(text, proc).Image(image); }

Bar::Item& Bar::Sub(bool enable, const char *text, const Function<void (Bar&)>& submenu)
{
	return Add(enable, text, Callback1<Bar&>() << submenu);
}

Bar::Item& Bar::Sub(const char *text, const Function<void (Bar&)>& submenu)
{
	return Add(text, Callback1<Bar&>() << submenu);
}

Bar::Item& Bar::Sub(bool enable, const char *text, const UPP::Image& image, const Function<void (Bar&)>& submenu)
{
	return Add(enable, text, image, Callback1<Bar&>() << submenu);
}

Bar::Item& Bar::Sub(const char *text, const UPP::Image& image, const Function<void (Bar&)>& submenu)
{
	return Add(text, image, Callback1<Bar&>() << submenu);
}

void   Bar::ToolSeparator()                { if(IsToolBar()) Separator(); }
void   Bar::ToolBreak()                    { if(IsToolBar()) Break(); }
void   Bar::ToolGap(int size)              { if(IsToolBar()) Gap(size); }

void   Bar::AddTool(Ctrl& ctrl)            { if(IsToolBar()) Add(ctrl); }
void   Bar::AddTool(Ctrl& ctrl, Size sz)   { if(IsToolBar()) Add(ctrl, sz); }

void   Bar::AddKey(dword key, Event<>  cb) {}

void Bar::AddKey(KeyInfo& (*key)(), Event<>  cb)
{
	KeyInfo& k = (*key)();
	AddKey(k.key[0], cb);
	AddKey(k.key[1], cb);
	AddKey(k.key[2], cb);
	AddKey(k.key[3], cb);
}

Bar::Item&  Bar::Add(bool enable, const UPP::Image& image, Event<>  cb)
{ return IsToolBar() ? AddItem(cb).Image(image).Enable(enable) : NilItem(); }

Bar::Item&  Bar::Add(const UPP::Image& image, Event<>  cb)
{ return IsToolBar() ? Add(true, image, cb) : NilItem(); }

class Bar::ScanKeys : public Bar {
	struct KeyItem : public Bar::Item {
		bool          enabled;
		Vector<dword> key;
		Event<>       action;

		virtual Item& Text(const char *)             { return *this; }
		virtual Item& Check(bool check)              { return *this; }
		virtual Item& Radio(bool check)              { return *this; }
		virtual Item& Key(dword _key)                { if(_key) key.Add(_key); return *this; }
		virtual Item& Image(const UPP::Image& img)   { return *this;  }
		virtual Item& Enable(bool _enable = true)    { enabled = _enable; return *this; }
		virtual Item& Tip(const char *tip)           { return *this; }
		virtual Item& Help(const char *help)         { return *this; }
		virtual Item& Topic(const char *help)        { return *this; }
		virtual Item& Description(const char *help)  { return *this; }
	};

	Array<KeyItem> key;
	KeyItem        nilitem;
	int            level;

protected:
	virtual Item&  AddItem(Event<>  cb);
	virtual Item&  AddSubMenu(Event<Bar&> proc);
	virtual void   AddCtrl(Ctrl *ctrl, int gapsize) {};
	virtual void   AddCtrl(Ctrl *ctrl, Size sz) {};

public:
	virtual bool   IsMenuBar() const                 { return true; }
	virtual bool   IsToolBar() const                 { return true; }
	virtual bool   IsScanKeys() const                { return true; }

	virtual bool   IsEmpty() const                   { return key.IsEmpty(); }
	virtual void   Separator()                       {}
	virtual void   AddKey(dword key, Event<> cb);

	bool Do(dword key);

	ScanKeys()                                       { level = 0; }
};

Bar::Item& Bar::ScanKeys::AddItem(Event<>  cb)
{
	KeyItem& m = key.Add();
	m.action = cb;
	m.enabled = true;
	return m;
}

Bar::Item&  Bar::ScanKeys::AddSubMenu(Event<Bar&> proc)
{
	level++;
	if(level > 5)
		return NilItem();;
	proc(*this);
	level--;
	return key.GetCount() ? key.Top() : nilitem;
}

void   Bar::ScanKeys::AddKey(dword _key, Event<>  cb)
{
	if(_key) {
		KeyItem& m = key.Add();
		m.action = cb;
		m.enabled = true;
		m.key.Add(_key);
	}
}

bool   Bar::ScanKeys::Do(dword k)
{
	for(int i = 0; i < key.GetCount(); i++) {
		KeyItem& m = key[i];
		if(m.enabled && FindIndex(m.key, k) >= 0) {
			m.action();
			return true;
		}
	}
	return false;
}

bool   Bar::Scan(Event<Bar&> proc, dword key)
{
	ScanKeys keys;
	proc(keys);
	return keys.Do(key);
}

bool BarCtrl::IsEmpty() const
{
	return pane.IsEmpty();
}

void BarCtrl::Separator()
{
	AddNC(separator.At(sii++).Margin(lsepm, rsepm).SetStyle(*sepstyle).SetSize(ssize));
}

void BarCtrl::AddCtrl(Ctrl *ctrl, Size sz)
{
	SizeCtrl& m = sizer.DoIndex(zii++);
	while(m.GetFirstChild())
		m.RemoveChild(m.GetFirstChild());
	LogPos p = ctrl->GetPos();
	if(p.x.IsEmpty()) {
		if(sz.cx == INT_MAX)
			ctrl->HSizePos();
		else
			ctrl->HCenterPos(sz.cx ? sz.cx : MINSIZE);
	}
	if(p.y.IsEmpty()) {
		if(sz.cy == INT_MAX)
			ctrl->VSizePos();
		else
			ctrl->VCenterPos(sz.cy ? sz.cy : MINSIZE);
	}
	LLOG("AddCtrl Ctrl::GetSize: " << ctrl->GetSize());
	m.SetSize(sz);
	m.AddChild(ctrl);
	AddNC(m);
	LLOG("AddCtrl2 Ctrl::GetSize: " << ctrl->GetSize());
}

Size BarCtrl::SizeCtrl::GetMinSize() const
{
	Size sz = size;
	Size chsz = Size(8, 8);
	if(GetFirstChild())
		chsz = GetFirstChild()->GetStdSize();
	sz.cx = sz.cx <= 0 ? chsz.cx : sz.cx;
	sz.cy = sz.cy <= 0 ? chsz.cy : sz.cy;
	return sz;
}

Value BarCtrl::GetBackground() const
{
	return Null;
}

void BarCtrl::Clear()
{
	separator.Clear();
	sizer.Clear();
	pane.Clear();
}

void BarCtrl::IClear()
{
	sii = zii = 0;
	pane.IClear();
}

void BarCtrl::IFinish()
{
	separator.SetCount(sii);
	sizer.SetCount(zii);
}

BarCtrl *BarCtrl::GetBarCtrlParent(Ctrl *q)
{
	while(q) {
		BarCtrl *w = dynamic_cast<BarCtrl *>(q);
		if(w) return w;
		q = q->GetParent();
	}
	return NULL;
}

void BarCtrl::SendHelpLine(Ctrl *q)
{
	String s = q->GetHelpLine();
	if(!UPP::IsEmpty(s)) {
		BarCtrl *b = GetBarCtrlParent(q);
		if(b) b->WhenHelp(s);
	}
}

void BarCtrl::ClearHelpLine(Ctrl *q)
{
	BarCtrl *b = GetBarCtrlParent(q);
	if(b) b->WhenHelp(Null);
}

void BarCtrl::FrameLayout(Rect& r)
{
	switch(align) {
	case BAR_LEFT:
		LayoutFrameLeft(r, this, pane.Repos(false, r.Height()).cx);
		break;
	case BAR_RIGHT:
		LayoutFrameRight(r, this, pane.Repos(false, r.Height()).cx);
		break;
	case BAR_BOTTOM:
		LayoutFrameBottom(r, this, pane.Repos(true, r.Width()).cy);
		break;
	default:
		LayoutFrameTop(r, this, pane.Repos(true, r.Width()).cy);
		break;
	}
}

void BarCtrl::FrameAddSize(Size& sz)
{
	if(align == BAR_LEFT || align == BAR_RIGHT)
		sz.cx += pane.GetPaneSize(true, sz.cy).cx;
	else
		sz.cy += pane.GetPaneSize(true, sz.cx).cy;
}

void BarCtrl::FrameAdd(Ctrl& parent)
{
	parent.Add(*this);
}

void BarCtrl::FrameRemove()
{
	Remove();
}

void BarCtrl::AddCtrl(Ctrl *ctrl, int gapsize)
{
	pane.Add(ctrl, gapsize);
}

BarCtrl& BarCtrl::Align(int al) {
	align = al;
	SyncBar();
	return *this;
}

void BarCtrl::SyncBar()
{
	if(IsChild() && InFrame())
		UpdateParentLayout();
	else
		Layout();
}

void BarCtrl::Layout()
{
	LLOG("BarCtrl::Layout");
	if(IsChild()) {
		bool dowrap = InFrame() && wrap >= 0 || wrap > 0;
		if(GetAlign() == BAR_LEFT || GetAlign() == BAR_RIGHT)
			pane.Repos(false, dowrap ? GetSize().cy : INT_MAX);
		else
			pane.Repos(true, dowrap ? GetSize().cx : INT_MAX);
	}
}

void BarCtrl::PaintBar(Draw& w, const SeparatorCtrl::Style& ss, const Value& p, const Value& iconbar, int iconsz)
{
	pane.PaintBar(w, ss, p, iconbar, iconsz);
}

BarCtrl::BarCtrl() {
	align = BAR_TOP;
	Ctrl::Add(pane.SizePos());
	lsepm = rsepm = 2;
	ssize = 8;
	sii = zii = 0;
	NoWantFocus();
	pane.WhenLeftClick = Proxy(WhenLeftClick);
	wrap = 0;
	sepstyle = &SeparatorCtrl::StyleDefault();
}

BarCtrl::~BarCtrl() {}

void LRUList::Serialize(Stream& stream)
{
	StreamContainer(stream, lru);
}

void LRUList::Select(String f, Event<const String&> WhenSelect)
{
	WhenSelect(f);
}

void LRUList::operator()(Bar& bar, Event<const String&> WhenSelect, int count, int from)
{
	if(bar.IsMenuBar() && !lru.IsEmpty()) {
		bar.Separator();
		char n = '1';
		for(int i = from; i < min(from + count, lru.GetCount()); i++) {
			bar.Add(String("&") + n + ' ' + GetFileName(lru[i]),
			        THISBACK2(Select, lru[i], WhenSelect));
			n = n == '9' ? 'A' : n + 1;
		}
	}
}

void LRUList::NewEntry(const String& path)
{
	LruAdd(lru, path, limit);
}

void LRUList::RemoveEntry(const String& path)
{
	int q = FindIndex(lru, path);
	if(q >= 0)
		lru.Remove(q);
}

}
