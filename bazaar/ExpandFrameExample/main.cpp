#include <CtrlLib/CtrlLib.h>
#include <ExpandFrame/ExpandFrame.h>
using namespace Upp;

#define LAYOUTFILE <ExpandFrameExample/ExpandFrameExample.lay>
#include <CtrlCore/lay.h>

// Simple window definition
class ExpandFrameExample : public TopWindow {
public:
	typedef ExpandFrameExample CLASSNAME;
	ExpandFrameExample();
	
	void OnHorz();
private:
	ExpanderCtrl 					expander;
	WithPane1Layout<ParentCtrl> 	pane1;
	WithPane2Layout<ParentCtrl> 	pane2;
	DocEdit							editor;
};


ExpandFrameExample::ExpandFrameExample()
{
	Size sz = Size(400, 500);
	Title("Expandable Frame Example").Sizeable();
	SetMinSize(sz);
	SetRect(sz);
	        
	// Add ExpanderCtrl to the window
	Add(expander.SizePos());
	
	// Adding layouts to the expanderctrl
	CtrlLayout(pane1);
	pane1.horz <<= THISBACK(OnHorz);
	expander.AddExpander(pane1, true).SetTitle("Pane 1"); // Open 
	
	CtrlLayout(pane2);
	expander.AddExpander(pane2, false).SetTitle("Pane 2"); // Not open
	
	// Add a DocEdit to	the ExpanderCtrl
	editor <<= "ExpanderCtrl example application";
	expander.AddExpander(editor, true, 300).SetTitle("Editor");
}

void ExpandFrameExample::OnHorz()
{
	expander.Horz(pane1.horz);
}

GUI_APP_MAIN
{
	ExpandFrameExample().Run();
}

