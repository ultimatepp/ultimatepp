#include "ScatterCtrl.h"

NAMESPACE_UPP

#define IMAGECLASS ScatterImg
#define IMAGEFILE <ScatterCtrl/ScatterCtrl.iml>
#include <Draw/iml.h>

#define TFILE <ScatterCtrl/ScatterCtrl.t>
#include <Core/t.h>

#ifdef PLATFORM_WIN32

void ScatterCtrl::SaveAsMetafile(const char* file)
{
	GuiLock __;
	WinMetaFileDraw wmfd;	
	wmfd.Create(copyRatio*ScatterDraw::GetSize().cx, copyRatio*ScatterDraw::GetSize().cy, "ScatterCtrl", "chart", file);
	SetDrawing<Draw>(wmfd, ScatterDraw::GetSize(), copyRatio);	
	wmfd.Close();	
}

void ScatterCtrl::SaveToClipboard(bool saveAsMetafile) 
{
	GuiLock __;
	if (saveAsMetafile) {
		WinMetaFileDraw wmfd;	
		wmfd.Create(copyRatio*ScatterDraw::GetSize().cx, copyRatio*ScatterDraw::GetSize().cy, "ScatterCtrl", "chart");
		SetDrawing<Draw>(wmfd, ScatterDraw::GetSize(), copyRatio);	
		WinMetaFile wmf = wmfd.Close();	 
		wmf.WriteClipboard();
	} else {
		Image img = GetImage(ScatterDraw::GetSize(), copyRatio);
		WriteClipboardImage(img);	
	}
}
#else

void ScatterCtrl::SaveToClipboard(bool) 
{
	GuiLock __;
	Image img = GetImage(ScatterDraw::GetSize(), copyRatio);
	WriteClipboardImage(img);
}

#endif

void ScatterCtrl::Paint(Draw& w) 
{
	GuiLock __;
	if (IsNull(highlight_0) && highlighting) {
		highlighting = false;
		KillTimeCallback();
	}
	if (!IsNull(highlight_0) && !highlighting) {
		highlighting = true;
		SetTimeCallback(-200, THISBACK(TimerCallback));
	}
	TimeStop t;
	lastRefresh0_ms = GetTickCount();
	if (IsEnabled()) {
		if (mode == MD_DRAW) {
			ScatterCtrl::SetDrawing(w, GetSize(), 1);
			PlotTexts(w, GetSize(), 1);
		} else {
			ImageBuffer ib(GetSize());
			BufferPainter bp(ib, mode);
			ScatterCtrl::SetDrawing(bp, GetSize(), 1);
			w.DrawImage(0, 0, ib);
			PlotTexts(w, GetSize(), 1);
		}
		if (HasFocus()) {
			w.DrawLine(0, 0, GetSize().cx, 0, 2, LtGray());
			w.DrawLine(0, 0, 0, GetSize().cy, 2, LtGray());
			int delta = -2;
#ifdef PLATFORM_WIN32
			delta = 0;
#endif
			w.DrawLine(GetSize().cx+delta, 0, GetSize().cx+delta, GetSize().cy, 2, LtGray());
			w.DrawLine(0, GetSize().cy+delta, GetSize().cx, GetSize().cy+delta, 2, LtGray());
		}
	}
	lastRefresh_ms = t.Elapsed();
}

void ScatterCtrl::TimerCallback() {
	Refresh();
}

