#include "ScatterCtrl.h"

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
	wmfd.Create(saveSize.cx, saveSize.cy, "ScatterCtrl", "chart", file);
	ScatterDraw::SetSize(saveSize);
	ScatterDraw::SetDrawing<Draw>(wmfd);	
	wmfd.Close();	
}

void ScatterCtrl::SaveToClipboard(bool saveAsMetafile) 
{
	GuiLock __;
	if (saveAsMetafile) {
		WinMetaFileDraw wmfd;	
		wmfd.Create(saveSize.cx, saveSize.cy, "ScatterCtrl", "chart");
		ScatterDraw::SetSize(saveSize);
		ScatterDraw::SetDrawing<Draw>(wmfd, false);	
		WinMetaFile wmf = wmfd.Close();	 
		wmf.WriteClipboard();
	} else {
		ScatterDraw::SetSize(saveSize);
		Image img = GetImage();
		WriteClipboardImage(img);	
	}
}
#else

void ScatterCtrl::SaveToClipboard(bool) 
{
	GuiLock __;
	ScatterDraw::SetSize(saveSize);
	Image img = GetImage();
	WriteClipboardImage(img);
}

#endif

#ifdef PLATFORM_WIN32

String FixPathName(const String &path) {
	const char *forbstr[] = {"CON", "PRN", "AUX", "NUL", "COM", "LPT", ""};
	for (int i = 0; forbstr[i][0] != '\0'; ++i) {
		if (path.StartsWith(forbstr[i])) 
			return String("_") + path;  
	}
	
	String ret;	
	const char *forbchar = "<>:\"/\\|?*";
	for (int i = 0; i < path.GetCount(); ++i) {
		int ch = path[i];
		if (strchr(forbchar, ch))
			ret << "_";
		else
			ret << String(ch, 1);
	}
	ret.Replace("..", ".");
	
	return ret;
}

#else

String FixPathName(const String &path) {
	String ret = path;	

	ret.Replace("/", "_");
	
	return ret;
}

#endif

void ScatterCtrl::LoadControl() {
	GuiLock __;
	
	FileSel fs;
	fs.Type(Format(t_("%s data file"), "json"), "*.json");
	fs.Type(Format(t_("%s data file"), "xml"), "*.xml");
	fs.Type(Format(t_("%s data file"), "bin"), "*.bin");
	fs.AllFilesType();
	if (!defaultDataFile.IsEmpty())
		fs = defaultDataFile;
	else if (!GetTitle().IsEmpty())
		fs = FixPathName(GetTitle()) + ".json";
	else
		fs = String(t_("Scatter plot data")) + ".json";
	
	String ext = GetFileExt(~fs);
	fs.DefaultExt(ext);
	int idt = 0;
	if (ext == ".json")
		idt = 0;
	else if (ext == ".xml")
		idt = 1;
	else if (ext == ".bin")
		idt = 2;
	fs.ActiveType(idt);
	
	fs.ActiveDir(GetFileFolder(defaultDataFile));
	fs.type.WhenAction = THISBACK1(OnTypeDataFile, &fs); 
    if(!fs.ExecuteOpen(t_("Loading plot data from file"))) {
        Exclamation(t_("Plot has not been loaded"));
        return;
    }
    String fileName = defaultDataFile = ~fs;
    
	if (GetFileExt(fileName) == ".json") {
		if (!LoadFromJsonFile(*this, fileName)) {
			if (!LoadFromJsonFile(*static_cast<ScatterDraw*>(this), fileName))
				Exclamation(t_("Plot has not been loaded"));
		}
	} else if (GetFileExt(fileName) == ".xml") {	
		if (!LoadFromXMLFile(*this, fileName))
			Exclamation(t_("Plot has not been loaded"));
	} else if (GetFileExt(fileName) == ".bin") {	
		if (!LoadFromFile(*this, fileName))
			Exclamation(t_("Plot has not been loaded"));
	} else
		Exclamation(Format(t_("File format \"%s\" not found"), GetFileExt(fileName)));
}

