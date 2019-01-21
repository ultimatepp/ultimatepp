#include <Core/config.h>

#ifdef PLATFORM_COCOA

#include <Carbon/Carbon.h> 
#include <CtrlLib/CtrlLib.h>
#include <CtrlCore/CocoMM.h>
#include "ChCocoMM.h"

void Coco_ThemePaint(void *cgcontext, const Upp::Rect& r, int type, int value, int state, bool focus)
{
	auto cg = (CGContextRef) cgcontext	;
	if(Upp::IsUHDMode())
		CGContextScaleCTM(cg, 2, 2);
	CGRect cr = CGRectMake(r.left, r.top, r.Width(), r.Height());
	int st = Upp::decode(state,
	                     Upp::CTRL_PRESSED, kThemeStatePressed,
	                     Upp::CTRL_DISABLED, kThemeStateInactive,
	                     kThemeStateActive);

	if(type == COCO_BACKGROUND) {
		HIThemeBackgroundDrawInfo bgi;
		bgi.version = 0;
		bgi.kind = value;
		bgi.state = st;
		HIThemeDrawBackground(&cr, &bgi, cg, kHIThemeOrientationNormal);
	}
	else
	if(type == COCO_MENUITEM) {
		HIThemeMenuItemDrawInfo mdi;
		mdi.version = 0;
		mdi.itemType = kThemeMenuItemPlain|kThemeMenuItemPopUpBackground;
		if(state == Upp::CTRL_DISABLED)
			mdi.state = kThemeMenuDisabled;
		else {
			mdi.state = kThemeMenuActive;
            if(state != Upp::CTRL_NORMAL)
                mdi.state |= kThemeMenuSelected;
		}
		HIThemeDrawMenuItem(&cr, &cr, &mdi, cg, kHIThemeOrientationNormal, &cr);
	}
	else
	if(type == COCO_MENU) {
        HIThemeMenuDrawInfo mni;
		mni.version = 0;
		mni.menuType = kThemeMenuTypePopUp;
		HIThemeDrawMenuBackground(&cr, &mni, cg, kHIThemeOrientationNormal);
	}
	else
	if(type == COCO_NSCOLOR) {
		CGContextSaveGState(cg);
		CGContextSetFillColorWithColor(cg, Upp::decode(value,
			COCO_PAPER, [NSColor textBackgroundColor].CGColor,
			COCO_SELECTEDTEXT, [NSColor selectedTextColor].CGColor,
			COCO_SELECTEDPAPER, [NSColor selectedTextBackgroundColor].CGColor,
			COCO_DISABLED, [NSColor disabledControlTextColor].CGColor,
			COCO_WINDOW, [NSColor windowBackgroundColor].CGColor,
			COCO_SELECTEDMENUTEXT, [NSColor selectedMenuItemTextColor].CGColor,
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
	if(type == COCO_SCROLLTHUMB || type == COCO_SCROLLTRACK) {
		HIThemeTrackDrawInfo tdi;
		memset(&tdi, 0, sizeof(tdi));
		tdi.kind = kThemeScrollBarMedium;
	    tdi.bounds = cr;
		tdi.min = 0;
		tdi.max = 1;
		tdi.value = 0;
		tdi.attributes = kThemeTrackNoScrollBarArrows|
		                 (type == COCO_SCROLLTHUMB ? kThemeTrackHideTrack|kThemeTrackShowThumb : 0)|
		                 (value * kThemeTrackHorizontal);
		tdi.enableState = state == Upp::CTRL_DISABLED ? kThemeTrackDisabled : kThemeTrackActive;
        tdi.trackInfo.scrollbar.viewsize = 200;
        if(type == COCO_SCROLLTHUMB && state == Upp::CTRL_HOT || state == Upp::CTRL_PRESSED)
	        tdi.trackInfo.scrollbar.pressState |= kThemeThumbPressed;

        HIThemeDrawTrack(&tdi, &cr, cg, kHIThemeOrientationNormal);
	}
	else
	if(type == COCO_NSIMAGE) {
		NSImage *img = [NSImage imageNamed:(value ? NSImageNameInfo : NSImageNameCaution)];
	    NSGraphicsContext *gc = [NSGraphicsContext graphicsContextWithCGContext:cg flipped:YES];
	    NSGraphicsContext* cgc = [NSGraphicsContext currentContext];
	    [NSGraphicsContext setCurrentContext:gc];
	    [img drawInRect:NSMakeRect(0, 0, 48, 48)];
		[NSGraphicsContext setCurrentContext:cgc];
	}
	else {
		HIThemeButtonDrawInfo bdi;
		memset(&bdi, 0, sizeof(bdi));
	    bdi.value = Upp::decode(value, 0, kThemeButtonOff, 1, kThemeButtonOn, kThemeButtonMixed);
	    bdi.state = st;
	    bdi.kind = Upp::decode(type, COCO_CHECKBOX, (int)kThemeCheckBox,
	                                 COCO_RADIOBUTTON, (int)kThemeSmallRadioButton,
	                                 COCO_BEVELBUTTON, (int)kThemeBevelButtonMedium,
	                                 COCO_ROUNDEDBUTTON, (int)kThemeRoundedBevelButton,
	                                 COCO_COMBOBOX, (int)kThemeComboBox,
	                                 (int)kThemePushButtonNormal);
	#ifdef _DEBUG
		if(type < 0)
			bdi.kind = -type;
	#endif
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