void ScatterCtrl::ProcessPopUp(const Point & pt)
{
	double _x  = (popLT.x - hPlotLeft)*xRange/(GetSize().cx - (hPlotLeft + hPlotRight)-1) + xMin;		
	double _y  = -(popLT.y - vPlotTop - titleHeight)*yRange/(GetSize().cy - (vPlotTop + vPlotBottom + titleHeight)+1) + yMin + yRange;		
	double _y2 = -(popLT.y - vPlotTop - titleHeight)*yRange2/(GetSize().cy - (vPlotTop + vPlotBottom + titleHeight)+1) + yMin2 + yRange2;		
	double x   = (pt.x - hPlotLeft)*xRange/(GetSize().cx - (hPlotLeft + hPlotRight)-1) + xMin;		
	double y   = -(pt.y - vPlotTop - titleHeight)*yRange/(GetSize().cy - (vPlotTop + vPlotBottom + titleHeight)+1) + yMin + yRange;		
	double y2  = -(pt.y - vPlotTop - titleHeight)*yRange2/(GetSize().cy - (vPlotTop + vPlotBottom + titleHeight)+1) + yMin2 + yRange2;		
	
	double dx  = fabs(x  - _x);
	double dy  = fabs(y  - _y);
	double dy2 = fabs(y2 - _y2);

	String strx, _strx, dstrx, stry, _stry, dstry;
	if (cbModifFormatX) {
		cbModifFormatX(strx,  0, x); 		
		strx.Replace("\n", " ");
		cbModifFormatX(_strx, 0, _x); 		
		_strx.Replace("\n", " ");
	} else {
		strx  = VariableFormatX(x);
		_strx = VariableFormatX(_x);
	}
	if (cbModifFormatDeltaX) {
		cbModifFormatDeltaX(dstrx, 0, dx);	
		dstrx.Replace("\n", " "); 
	} else 
		dstrx = VariableFormatX(dx);
	
	if (cbModifFormatY) {
		cbModifFormatY(stry,  0, y);		
		stry.Replace("\n", " ");
		cbModifFormatY(_stry, 0, _y);		
		_stry.Replace("\n", " ");
	} else {
		stry  = VariableFormatY(y);
		_stry = VariableFormatY(_y);
	}
	if (cbModifFormatDeltaY) {
		cbModifFormatDeltaY(dstry, 0, dy);	
		dstry.Replace("\n", " ");
	} else 
		dstry = VariableFormatY(dy);
	
	String str = popTextX + ": " + _strx;
	if (strx != _strx)
		str << "; " + popTextX + "': " + strx + "; Δ" + popTextX + ": " + dstrx;
	str << "\n" + popTextY + ": " + _stry;
	if (stry != _stry)	
 		str << "; " + popTextY + "': " + stry + "; Δ" + popTextY + ": " + dstry;
	if (drawY2Reticle) {
		String stry2, _stry2, dstry2;
		if (cbModifFormatY2) {
			cbModifFormatY2(stry2,  0, y2);			
			stry2.Replace("\n", " ");
			cbModifFormatY2(_stry2, 0, _y2);		
			_stry2.Replace("\n", " ");
		} else {
			stry2  = VariableFormatY2(y2);
			_stry2 = VariableFormatY2(_y2);
		}
		if (cbModifFormatDeltaY2) {
			cbModifFormatDeltaY2(dstry2, 0, dy2);	
			dstry2.Replace("\n", " ");
		} else 
			dstry2 = VariableFormatY(dy2);
		
		str << "\n" + popTextY2 + ": " + _stry2;
		if (stry2 != _stry2)		
			str << "; " + popTextY2 + ": " + stry2 + "; Δ" + popTextY2 + ": " + dstry2;
	}
	const Point p2 = pt + offset;
	popText.SetText(str).Move(this, p2.x, p2.y);
}

void ScatterCtrl::DoMouseAction(bool down, Point pt, ScatterAction action, int value)
{
	if (!down) {
		Scrolling(false, pt);
		LabelPopUp(false, pt);
		ZoomWindow(false, pt);
	}
	switch (action) {
	case SCROLL: 		Scrolling(down, pt);
						break;
	case ZOOM_H_ENL:	 
	case ZOOM_H_RED:	MouseZoom(value, true, false); 
						break;
	case ZOOM_V_ENL:
	case ZOOM_V_RED:	MouseZoom(value, false, true); 
						break;
	case SHOW_COORDINATES:	LabelPopUp(down, pt);
							break;
	case ZOOM_WINDOW:	ZoomWindow(down, pt);
						break;						
	case CONTEXT_MENU:	if(showContextMenu)
							MenuBar::Execute(THISBACK(ContextMenu));
						break;
	case NO_ACTION:;
	}
}