void ScatterCtrl::OnTypeDataFile(FileSel *_fs) 
{
	FileSel &fs = *_fs;
	int id = fs.type.GetIndex();
	
	if (id == 0)
		fs.file = ForceExt(~fs, ".json");
	else if (id == 1)
		fs.file = ForceExt(~fs, ".xml");
	else if (id == 2)
		fs.file = ForceExt(~fs, ".bin");
	
}

void ScatterCtrl::SaveControl() {
	GuiLock __; 

	FileSel fs;
	fs.Type(Format(t_("%s data file"), "json"), "*.json");
	fs.Type(Format(t_("%s data file"), "xml"), "*.xml");
	fs.Type(Format(t_("%s data file"), "bin"), "*.bin");
	fs.AllFilesType();
	if (!defaultDataFile.IsEmpty())
		fs = defaultDataFile;
	else if (!GetTitle().IsEmpty())
		fs = FixPathName(GetTitle()) + ".json";
	else
		fs = String(t_("Scatter plot data")) + ".json";
	
	String ext = GetFileExt(~fs);
	fs.DefaultExt(ext);
	int idt = 0;
	if (ext == ".json")
		idt = 0;
	else if (ext == ".xml")
		idt = 1;
	else if (ext == ".bin")
		idt = 2;
	fs.ActiveType(idt);
	
	fs.ActiveDir(GetFileFolder(defaultDataFile));
	fs.type.WhenAction = THISBACK1(OnTypeDataFile, &fs); 
    if(!fs.ExecuteSaveAs(t_("Saving plot data to file"))) {
        Exclamation(t_("Plot has not been saved"));
        return;
    }
    String fileName = defaultDataFile = ~fs;
        
	if (GetFileExt(fileName) == ".json") {
		if (!StoreAsJsonFile(*this, fileName, true))
			Exclamation(t_("Plot has not been saved"));
	} else if (GetFileExt(fileName) == ".xml") {
		if (!StoreAsXMLFile(*this, fileName))
			Exclamation(t_("Plot has not been saved"));
	} else if (GetFileExt(fileName) == ".bin") {	
		if (!StoreToFile(*this, fileName))
			Exclamation(t_("Plot has not been saved"));
	} else
		Exclamation(Format(t_("File format \"%s\" not found"), GetFileExt(fileName)));
}

void ScatterCtrl::Paint0(Draw& w, const Size &sz) {
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
			ScatterCtrl::SetDrawing(w, sz);
			PlotTexts(w, !mouseHandlingX, !mouseHandlingY);
		} else {
			ImageBuffer ib(sz);
			BufferPainter bp(ib, mode);
			bp.LineCap(LINECAP_BUTT);
			bp.LineJoin(LINEJOIN_MITER);
			ScatterCtrl::SetDrawing(bp, sz);
			w.DrawImage(0, 0, ib);
			PlotTexts(w, !mouseHandlingX, !mouseHandlingY);
		}
		if (HasFocus()) {
			w.DrawLine(0, 0, sz.cx, 0, 2, LtGray());
			w.DrawLine(0, 0, 0, sz.cy, 2, LtGray());
			int delta = -2;
#ifdef PLATFORM_WIN32
			delta = 0;
#endif
			w.DrawLine(sz.cx+delta, 0, sz.cx+delta, sz.cy, 2, LtGray());
			w.DrawLine(0, sz.cy+delta, sz.cx, sz.cy+delta, 2, LtGray());
		}
	}
	lastRefresh_ms = t.Elapsed();
}
		
			
void ScatterCtrl::Paint(Draw& w) {
	if (rotate == Angle_0)
		Paint0(w, GetSize());
	else if (rotate == Angle_90) {
		Size sz(GetSize().cy, GetSize().cx);
		ImageDraw dw(sz);
		Paint0(dw, sz);
		w.DrawImage(0, 0, GetSize().cx, GetSize().cy, RotateClockwise(dw));
	} else if (rotate == Angle_270) {
		Size sz(GetSize().cy, GetSize().cx);
		ImageDraw dw(sz);
		Paint0(dw, sz);
		w.DrawImage(0, 0, GetSize().cx, GetSize().cy, RotateAntiClockwise(dw));
	} else if (rotate == Angle_180) {
		Size sz(GetSize());
		ImageDraw dw(sz);
		Paint0(dw, sz);
		w.DrawImage(0, 0, GetSize().cx, GetSize().cy, Rotate180(dw));
	}
}

