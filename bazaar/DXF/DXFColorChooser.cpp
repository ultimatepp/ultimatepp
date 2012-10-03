#include "DXFColorChooser.h"

//////////////////////////////////////////////////////////////////////////
// Chooser dialog

uint32 DXF_COLOR_TABLE[] =
{
	0x000000, 0x0000FF, 0x00FFFF, 0x00FF00, 0xFFFF00, 0xFF0000, 0xFF00FF, 0xFFFFFF,
	0x414141, 0x808080, 0x0000FF, 0xAAAAFF, 0x0000BD, 0x7E7EBD, 0x000081, 0x565681,
	0x000068, 0x454568, 0x00004F, 0x35354F, 0x003FFF, 0xAABFFF, 0x002EBD, 0x7E8DBD,
	0x001F81, 0x566081, 0x001968, 0x454E68, 0x00134F, 0x353B4F, 0x007FFF, 0xAAD4FF,
	0x005EBD, 0x7E9DBD, 0x004081, 0x566B81, 0x003468, 0x455668, 0x00274F, 0x35424F,
	0x00BFFF, 0xAAEAFF, 0x008DBD, 0x7EADBD, 0x006081, 0x567681, 0x004E68, 0x455F68,
	0x003B4F, 0x35494F, 0x00FFFF, 0xAAFFFF, 0x00BDBD, 0x7EBDBD, 0x008181, 0x568181,
	0x006868, 0x456868, 0x004F4F, 0x354F4F, 0x00FFBF, 0xAAFFEA, 0x00BD8D, 0x7EBDAD,
	0x008160, 0x568176, 0x00684E, 0x45685F, 0x004F3B, 0x354F49, 0x00FF7F, 0xAAFFD4,
	0x00BD5E, 0x7EBD9D, 0x008140, 0x56816B, 0x006834, 0x456856, 0x004F27, 0x354F42,
	0x00FF3F, 0xAAFFBF, 0x00BD2E, 0x7EBD8D, 0x00811F, 0x568160, 0x006819, 0x45684E,
	0x004F13, 0x354F3B, 0x00FF00, 0xAAFFAA, 0x00BD00, 0x7EBD7E, 0x008100, 0x568156,
	0x006800, 0x456845, 0x004F00, 0x354F35, 0x3FFF00, 0xBFFFAA, 0x2EBD00, 0x8DBD7E,
	0x1F8100, 0x608156, 0x196800, 0x4E6845, 0x134F00, 0x3B4F35, 0x7FFF00, 0xD4FFAA,
	0x5EBD00, 0x9DBD7E, 0x408100, 0x6B8156, 0x346800, 0x566845, 0x274F00, 0x424F35,
	0xBFFF00, 0xEAFFAA, 0x8DBD00, 0xADBD7E, 0x608100, 0x768156, 0x4E6800, 0x5F6845,
	0x3B4F00, 0x494F35, 0xFFFF00, 0xFFFFAA, 0xBDBD00, 0xBDBD7E, 0x818100, 0x818156,
	0x686800, 0x686845, 0x4F4F00, 0x4F4F35, 0xFFBF00, 0xFFEAAA, 0xBD8D00, 0xBDAD7E,
	0x816000, 0x817656, 0x684E00, 0x685F45, 0x4F3B00, 0x4F4935, 0xFF7F00, 0xFFD4AA,
	0xBD5E00, 0xBD9D7E, 0x814000, 0x816B56, 0x683400, 0x685645, 0x4F2700, 0x4F4235,
	0xFF3F00, 0xFFBFAA, 0xBD2E00, 0xBD8D7E, 0x811F00, 0x816056, 0x681900, 0x684E45,
	0x4F1300, 0x4F3B35, 0xFF0000, 0xFFAAAA, 0xBD0000, 0xBD7E7E, 0x810000, 0x815656,
	0x680000, 0x684545, 0x4F0000, 0x4F3535, 0xFF003F, 0xFFAABF, 0xBD002E, 0xBD7E8D,
	0x81001F, 0x815660, 0x680019, 0x68454E, 0x4F0013, 0x4F353B, 0xFF007F, 0xFFAAD4,
	0xBD005E, 0xBD7E9D, 0x810040, 0x81566B, 0x680034, 0x684556, 0x4F0027, 0x4F3542,
	0xFF00BF, 0xFFAAEA, 0xBD008D, 0xBD7EAD, 0x810060, 0x815676, 0x68004E, 0x68455F,
	0x4F003B, 0x4F3549, 0xFF00FF, 0xFFAAFF, 0xBD00BD, 0xBD7EBD, 0x810081, 0x815681,
	0x680068, 0x684568, 0x4F004F, 0x4F354F, 0xBF00FF, 0xEAAAFF, 0x8D00BD, 0xAD7EBD,
	0x600081, 0x765681, 0x4E0068, 0x5F4568, 0x3B004F, 0x49354F, 0x7F00FF, 0xD4AAFF,
	0x5E00BD, 0x9D7EBD, 0x400081, 0x6B5681, 0x340068, 0x564568, 0x27004F, 0x42354F,
	0x3F00FF, 0xBFAAFF, 0x2E00BD, 0x8D7EBD, 0x1F0081, 0x605681, 0x190068, 0x4E4568,
	0x13004F, 0x3B354F, 0x333333, 0x505050, 0x696969, 0x828282, 0xBEBEBE, 0xFFFFFF
};