void ScatterCtrl::DoKeyAction(ScatterAction action)
{
	switch (action) {
	case ZOOM_H_ENL:	MouseZoom(-120, true, false);		break;
	case ZOOM_H_RED:	MouseZoom(120, true, false); 		break;
	case ZOOM_V_ENL:	MouseZoom(-120, false, true); 		break;
	case ZOOM_V_RED:	MouseZoom(120, false, true); 		break;
	case SCROLL_LEFT:	ScatterDraw::Scroll(0.2, 0); 		break;
	case SCROLL_RIGHT:	ScatterDraw::Scroll(-0.2, 0); 		break;
	case SCROLL_UP:		ScatterDraw::Scroll(0, -0.2); 		break;
	case SCROLL_DOWN:	ScatterDraw::Scroll(0, 0.2); 		break;
	case NO_ACTION:;
	}	
}

void ScatterCtrl::AddMouseBehavior(bool ctrl, bool alt, bool shift, bool left, bool middle, int middlewheel, bool right, ScatterAction action)
{
	mouseBehavior << MouseBehavior(ctrl, alt, shift, left, middle, middlewheel, right, action);
}

void ScatterCtrl::RemoveMouseBehavior(ScatterAction action)	
{
	for (int i = mouseBehavior.GetCount() - 1; i >= 0 ; --i)
		if (mouseBehavior[i].action == action) 
			mouseBehavior.Remove(i);
}

void ScatterCtrl::ClearMouseBehavior()
{
	mouseBehavior.Clear();
}	

#ifdef PLATFORM_POSIX
int GetKeyCodeX(int key);
#endif

void ScatterCtrl::AddKeyBehavior(bool ctrl, bool alt, bool shift, int key, bool isVirtualKey, ScatterAction action) 
{
	if (!isVirtualKey) {
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
		HKL hKeyboardLayout = ::GetKeyboardLayout(0);
		key = VkKeyScanExW(key, hKeyboardLayout) + K_DELTA;
	}
#else
		key = key + K_DELTA;//GetKeyCodeX(key);
	}
#endif
	keyBehavior << KeyBehavior(ctrl, alt, shift, key, isVirtualKey, action);
}

void ScatterCtrl::RemoveKeyBehavior(ScatterAction action) 
{
	for (int i = keyBehavior.GetCount() - 1; i >= 0 ; --i)
		if (keyBehavior[i].action == action) 
			keyBehavior.Remove(i);
}

void ScatterCtrl::ClearKeyBehavior() 
{
	keyBehavior.Clear();
}

void ScatterCtrl::ProcessMouse(bool down, Point &pt, bool ctrl, bool alt, bool shift, bool left, bool middle, int middleWheel, bool right) 
{
	for (int i = 0; i < mouseBehavior.GetCount(); ++i) {
		if (mouseBehavior[i].ctrl   == ctrl   && mouseBehavior[i].alt   == alt   &&
		    mouseBehavior[i].shift  == shift  && mouseBehavior[i].left  == left  &&
		    mouseBehavior[i].middle == middle && mouseBehavior[i].right == right &&
		    ((mouseBehavior[i].middleWheel == 0) || mouseBehavior[i].middleWheel == ((middleWheel > 0) - (middleWheel < 0))))
		    DoMouseAction(down, pt, mouseBehavior[i].action, middleWheel);
	}	
}

bool ScatterCtrl::ProcessKey(int key) 
{
	if (key & K_KEYUP)
		return true;
	bool ctrl = key & K_CTRL;
	bool alt = key & K_ALT;
	bool shift = key & K_SHIFT;
	key &= ~(K_CTRL | K_ALT | K_SHIFT | K_KEYUP); 
	bool processed = false;
	for (int i = 0; i < keyBehavior.GetCount(); ++i) {
		if (keyBehavior[i].ctrl  == ctrl  && keyBehavior[i].alt == alt && keyBehavior[i].shift == shift && keyBehavior[i].key == key) {
			processed = true;
		    DoKeyAction(keyBehavior[i].action);
		}
	}
	return processed;
}

bool ScatterCtrl::PointInPlot(Point &pt) 
{
	return 	hPlotLeft <= pt.x && 				pt.x <= (GetSize().cx - hPlotRight) && 
		  	(vPlotTop + titleHeight) <= pt.y && pt.y <= (GetSize().cy - vPlotBottom);
}

