#ifndef _Skulpture_Theme_h_
#define _Skulpture_Theme_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class Theme
{
private:
	friend void ApplyDummy();
	
	TextSettings ini;
	String dir;
	
	bool useButton;
	
	void LoadEditField(EditField::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file);
	void LoadButton(Button::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file);
	void LoadToolButton(ToolButton::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file);
	void LoadMenuBar(MenuBar::Style& d, const VectorMap<String, String>& set, const String& dir);
	void LoadOption0(const VectorMap<String, String>& set, const String& dir, const String& file);
	void LoadOption1(const VectorMap<String, String>& set, const String& dir, const String& file);
	void LoadOption2(const VectorMap<String, String>& set, const String& dir, const String& file);
	void LoadSwitch0(const VectorMap<String, String>& set, const String& dir, const String& file);
	void LoadSwitch1(const VectorMap<String, String>& set, const String& dir, const String& file);
	void LoadScrollBar(ScrollBar::Style& d, const VectorMap<String, String>& set, const String& dir);
	void LoadMultiButton(MultiButton::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file);
	void LoadTabCtrl(TabCtrl::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file);
	void LoadProgress(ProgressIndicator::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file);
	void LoadHeader(HeaderCtrl::Style& d, const VectorMap<String, String>& set, const String& dir, const String& file);
	
	Value StringToObject(const String& s, const String& def = "");
	
	void Load();
	int GetGroup(const String& group);
	
public:
	Theme& Load(const String& fileName);
	Theme& Apply();
	
	bool   HasButton()						{ return GetGroup("button") >= 0; }
	Theme& UseButton(bool use)				{ useButton = use; return *this; }	
		
	Theme(): useButton(true) 			{}
};

#endif