void ScatterCtrl::TimerCallback() {
	Refresh();
}

void ScatterCtrl::ProcessPopUp(const Point &pt)
{
	double _x  = GetRealPosX(popLT.x);
	double _y  = GetRealPosY(popLT.y);
	double _y2 = GetRealPosY2(popLT.y);
	double x   = GetRealPosX(pt.x);
	double y   = GetRealPosY(pt.y);
	double y2  = GetRealPosY2(pt.y);
	
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
	
	String _str, str, strdx, strdy;
	
	_str = popTextX + ": " + _strx;
	if (strx != _strx) {
		str = popTextX + ": " + strx;
		strdx = popTextX + ": " + dstrx;
	}
	_str << "\n" + popTextY + ": " + _stry;
	if (stry != _stry) {	
 		str << "\n" + popTextY + ": " + stry;
 		strdy = popTextY + ": " + dstry;
	}
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
		
		_str << "\n" + popTextY2 + ": " + _stry2;
		if (stry2 != _stry2) {		
			str << "\n" + popTextY2 + ": " + stry2;
			strdy << "\n" + popTextY2 + ": " + dstry2;
		}
	}
	if (surf) {
		double _z  = surf->z(_x, _y);
		if (!IsNull(_z)) {
			_str << "\n" << popTextZ << ": " << VariableFormatZ(_z);
			if (strx != _strx || stry != _stry) {
				double z = surf->z(x, y);
				if (!IsNull(z)) 
					str << "\n" << popTextZ << ": " << VariableFormatZ(z) << "; " 
								<< t_("Δ") << popTextZ << ": " << VariableFormatZ(z - _z);	
			}
		}
	}
	const Point _popPoint = popLT - popOffset;
	const Point popPointdy(popLT.x, popLT.y + (pt.y - popLT.y)/2);
	const Point popPointdx(popLT.x + (pt.x - popLT.x)/2, pt.y);
	Point popPoint = pt + popOffset;
	
	popTextBegin.SetText(_str);
	Size sz = popTextBegin.GetSize();
	Point p1(_popPoint.x - sz.cx, _popPoint.y - sz.cy);
	popTextBegin.Move(this, MousePointUnrot(p1));	
	if (!strdx.IsEmpty()) {
		popTextHoriz.Show();
		popTextHoriz.SetText(strdx);
		Size sz = popTextHoriz.GetSize();
		Point p2(popPointdx.x - sz.cx/2, popPointdx.y - sz.cy/2);
		popTextHoriz.Move(this, MousePointUnrot(p2));	
	} else
		popTextHoriz.Hide();
	if (!strdy.IsEmpty()) {
		popTextVert.Show();
		popTextVert.SetText(strdy);
		Size sz = popTextVert.GetSize();
		Point p3(popPointdy.x - sz.cx/2, popPointdy.y - sz.cy/2);
		popTextVert.Move(this, MousePointUnrot(p3));	
	} else
		popTextVert.Hide();
	if (!str.IsEmpty()) {
		popTextEnd.Show();
		popTextEnd.SetText(str).Move(this, MousePointUnrot(popPoint));
	} else
		popTextEnd.Hide();
}

void ScatterCtrl::ProcessClickSeries(const Point &pt)
{
	double posx = GetRealPosX(pt.x);
	double posy = GetRealPosY(pt.y);
	double dx = 2*GetPixelThickX();
	double dy = 2*GetPixelThickY();
}
						