void ScatterCtrl::LabelPopUp(bool down, Point &pt) 
{
	if (down) {
		if(showInfo && PointInPlot(pt)) {
			popText.AppearOnly(this);
			
			isLabelPopUp = true;
			if (IsNull(popLT))
				popLT = pt;
			popRB = pt;
			Rect wa = GetWorkArea();
			Rect rc = GetScreenRect();
			if (wa.right - (rc.left + pt.x) < 200)
				pt.x -= 200;
			if (wa.bottom - (rc.top + pt.y) < 200)
				pt.y -= 200;
			ProcessPopUp(pt);		
		} 
	} else {
		if(showInfo && isLabelPopUp) {
			popText.Close();
			isLabelPopUp = isZoomWindow = false;
			popLT = popRB = Null;
			Refresh();
		}		
	}
}

void ScatterCtrl::ZoomWindow(bool down, Point &pt) 
{
	if (down) {
		if (PointInPlot(pt)) {
			isZoomWindow = true;
			if (IsNull(popLT))
				popLT = pt;
			popRB = pt;
		}
	} else {
		if(isZoomWindow) {
			isLabelPopUp = isZoomWindow = false;
			
			if (popLT.x > popRB.x)
				Swap(popLT, popRB);
			double LTx, LTy, LTy2, RBx, RBy, RBy2;
			LTx = (popLT.x - hPlotLeft)*xRange/(GetSize().cx - (hPlotLeft + hPlotRight)-1) + xMin;		
			LTy = -(popLT.y - vPlotTop - titleHeight)*yRange/(GetSize().cy - (vPlotTop + vPlotBottom + titleHeight)+1) + yMin + yRange;		
			LTy2 = -(popLT.y - vPlotTop - titleHeight)*yRange2/(GetSize().cy - (vPlotTop + vPlotBottom + titleHeight)+1) + yMin2 + yRange2;		
			RBx = (popRB.x - hPlotLeft)*xRange/(GetSize().cx - (hPlotLeft + hPlotRight)-1) + xMin;		
			RBy = -(popRB.y - vPlotTop - titleHeight)*yRange/(GetSize().cy - (vPlotTop + vPlotBottom + titleHeight)+1) + yMin + yRange;		
			RBy2 = -(popRB.y - vPlotTop - titleHeight)*yRange2/(GetSize().cy - (vPlotTop + vPlotBottom + titleHeight)+1) + yMin2 + yRange2;		
			
			if ((RBx - LTx) <= 0 || (LTy - RBy) <= 0 || (LTy2 - RBy2) <= 0)
				return;
			
			SetXYMinLinked(LTx, RBy, RBy2);
			SetRangeLinked(RBx - LTx, LTy - RBy, LTy2 - RBy2);

			popLT = popRB = Null;
			Refresh();
			Refresh();
		}		
	}	
}

#ifdef PLATFORM_LINUX
	#include <X11/cursorfont.h>
#endif

void ScatterCtrl::Scrolling(bool down, Point &pt, bool isOut)
{
	static Image mouseImg;
	if (down) {
		if((mouseHandlingX || mouseHandlingY) && PointInPlot(pt)) {
			butDownX = pt.x;
			butDownY = pt.y;	
			isScrolling = true;
			INTERLOCKED { 
				mouseImg = Ctrl::OverrideCursor(Image::SizeAll());
			}
		}
	} else {
		if (isScrolling) {
			if (!isOut)
				MouseMove(pt, 0);
			isScrolling = false;
			Ctrl::OverrideCursor(mouseImg);
		} 
	}
}

bool ScatterCtrl::Key(dword key, int count)
{
	if (!ProcessKey(key)) {
		if (key == K_CTRL_P)
			DoShowEditDlg();
		else if (key == K_CTRL_D)
			DoShowData();
		else if (key == K_CTRL_C)
			SaveToClipboard(false);
		else if (key == K_CTRL_S)
			SaveToFile(Null);
		else
			return false;
	}
	return true;
}

void ScatterCtrl::GotFocus()
{
	Refresh();
}

void ScatterCtrl::LostFocus()
{
	Refresh();	
}

