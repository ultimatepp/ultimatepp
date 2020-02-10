#ifndef _CtrlLib_ChCocoMM_h_
#define _CtrlLib_ChCocoMM_h_

#ifndef _CtrlLib_ChMacMM_h_
#define _CtrlLib_ChMacMM_h_

enum ChCocoControlTypes {
	COCO_BUTTON,
	COCO_CHECKBOX,
	COCO_RADIOBUTTON,
	COCO_BEVELBUTTON,
	COCO_POPUPBUTTON,
	COCO_SCROLLTHUMB,
	COCO_SCROLLTRACK,
	COCO_NSCOLOR,
	COCO_NSIMAGE,
	COCO_TEXTFIELD,
};

enum ChCocoNSColor {
	COCO_TEXT,
	COCO_PAPER,
	COCO_SELECTEDTEXT,
	COCO_SELECTEDPAPER,
	COCO_DISABLED,
	COCO_WINDOW,
	COCO_SELECTEDMENUTEXT,
};

void Coco_PaintCh(void *cgcontext, int type, int value, int state);

#endif

#endif
