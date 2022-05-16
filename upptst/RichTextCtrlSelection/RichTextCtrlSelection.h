#ifndef _RichTextCtrlSelection_RichTextCtrlSelection_h
#define _RichTextCtrlSelection_RichTextCtrlSelection_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <RichTextCtrlSelection/RichTextCtrlSelection.lay>
#include <CtrlCore/lay.h>

class RichTextCtrlSelection : public WithRichTextCtrlSelectionLayout<TopWindow> {
public:
	RichTextCtrlSelection();
};

#endif