void ScatterCtrl::LeftDown(Point pt, dword keyFlags) 
{
	if(!HasFocus()) 
		SetFocus();
	isLeftDown = true;
	ProcessMouse(true, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, true, false, 0, false);
}

void ScatterCtrl::LeftDouble(Point pt, dword)
{
	if(!HasFocus()) 
		SetFocus();
	if (!PointInPlot(pt))
		MenuBar::Execute(THISBACK(ContextMenu));
}

void ScatterCtrl::LeftUp(Point pt, dword keyFlags)
{
	isLeftDown = false;
	ProcessMouse(false, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, true, false, 0, false); 
}

void ScatterCtrl::MiddleDown(Point pt, dword keyFlags)
{
	if(!HasFocus()) 
		SetFocus();
	ProcessMouse(true, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, false, true, 0, false);
}

void ScatterCtrl::MiddleUp(Point pt, dword keyFlags)
{
	ProcessMouse(false, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, false, true, 0, false);
}

void ScatterCtrl::RightDown(Point pt, dword keyFlags) 
{
	if(!HasFocus()) 
		SetFocus();
	ProcessMouse(true, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, false, false, 0, true);
}

void ScatterCtrl::RightUp(Point pt, dword keyFlags)
{
	ProcessMouse(false, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, false, false, 0, true); 
}

void ScatterCtrl::MouseWheel(Point pt, int zdelta, dword keyFlags) 
{
	if (zdelta == 0)
		return;
	if(!HasFocus()) 
		SetFocus();
	ProcessMouse(true, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, false, false, zdelta, false);
}

void ScatterCtrl::MouseMove(Point pt, dword)
{
	if (isScrolling) {
		double factorX = 0, factorY = 0;
		int shiftX = pt.x - butDownX;
		if (mouseHandlingX && shiftX != 0) 
			factorX = double(shiftX)/(GetSize().cx - (hPlotLeft + hPlotRight) - 1);
		int shiftY = pt.y - butDownY;
		if (mouseHandlingY && shiftY != 0) 
			factorY = double(shiftY)/(GetSize().cy - (vPlotTop + vPlotBottom) - 1);
		butDownX = pt.x;
		butDownY = pt.y;
		if ((mouseHandlingX && shiftX != 0) || (mouseHandlingY && shiftY != 0)) 
			ScatterDraw::Scroll(factorX, -factorY);
	} 
	if(isLabelPopUp) {
		if (showInfo && PointInPlot(pt)) {
			if (IsNull(popLT))
				popLT = pt;
			popRB = pt;
			ProcessPopUp(pt);
			popText.AppearOnlyOpen(this);
			Refresh();
		}
	} else if (isZoomWindow) {
		if (PointInPlot(pt)) {
			if (IsNull(popLT))
				popLT = pt;
			popRB = pt;
			Refresh();
		}
	}
}

void ScatterCtrl::MouseLeave()
{
	Point p = Null;
	Scrolling(false, p, true);
	isLeftDown = false;
	if (isLabelPopUp || isZoomWindow) {
		popText.Close();
		isLabelPopUp = isZoomWindow = false;
		popLT = popRB = Null;
		Refresh();
	}
}

void ScatterCtrl::MouseZoom(int zdelta, bool hor, bool ver) 
{
	double scale = zdelta > 0 ? zdelta/100. : -100./zdelta;
//	if (hor && (lastxRange < xRange*scale))
//		return;
//	if (ver && (lastyRange < yRange*scale))
//		return;
	if (lastRefresh_sign != ((scale >= 0) ? 1 : -1))	
		lastRefresh_ms = Null;					// Change of scale sign resets lastRefresh check
	if (GetTickCount() - lastRefresh0_ms > 1000)
		lastRefresh_ms = Null;					// 1 sec resets lastRefresh check
	if (!IsNull(lastRefresh_ms) && (lastRefresh_ms > maxRefresh_ms))
		return;
	Zoom(scale, mouseHandlingX, mouseHandlingY);
}

Image ScatterCtrl::CursorImage(Point p, dword keyflags)
{
	if (isZoomWindow)
		return ScatterImg::ZoomPlus();
	else if (isLeftDown)
		return ScatterImg::cursor1();
	return Image::Arrow();
}

