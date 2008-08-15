#ifndef _CppAnalyse_AnalyseGui_h
#define _CppAnalyse_AnalyseGui_h

#include <CtrlLib/CtrlLib.h>

class WarningDisplay : public Display
{
	int limit;
	Color warningColor;
public:
	WarningDisplay(int limit, Color warningColor = Color(255, 128, 128));
	void PaintBackground(Draw& w, const Rect& r, const Value& q,
	                     Color ink, Color paper, dword style) const;
};

class AnalyseGui : public TopWindow
{
public:
	typedef AnalyseGui CLASSNAME;
	MenuBar      menu;
	Splitter     splitter;
	LineEdit     source;
	ArrayCtrl    chart;
	LineEdit     textMetric;
	WarningDisplay ccDisplay, llocDisplay, depthDisplay;

	AnalyseGui();
	void MainMenu(Bar& menu);
	void Open();
	void UpdateMetric();
	void GotoFunction();
};

#endif
