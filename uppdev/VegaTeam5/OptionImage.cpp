#include "OptionImage.h"

void OptionImage::ReFocus(){
	SetFocus();
}

OptionImage::OptionImage()
{
	WhenAction = THISBACK(ReFocus);
}


void OptionImage::Paint(Draw& w) {
	Size sz = GetSize();
	if(!IsTransparent())
		w.DrawRect(0, 0, sz.cx, sz.cy, SColorFace);
	Size isz = imgYes.GetSize();  //although maybe it would be good to calc from the biggest...?
	Size tsz = GetSmartTextSize(w, label, font);
	int ty = (sz.cy - tsz.cy) / 2; //text up-down
	int iy = (tsz.cy - isz.cy) / 2 + ty;
	bool ds = !IsShowEnabled();

		Image m = (!notnull || threestate) && IsNull(option) ? ds ? imgMaybe
				                      : IsPush() ? imgYes
							                     : imgMaybe :
			      option == 1 ? ds ? (switchimage ? imgYes : imgYes)
				                   : IsPush() ? (switchimage ? imgYes : imgYes)
							                  : (switchimage ? imgYes : imgYes)
				              : ds ? (switchimage ? imgNo : imgNo)
				                   : IsPush() ? (switchimage ? imgNo : imgNo)
							                  : (switchimage ? imgNo : imgNo);
		w.DrawImage(0, iy, m);
	//	if(!switchimage) //for borders - need to adjust sizes...
	//		w.DrawImage(0, iy, ds ? CtrlImg::optionedged : blackedge ? CtrlImg::blackoptionedge : CtrlImg::optionedge);

	DrawSmartText(w, isz.cx + 4, ty, tsz.cx, label, font, ds ? SGray : SBlack, VisibleAccessKeys() ? accesskey : 0);
	//text length problem - tsz.cx - doesn't work? - adjust *.lay!!!
}




////////////// to be removed ////////////////////////
/*
void ArrayCtrlA::mysync()
{
//	ChildGotFocus();

//	Point p = FindCellCtrl(GetFocusCtrl());

//	SetCursor(p.y);
}

ArrayCtrlA::ArrayCtrlA()
{
//	WhenCtrlsAction = THISBACK(mysync);
//	WhenEnterRow = THISBACK(mysync);
}
*/
