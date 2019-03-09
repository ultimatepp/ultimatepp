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
	virtual void Layout();

	SlideShowSettingsDlg settings;
	SlideShowView   text;
	RichTextView    title;
	Vector<String>  path;
	int             page;
	int             rp;

	int             zoom = 16;
	int             margins = 1;
	int             titleh = DPI(200);

	void SetPage();
	void Settings();
	
	void Serialize(Stream& s);

	SlideShow();
	~SlideShow();
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
	title_plus.WhenRepeat = title_plus ^= [=] { slideshow->Key(K_SHIFT|K_ALT|K_ADD, 1); };
	title_minus.WhenRepeat = title_minus ^= [=] { slideshow->Key(K_SHIFT|K_ALT|K_SUBTRACT, 1); };
}

void SlideShow::Serialize(Stream& s)
{
	int version = 1;
	s / version;
	s % zoom % margins % titleh;
	bool h = ~settings.title;
	s % h;
	settings.title <<= h;
	h = ~settings.vcenter;
	s % h;
	settings.vcenter <<= h;
}

INITBLOCK {
	RegisterGlobalConfig("SlideShow");
};

SlideShow::SlideShow()
{
	text.slideshow = settings.slideshow = this;
	FullScreen();
	Add(text.SizePos());
	text.NoHyperlinkDecoration();
	text.NoSb();
	text.SetFrame(NullFrame());
	rp = -1;
	title.NoSb();
	Add(title);
	title.SetFrame(NullFrame());
	LoadFromGlobal(*this, "SlideShow");
}

SlideShow::~SlideShow()
{
	StoreToGlobal(*this, "SlideShow");
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
	case K_CTRL|K_ALT|K_ADD:
		titleh = min(titleh + 5, DPI(500));
		break;
	case K_CTRL|K_ALT|K_SUBTRACT:
		titleh = max(titleh - 5, 10);
		break;
	case K_ALT|K_ADD:
		margins = min(margins + 1, 100);
		break;
	case K_ALT|K_SUBTRACT:
		margins = max(margins - 1, 1);
		break;
	case K_CTRL|K_ADD:
		zoom = min(zoom + 1, 100);
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

void SlideShow::Layout()
{
	SetPage();
}

void SlideShow::SetPage()
{
	page = minmax(page, 0, path.GetCount() - 1);
	Topic t;
	if(page >= 0)
		t = ReadTopic(LoadFile(path[page]));
	if(settings.title) {
		title <<= "{{1@C~ [*R+" + AsString(titleh) + " " + t.title + "}}";
		title.HMargins(DPI(8) * margins);
		title.VMargins(0);
		int cy = min(title.GetHeight() + DPI(20), GetSize().cx / 2);
		title.HSizePos().TopPos(0, cy);
		text.VSizePos(cy, 0);
		title.Show();
	}
	else {
		text.VSizePos(0, 0);
		title.Hide();
	}

	text.SetZoom(Zoom(DPI(zoom), 100));
	text.VCenter(settings.vcenter);
	text.HMargins(DPI(8) * margins);
	if(page != rp) {
		rp = page;
		text <<= t.text;
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
	ss.page = 0;
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
