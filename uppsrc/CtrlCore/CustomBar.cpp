#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace Upp {

extern Event<const TopWindow *, TopWindow::CustomTitleBarMetrics&> custom_titlebar_metrics__;
extern Function<bool (const TopWindow *)> is_custom_titlebar__;
extern Function<Ctrl *(TopWindow *, Color, int)> custom_titlebar_make__;

void TopWindow::Init()
{
	custom_titlebar_metrics__ = [](const TopWindow *tw, TopWindow::CustomTitleBarMetrics& m) {
		if(!tw->custom_bar)
			return;
		m.height = GetGtkTitleBarHeight(tw);
		m.lm = 0;
		m.rm = 0;
		/*
		Image icon = tw->GetIcon();
		if(!IsNull(icon))
			m.lm = DPI(4) + min(icon.GetWidth(), 32);
		m.rm = (tw->IsZoomable() ? 3 : 1) * GetGtkTitleBarButtonWidth();
		*/
	};

	is_custom_titlebar__ = [](const TopWindow *win) {
		return win->IsCustomTitleBar__();
	};
	
	custom_titlebar_make__ = [=](TopWindow *win, Color bk, int mincy) -> Ctrl * {
		return win->MakeCustomTitleBar__(bk, mincy);
	};
}

int Ctrl::GetGtkTitleBarHeight(const TopWindow *tw)
{
	return max(tw->custom_titlebar_cy, IsUHDMode() ? 60 : 31);
}

int Ctrl::GetGtkTitleBarButtonWidth()
{
	return IsUHDMode() ? 94 : 47;
}

void TopWindow::SyncCustomBar()
{
	if(custom_bar_frame)
		custom_bar_frame->Height(GetCustomTitleBarMetrics().height);
	if(custom_bar) {
		SetCustomBarColor(Nvl(custom_titlebar_bk, SColorFace()));
		auto cm = GetCustomTitleBarMetrics();
		custom_bar->VSizePos().HSizePos(DPI(6), DPI(6));
		RefreshFrame(0, 0, GetRect().Width(), cm.height);
	}
}

bool TopWindow::IsCustomTitleBar__() const
{
	return custom_bar;
}

Ctrl *TopWindow::MakeCustomTitleBar__(Color bk, int mincy)
{
	DLOG("===== MakeCustomTitleBar");
	DDUMP(~custom_bar);
	DDUMP(~custom_bar_frame);
	if(!custom_bar) {
		custom_bar_frame.Create();
		custom_bar_frame->Transparent();
		custom_bar.Create();
	}
	DLOG("====");
	DDUMP(~custom_bar);
	DDUMP(~custom_bar_frame);
	if(custom_bar) {
		if(&GetFrame(0) != ~custom_bar_frame)
			RemoveFrame(*custom_bar_frame);
		if(&GetFrame(0) == &NullFrame())
			SetFrame(0, *custom_bar_frame);
		else
			InsertFrame(0, *custom_bar_frame);
		custom_bar_frame->Add(*custom_bar);
	}
	custom_titlebar_bk = bk;
	custom_titlebar_cy = mincy;
	SyncCustomBar();
	return ~custom_bar;
}

}

#endif