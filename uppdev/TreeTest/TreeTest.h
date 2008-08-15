#ifndef _TreeTest_TreeTest_h
#define _TreeTest_TreeTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <TreeTest/TreeTest.lay>
#include <CtrlCore/lay.h>



class TreeTest : public WithTreeTestLayout<TopWindow> {
public:
	typedef TreeTest CLASSNAME;
	TreeTest();


	TreeCtrl m_tree;

	StatusBar status;
	InfoCtrl    			m_pos;
	InfoCtrl				m_val;

	SplitterFrame m_sf;

	ToolBar tool;

	MenuBar menu;

	void MainMenu(Bar& bar);

	bool m_bPan;

	void LeftDown(Point p, dword keyflags);
	void LeftUp(Point p, dword keyflags);
	void ShowCoords(Point pos);
	void MouseMove(Point pos, dword flags);
	void Paint(Draw& w);
	void DrawView(Draw& w);
	void DrawGrid(Draw& w);


	Point MPtoLP(Pointf pt);
	Size MPtoLP(Sizef szf);
	Rect MPtoLP(Rectf r);
	Pointf LPtoMP(Point pt);

	Point m_ptPanLP;
	Rectf m_rcBnds;
	double m_Scale;
	Rect m_rcClient;
	Pointf m_ptPan;
	
	Image m_view;

	double GetMapScale();

};

#endif

