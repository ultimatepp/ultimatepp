#include "Theme.h"
#include <plugin/zip/zip.h>

Value StringToObject(const String& s, const String& file = "") {
	Vector<String> v = Split(s, ' ');
	if (v.GetCount() == 0)
		return Null;
	if (v[0] == "png") {
		Image im = PNGRaster().LoadFileAny(file);
		ImageBuffer img; 
		img = im;
		if (img.IsEmpty())
			return Null;
		
		if (v.GetCount() == 6 && v[1] == "hot") {
			img.SetHotSpot(Point(StrInt(v[2]), StrInt(v[3])));
		 	img.Set2ndSpot(Point(StrInt(v[4]), StrInt(v[5])));
	 	}
	 	return (Image)img;
	}
	else if (v[0] == "color")
		return Color(StrInt(v[1]), StrInt(v[2]), StrInt(v[3]));
	else if (v[0] == "rect")
		return Rect(StrInt(v[1]), StrInt(v[2]), StrInt(v[3]), StrInt(v[3]));
	else if (v[0] == "null")
		return "null";
	else 
		return StrInt(v[0]);
}

inline String GetMap(const VectorMap<String, String>& set, const String& s) {
	int k = set.Find(s);
	return (k >= 0) ? set[k] : "";
}

String MakePath(const String& folder, const String& cat) {
	return folder + DIR_SEP + cat;
}

template <class T>
void SetIfNotNull(T& t, const Value& v) {
	if (v.Is<String>() && (String)v == "null")
		t = Null;
	else
		if (!IsNull(v))
	    	t = v;
}

class ThemeHelper {
private:
	const VectorMap<String, String>& set;
	const String& dir;
	const String& file;
public:
	ThemeHelper(const VectorMap<String, String>& aset, const String& adir, const String& afile):
			set(aset), dir(adir), file(afile)			{};
			
	ThemeHelper& operator ()(int& t, const String& s);
	ThemeHelper& operator ()(bool& t, const String& s);
	ThemeHelper& operator ()(Value& t, const String& s);
	ThemeHelper& operator ()(Rect& t, const String& s);
	ThemeHelper& operator ()(Point& t, const String& s);
	ThemeHelper& operator ()(Color& t, const String& s);
	ThemeHelper& operator ()(Size& t, const String& s);
	
	template <typename T>
	ThemeHelper& operator ()(T v[], String val, int count);			
};

ThemeHelper& ThemeHelper::operator()(int& t, const String& s) {
	Value v = StringToObject(GetMap(set, s));
	if (!v.IsNull() && v.Is<int>())
		t = v;
	return *this;
}

ThemeHelper& ThemeHelper::operator()(bool& t, const String& s) {
	Value v = StringToObject(GetMap(set, s));
	if (!v.IsNull() && v.Is<int>())
		t = v;
	return *this;
}

template <typename T>
ThemeHelper& ThemeHelper::operator ()(T v[], String val, int count) {
	String nv = ToLower(val);
	String all = GetMap(set, nv + "[]");
	Vector<String> ret;
	ret.SetCount(count, "");
	
	if (!all.IsEmpty())
		for (int i = 0; i < count; i++)
			ret[i] = all;
	
	for (int i = 0; i < count; i++) {
		String temp = GetMap(set, nv + "[" + IntStr(i) + "]");
		if (!temp.IsEmpty())
			ret[i] = temp;
	}	
	
	for (int i = 0; i < count; i++) 
		SetIfNotNull(v[i], StringToObject(ret[i], MakePath(dir, file + val + IntStr(i)+ ".png")));
	
	return *this;
}

ThemeHelper& ThemeHelper::operator ()(Value& t, const String& s) {
	String nw = ToLower(s);
	Value v = StringToObject(GetMap(set, nw), MakePath(dir, file  + s + ".png"));
	if (v.Is<String>() && (String)v == "null")
		t = Null;
	else
		if (!IsNull(v))
	    	t = v;
	return *this;
}

ThemeHelper& ThemeHelper::operator ()(Rect& t, const String& s) {
	Value v = StringToObject(GetMap(set, s));
	if (!v.IsNull() && v.Is<Rect>())
		t = v;
	return *this;
}