const char *STD_DXF_COLOR_NAMES[] =
{
	tt_("DXF\vred"),
	tt_("DXF\vyellow"),
	tt_("DXF\vgreen"),
	tt_("DXF\vcyan"),
	tt_("DXF\vblue"),
	tt_("DXF\vmagenta"),
	tt_("DXF\vwhite")
};

#define LAYOUTFILE <DXF/DXFColorChooser.lay>
#include <CtrlCore/lay.h>

// the chooser dialog
class DXFColorChooserDlg : public WithDXFColorChooserLayout<TopWindow>
{
	private:
	
		// fills big colors panes
		void fillBig(ImageCtrl &ctrl, int startCol);
		
		// fills small color panes
		void fillSmall(ImageCtrl &ctrl, int startCol, int count);
		
		// mouse event handler for childs
		void ChildMouseEvent(Ctrl *ctrl, int event, Point p, int zdelta,dword keyflags);
		
		// sets labels with index and rgb color values
		void SetLabels(int idx);
	
	protected:
	
	public:
	
		typedef DXFColorChooserDlg CLASSNAME;

		// constructor
		DXFColorChooserDlg(uint32 c);
		
		// the color
		int color, prevColor;
};

// constructor
DXFColorChooserDlg::DXFColorChooserDlg(uint32 c)
{
	// initializes layout
	CtrlLayout(*this);
	
	color = prevColor = c;

	// setup button handlers
	okBtn.Ok() <<= Acceptor(IDOK);
	cancelBtn.Cancel() <<= Breaker(IDCANCEL);
	WhenClose = Breaker(IDCANCEL);
	
	fillBig(upperImg, 10);
	fillBig(lowerImg, 11);
	
	fillSmall(firstImg, 1, 9);
	fillSmall(lastImg, 250, 6);
	
	int aCol = DXF_COLOR_TABLE[color & 0xFF];
	// correct blue part -- if == 0, upp shows red color as black...
	if((aCol & 0xff0000) == 0x00)
		aCol |= 0x010000;
	
	prevPreviewImg.Color(Color(aCol, 0));
	curPreviewImg.Color(Color(aCol, 0));
	if(color >= 1 && color <= 7)
		colorEdit <<= GetLngString(STD_DXF_COLOR_NAMES[color - 1]);
	else
		colorEdit <<= Format("%d", color);
	
	colorEdit.SetReadOnly();
	colorEdit.NoWantFocus();
}

// sets labels with index and rgb color values
void DXFColorChooserDlg::SetLabels(int idx)
{
	if(idx == -1)
	{
		indexLbl.SetText(t_("DXF\vIndex color :"));
		rgbLbl.SetText(t_("DXF\vRed,Green,Blue :"));
		return;
	}
	uint32 aCol = DXF_COLOR_TABLE[idx];
	int r = GetRValue(aCol);
	int g = GetGValue(aCol);
	int b = GetBValue(aCol);
	indexLbl.SetText(Format(String(t_("DXF\vIndex color :")) + " %d", idx));
	rgbLbl.SetText(Format(String(t_("DXF\vRed,Green,Blue :")) + "%d,%d,%d", r, g, b));
	Refresh();
//	Ctrl::ProcessEvents();
}
	
// mouse event handler for childs
void DXFColorChooserDlg::ChildMouseEvent(Ctrl *ctrl, int event, Point p, int zdelta,dword keyflags)
{
	int nextColor = 0;
	if(ctrl == &upperImg && event != MOUSELEAVE)
	{
		Size sz = upperImg.GetSize();
		int col = p.x / (sz.cx / 24);
		int row = 4 - p.y / (sz.cy / 5);
		nextColor = 10 + ((col * 5) + row) * 2;
		SetLabels(nextColor);
	}
	else if(ctrl == &lowerImg && event != MOUSELEAVE)
	{
		Size sz = lowerImg.GetSize();
		int col = p.x / (sz.cx / 24);
		int row = p.y / (sz.cy / 5);
		nextColor = 11 + ((col * 5) + row) * 2;
		SetLabels(nextColor);
	}
	else if(ctrl == &firstImg && event != MOUSELEAVE)
	{
		Size sz = firstImg.GetSize();
		int col = p.x / (sz.cx / 9);
		nextColor = 1 + col;
		SetLabels(nextColor);
	}
	else if(ctrl == &lastImg && event != MOUSELEAVE)
	{
		Size sz = lastImg.GetSize();
		int col = p.x / (sz.cx / 5);
		nextColor = 250 + col;
		SetLabels(nextColor);
	}
	else if(ctrl == &curPreviewImg && event != MOUSELEAVE)
	{
		nextColor = color;
		SetLabels(color);
	}
	else if(ctrl == &prevPreviewImg && event != MOUSELEAVE)
	{
		nextColor = prevColor;
		SetLabels(prevColor);
	}
	else
	{
		SetLabels(-1);
		return;
	}
	if(event == LEFTDOWN)
	{
		color = nextColor;
		int aCol = DXF_COLOR_TABLE[color & 0xFF];
		// correct blue part -- if == 0, upp shows red color as black...
		if((aCol & 0xff0000) == 0x00)
			aCol |= 0x010000;
		curPreviewImg.Color(Color(aCol, 0));
		if(color >= 1 && color <= 7)
			colorEdit <<= GetLngString(STD_DXF_COLOR_NAMES[color - 1]);
		else
			colorEdit <<= Format("%d", color);
	}
}