ScatterCtrl &ScatterCtrl::SetMouseHandling(bool valx, bool valy) 
{
	mouseHandlingX = valx;
	mouseHandlingY = valy;
	return *this;
}

void ScatterCtrl::ContextMenu(Bar& bar)
{
	if (mouseHandlingX || mouseHandlingY) {
		bar.Add(t_("Fit to data"), 	ScatterImg::ShapeHandles(), THISBACK1(FitToData, mouseHandlingY));
		bar.Add(t_("Zoom +"), 		ScatterImg::ZoomPlus(), 	THISBACK3(Zoom, 1/1.2, true, mouseHandlingY));
		bar.Add(t_("Zoom -"), 		ScatterImg::ZoomMinus(), 	THISBACK3(Zoom, 1.2, true, mouseHandlingY));
	}
	bar.Add(t_("Attach X axis"), Null, THISBACK(ChangeMouseHandlingX)).Check(!mouseHandlingX);
	if (mouseHandlingX) {
		bar.Add(t_("Scroll Left"), 	ScatterImg::LeftArrow(), 	THISBACK2(ScatterDraw::Scroll, 0.2, 0)).Key(K_CTRL_LEFT);
		bar.Add(t_("Scroll Right"), ScatterImg::RightArrow(), 	THISBACK2(ScatterDraw::Scroll, -0.2, 0)).Key(K_CTRL_RIGHT);
	}
	bar.Add(t_("Attach Y axis"), Null, THISBACK(ChangeMouseHandlingY)).Check(!mouseHandlingY);
	if (mouseHandlingY) {
		bar.Add(t_("Scroll Up"), 	ScatterImg::UpArrow(), 	THISBACK2(ScatterDraw::Scroll, 0, -0.2)).Key(K_CTRL_UP);
		bar.Add(t_("Scroll Down"), 	ScatterImg::DownArrow(), THISBACK2(ScatterDraw::Scroll, 0, 0.2)).Key(K_CTRL_DOWN);			
	}
	if (mouseHandlingX || mouseHandlingY)
		bar.Separator();
#ifndef _DEBUG
	if (showPropDlg)
#endif	
	{
		bar.Add(t_("Properties"), ScatterImg::Gear(), THISBACK(DoShowEditDlg)).Key(K_CTRL_P);		
		bar.Add(t_("Data"), ScatterImg::Database(), THISBACK(DoShowData)).Key(K_CTRL_D);		
		bar.Separator();
	}
	bar.Add(t_("Copy"), ScatterImg::Copy(), 		THISBACK1(SaveToClipboard, false)).Key(K_CTRL_C);
	bar.Add(t_("Save to file"), ScatterImg::Save(), THISBACK1(SaveToFile, Null)).Key(K_CTRL_S);
}

void ScatterCtrl::SaveToFile(String fileName)
{
	GuiLock __;
	if (IsNull(fileName)) {
		fileToSave.Set(ForceExt(GetTitle(), ".jpg"));
		fileToSave.ClearTypes();
		fileToSave.Type(Format(t_("%s file"), "JPEG"), "*.jpg");
		fileToSave.Type(Format(t_("%s file"), "PNG"), "*.png");
	    if(!fileToSave.ExecuteSaveAs(t_("Saving plot to PNG or JPEG file"))) {
	        Exclamation(t_("Plot has not been saved"));
	        return;
	    }
        fileName = fileToSave;
	} 
	if (GetFileExt(fileName) == ".png") {
		PNGEncoder encoder;
		encoder.SaveFile(fileName, GetImage(ScatterDraw::GetSize(), copyRatio));
	} else if (GetFileExt(fileName) == ".jpg") {	
		JPGEncoder encoder(90);
		encoder.SaveFile(fileName, GetImage(ScatterDraw::GetSize(), copyRatio));		
	} else
		Exclamation(Format(t_("File format \"%s\" not found"), GetFileExt(fileName)));
}

ScatterCtrl &ScatterCtrl::AddSeries(ArrayCtrl &data, bool useCols, int idX, int idY, int idZ, int beginData, int numData)
{
	AddSeries<ArrayCtrlSource>(data, useCols, idX, idY, beginData, numData);
	return *this;
}