ThemeHelper& ThemeHelper::operator ()(Point& t, const String& s) {
	Value v = StringToObject(GetMap(set, s));
	if (!v.IsNull() && v.Is<Point>())
		t = v;
	return *this;
}

ThemeHelper& ThemeHelper::operator ()(Color& t, const String& s) {
	Value v = StringToObject(GetMap(set, s));
	if (!v.IsNull() && v.Is<Color>())
		t = v;
	return *this;
}

ThemeHelper& ThemeHelper::operator ()(Size& t, const String& s) {
	Value v = StringToObject(GetMap(set, s));
	if (!v.IsNull() && v.Is<Size>())
		t = v;
	return *this;
}

inline void SetChImg(int i, const Image& img)
{
	if (!img.IsEmpty())
	    CtrlsImg::Set(i, img);
}

Vector<String> Theme::Fill(const VectorMap<String, String>& set, String val, int count) {
	String all = GetMap(set, val + "[]");
	Vector<String> ret;
	ret.SetCount(count, "");
	
	if (!all.IsEmpty())
		for (int i = 0; i < count; i++)
			ret[i] = all;
	
	for (int i = 0; i < count; i++) {
		String temp = GetMap(set, val + "[" + IntStr(i) + "]");
		if (!temp.IsEmpty())
			ret[i] = temp;
	}	
	
	return ret;
}

void Theme::LoadButton(Button::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file)
{
	ThemeHelper(set, dir, file)
		(d.focusmargin,		"focusmargin")
		(d.look,			"Look",			4)
		(d.monocolor,		"monocolor",	4)
		(d.overpaint,		"overpaint")
		(d.pressoffset,		"pressoffset")
		(d.textcolor,		"textcolor",	4)
		(d.transparent,		"transparent")
	;
}

void Theme::LoadScrollBar(ScrollBar::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file) {
	ThemeHelper(set, dir, file)
		(d.hlower,			"HLower",		4)
		(d.hthumb,			"HThumb",		4)
		(d.hupper,			"HUpper",		4)
		(d.vlower,			"VLower",		4)
		(d.vthumb,			"VThumb",		4)
		(d.vupper,			"VUpper",		4)
	;
}

void Theme::LoadOption0(const VectorMap<String, String>& set, const String& dir, const String& file) {
	Vector<String> look = Fill(set, "look");
	 
	SetChImg(CtrlsImg::I_O0, StringToObject(look[0], dir + DIR_SEP + file + IntStr(0)+ ".png"));
	SetChImg(CtrlsImg::I_O0h, StringToObject(look[1], dir + DIR_SEP + file + IntStr(1)+ ".png"));
	SetChImg(CtrlsImg::I_O0p, StringToObject(look[2], dir + DIR_SEP + file + IntStr(2)+ ".png"));
	SetChImg(CtrlsImg::I_O0d, StringToObject(look[3], dir + DIR_SEP + file + IntStr(3)+ ".png"));
}

void Theme::LoadOption1(const VectorMap<String, String>& set, const String& dir, const String& file) {
	Vector<String> look = Fill(set, "look");
	 
	SetChImg(CtrlsImg::I_O1, StringToObject(look[0], dir + DIR_SEP + file + IntStr(0)+ ".png"));
	SetChImg(CtrlsImg::I_O1h, StringToObject(look[1], dir + DIR_SEP + file + IntStr(1)+ ".png"));
	SetChImg(CtrlsImg::I_O1p, StringToObject(look[2], dir + DIR_SEP + file + IntStr(2)+ ".png"));
	SetChImg(CtrlsImg::I_O1d, StringToObject(look[3], dir + DIR_SEP + file + IntStr(3)+ ".png"));
}

void Theme::LoadOption2(const VectorMap<String, String>& set, const String& dir, const String& file) {
	Vector<String> look = Fill(set, "look");
	 
	SetChImg(CtrlsImg::I_O2, StringToObject(look[0], dir + DIR_SEP + file + IntStr(0)+ ".png"));
	SetChImg(CtrlsImg::I_O2h, StringToObject(look[1], dir + DIR_SEP + file + IntStr(1)+ ".png"));
	SetChImg(CtrlsImg::I_O2p, StringToObject(look[2], dir + DIR_SEP + file + IntStr(2)+ ".png"));
	SetChImg(CtrlsImg::I_O2d, StringToObject(look[3], dir + DIR_SEP + file + IntStr(3)+ ".png"));
}

