#include "Theme.h"

Theme* sUglyHack = NULL;

void ApplyDummy()
{
	if (sUglyHack == NULL)
		return;
	
	Theme& t = *sUglyHack;
	t.Load();	
}

Value Theme::StringToObject(const String& s, const String& def) {
	Vector<String> v = Split(s, ' ');
	if (v.GetCount() == 0)
		return Null;
	if (v[0] == "png") {
		ImageBuffer img = PNGRaster().LoadFileAny(AppendFileName(dir, def));
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
	else if (v[0] == "null")
		return "null";
	else 
		return StrInt(v[0]);
}

inline void SetChImg(int i, const Image& img)
{
	if (!img.IsEmpty())
	    CtrlsImg::Set(i, img);
}

template <typename T, typename V>
inline void SetIfNotNull(T& t, V v)
{
	if (v.Is<String>() && (String)v == "null")
		t = Null;
	else
		if (!IsNull(v))
	    	t = v;
}

inline String GetMap(const VectorMap<String, String>& set, const String& s) {
	int k = set.Find(s);
	return (k >= 0) ? set[k] : "";
}

Vector<String> Fill(const VectorMap<String, String>& set, String val, int count = 4) {
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
	Vector<String> look = Fill(set, "look");
	for (int i = 0; i < 4; i++) 
		SetIfNotNull(d.look[i], StringToObject(look[i], dir + "\\" + file + IntStr(i)+ ".png"));
	
	Vector<String> text = Fill(set, "textcolor");
	for (int i = 0; i < 4; i++)
		SetIfNotNull(d.textcolor[i], StringToObject(text[i]));

	Vector<String> mono = Fill(set, "monocolor");
	for (int i = 0; i < 4; i++)
		SetIfNotNull(d.monocolor[i], StringToObject(text[i]));
	
	SetIfNotNull(d.focusmargin, StringToObject(GetMap(set, "focusmargin")));;		
}

void Theme::LoadScrollBar(ScrollBar::Style& d, const VectorMap<String, String>& set, const String& dir) {
	Vector<String> vthumb = Fill(set, "vthumb");
	for (int i = 0; i < 4; i++) 
		SetIfNotNull(d.vthumb[i], StringToObject(vthumb[i], dir + "\\" + "ScrollVThumb" + IntStr(i)+ ".png"));
	
	Vector<String> vupper = Fill(set, "vupper");
	for (int i = 0; i < 4; i++) 
		SetIfNotNull(d.vupper[i], StringToObject(vupper[i], dir + "\\" + "ScrollVUpper" + IntStr(i)+ ".png"));
	
	Vector<String> vlower = Fill(set, "vlower");
	for (int i = 0; i < 4; i++) 
		SetIfNotNull(d.vlower[i], StringToObject(vlower[i], dir + "\\" + "ScrollVLower" + IntStr(i)+ ".png"));
	
	Vector<String> hthumb = Fill(set, "hthumb");
	for (int i = 0; i < 4; i++) 
		SetIfNotNull(d.hthumb[i], StringToObject(hthumb[i], dir + "\\" + "ScrollHThumb" + IntStr(i)+ ".png"));
	
	Vector<String> hupper = Fill(set, "hupper");
	for (int i = 0; i < 4; i++) 
		SetIfNotNull(d.hupper[i], StringToObject(hupper[i], dir + "\\" + "ScrollHUpper" + IntStr(i)+ ".png"));
	
	Vector<String> hlower = Fill(set, "hlower");
	for (int i = 0; i < 4; i++) 
		SetIfNotNull(d.hlower[i], StringToObject(hlower[i], dir + "\\" + "ScrollHLower" + IntStr(i)+ ".png"));
}

void Theme::LoadOption0(const VectorMap<String, String>& set, const String& dir, const String& file) {
	Vector<String> look = Fill(set, "look");
	 
	SetChImg(CtrlsImg::I_O0, StringToObject(look[0], dir + "\\" + file + IntStr(0)+ ".png"));
	SetChImg(CtrlsImg::I_O0h, StringToObject(look[1], dir + "\\" + file + IntStr(1)+ ".png"));
	SetChImg(CtrlsImg::I_O0p, StringToObject(look[2], dir + "\\" + file + IntStr(2)+ ".png"));
	SetChImg(CtrlsImg::I_O0d, StringToObject(look[3], dir + "\\" + file + IntStr(3)+ ".png"));
}

void Theme::LoadOption1(const VectorMap<String, String>& set, const String& dir, const String& file) {
	Vector<String> look = Fill(set, "look");
	 
	SetChImg(CtrlsImg::I_O1, StringToObject(look[0], dir + "\\" + file + IntStr(0)+ ".png"));
	SetChImg(CtrlsImg::I_O1h, StringToObject(look[1], dir + "\\" + file + IntStr(1)+ ".png"));
	SetChImg(CtrlsImg::I_O1p, StringToObject(look[2], dir + "\\" + file + IntStr(2)+ ".png"));
	SetChImg(CtrlsImg::I_O1d, StringToObject(look[3], dir + "\\" + file + IntStr(3)+ ".png"));
}

void Theme::LoadSwitch0(const VectorMap<String, String>& set, const String& dir, const String& file) {
	Vector<String> look = Fill(set, "look");
	 
	SetChImg(CtrlsImg::I_S0, StringToObject(look[0], dir + "\\" + file + IntStr(0)+ ".png"));
	SetChImg(CtrlsImg::I_S0h, StringToObject(look[1], dir + "\\" + file + IntStr(1)+ ".png"));
	SetChImg(CtrlsImg::I_S0p, StringToObject(look[2], dir + "\\" + file + IntStr(2)+ ".png"));
	SetChImg(CtrlsImg::I_S0d, StringToObject(look[3], dir + "\\" + file + IntStr(3)+ ".png"));
}

void Theme::LoadSwitch1(const VectorMap<String, String>& set, const String& dir, const String& file) {
	Vector<String> look = Fill(set, "look");
	 
	SetChImg(CtrlsImg::I_S1, StringToObject(look[0], dir + "\\" + file + IntStr(0)+ ".png"));
	SetChImg(CtrlsImg::I_S1h, StringToObject(look[1], dir + "\\" + file + IntStr(1)+ ".png"));
	SetChImg(CtrlsImg::I_S1p, StringToObject(look[2], dir + "\\" + file + IntStr(2)+ ".png"));
	SetChImg(CtrlsImg::I_S1d, StringToObject(look[3], dir + "\\" + file + IntStr(3)+ ".png"));
}

void Theme::LoadToolButton(ToolButton::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file) {
	Vector<String> look = Fill(set, "look", 6);
	for (int i = 0; i < 6; i++) 
		SetIfNotNull(d.look[i], StringToObject(look[i], dir + "\\" + file + IntStr(i)+ ".png"));
	
	Vector<String> text = Fill(set, "textcolor", 6);
	for (int i = 0; i < 6; i++)
		SetIfNotNull(d.textcolor[i], StringToObject(text[i]));
}

void Theme::LoadMenuBar(MenuBar::Style& d, const VectorMap<String, String>& set, const String& dir) {
	SetIfNotNull(d.look, StringToObject(GetMap(set, "look")));
	SetIfNotNull(d.arealook, StringToObject(GetMap(set, "arealook")));
	SetIfNotNull(d.itemtext, StringToObject(GetMap(set, "itemtext")));
	SetIfNotNull(d.leftgap, StringToObject(GetMap(set, "leftgap")));
	SetIfNotNull(d.popupiconbar, StringToObject(GetMap(set, "popupiconbar")));
	SetIfNotNull(d.popupbody, StringToObject(GetMap(set, "popupbody")));
	SetIfNotNull(d.separator.l1, StringToObject(GetMap(set, "separatorl1")));
	SetIfNotNull(d.separator.l2, StringToObject(GetMap(set, "separatorl2")));
	
	SetIfNotNull(d.item, StringToObject(GetMap(set, "item"), dir + "\\" + "MenuItem.png"));
	
	Vector<String> ti = Fill(set, "topitem", 3);
	for (int i = 0; i < 3; i++)
		SetIfNotNull(d.topitem[i], StringToObject(ti[i], dir + "\\" + "MenuTop" + IntStr(i)+ ".png"));
	
	Vector<String> tt = Fill(set, "topitemtext", 3);
	for (int i = 0; i < 3; i++)
		SetIfNotNull(d.topitemtext[i], StringToObject(tt[i]));
	
	SetIfNotNull(d.popupframe, StringToObject(GetMap(set, "popupframe"), dir + "\\" + "MenuFrame.png"));
}

void Theme::LoadEditField(EditField::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file) {
	Vector<String> edge = Fill(set, "edge");
	for (int i = 0; i < 4; i++) 
		SetIfNotNull(d.edge[i], StringToObject(edge[i], dir + "\\" + file + IntStr(i)+ ".png"));
}

void Theme::LoadMultiButton(MultiButton::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file) {
	Vector<String> look = Fill(set, "look", 4);
	for (int i = 0; i < 4; i++) 
		SetIfNotNull(d.look[i], StringToObject(look[i], dir + "\\" + file + "Look" + IntStr(i)+ ".png"));
	Vector<String> trivial = Fill(set, "trivial", 4);
	for (int i = 0; i < 4; i++) 
		SetIfNotNull(d.trivial[i], StringToObject(trivial[i], dir + "\\" + file + "Trivial" + IntStr(i)+ ".png"));
	Vector<String> edge = Fill(set, "edge", 4);
	for (int i = 0; i < 4; i++) 
		SetIfNotNull(d.edge[i], StringToObject(edge[i], dir + "\\" + file + "Edge" + IntStr(i)+ ".png"));
	SetIfNotNull(d.trivialborder, StringToObject(GetMap(set, "trivialborder")));
}

void Theme::LoadTabCtrl(TabCtrl::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file) {
	SetIfNotNull(d.body, StringToObject(GetMap(set, "body"), dir + "\\" + file + "Body.png"));
	
	Vector<String> first = Fill(set, "first", 4);
	for (int i = 0; i < 4; i++) 
		SetIfNotNull(d.first[i], StringToObject(first[i], dir + "\\" + file + "First" + IntStr(i)+ ".png"));
	
	Vector<String> normal = Fill(set, "normal", 4);
	for (int i = 0; i < 4; i++) 
		SetIfNotNull(d.normal[i], StringToObject(normal[i], dir + "\\" + file + "Normal" + IntStr(i)+ ".png"));
	
	Vector<String> last = Fill(set, "last", 4);
	for (int i = 0; i < 4; i++) 
		SetIfNotNull(d.last[i], StringToObject(last[i], dir + "\\" + file + "Last" + IntStr(i)+ ".png"));
	
	d.sel.top = 0;
}
                     
void Theme::Load(const String& path) {
	ini.Clear();
	
	dir = path;
	ini.Load(AppendFileName(dir, "theme.ini"));
	
	sUglyHack = this;
	Ctrl::SetSkin(ApplyDummy);
	sUglyHack = NULL;
}

void Theme::Load() {
	for (int i = 0; i < ini.GetGroupCount(); i++) 	{
		String group = ToLower(ini.GetGroupName(i));
		VectorMap<String, String> set;
		for (int k = 0; k < ini.GetKeyCount(i); k++) {
			set.Add(ini.GetKey(i, k), ini.Get(i, k));
		}
		
		if (group  == "button")
			LoadButton(Button::StyleNormal().Write(), set, "Button", "Button");
		else if (group  == "okbutton")
			LoadButton(Button::StyleOk().Write(), set, "OkButton", "OkButton");
		else if (group  == "edgebutton")
			LoadButton(Button::StyleEdge().Write(), set, "EdgeButton", "EdgeButton");
		else if (group  == "option0")
			LoadOption0(set, "Option", "Option0");
		else if (group  == "option1")
			LoadOption1(set, "Option", "Option1");
		else if (group  == "switch0")
			LoadSwitch0(set, "Switch", "Switch0");
		else if (group  == "switch1")
			LoadSwitch1(set, "Switch", "Switch1");
		else if (group  == "spininc")
			LoadButton(SpinButtons::StyleDefault().Write().inc, set, "Spin", "SpinInc");
		else if (group  == "spindec")
			LoadButton(SpinButtons::StyleDefault().Write().dec, set, "Spin", "SpinDec");
		else if (group  == "scrollupbutton")
			LoadButton(ScrollBar::StyleDefault().Write().up, set, "ScrollBar", "ScrollUpButton");
		else if (group  == "scrolldownbutton")
			LoadButton(ScrollBar::StyleDefault().Write().down, set, "ScrollBar", "ScrollDownButton");
		else if (group  == "scrollleftbutton")
			LoadButton(ScrollBar::StyleDefault().Write().left, set, "ScrollBar", "ScrollLeftButton");
		else if (group  == "scrollrightbutton")
			LoadButton(ScrollBar::StyleDefault().Write().right, set, "ScrollBar", "ScrollRightButton");
		else if (group  == "scrollbar")
			LoadScrollBar(ScrollBar::StyleDefault().Write(), set, "ScrollBar");
		else if (group  == "toolbutton")
			LoadToolButton(ToolButton::StyleDefault().Write(), set, "ToolButton", "ToolButton");
		else if (group  == "menubar")
			LoadMenuBar(MenuBar::StyleDefault().Write(), set, "MenuBar");
		else if (group  == "editfield") {
			LoadEditField(EditField::StyleDefault().Write(), set, "EditField", "EditField");
			ViewEdge_Write(EditField::StyleDefault().edge[0]);
		}
		else if (group == "dropchoice")
			LoadMultiButton(DropChoice::StyleFrame().Write(), set, "DropChoice", "DropChoice");
		else if (group == "droplist") 
			LoadMultiButton(DropList::StyleDefault().Write(), set, "DropList", "DropList");
		else if (group == "tabctrl") 
			LoadTabCtrl(TabCtrl::StyleDefault().Write(), set, "TabCtrl", "Tab");
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
		}
	}
}