void ScatterCtrl::InsertSeries(int id, ArrayCtrl &data, bool useCols, int idX, int idY, int idZ, int beginData, int numData)
{
	InsertSeries<ArrayCtrlSource>(id, data, useCols, idX, idY, beginData, numData);
}

ScatterCtrl &ScatterCtrl::AddSeries(GridCtrl &data, bool useCols, int idX, int idY, int idZ, int beginData, int numData)
{
	AddSeries<GridCtrlSource>(data, useCols, idX, idY, beginData, numData);
	return *this;
}

void ScatterCtrl::InsertSeries(int id, GridCtrl &data, bool useCols, int idX, int idY, int idZ, int beginData, int numData)
{
	InsertSeries<GridCtrlSource>(id, data, useCols, idX, idY, beginData, numData);
}

ScatterCtrl::ScatterCtrl() : offset(10,12), copyRatio(1), isLeftDown(false)
{
	showInfo = mouseHandlingX = mouseHandlingY = isScrolling = isLabelPopUp = isZoomWindow = false;
	WantFocus();
	popTextX = "x";
	popTextY = "y1";
	popTextY2 = "y2";
	popLT = popRB = Null;
	showContextMenu = false;
	showPropDlg = false;
	Color(graphColor);	
	BackPaint();
	popText.SetColor(SColorFace);        

	lastRefresh_ms = Null;
	maxRefresh_ms = 500;
	highlighting = false;
	ShowInfo().ShowContextMenu().ShowPropertiesDlg();
	
	AddMouseBehavior(false, false, false, true , false, 0, false, ScatterCtrl::SHOW_COORDINATES); 
	AddMouseBehavior(true,  false, false, true , false, 0, false, ScatterCtrl::ZOOM_WINDOW);
	AddMouseBehavior(false, false, false, false, false, 0, true , ScatterCtrl::CONTEXT_MENU);
	AddMouseBehavior(false, false, false, false, true , 0, false, ScatterCtrl::SCROLL);
	AddMouseBehavior(false, false, false, false, false, 1, false, ScatterCtrl::ZOOM_H_RED);
	AddMouseBehavior(false, false, false, false, false, 1, false, ScatterCtrl::ZOOM_V_RED);
	AddMouseBehavior(false, false, false, false, false,-1, false, ScatterCtrl::ZOOM_H_ENL);
	AddMouseBehavior(false, false, false, false, false,-1, false, ScatterCtrl::ZOOM_V_ENL);
	AddMouseBehavior(false, false, false, false, false, 0, false, ScatterCtrl::NO_ACTION);

	AddKeyBehavior(true,  false, false, '+',		false, 	ScatterCtrl::ZOOM_H_ENL); 
	AddKeyBehavior(true,  false, false, K_ADD, 		true,	ScatterCtrl::ZOOM_H_ENL);
	AddKeyBehavior(true,  false, false, '+',		false, 	ScatterCtrl::ZOOM_V_ENL);
	AddKeyBehavior(true,  false, false, K_ADD, 		true, 	ScatterCtrl::ZOOM_V_ENL);
	AddKeyBehavior(true,  false, false, '-', 		false,  ScatterCtrl::ZOOM_H_RED); 
	AddKeyBehavior(true,  false, false, K_SUBTRACT, true, 	ScatterCtrl::ZOOM_H_RED); 
	AddKeyBehavior(true,  false, false, '-', 	 	false, 	ScatterCtrl::ZOOM_V_RED);
	AddKeyBehavior(true,  false, false, K_SUBTRACT,	true, 	ScatterCtrl::ZOOM_V_RED);
	AddKeyBehavior(true,  false, false, K_LEFT, 	true, 	ScatterCtrl::SCROLL_LEFT);
	AddKeyBehavior(true,  false, false, K_RIGHT, 	true, 	ScatterCtrl::SCROLL_RIGHT);
	AddKeyBehavior(true,  false, false, K_UP,   	true, 	ScatterCtrl::SCROLL_UP);
	AddKeyBehavior(true,  false, false, K_DOWN, 	true, 	ScatterCtrl::SCROLL_DOWN);
}

END_UPP_NAMESPACE