void Theme::LoadSwitch0(const VectorMap<String, String>& set, const String& dir, const String& file) {
	Vector<String> look = Fill(set, "look");
	 
	SetChImg(CtrlsImg::I_S0, StringToObject(look[0], dir + DIR_SEP + file + IntStr(0)+ ".png"));
	SetChImg(CtrlsImg::I_S0h, StringToObject(look[1], dir + DIR_SEP + file + IntStr(1)+ ".png"));
	SetChImg(CtrlsImg::I_S0p, StringToObject(look[2], dir + DIR_SEP + file + IntStr(2)+ ".png"));
	SetChImg(CtrlsImg::I_S0d, StringToObject(look[3], dir + DIR_SEP + file + IntStr(3)+ ".png"));
}

void Theme::LoadSwitch1(const VectorMap<String, String>& set, const String& dir, const String& file) {
	Vector<String> look = Fill(set, "look");
	 
	SetChImg(CtrlsImg::I_S1, StringToObject(look[0], dir + DIR_SEP + file + IntStr(0)+ ".png"));
	SetChImg(CtrlsImg::I_S1h, StringToObject(look[1], dir + DIR_SEP + file + IntStr(1)+ ".png"));
	SetChImg(CtrlsImg::I_S1p, StringToObject(look[2], dir + DIR_SEP + file + IntStr(2)+ ".png"));
	SetChImg(CtrlsImg::I_S1d, StringToObject(look[3], dir + DIR_SEP + file + IntStr(3)+ ".png"));
}

void Theme::LoadToolBar(ToolBar::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file) {
	ThemeHelper(set, dir, file)
		(d.arealook,		"AreaLook")
		(d.look,			"Look")
	;
}

void Theme::LoadToolButton(ToolButton::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file) {
	ThemeHelper(set, dir, file)
		(d.contrast,		"contrast",		6)
		(d.light,			"light",		6)
		(d.look,			"Look",			6)
		(d.offset,			"offset",		6)
		(d.textcolor,		"textcolor",	6)
	;
	ToolBar::StyleDefault().Write().buttonstyle = d;
}

void Theme::LoadMenuBar(MenuBar::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file) {
	ThemeHelper(set, dir, file)
		(d.arealook,		"AreaLook")
		(d.item,			"Item")
		(d.itemtext,		"itemtext")
		(d.leftgap,			"leftgap")
		(d.look,			"Look")
		(d.lsepm,			"lsepm")
		(d.maxiconsize,		"maxiconsize")
		(d.popupbody,		"PopupBody")
		(d.popupframe,		"PopupFrame")
		(d.popupiconbar,	"PopupIconBar")
		(d.pullshift,		"pullshift")
		(d.rsepm,			"rsepm")
		(d.textgap,			"textgap")
		(d.topbar,			"TopBar")
		(d.topitem,			"TopItem",		3)
		(d.topitemtext,		"topitemtext",	3)
		(d.separator.l1,	"separatorl1")
		(d.separator.l2,	"separatorl2")
	;
}

void Theme::LoadEditField(EditField::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file) {
	ThemeHelper(set, dir, file)
		(d.activeedge, 		"activeedge")
		(d.disabled, 		"disabled")
		(d.edge, 			"Edge",			4)
		(d.focus, 			"focus")
		(d.invalid, 		"invalid")
		(d.paper, 			"paper")
		(d.selected, 		"selected")
		(d.selectedtext, 	"selectedtext")
		(d.text, 			"text")
		(d.textdisabled, 	"textdisabled")
	;
}

void Theme::LoadMultiButton(MultiButton::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file) {
	ThemeHelper(set, dir, file)
		(d.activeedge, 		"activeedge")
		(d.border, 			"border")
		(d.edge, 			"Edge",			4)
		(d.fmonocolor, 		"fmonocolor",	4)
		(d.left, 			"Left",			4)
		(d.lmiddle, 		"LMiddle",		4)
		(d.loff, 			"loff")
		(d.look, 			"Look",			4)
		(d.margin, 			"margin")
		(d.monocolor, 		"monocolor",	4)
		(d.overpaint, 		"overpaint")
		(d.pressoffset, 	"pressoffset")
		(d.right, 			"Right",		4)
		(d.rmiddle, 		"RMiddle",		4)
		(d.roff, 			"roff")
		(d.trivial, 		"Trivial",		4)
		(d.trivialborder, 	"trivialborder")
		(d.trivialsep, 		"trivialsep")
		(d.usetrivial, 		"usetrivial")
	;
}

