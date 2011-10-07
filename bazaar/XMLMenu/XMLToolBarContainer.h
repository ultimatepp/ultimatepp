#ifndef _XMLToolBarContainer_h_
#define _XMLToolBarContainer_h_

#include "XMLToolBar.h"

NAMESPACE_UPP

// border of controls containing toolbars
class XMLToolBarFrameBorder : public CtrlFrame
{
	private:
	
		XMLToolBar::XMLToolBarState state;
	
	protected:
	
	public:
	
		void SetState(XMLToolBar::XMLToolBarState _state) { state = _state; }

		// frame layout functions
		virtual void FrameLayout(Rect &r);
		virtual void FrameAddSize(Size &s);
	
		// frame painting
		virtual void FramePaint(Draw& w, const Rect& r);
		
		XMLToolBarFrameBorder();
		
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
class XMLToolBarContainer : public ParentCtrl
{
	private:
	
		XMLToolBarFrameBorder border;
	
	protected:
	
	public:

		void SetState(XMLToolBar::XMLToolBarState state);
		void FrameAddSize(Size &s);
		void FrameLayout(Rect &r);
		XMLToolBarContainer();
};

END_UPP_NAMESPACE

#endif
