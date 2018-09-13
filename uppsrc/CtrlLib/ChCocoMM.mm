#include <CtrlCore/CocoMM.h>

#ifdef PLATFORM_COCOA

#include <Carbon/Carbon.h> 
#include "ChCocoMM.h"
#include <CtrlLib/CtrlLib.h>

void Coco_ThemePaint(void *cgcontext, const Upp::Rect& r, int type, int value, int state, bool focus)
{
	auto cg = (CGContextRef) cgcontext;
	CGRect cr = CGRectMake(r.left, r.top, r.Width(), r.Height());
	if(type == COCO_NSCOLOR) {
		CGContextSaveGState(cg);
		CGContextSetFillColorWithColor(cg, Upp::decode(value,
			COCO_PAPER, [NSColor textBackgroundColor].CGColor,
			COCO_SELECTEDTEXT, [NSColor selectedTextColor].CGColor,
			COCO_SELECTEDPAPER, [NSColor selectedTextBackgroundColor].CGColor,
			COCO_DISABLED, [NSColor disabledControlTextColor].CGColor,
			[NSColor textColor].CGColor
		));
		CGContextFillRect(cg, cr);
		CGContextRestoreGState(cg);
	}
	else
	if(type == COCO_BRUSH) {
		CGContextSaveGState(cg);
		HIThemeSetFill((ThemeBrush)value, NULL, cg, kHIThemeOrientationNormal);
		CGContextFillRect(cg, cr);
		CGContextRestoreGState(cg);
	}
	else
	if(type == COCO_SCROLLTHUMB || type ==COCO_SCROLLTRACK) {
		HIThemeTrackDrawInfo tdi;
		memset(&tdi, 0, sizeof(tdi));
		tdi.kind = kThemeMediumScrollBar;
	    tdi.bounds = cr;
		tdi.min = 0;
		tdi.max = 100;
		tdi.value = 0;
		tdi.attributes = kThemeTrackNoScrollBarArrows|
		                 (type == COCO_SCROLLTHUMB ? kThemeTrackHideTrack|kThemeTrackShowThumb : 0);
		tdi.enableState = state == Upp::CTRL_DISABLED ? kThemeTrackDisabled : kThemeTrackActive;
        tdi.trackInfo.scrollbar.viewsize = 50;
        tdi.trackInfo.scrollbar.pressState = state == Upp::CTRL_PRESSED ? kThemeThumbPressed : 0;

        HIThemeDrawTrack(&tdi, &cr, cg, kHIThemeOrientationNormal);
	}
	else {
		HIThemeButtonDrawInfo bdi;
		memset(&bdi, 0, sizeof(bdi));
	    bdi.value = Upp::decode(value, 0, kThemeButtonOff, 1, kThemeButtonOn, kThemeButtonMixed);
	    bdi.state = Upp::decode(state,// Upp::CTRL_HOT, kThemeStateRollover,
	                                   Upp::CTRL_PRESSED, kThemeStatePressed,
	                                   Upp::CTRL_DISABLED, kThemeStateInactive,
	                                   kThemeStateActive);
	    bdi.kind = Upp::decode(type, COCO_CHECKBOX, (int)kThemeCheckBox,
	                                 COCO_RADIOBUTTON, (int)kThemeRadioButton,
	                                 (int)kThemePushButtonNormal);
	    bdi.adornment = focus ? kThemeAdornmentFocus : kThemeAdornmentNone;
	
		HIThemeDrawButton(&cr, &bdi, cg, kHIThemeOrientationNormal, 0);
	}
}

int   Coco_Metric(int k)
{
	SInt32 m;
	GetThemeMetric(k, &m);
	return m;
}

#endif
