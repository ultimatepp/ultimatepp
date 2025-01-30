#include "ide.h"

#ifdef PLATFORM_WIN32
#include "shellapi.h"
#endif

#define TOPICFILE <ide/app.tpp/all.i>
#include <Core/topic_group.h>

#ifndef bmYEAR
#include <build_info.h>
#endif

String SplashCtrl::GenerateVersionInfo(bool qtf)
{
	String h;
	
	char separator = qtf ? '\n' : ' ';

#ifdef bmGIT_HASH
	String rev = bmGIT_HASH;
	String dr = rev;
	if(dr.GetCount() > 8)
		dr.Trim(8);
	h << "Revision: ";
	if(qtf)
		h << "\1[^https://github.com/ultimatepp/ultimatepp/commit/" << rev << "^ ";
	h << dr;
	if(qtf)
		h << "]\1";
	h << separator;
#endif

	h << "Build: " << GenerateVersionNumber();
#ifdef bmGIT_BRANCH
	h << " " << bmGIT_BRANCH << "\n";
#endif

	if(sizeof(void *) == 8)
		h << "64 bit";
	else
		h << "32 bit";
#ifdef _MSC_VER
	h << " MSC";
#endif
#if __GNUC__
#if __clang__
	h << " CLANG";
#else
	h << " GCC";
#endif
#endif

#if __cplusplus >= 202300
	h << " C++23";
#elif __cplusplus >= 202000
	h << " C++20";
#elif __cplusplus >= 201700
	h << " C++17";
#elif __cplusplus >= 201400
	h << " C++14";
#elif __cplusplus >= 201100
	h << " C++11";
#endif

#if CPU_ARM
	h << " ARM";
#endif

#if CPU_SIMD
	h << " SIMD";
#endif

#ifdef GUI_GTK
	h << " Gtk";
#endif
#ifdef FLATPAK
	h << " Flatpak";
#endif
	h << separator;

#ifdef bmTIME
	h << "Compiled: " << bmTIME;
#endif

	h << separator;
	String p = GetExeFilePath();
	if(p.GetCount() > 30)
		p = ".." + p.Mid(max(p.GetCount() - 30, 0));
	h << p;

	return h;
}

String SplashCtrl::GenerateVersionNumber()
{
#ifdef bmSVN_REVISION
	return bmSVN_REVISION;
#endif
#ifdef bmGIT_REVCOUNT
	return AsString(atoi(bmGIT_REVCOUNT) + 2270);
#endif
	return IDE_VERSION;
}

Size SplashCtrl::MakeLogo(Ctrl& parent, Array<Ctrl>& ctrl)
{
	Image logo = IdeImg::logo();
	Size  isz = logo.GetSize();
	ImageCtrl& l = ctrl.Create<ImageCtrl>();
	RichTextCtrl& v1 = ctrl.Create<RichTextCtrl>();
	l.SetImage(logo);
	Size sz = Size(isz.cx, isz.cy/* + 80*/);

	Index<String> classes;
	Index<String> items;

	for(const auto& f : ~CodeIndex())
		for(const AnnotationItem& m : f.value.items) {
			if(IsStruct(m.kind))
				classes.FindAdd(m.id);
			items.FindAdd(m.id);
		}

	String h;
	h << "[A+65 \1" << GenerateVersionInfo(true) << "\n";
	h << "Using: " << MemoryUsedKb()
#ifdef PLATFORM_COCOA
		<< " KB of U++ heap\n";
#else
		<< " KB\n";
#endif
	if(items.GetCount())
		h << classes.GetCount() << " classes, " << items.GetCount() << " items\n";
	if(IsUHDMode())
		h << "UHD mode";
#if 0
	h << "\n1\n2\n3\n4\n5\n6"; // for size testing
#endif
	v1.NoSb();
	v1 = h;
	v1.HSizePos(DPI(250), DPI(10)).BottomPos(0, DPI(180));
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
	SetTimeCallback(750, [] {
		HideSplash();
	});
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