void Theme::LoadTabCtrl(TabCtrl::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file) {
	ThemeHelper(set, dir, file)
		(d.body, 			"Body")
		(d.both, 			"Both",			4)
		(d.edge, 			"edge")
		(d.extendleft, 		"extendleft")
		(d.first, 			"First",		4)
		(d.last, 			"Last",			4)
		(d.margin, 			"margin")
		(d.normal, 			"Normal",		4)
		(d.sel, 			"sel")
		//tabheight
		(d.text_color, 		"text_color",	4)
	;
}
      
void Theme::LoadProgress(ProgressIndicator::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file) {
	ThemeHelper(set, dir, file)
		(d.bound, 			"bound")
		(d.classic, 		"classic")
		(d.hchunk, 			"HChunk")
		(d.hlook, 			"HLook")
		(d.vchunk, 			"VChunk")
		(d.vlook, 			"VLook")
	;
}
        
void Theme::LoadHeader(HeaderCtrl::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file) {
	ThemeHelper(set, dir, file)
		(d.gridadjustment, 	"gridadjustment")
		(d.look, 			"Look",			4)
		(d.pressoffset, 	"pressoffset")
	;
}

Theme& Theme::Load(const String& path) {
	ini.Clear();
	
	if (DirectoryExists(path) && FileExists(AppendFileName(path, "theme.ini"))) {
		folder = path;
		ini.Load(AppendFileName(folder, "theme.ini"));
		return *this;
	}
	
	if (FileExists(path)) {
#ifdef PLATFORM_WIN32
		WString temp = FromUtf8(GetTempPath());
		WStringBuffer b(MAX_PATH);
		GetLongPathNameW(temp, b, MAX_PATH);
		b.Strlen();
		String outdir = ToUtf8((WString)b);
#else
		String outdir = GetTempPath();
#endif
		outdir = AppendFileName(outdir, "UppTheme" + Uuid::Create().ToString() + "\\");
		RealizeDirectory(outdir);
		
		FileUnZip unzip(path);
		while(unzip) {
			String fn = AppendFileName(outdir, unzip.GetPath());
			if(unzip.IsFolder()) {
				RealizeDirectory(fn);
				unzip.SkipFile();
			}
			else {
				RealizePath(fn);
				FileOut out(fn);
				unzip.ReadFile(out);
			}
		}
		
		folder = outdir;
		ini.Load(AppendFileName(outdir, "theme.ini"));
		
		return *this;
	}
	
	return *this;
}

int Theme::GetGroup(const String& group) {
	for (int i = 0; i < ini.GetGroupCount(); i++)
		if (ToLower(ini.GetGroupName(i)) == group)
			return i;
	return -1;
}

