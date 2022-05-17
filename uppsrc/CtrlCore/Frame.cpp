#include "CtrlCore.h"

namespace Upp {

void CtrlFrame::FramePaint(Draw& draw, const Rect& r) {}
void CtrlFrame::FrameAdd(Ctrl& ctrl) {}
void CtrlFrame::FrameRemove() {}
int CtrlFrame::OverPaint() const { return 0; }

void NullFrameClass::FrameLayout(Rect& r) {}
void NullFrameClass::FramePaint(Draw& draw, const Rect& r) {}
void NullFrameClass::FrameAddSize(Size& sz) {}

CtrlFrame& NullFrame() { return Single<NullFrameClass>(); }

void MarginFrame::FrameAdd(Ctrl& parent)
{
	owner = &parent;
}

void MarginFrame::FrameRemove()
{
	owner = NULL;
}

void MarginFrame::FrameLayout(Rect& r)
{
	r.left += margins.left;
	r.right -= margins.right;
	r.top += margins.top;
	r.bottom -= margins.bottom;
}

void MarginFrame::FramePaint(Draw& w, const Rect& r)
{
	if(IsNull(color))
		return;
	w.DrawRect(r.left, r.top, r.Width(), margins.top, color);
	w.DrawRect(r.left, r.bottom - margins.bottom, r.Width(), margins.bottom, color);
	int h = r.GetHeight() - margins.top - margins.bottom;
	w.DrawRect(r.left, r.top + margins.top, margins.left, h, color);
	w.DrawRect(r.right - margins.right, r.top + margins.top, margins.right, h, color);
}

void MarginFrame::SetMargins(const Rect& r)
{
	margins = r;
	if(owner)
		owner->RefreshLayout();
}

void MarginFrame::SetColor(Color c)
{
	color = c;
	if(owner)
		owner->RefreshLayout();
}

void MarginFrame::FrameAddSize(Size& sz)
{
	sz.cx += margins.left + margins.right;
	sz.cy += margins.bottom + margins.top;
}

MarginFrame::MarginFrame()
{
	margins = Rect(0, 0, 0, 0);
	color = SColorFace();
	owner = nullptr;
}

void BorderFrame::FrameLayout(Rect& r)
{
	Size sz = r.GetSize();
	int n = (int)(intptr_t)*border;
	if(sz.cx >= 2 * n && sz.cy >= 2 * n)
		r.Deflate(n);
}

void BorderFrame::FrameAddSize(Size& sz)
{
	sz += 2 * (int)(intptr_t)*border;
}

void BorderFrame::FramePaint(Draw& draw, const Rect& r)
{
	Size sz = r.GetSize();
	int n = (int)(intptr_t)*border;
	if(sz.cx >= 2 * n && sz.cy >= 2 * n)
		DrawBorder(draw, r.left, r.top, r.Width(), r.Height(), border);
}

CtrlFrame& InsetFrame() { static BorderFrame h(InsetBorder()); return h; }
CtrlFrame& ThinInsetFrame() { static BorderFrame h(ThinInsetBorder()); return h; }
CtrlFrame& ButtonFrame() { static BorderFrame h(ButtonBorder()); return h; }
CtrlFrame& BlackFrame() { static BorderFrame h(BlackBorder()); return h; }
CtrlFrame& WhiteFrame() { static BorderFrame h(WhiteBorder()); return h; }
CtrlFrame& OutsetFrame() { static BorderFrame h(OutsetBorder()); return h; }
CtrlFrame& ThinOutsetFrame() { static BorderFrame h(ThinOutsetBorder()); return h; }

CH_COLOR(FieldFrameColor, Blend(SColorHighlight, SColorShadow));

class XPFieldFrameCls : public CtrlFrame {
	virtual void FrameLayout(Rect& r)                   { r.Deflate(2); }
	virtual void FramePaint(Draw& w, const Rect& r) {
		DrawFrame(w, r, FieldFrameColor());
		DrawFrame(w, r.Deflated(1), SColorPaper);
	}
	virtual void FrameAddSize(Size& sz) { sz += 4; }
};

class XPEditFieldFrameCls : public CtrlFrame {
	virtual void FrameLayout(Rect& r)                   { r.Deflate(1); }
	virtual void FramePaint(Draw& w, const Rect& r) {
		DrawFrame(w, r, FieldFrameColor());
	}
	virtual void FrameAddSize(Size& sz) { sz += 2; }
};

CtrlFrame& XPFieldFrame() { return Single<XPFieldFrameCls>(); }
CtrlFrame& XPEditFieldFrame() { return Single<XPEditFieldFrameCls>(); }

CH_INT(EditFieldIsThin, 0);

CtrlFrame& FieldFrame() { return GUI_GlobalStyle() >= GUISTYLE_XP ? XPFieldFrame() : InsetFrame(); }

CH_VALUE(TopSeparator1, SColorShadow());
CH_VALUE(TopSeparator2, SColorLight());

class TopSeparatorFrameCls : public CtrlFrame {
	virtual void FrameLayout(Rect& r)                   { r.top += 2; }
	virtual void FramePaint(Draw& w, const Rect& r) {
		ChPaint(w, r.left, r.top, r.Width(), 1, TopSeparator1());
		ChPaint(w, r.left, r.top + 1, r.Width(), 1, TopSeparator2());
	}
	virtual void FrameAddSize(Size& sz) { sz.cy += 2; }
};

class BottomSeparatorFrameCls : public CtrlFrame {
	virtual void FrameLayout(Rect& r)                   { r.bottom -= 2; }
	virtual void FramePaint(Draw& w, const Rect& r) {
		w.DrawRect(r.left, r.bottom - 2, r.Width(), 1, SColorShadow);
		w.DrawRect(r.left, r.bottom - 1, r.Width(), 1, SColorLight);
	}
	virtual void FrameAddSize(Size& sz) { sz.cy += 2; }
};

class LeftSeparatorFrameCls : public CtrlFrame {
	virtual void FrameLayout(Rect& r)                   { r.left += 2; }
	virtual void FramePaint(Draw& w, const Rect& r) {
		w.DrawRect(r.left, r.top, 1, r.Height(), SColorShadow);
		w.DrawRect(r.left + 1, r.top, 1, r.Height(), SColorLight);
	}
	virtual void FrameAddSize(Size& sz) { sz.cx += 2; }
};

class RightSeparatorFrameCls : public CtrlFrame {
	virtual void FrameLayout(Rect& r)                   { r.right -= 2; }
	virtual void FramePaint(Draw& w, const Rect& r) {
		w.DrawRect(r.right - 2, r.top, 1, r.Height(), SColorShadow);
		w.DrawRect(r.right - 1, r.top, 1, r.Height(), SColorLight);
	}
	virtual void FrameAddSize(Size& sz) { sz.cx += 2; }
};

class RightGapFrameCls : public CtrlFrame {
	virtual void FrameLayout(Rect& r)                   { r.right -= DPI(2); }
	virtual void FramePaint(Draw& w, const Rect& r) {
		w.DrawRect(r.right - DPI(2), r.top, DPI(2), r.Height(), SColorFace());
	}
	virtual void FrameAddSize(Size& sz) { sz.cx += DPI(2); }
};

CtrlFrame& BottomSeparatorFrame() { return Single<BottomSeparatorFrameCls>(); }
CtrlFrame& TopSeparatorFrame()    { return Single<TopSeparatorFrameCls>(); }
CtrlFrame& RightSeparatorFrame()  { return Single<RightSeparatorFrameCls>(); }
CtrlFrame& LeftSeparatorFrame()   { return Single<LeftSeparatorFrameCls>(); }

CtrlFrame& RightGapFrame()  { return Single<RightGapFrameCls>(); }

CH_INT(FrameButtonWidth, DPI(17));
CH_INT(ScrollBarArrowSize, FrameButtonWidth());

void LayoutFrameLeft(Rect& r, Ctrl *ctrl, int cx)
{
	if(ctrl) {
		cx *= ctrl->IsShown();
		ctrl->SetFrameRect(r.left, r.top, cx, r.Height());
		r.left += cx;
	}
}

void LayoutFrameRight(Rect& r, Ctrl *ctrl, int cx)
{
	if(ctrl) {
		cx *= ctrl->IsShown();
		ctrl->SetFrameRect(r.right - cx, r.top, cx, r.Height());
		r.right -= cx;
	}
}

void LayoutFrameTop(Rect& r, Ctrl *ctrl, int cy)
{
	if(ctrl) {
		cy *= ctrl->IsShown();
		ctrl->SetFrameRect(r.left, r.top, r.Width(), cy);
		r.top += cy;
	}
}

void LayoutFrameBottom(Rect& r, Ctrl *ctrl, int cy)
{
	if(ctrl) {
		cy *= ctrl->IsShown();
		ctrl->SetFrameRect(r.left, r.bottom - cy, r.Width(), cy);
		r.bottom -= cy;
	}
}

}
