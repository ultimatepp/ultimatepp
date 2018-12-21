#include "ide.h"

#ifdef PLATFORM_WIN32
#include "shellapi.h"
#endif

#define TOPICFILE <ide/app.tpp/all.i>
#include <Core/topic_group.h>

#ifndef bmYEAR
#include <build_info.h>
#endif

String SplashCtrl::GenerateVersionInfo(char separator)
{
	String h;
	
	h << "Version: " << GenerateVersionNumber();
	h << separator;
	if(sizeof(void *) == 8)
		h << "(64 bit)";
	else
		h << "(32 bit)";
#ifdef _MSC_VER
	h << " (MSC)";
#endif
#if __GNUC__
#if __clang__
	h << " (CLANG)";
#else
	h << " (GCC)";
#endif
#endif
#if __cplusplus >= 201100
	h << " (C++11)";
#endif
#if __cplusplus >= 201400
	h << " (C++14)";
#endif
#ifdef GUI_GTK
	h << " (Gtk)";
#endif
	h << separator;
#ifdef bmTIME
	h << "Compiled: " << bmTIME;
#endif

	return h;
}

String SplashCtrl::GenerateVersionNumber()
{
#ifdef bmSVN_REVISION
	return bmSVN_REVISION;
#else
	return IDE_VERSION;
#endif
}

Size SplashCtrl::MakeLogo(Ctrl& parent, Array<Ctrl>& ctrl)
{
	Image logo = IdeImg::logo();
	Size  isz = logo.GetSize();
	ImageCtrl& l = ctrl.Create<ImageCtrl>();
	Label& v1 = ctrl.Create<Label>();
	l.SetImage(logo);
	Size sz = Size(isz.cx, isz.cy/* + 80*/);
	const CppBase& cpp = CodeBase();
	int total = 0;
	for(int i = 0; i < cpp.GetCount(); i++)
		total += cpp[i].GetCount();
	String h;
	h << GenerateVersionInfo() << "\n";
	h << "Using: " << MemoryUsedKb() << " KB\n";
	if(cpp.GetCount())
		h << "CodeBase: " << cpp.GetCount() << " classes, " << total << " items\n";
	if(IsUHDMode())
		h << "UHD mode\n";
	v1 = h;
	v1.HSizePos(DPI(220), DPI(10)).BottomPos(DPI(20), Arial(DPI(20)).GetHeight() * 5);
	v1.SetFont(Arial(DPI(10)));
	v1.SetInk(Black());
	l.Add(v1);
	parent.Add(ctrl.Create<StaticRect>().Color(White).SizePos());
	parent.Add(l.TopPos(0, isz.cy).LeftPos(0, isz.cx));
	return sz;
}

SplashCtrl::SplashCtrl()
{
	SetRect(GetWorkArea().CenterRect(MakeLogo(*this, ctrl) + 2));
	SetFrame(BlackFrame());
}

void HideSplash()
{
	if(Single<SplashCtrl>().IsOpen())
		Single<SplashCtrl>().Close();
}

void ShowSplash()
{
	Single<SplashCtrl>().PopUp(nullptr, false, false);
	SetTimeCallback(750, [] { HideSplash(); });
}

bool IsSplashOpen()
{
	return Single<SplashCtrl>().IsOpen();
}

class AboutDlg : public TopWindow
{
public:
	AboutDlg()
	{
		Size isz = SplashCtrl::MakeLogo(*this, ctrl);
		int cx = min(isz.cx * 2, GetWorkArea().GetWidth());
		SetRect(0, 0, cx, isz.cy);
		about.SetQTF(GetTopic("ide/app/About_en-us"), Zoom(DPI(120), 1024));
		about.SetZoom(Zoom(1, 1));
		about.RightPos(0, cx - isz.cx - DPI(1)).VSizePos();
		about.HMargins(Zx(4));
		about.SetFrame(NullFrame());
		about.NoLazy();
		Background(PaintRect(ColorDisplay(), SColorPaper()));
		Add(about);
		Title("About TheIDE");
	}
	
	bool Key(dword key, int) override
	{
		switch (key) {
			case (K_ALT_M):
				MemoryProfileInfo();
				return true;
			case (K_ESCAPE):
				Close();
				return true;
			default:
				return false;
		}
	}
	
private:
	Array<Ctrl>  ctrl;
	RichTextView about;
};

void Ide::About()
{
	AboutDlg().Execute();
}