Theme& Theme::Apply() {
	for (int i = 0; i < ini.GetGroupCount(); i++) 	{
		String group = ToLower(ini.GetGroupName(i));
		VectorMap<String, String> set;
		for (int k = 0; k < ini.GetKeyCount(i); k++) {
			set.Add(ini.GetKey(i, k), ini.Get(i, k));
		}
		
		if (group  == "button" && useButton)
			LoadButton(Button::StyleNormal().Write(), set, MakePath(folder, "Button"), "Button");
		else if (group  == "okbutton" && useOkButton)
			LoadButton(Button::StyleOk().Write(), set, MakePath(folder, "OkButton"), "OkButton");
		else if (group  == "edgebutton" && useEdgeButton)
			LoadButton(Button::StyleEdge().Write(), set, MakePath(folder, "EdgeButton"), "EdgeButton");
		else if (group  == "leftedgebutton" && useLEdgeButton)
			LoadButton(Button::StyleLeftEdge().Write(), set, MakePath(folder, "LeftEdgeButton"), "LeftEdgeButton");
		else if (group  == "option0" && useOption)
			LoadOption0(set, MakePath(folder, "Option"), "Option0");
		else if (group  == "option1" && useOption)
			LoadOption1(set, MakePath(folder, "Option"), "Option1");
		else if (group  == "option2" && useOption)
			LoadOption2(set, MakePath(folder, "Option"), "Option2");
		else if (group  == "switch0" && useSwitch)
			LoadSwitch0(set, MakePath(folder, "Switch"), "Switch0");
		else if (group  == "switch1" && useSwitch)
			LoadSwitch1(set, MakePath(folder, "Switch"), "Switch1");
		else if (group  == "spininc" && useOption)
			LoadButton(SpinButtons::StyleDefault().Write().inc, set, MakePath(folder, "Spin"), "SpinInc");
		else if (group  == "spindec")
			LoadButton(SpinButtons::StyleDefault().Write().dec, set, MakePath(folder, "Spin"), "SpinDec");
		else if (group  == "scrollupbutton")
			LoadButton(ScrollBar::StyleDefault().Write().up, set, MakePath(folder, "ScrollBar"), "ScrollUpButton");
		else if (group  == "scrolldownbutton")
			LoadButton(ScrollBar::StyleDefault().Write().down, set, MakePath(folder, "ScrollBar"), "ScrollDownButton");
		else if (group  == "scrollleftbutton")
			LoadButton(ScrollBar::StyleDefault().Write().left, set, MakePath(folder, "ScrollBar"), "ScrollLeftButton");
		else if (group  == "scrollrightbutton")
			LoadButton(ScrollBar::StyleDefault().Write().right, set, MakePath(folder, "ScrollBar"), "ScrollRightButton");
		else if (group  == "scrollbar")
			LoadScrollBar(ScrollBar::StyleDefault().Write(), set, MakePath(folder, "ScrollBar"), "Scroll");
		else if (group  == "toolbutton")
			LoadToolButton(ToolButton::StyleDefault().Write(), set, MakePath(folder, "ToolButton"), "ToolButton");
		else if (group  == "menubar")
			LoadMenuBar(MenuBar::StyleDefault().Write(), set, MakePath(folder, "MenuBar"), "Menu");
		else if (group  == "toolbar")
			LoadToolBar(ToolBar::StyleDefault().Write(), set, MakePath(folder, "ToolBar"), "ToolBar");
		else if (group  == "editfield") 
			LoadEditField(EditField::StyleDefault().Write(), set, MakePath(folder, "EditField"), "EditField");
		else if (group  == "viewedge") {
			Value edge;
			ThemeHelper(set, MakePath(folder, "ViewEdge"), "ViewEdge")
				(edge, "Edge")
			;
			ViewEdge_Write(edge);
		}
		else if (group == "dropchoice" && useDropC)
			LoadMultiButton(DropChoice::StyleFrame().Write(), set, MakePath(folder, "DropChoice"), "DropChoice");
		else if (group == "droplist" && useDropL) 
			LoadMultiButton(DropList::StyleDefault().Write(), set, MakePath(folder, "DropList"), "DropList");
		else if (group == "tabctrl" && useTab) 
			LoadTabCtrl(TabCtrl::StyleDefault().Write(), set, MakePath(folder, "TabCtrl"), "Tab");
		else if (group == "progress" && useProgress) 
			LoadProgress(ProgressIndicator::StyleDefault().Write(), set, MakePath(folder, "Progress"), "Progress");
		else if (group == "headerctrl" && useHeader) 
			LoadHeader(HeaderCtrl::StyleDefault().Write(), set, MakePath(folder, "HeaderCtrl"), "Header");
		else if (group  == "colors") {
			String cf = GetMap(set, "colorface");
			if (!cf.IsEmpty())
				SColorFace_Write(StringToObject(cf));
			
			String cs = GetMap(set, "colorshadow");
			if (!cs.IsEmpty())
				SColorShadow_Write(StringToObject(cs));
			
			String cl = GetMap(set, "colorlight");
			if (!cl.IsEmpty())
				SColorLight_Write(StringToObject(cl));
			
			String cm = GetMap(set, "colormark");
			if (!cm.IsEmpty())
				SColorMark_Write(StringToObject(cm));
			
			String cd = GetMap(set, "colordissabled");
			if (!cd.IsEmpty())
				SColorDisabled_Write(StringToObject(cd));
		}
	}
	return *this;
}