// fills big colors panes
void DXFColorChooserDlg::fillBig(ImageCtrl &ctrl, int startCol)
{
	Size imgSize = ctrl.GetSize();
	Size spaceSize(imgSize.cx / 24, imgSize.cy / 5);
	Size boxSize(spaceSize.cx - 2, spaceSize.cy - 2);
	int dir;
	if(startCol & 1)
		dir = -1;
	else
		dir = 1;
	ImageDraw dw(imgSize);
	dw.DrawRect(0, 0, imgSize.cx, imgSize.cy, SColorFace());
	for(int iCol = 0; iCol < 24; iCol++)
	{
		for(int iRow = 0; iRow < 5; iRow++)
		{
			Point pos;
			if(dir == 1)
				pos = Point(spaceSize.cx * iCol, imgSize.cy - spaceSize.cy * (iRow + 1));
			else
				pos = Point(spaceSize.cx * iCol, spaceSize.cy * iRow);
			pos.x += 1;
			pos.y += 1;
			uint32 aCol = DXF_COLOR_TABLE[startCol & 0xFF];
			// correct blue part -- if == 0, upp shows red color as black...
			if((aCol & 0xff0000) == 0x00)
				aCol |= 0x010000;
			dw.DrawRect(pos.x, pos.y, boxSize.cx, boxSize.cy, Color(aCol, 0));
			startCol += 2;
		}
	}
	ctrl.SetImage(dw);
}

// fills small color panes
void DXFColorChooserDlg::fillSmall(ImageCtrl &ctrl, int startCol, int count)
{
	Size imgSize = ctrl.GetSize();
	Size spaceSize(imgSize.cx / count, imgSize.cy);
	Size boxSize(spaceSize.cx - 2, spaceSize.cy - 2);
	ImageDraw dw(imgSize);
	dw.DrawRect(0, 0, imgSize.cx, imgSize.cy, SColorFace());
	for(int iCol = 0; iCol < count; iCol++)
	{
		Point pos = Point(spaceSize.cx * iCol + 1, 1);
		uint32 aCol = DXF_COLOR_TABLE[startCol++ & 0xFF];
		// correct blue part -- if == 0, upp shows red color as black...
		if((aCol & 0xff0000) == 0x00)
			aCol |= 0x010000;
		dw.DrawRect(pos.x, pos.y, boxSize.cx, boxSize.cy, Color(aCol, 0));
	}
	ctrl.SetImage(dw);
}
	
//////////////////////////////////////////////////////////////////////////
// Chooser ctrl
// constructor
DXFColorChooser::DXFColorChooser()
{
	// white by default
	color = 7;
	
	AddFrame(InsetFrame());
}

// paint control
void DXFColorChooser::Paint(Draw &w)
{
	Size sz = GetView().GetSize();
	Size rectSize(sz.cy - 2, sz.cy - 2);
	w.DrawLine(1, 1, rectSize.cx + 1, 1, Black());
	w.DrawLine(rectSize.cx + 1, 1, rectSize.cx + 1, rectSize.cy + 1, Black());
	w.DrawLine(rectSize.cx + 1, rectSize.cy + 1, 1, rectSize.cy + 1, Black());
	w.DrawLine(1, rectSize.cy + 1, 1, 1, Black());
	uint32 aCol = DXF_COLOR_TABLE[color & 0xFF];
	// correct blue part -- if == 0, upp shows red color as black...
	if((aCol & 0xff0000) == 0x00)
		aCol |= 0x010000;
	w.DrawRect(2, 2, rectSize.cx - 2, rectSize.cy - 2, Color(aCol, 0));
	int txtPartSize = sz.cx - rectSize.cx - 2;
	w.DrawRect(rectSize.cx + 2, 0, txtPartSize, sz.cy, White());
	
	String txtCol = color >= 1 && color <= 7 ? GetLngString(STD_DXF_COLOR_NAMES[color - 1]) : Format("%d", color);
	Size txtSize = GetTextSize(txtCol, StdFont());
	w.DrawText(rectSize.cx + (txtPartSize - txtSize.cx) / 2, (sz.cy - txtSize.cy) / 2, txtCol);
}
	
// handle mouse clicks
void DXFColorChooser::LeftDown(Point p, dword)
{
	DXFColorChooserDlg dlg(color);
	if(dlg.RunAppModal() == IDOK)
	{
		SetColor(dlg.color);
		WhenAction();
	}
}
