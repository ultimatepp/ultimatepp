#include "ide.h"

struct SlideShow;

struct SlideShowView : RichTextView {
	SlideShow *slideshow;
	
	virtual void RightDown(Point p, dword keyflags);
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
};

struct SlideShowSettingsDlg : WithSlideShowSettings<TopWindow> {
	SlideShow *slideshow;

	typedef SlideShowSettingsDlg CLASSNAME;

	SlideShowSettingsDlg();
};

struct SlideShow : TopWindow {
	virtual bool Key(dword key, int count);

	SlideShowSettingsDlg settings;
	SlideShowView   text;
	Vector<String> path;
	int            page;
	int            rp;

	int            zoom = 4;
	int            margins = 1;

	void SetPage();
	void Settings();

	SlideShow();
};

SlideShowSettingsDlg::SlideShowSettingsDlg()
{
	CtrlLayout(*this, "Settings");
	
	vcenter <<= true;
	title <<= false;
	
	vcenter << [=] { slideshow->SetPage(); };
	title << [=] { slideshow->SetPage(); };
	
	zoom_plus.WhenRepeat = zoom_plus ^= [=] { slideshow->Key(K_CTRL|K_ADD, 1); };
	zoom_minus.WhenRepeat = zoom_minus ^= [=] { slideshow->Key(K_CTRL|K_SUBTRACT, 1); };
	margin_plus.WhenRepeat = margin_plus ^= [=] { slideshow->Key(K_ALT|K_ADD, 1); };
	margin_minus.WhenRepeat = margin_minus ^= [=] { slideshow->Key(K_ALT|K_SUBTRACT, 1); };
}

SlideShow::SlideShow()
{
	text.slideshow = settings.slideshow = this;
	FullScreen();
	Add(text.SizePos());
	text.NoHyperlinkDecoration();
	text.NoSb();
	rp = -1;
}

void SlideShow::Settings()
{
	if(settings.IsOpen())
		settings.Close();
	else
		settings.Open(this);
}

void SlideShowView::RightDown(Point p, dword)
{
	slideshow->Settings();
}

void SlideShowView::MouseWheel(Point p, int zdelta, dword keyflags)
{
	if(keyflags & K_CTRL) {
		slideshow->Key(zdelta < 0 ? K_CTRL|K_SUBTRACT : K_CTRL|K_ADD, 1);
		return;
	}
	if(keyflags & K_ALT) {
		slideshow->Key(zdelta < 0 ? K_ALT|K_SUBTRACT : K_ALT|K_ADD, 1);
		return;
	}
	RichTextView::MouseWheel(p, zdelta, keyflags);
}

bool SlideShow::Key(dword key, int count)
{
	switch(key) {
	case K_ENTER:
		Settings();
		return true;
	case K_ESCAPE:
		Break();
		return true;
	case K_LEFT:
	case K_PAGEUP:
	case K_SPACE:
		page--;
		break;
	case K_RIGHT:
	case K_PAGEDOWN:
		page++;
		break;
	case K_ALT|K_ADD:
		margins = min(margins + 1, 100);
		break;
	case K_ALT|K_SUBTRACT:
		margins = max(margins - 1, 1);
		break;
	case K_CTRL|K_ADD:
		zoom = min(zoom + 1, 20);
		break;
	case K_CTRL|K_SUBTRACT:
		zoom = max(zoom - 1, 1);
		break;
	case K_CTRL_V:
		settings.vcenter <<= !settings.vcenter;
		break;
	default:
		return true;
	}
	SetPage();
	return true;
}

void SlideShow::SetPage()
{
	text.SetZoom(Zoom(DPI(zoom), 20));
	text.VCenter(settings.vcenter);
	text.HMargins(8 * margins);
	page = minmax(page, 0, path.GetCount() - 1);
	if(page != rp) {
		rp = page;
		text <<= ReadTopic(LoadFile(path[page])).text;
	}
}

void TopicCtrl::SShow()
{
	SlideShow ss;
	TopicLink tl = ParseTopicLink(GetCurrent());
	if(IsNull(tl.package))
		return;
	String folder = AppendFileName(PackageDirectory(tl.package), tl.group + ".tpp");
	FindFile ff(AppendFileName(folder, "*.tpp"));
	Array<String> l;
	while(ff) {
		if(ff.IsFile())
			l.Add(GetFileTitle(ff.GetName()));
		ff.Next();
	}
	if(l.GetCount() == 0)
		return;
	Sort(l);
	for(int i = 0; i < l.GetCount(); i++) {
		if(tl.topic == l[i])
			ss.page = i;
		ss.path.Add(AppendFileName(folder, l[i] + ".tpp"));
	}
	ss.SetPage();
	ss.Run();
	tl.topic = l[ss.page];
	GoTo(TopicLinkString(tl));
}