void ScatterCtrl::DoMouseAction(bool down, Point pt, ScatterAction action, int wheel)
{
	if (!down) {
		Scrolling(false, pt);
		LabelPopUp(false, pt);
		ZoomWindow(false, pt);
	}
	switch (action) {
	case SCROLL: 			Scrolling(down, pt);
							break;
	case ZOOM_H_ENL:	 	MouseZoom(wheel, true, false); 
							break;
	case ZOOM_H_RED:		MouseZoom(-wheel, true, false); 
							break;
	case ZOOM_V_ENL:		MouseZoom(wheel, false, true); 
							break;
	case ZOOM_V_RED:		MouseZoom(-wheel, false, true); 
							break;
	case SHOW_COORDINATES:	LabelPopUp(down, pt);
							break;
	case ZOOM_WINDOW:		ZoomWindow(down, pt);
							break;						
	case CONTEXT_MENU:		if(showContextMenu && !down)
								MenuBar::Execute(THISBACK(ContextMenu));
							break;
	case NO_ACTION:
	case SCROLL_LEFT:
	case SCROLL_RIGHT:
	case SCROLL_UP:
	case SCROLL_DOWN:
	case ZOOM_FIT:;
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
	case ZOOM_FIT:		ScatterDraw::ZoomToFit(true, true);	break;
	case NO_ACTION: 
	case SCROLL:
	case SHOW_COORDINATES:
	case ZOOM_WINDOW:
	case CONTEXT_MENU:;
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
		    ((mouseBehavior[i].middleWheel == 0) || sgn(mouseBehavior[i].middleWheel) == sgn(middleWheel))) {
		    DoMouseAction(down, pt, mouseBehavior[i].action, mouseBehavior[i].middleWheel*middleWheel);
			break;
		}
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

void ScatterCtrl::LabelPopUp(bool down, Point &pt) 
{
	if (down) {
		if(showInfo && PointInPlot(pt)) {
			popTextBegin.AppearOnly(this);
			popTextVert.AppearOnly(this);
			popTextHoriz.AppearOnly(this);
			popTextEnd.AppearOnly(this);
			
			isLabelPopUp = true;
			if (IsNull(popLT))
				popLT = pt;
			popRB = pt;
			/*Rect wa = GetWorkArea();
			Rect rc = GetScreenRect();
			if (wa.right - (rc.left + pt.x) < 200)
				pt.x -= 200;
			if (wa.bottom - (rc.top + pt.y) < 200)
				pt.y -= 200;*/
			//ProcessClickSeries(pt); TBD
			ProcessPopUp(pt);		
		} 
	} else {
		if(showInfo && isLabelPopUp) {
			popTextBegin.Close();
			popTextVert.Close();
			popTextHoriz.Close(); 
			popTextEnd.Close();
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
				::Swap(popLT, popRB);
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
			DoShowEditDlg(0);
		else if (key == K_CTRL_D)
			DoShowData();
		else if (key == K_CTRL_C)
			SaveToClipboard(true);
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
	MousePointRot(pt);
	if(!HasFocus()) 
		SetFocus();
	mouseAction = LEFT_DOWN;
	ProcessMouse(true, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, true, false, 0, false);
	WhenMouseClick(pt, keyFlags, mouseAction);
}

void ScatterCtrl::LeftDouble(Point pt, dword keyFlags)
{
	MousePointRot(pt);
	if(!HasFocus()) 
		SetFocus();
	if (PointInLegend(pt))
		DoShowEditDlg(3);
	else if (PointInBorder(pt))
		DoShowEditDlg(0);
	WhenMouseClick(pt, keyFlags, LEFT_DOUBLE);
}

void ScatterCtrl::LeftUp(Point pt, dword keyFlags)
{
	MousePointRot(pt);
	mouseAction = LEFT_UP;
	ProcessMouse(false, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, true, false, 0, false); 
	WhenMouseClick(pt, keyFlags, mouseAction);
}

void ScatterCtrl::MiddleDown(Point pt, dword keyFlags)
{
	MousePointRot(pt);
	if(!HasFocus()) 
		SetFocus();
	mouseAction = MIDDLE_DOWN;
	ProcessMouse(true, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, false, true, 0, false);
	WhenMouseClick(pt, keyFlags, mouseAction);
}

void ScatterCtrl::MiddleUp(Point pt, dword keyFlags)
{
	MousePointRot(pt);
	mouseAction = MIDDLE_UP;
	ProcessMouse(false, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, false, true, 0, false);
	WhenMouseClick(pt, keyFlags, mouseAction);
}

void ScatterCtrl::RightDown(Point pt, dword keyFlags) 
{
	MousePointRot(pt);
	if(!HasFocus()) 
		SetFocus();
	mouseAction = RIGHT_DOWN;
	ProcessMouse(true, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, false, false, 0, true);
	WhenMouseClick(pt, keyFlags, mouseAction);
}

void ScatterCtrl::RightUp(Point pt, dword keyFlags)
{
	MousePointRot(pt);
	mouseAction = RIGHT_UP;
	ProcessMouse(false, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, false, false, 0, true); 
	WhenMouseClick(pt, keyFlags, mouseAction);
}

void ScatterCtrl::MouseWheel(Point pt, int zdelta, dword keyFlags) 
{
	MousePointRot(pt);
	if (zdelta == 0)
		return;
	if(!HasFocus()) 
		SetFocus();
	ProcessMouse(true, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, false, false, zdelta, false);
}

void ScatterCtrl::MouseMove(Point pt, dword keyFlags)
{
	MousePointRot(pt);
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
			popTextBegin.AppearOnlyOpen(this);
			popTextHoriz.AppearOnlyOpen(this);
			popTextVert.AppearOnlyOpen(this);
			popTextEnd.AppearOnlyOpen(this);
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
	switch (mouseAction) {
	case LEFT_DOWN:		mouseAction = LEFT_MOVE;	break;
	case MIDDLE_DOWN:	mouseAction = MIDDLE_MOVE;	break;
	case RIGHT_DOWN:	mouseAction = RIGHT_MOVE;	break;
	}
	WhenMouseClick(pt, keyFlags, mouseAction);
}

void ScatterCtrl::MouseLeave()
{
	Point p = Null;
	Scrolling(false, p, true);
	mouseAction = NONE;
	if (isLabelPopUp || isZoomWindow) {
		popTextBegin.Close();
		popTextVert.Close();
		popTextHoriz.Close(); 
		popTextEnd.Close();		
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
	else if (mouseAction == LEFT_DOWN)
		return ScatterImg::cursor1();
	return Image::Arrow();
}

void ScatterCtrl::ContextMenu(Bar& bar)
{
	if (mouseHandlingX || mouseHandlingY) {
		bar.Add(t_("Fit to data"), ScatterImg::ShapeHandles(), THISBACK3(ZoomToFit, mouseHandlingX, mouseHandlingY, 0.)).Key(K_CTRL_F)
									.Help(t_("Zoom to fit visible all data"));
		bar.Add(t_("Zoom +"), 	   ScatterImg::ZoomPlus(), THISBACK3(Zoom, 1/1.2, true, mouseHandlingY)).Key(K_CTRL|K_ADD)
										.Help(t_("Zoom in (closer)"));
		bar.Add(t_("Zoom -"), 	   ScatterImg::ZoomMinus(), THISBACK3(Zoom, 1.2, true, mouseHandlingY)).Key(K_CTRL|K_SUBTRACT)
									.Help(t_("Zoom out (away)"));
	}
	bar.Add(t_("Attach X axis"), Null, THISBACK(ChangeMouseHandlingX)).Check(!mouseHandlingX)
									.Help(t_("Fix X axis so no zoom or scroll is possible"));
	if (mouseHandlingX) {
		bar.Add(t_("Scroll Left"), ScatterImg::LeftArrow(), THISBACK2(ScatterDraw::Scroll, 0.2, 0)).Key(K_CTRL_LEFT)
									.Help(t_("Scrolls plot to the left"));
		bar.Add(t_("Scroll Right"),ScatterImg::RightArrow(), THISBACK2(ScatterDraw::Scroll, -0.2, 0)).Key(K_CTRL_RIGHT)
									.Help(t_("Scrolls plot to the right"));
	}
	bar.Add(t_("Attach Y axis"),   Null, THISBACK(ChangeMouseHandlingY)).Check(!mouseHandlingY)
									.Help(t_("Fix Y axis so no zoom or scroll is possible"));
	if (mouseHandlingY) {
		bar.Add(t_("Scroll Up"),   ScatterImg::UpArrow(), THISBACK2(ScatterDraw::Scroll, 0, -0.2)).Key(K_CTRL_UP)
									.Help(t_("Scrolls plot up"));
		bar.Add(t_("Scroll Down"), ScatterImg::DownArrow(), THISBACK2(ScatterDraw::Scroll, 0, 0.2)).Key(K_CTRL_DOWN)
									.Help(t_("Scrolls plot down"));
	}
	if (mouseHandlingX || mouseHandlingY)
		bar.Separator();
#ifndef _DEBUG
	if (showPropDlg)
#endif	
	{
		bar.Add(t_("Properties"), ScatterImg::Gear(), THISBACK1(DoShowEditDlg, 0)).Key(K_CTRL_P)
									.Help(t_("Plot properties dialog"));
		bar.Add(t_("Data"), ScatterImg::Database(), THISBACK(DoShowData)).Key(K_CTRL_D)
									.Help(t_("Raw data table"));
	}
#ifndef _DEBUG
	if (showProcessDlg)
#endif
	{
		bar.Add(t_("Process"), ScatterImg::chart_curve_edit(), THISBACK(DoProcessing)).Key(K_SHIFT_P)
									.Help(t_("Data processing dialog"));
	}
#ifndef _DEBUG
	if (showPropDlg || showProcessDlg)
#endif
	{
		bar.Separator();
	}
	bar.Add(t_("Copy image"), ScatterImg::Copy(), 		THISBACK1(SaveToClipboard, false)).Key(K_CTRL_C)
									.Help(t_("Copy image to clipboard"));
	bar.Add(t_("Save image"), ScatterImg::Save(), THISBACK1(SaveToFile, Null)).Key(K_CTRL_S)
									.Help(t_("Save image to file"));
#ifndef _DEBUG
	if (showLoadData || showSaveData)
#endif
	{
		bar.Separator();
	}
	bar.Add(t_("Load plot"), THISBACK(LoadControl)).Help(t_("Load plot from file"));
	bar.Add(t_("Save plot"), THISBACK(SaveControl)).Help(t_("Save plot to file"));
}

void ScatterCtrl::OnTypeImage(FileSel *_fs)
{
	FileSel &fs = *_fs;
	int id = fs.type.GetIndex();
	
	if (id == 0)
		fs.file = ForceExt(GetFileName(~fs), ".jpg");
	else if (id == 1)
		fs.file = ForceExt(GetFileName(~fs), ".png");
	else if (id == 2)
		fs.file = ForceExt(GetFileName(~fs), ".pdf");
}

void ScatterCtrl::SaveToFile(String fileName)
{
	GuiLock __;
	if (IsNull(fileName)) {
		FileSel fs;
		fs.Type(Format(t_("%s bitmap file"), "jpeg"), "*.jpg");
		fs.Type(Format(t_("%s bitmap file"), "png"), "*.png");
		fs.Type(Format(t_("%s vector file"), "pdf"), "*.pdf");
		fs.AllFilesType();
		if (!defaultFileNamePlot.IsEmpty())
			fs = defaultFileNamePlot;
		else if (!GetTitle().IsEmpty())
			fs = FixPathName(GetTitle()) + ".jpg";
		else
			fs = String(t_("Scatter plot")) + ".jpg";
		
		String ext = GetFileExt(~fs);
		fs.DefaultExt(ext);
		int idt = 0;
		if (ext == ".jpg" || ext == ".jpeg")
			idt = 0;
		else if (ext == ".png")
			idt = 1;
		else if (ext == ".pdf")
			idt = 2;
		fs.ActiveType(idt);
	
		fs.ActiveDir(GetFileFolder(defaultFileNamePlot));
		fs.type.WhenAction = THISBACK1(OnTypeImage, &fs); 
	    if(!fs.ExecuteSaveAs(t_("Saving plot image to file"))) {
	        Exclamation(t_("Plot has not been saved"));
	        return;
	    }
        fileName = defaultFileNamePlot = ~fs;
	} 
	if (GetFileExt(fileName) == ".png") {
		WaitCursor waitcursor;
		PNGEncoder encoder;
		ScatterDraw::SetSize(saveSize);
		encoder.SaveFile(fileName, GetImage());
	} else if (GetFileExt(fileName) == ".jpg") {	
		WaitCursor waitcursor;
		JPGEncoder encoder(jpgQuality);
		ScatterDraw::SetSize(saveSize);
		encoder.SaveFile(fileName, GetImage());		
	} else if (GetFileExt(fileName) == ".pdf") {	
		WaitCursor waitcursor;
		PdfDraw pdf(saveSize.cx, saveSize.cy);
		ScatterDraw::SetSize(saveSize);
		ScatterDraw::SetDrawing(pdf, false);
		SaveFile(fileName, pdf.Finish());		
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

void ScatterCtrl::CheckButtonVisible() {
	processButton.Show(showButtons && showProcessDlg);
	dataButton.Show(showButtons && showPropDlg);
	propertiesButton.Show(showButtons && showPropDlg);
}
	
inline Point &ScatterCtrl::MousePointRot(Point &pt) {
	if (rotate == Angle_0) 
		return pt;
	else if (rotate == Angle_90) {
		Swap(pt.x, pt.y);
		pt.y = GetSize().cx - pt.y;
	} else if (rotate == Angle_270) {
		Swap(pt.x, pt.y);
		pt.x = GetSize().cy - pt.x;
	} else if (rotate == Angle_180) {
		pt.x = GetSize().cx - pt.x;
		pt.y = GetSize().cy - pt.y;
	}
	return pt;
}

inline Point &ScatterCtrl::MousePointUnrot(Point &pt) {
	if (rotate == Angle_0) 
		return pt;
	else if (rotate == Angle_90) {
		Swap(pt.x, pt.y);
		pt.x = GetSize().cy - pt.x;
	} else if (rotate == Angle_270) {
		Swap(pt.x, pt.y);
		pt.y = GetSize().cy - pt.y;
	} else if (rotate == Angle_180) {
		pt.x = GetSize().cx - pt.x;
		pt.y = GetSize().cy - pt.y;
	}
	return pt;
}

ScatterCtrl::ScatterCtrl() : popOffset(10, 12), mouseAction(NONE)
{
	showInfo = isScrolling = isLabelPopUp = isZoomWindow = false;
	WantFocus();
	popTextX = t_("x");
	popTextY = t_("y");
	popTextY2 = t_("y right");
	popTextZ = t_("z");
	popLT = popRB = Null;
	showContextMenu = false;
	showPropDlg = false;
	showProcessDlg = false;
	showButtons = false;
	showLoadData = showSaveData = false;
	defaultCSVseparator = ";";
	rotate = Angle_0;
	Color(graphColor);	
	BackPaint();
	popTextBegin.SetColor(SColorFace);  
	popTextVert.SetColor(SColorFace);  
	popTextHoriz.SetColor(SColorFace);  
	popTextEnd.SetColor(SColorFace);  

	saveSize = Size(1000, 800);
	jpgQuality = 90;
	
	lastRefresh_ms = Null;
	maxRefresh_ms = 500;
	highlighting = false;
	ShowInfo().ShowContextMenu().ShowPropertiesDlg();
	
	Add(processButton.RightPosZ(0, 15).TopPosZ(0, 15));
	processButton.Show(false);
	processButton.SetImage(ScatterImg::chart_curve_edit()).Tip(t_("Data processing dialog"));
	processButton.WhenAction = THISBACK(DoProcessing);
	
	Add(dataButton.RightPosZ(15, 15).TopPosZ(0, 15));
	dataButton.Show(false);
	dataButton.SetImage(ScatterImg::Database()).Tip(t_("Raw data table"));
	dataButton.WhenAction = THISBACK(DoShowData);
	
	Add(propertiesButton.RightPosZ(30, 15).TopPosZ(0, 15));
	propertiesButton.Show(false);
	propertiesButton.SetImage(ScatterImg::Gear()).Tip(t_("Plot properties dialog"));
	propertiesButton.WhenAction = THISBACK1(DoShowEditDlg, 0);
	
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
	AddKeyBehavior(true,  false, false, K_F, 		true, 	ScatterCtrl::ZOOM_FIT);
}

