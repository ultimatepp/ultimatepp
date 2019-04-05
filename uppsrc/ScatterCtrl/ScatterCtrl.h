#ifndef _ScatterCtrl_ScatterCtrl_h
#define _ScatterCtrl_ScatterCtrl_h

#include <ScatterDraw/ScatterDraw.h>
#include <plugin/jpg/jpg.h>
#include <PdfDraw/PdfDraw.h>
#include <GridCtrl/GridCtrl.h>

using namespace Upp;

#include "PopUpText.h"

class EditDoubleLostFocus : public EditDouble {
public:
	Callback WhenLostFocus;
	
	virtual void LostFocus() {
		WhenLostFocus();
		EditDouble::LostFocus();
	}
};

typedef WithSpin<double, EditDoubleLostFocus> EditDoubleLostFocusSpin;

class ArrayCtrlSource : public DataSource {
private:
	ArrayCtrl *data;
	bool useCols;
	Vector<int> ids;
	int beginData;
	int64 numData;

public:
	ArrayCtrlSource() : data(0), useCols(true), beginData(0), numData(Null) {ids << 0 << 1;}
	ArrayCtrlSource(ArrayCtrl &data, bool useCols = true, int idX = 0, int idY = 1, int beginData = 0, int numData = Null) : 
		data(&data), useCols(useCols), beginData(beginData), numData(numData) {
		Init(data, idY, idX, useCols, beginData, numData);
	}
	void Init(ArrayCtrl &_data, Vector<int> &_ids, bool _useCols = true, int _beginData = 0, int _numData = Null) {
		data = &_data;
		useCols = _useCols;
		
		ids.SetCount(_ids.GetCount());
		for (int i = 0; i < ids.GetCount(); ++i)
			ids[i] = _ids[i];
		beginData = _beginData;
		if (IsNull(_numData)) {
			if (!useCols)
				numData = data->GetColumnCount();
			else
				numData = data->GetCount();
		} else {
			if (!useCols)
				numData = min(_numData, data->GetColumnCount());
			else
				numData = min(_numData, data->GetCount());
		}
		numData -= beginData;
	}
	void Init(ArrayCtrl &_data, int idY, int idX, bool _useCols = true, int _beginData = 0, int _numData = Null) {
		ids << idY << idX;
		Init(_data, ids, _useCols, _beginData, _numData);
	}
	virtual inline double y(int64 id)	{return useCols ? data->Get(beginData + int(id), ids[0]) : data->Get(ids[0], beginData + int(id));};
	virtual inline double x(int64 id) {
		if (IsNull(ids[1]))
			return double(id);
		else
			return useCols ? data->Get(beginData + int(id), ids[1]) : data->Get(ids[1], beginData + int(id));
	}
	virtual inline double xn(int n, int64 id)	{return useCols ? data->Get(beginData + int(id), ids[n]) : data->Get(ids[n], beginData + int(id));}
	virtual inline int64 GetCount()	{return numData;};
};

class GridCtrlSource : public DataSource {
private:
	GridCtrl *data;
	bool useCols;
	Vector<int> ids;
	int beginData;
	int64 numData;

public:
	GridCtrlSource() : data(0), useCols(true), beginData(0), numData(Null)  {ids << 0 << 1;}
	GridCtrlSource(GridCtrl &data, bool useCols = true, int idX = 0, int idY = 1, int beginData = 0, int numData = Null) : 
		data(&data), useCols(useCols), beginData(beginData), numData(numData) {
		ids << idY << idX;
		Init(data, ids, useCols, beginData, numData);
	}
	GridCtrlSource(GridCtrl &data, Vector<int> &_ids, bool useCols = true, int beginData = 0, int numData = Null) : 
		data(&data), useCols(useCols), beginData(beginData), numData(numData) {
		Init(data, ids, useCols, beginData, numData);
	}
	void Init(GridCtrl &_data, Vector<int> &_ids, bool _useCols = true, int _beginData = 0, int _numData = Null) {
		data = &_data;
		useCols = _useCols;

		ids.SetCount(_ids.GetCount());
		for (int i = 0; i < ids.GetCount(); ++i)
			ids[i] = _ids[i];
		beginData = _beginData;
		if (IsNull(_numData)) {
			if (!useCols)
				numData = data->GetColumnCount();
			else
				numData = data->GetRowCount();
		} else {
			if (!useCols)
				numData = min(_numData, data->GetColumnCount());
			else
				numData = min(_numData, data->GetRowCount());
		}
		numData -= beginData;
	}
	void Init(GridCtrl &_data, int idY, int idX, bool _useCols = true, int _beginData = 0, int _numData = Null) {
		Vector<int> ids;
		ids << idY << idX;
		Init(_data, ids, _useCols, _beginData, _numData);
	}	
	double GetVal(int64 id, int idx) {
		if (IsNull(ids[idx]))
			return double(id);
		else {
			int r, c;
			if (useCols) {
				r = beginData + int(id);
				c = ids[idx];
			} else {
				r = ids[idx];
				c = beginData + int(id);
			}
			const Value &val = data->Get(r, c);
			if (val.Is<double>())
				return double(val);
			else
				return ScanDouble(val.ToString());
		}	
	}
	virtual inline double y(int64 id) {return GetVal(id, 0);}
	virtual inline double x(int64 id) {return GetVal(id, 1);}
	virtual inline double xn(int n, int64 id)	{return useCols ? data->Get(beginData + int(id), ids[n]) : data->Get(ids[n], beginData + int(id));}
	virtual inline int64 GetCount()	{return numData;};
};

class ScatterCtrl : public Ctrl, public ScatterDraw {
public:
	typedef ScatterCtrl CLASSNAME;
	
	ScatterCtrl();
	virtual ~ScatterCtrl() {};
	
	enum ScatterAction {NO_ACTION = 0, SCROLL, ZOOM_H_ENL, ZOOM_H_RED, ZOOM_V_ENL, ZOOM_V_RED, SHOW_COORDINATES, CONTEXT_MENU, ZOOM_WINDOW, 
					  SCROLL_LEFT, SCROLL_RIGHT, SCROLL_UP, SCROLL_DOWN, ZOOM_FIT};
	#define SHOW_INFO SHOW_COORDINATES
	
	struct MouseBehavior {
		MouseBehavior(bool ctrl, bool alt, bool shift, bool left, bool middle, int middleWheel, bool right, ScatterAction action) : 
			ctrl(ctrl), alt(alt), shift(shift), left(left), middle(middle), middleWheel(middleWheel), right(right), action(action) {}
		bool ctrl;
		bool alt;
		bool shift;
		bool left;
		bool middle;
		int middleWheel;
		bool right;
		ScatterAction action;
	};
	void AddMouseBehavior(bool ctrl, bool alt, bool shift, bool left, bool middle, int middlewheel, bool right, ScatterAction action);
	void RemoveMouseBehavior(ScatterAction action);	
	void ClearMouseBehavior();
	
	enum MouseAction {NONE, LEFT_DOWN, LEFT_UP, LEFT_DOUBLE, LEFT_MOVE, MIDDLE_DOWN, MIDDLE_UP, MIDDLE_MOVE, 
					  RIGHT_DOWN, RIGHT_UP, RIGHT_MOVE};
	Callback3<Point, dword, MouseAction> WhenMouseClick;
	
	struct KeyBehavior {		
		KeyBehavior(bool ctrl, bool alt, bool shift, int key, bool isVirtualKey, ScatterAction action) : 
			ctrl(ctrl), alt(alt), shift(shift), key(key), isVirtualKey(isVirtualKey), action(action) {}
		bool ctrl;
		bool alt;
		bool shift;
		int key;
		bool isVirtualKey;
		ScatterAction action;
	};
	void AddKeyBehavior(bool ctrl, bool alt, bool shift, int key, bool isVirtualKey, ScatterAction action); 
	void RemoveKeyBehavior(ScatterAction action);	
	void ClearKeyBehavior();
	
	ScatterCtrl& ShowContextMenu(bool show = true) 	{showContextMenu = show; return *this;}
	ScatterCtrl& ShowPropertiesDlg(bool show = true){
		showPropDlg = show; 	
	 	CheckButtonVisible();
		return *this;
	}
	ScatterCtrl& ShowProcessDlg(bool show = true) {
		showProcessDlg = show;  
		CheckButtonVisible();
		return *this;
	}
	ScatterCtrl& ShowButtons(bool show = true) {
		showButtons = show;
		CheckButtonVisible();
		return *this;
	}
	void CheckButtonVisible();
	
	ScatterCtrl& ShowLoadData(bool showLoadData) {
		this->showLoadData = showLoadData;
		return *this;
	}
	ScatterCtrl& ShowSaveData(bool showSaveData) {
		this->showSaveData = showSaveData;
		return *this;
	}
	
	ScatterCtrl& SetPopText(const String x, const String y1, const String y2, const String z = "z") 	
							{popTextX = x; popTextY = y1; popTextY2 = y2; popTextZ = z; return *this;}

	ScatterCtrl& ShowInfo(bool show = true)					{showInfo = show;		 return *this;}
	ScatterCtrl& ShowAllMenus()	{return ShowInfo().ShowContextMenu().ShowPropertiesDlg().ShowProcessDlg().ShowButtons();}
	
#ifdef PLATFORM_WIN32
	void SaveAsMetafile(const char* file);
#endif	
	void SaveToClipboard(bool saveAsMetafile = false);
	void SaveToFile(String fileName = Null);
	
	void LoadControl();
	void SaveControl();
	void OnChangeSaveCtrl();
	
	virtual void Refresh() {Ctrl::Refresh();};
	virtual Size GetSize() const {return Ctrl::GetSize();};
	
	ScatterCtrl& SetColor(const Upp::Color& _color)				{ScatterDraw::SetColor(_color);				return *this;};
	ScatterCtrl& SetGridColor(const Upp::Color& grid_color)		{ScatterDraw::SetGridColor(grid_color);		return *this;};
	ScatterCtrl& SetGridWidth(double grid_width) 				{ScatterDraw::SetGridWidth(grid_width); 	return *this;};
	ScatterCtrl& SetPlotAreaColor(const Upp::Color& p_a_color)	{ScatterDraw::SetPlotAreaColor(p_a_color); 	return *this;};
	ScatterCtrl& SetAxisColor(const Upp::Color& axis_color)		{ScatterDraw::SetAxisColor(axis_color);		return *this;};
	ScatterCtrl& SetAxisWidth(int axis_width)					{ScatterDraw::SetAxisWidth(axis_width);		return *this;};
	ScatterCtrl& SetTitle(const String& title)		 			{ScatterDraw::SetTitle(title); 				return *this;};
	ScatterCtrl& SetTitleFont(const Upp::Font& fontTitle) 		{ScatterDraw::SetTitleFont(fontTitle); 		return *this;};
	ScatterCtrl& SetTitleColor(const Upp::Color& colorTitle)	{ScatterDraw::SetTitleColor(colorTitle);	return *this;};
	ScatterCtrl& SetLabelsFont(const Upp::Font& fontLabels) 	{ScatterDraw::SetLabelsFont(fontLabels); 	return *this;};
	ScatterCtrl& SetLabelsColor(const Upp::Color& colorLabels)	{ScatterDraw::SetLabelsColor(colorLabels);	return *this;};
	ScatterCtrl& SetLabelX(const String& xLabel)				{ScatterDraw::SetLabelX(xLabel); 			return *this;};
	ScatterCtrl& SetLabelY(const String& yLabel)				{ScatterDraw::SetLabelY(yLabel); 			return *this;};
	ScatterCtrl& SetLabelY2(const String& yLabel)				{ScatterDraw::SetLabelY2(yLabel); 			return *this;};
	ScatterCtrl& SetPlotAreaMargin(int hLeft, int hRight, int vTop, int vBottom)
																{ScatterDraw::SetPlotAreaMargin(hLeft, hRight, vTop, vBottom); return *this;};
	ScatterCtrl& SetPlotAreaLeftMargin(int margin)				{ScatterDraw::SetPlotAreaLeftMargin(margin);return *this;};
	int GetPlotAreaLeftMargin()									{return ScatterDraw::GetPlotAreaLeftMargin();};
	ScatterCtrl& SetPlotAreaTopMargin(int margin)				{ScatterDraw::SetPlotAreaTopMargin(margin);	return *this;};
	int GetPlotAreaTopMargin()									{return ScatterDraw::GetPlotAreaTopMargin();};
	ScatterCtrl& SetPlotAreaRightMargin(int margin)				{ScatterDraw::SetPlotAreaRightMargin(margin);return *this;};
	int GetPlotAreaRightMargin()								{return ScatterDraw::GetPlotAreaRightMargin();};
	ScatterCtrl& SetPlotAreaBottomMargin(int margin)			{ScatterDraw::SetPlotAreaBottomMargin(margin);return *this;};
	int GetPlotAreaBottomMargin()								{return ScatterDraw::GetPlotAreaBottomMargin();};
	
	ScatterCtrl& ShowLegend(bool show = true) 					{ScatterDraw::ShowLegend(show); 			return *this;}
	bool GetShowLegend() 										{return ScatterDraw::GetShowLegend();}
	ScatterCtrl& SetLegendPos(const Point &pos) 				{ScatterDraw::SetLegendPos(pos);			return *this;}
	ScatterCtrl& SetLegendPosX(int x)			 				{ScatterDraw::SetLegendPosX(x);				return *this;}
	ScatterCtrl& SetLegendPosY(int y) 							{ScatterDraw::SetLegendPosY(y);				return *this;}
	Point& GetLegendPos() 										{return ScatterDraw::GetLegendPos();}
	ScatterCtrl& SetLegendNumCols(int num) 						{ScatterDraw::SetLegendNumCols(num);		return *this;}
	int GetLegendNumCols() 										{return ScatterDraw::GetLegendNumCols();}
	ScatterCtrl& SetLegendRowSpacing(int num) 					{ScatterDraw::SetLegendRowSpacing(num);		return *this;}
	int GetLegendRowSpacing() 									{return ScatterDraw::GetLegendRowSpacing();}
	ScatterCtrl& SetLegendAnchor(int anchor) 					{ScatterDraw::SetLegendAnchor((LEGEND_POS)anchor);	return *this;}
	int GetLegendAnchor() 										{return ScatterDraw::GetLegendAnchor();}
	ScatterCtrl& SetLegendFillColor(const Upp::Color &fill)		{ScatterDraw::SetLegendFillColor(fill);		return *this;}
	ScatterCtrl& SetLegendBorderColor(const Upp::Color &border)	{ScatterDraw::SetLegendBorderColor(border);	return *this;}
	Upp::Color& GetLegendFillColor() 							{return ScatterDraw::GetLegendFillColor();}
	Upp::Color& GetLegendBorderColor() 							{return ScatterDraw::GetLegendBorderColor();}
	
	using ScatterDraw::AddSeries; 
	ScatterCtrl &AddSeries(ArrayCtrl &data, bool useCols = true, int idX = 0, int idY = 1, int idZ = 2, int beginData = 0, int numData = Null);
	ScatterCtrl &AddSeries(GridCtrl &data, bool useCols = true, int idX = 0, int idY = 1, int idZ = 2, int beginData = 0, int numData = Null);
	using ScatterDraw::InsertSeries; 
	void InsertSeries(int id, ArrayCtrl &data, bool useCols = true, int idX = 0, int idY = 1, int idZ = 2, int beginData = 0, int numData = Null);	
	void InsertSeries(int id, GridCtrl &data, bool useCols = true, int idX = 0, int idY = 1, int idZ = 2, int beginData = 0, int numData = Null);	
	
	ScatterCtrl& SetMaxRefreshTime(int _maxRefresh_ms) 	{maxRefresh_ms = _maxRefresh_ms; return *this;}
	int GetMaxRefreshTime() 							{return maxRefresh_ms;}
	
	ScatterCtrl& SetDefaultCSVSeparator(String sep) 	{defaultCSVseparator = sep;	return *this;}
	String GetDefaultCSVSeparator() 					{return defaultCSVseparator;}
	
	ScatterCtrl &SetMouseHandling(bool valx = true, bool valy = false)			{ScatterDraw::SetMouseHandling(valx, valy);			return *this;} 
	ScatterCtrl &SetMouseHandlingLinked(bool valx = true, bool valy = false) 	{ScatterDraw::SetMouseHandlingLinked(valx, valy);	return *this;}
	
	ScatterCtrl &SetSaveSize(Size &size) 	{saveSize = size; return *this;}
	Size &GetSaveSize() 					{return saveSize;}
	ScatterCtrl &SetJPGQuality(int quality) {jpgQuality = quality; return *this;}
	int GetJPGQuality() 					{return jpgQuality;}

	enum Angle {Angle_0, Angle_90, Angle_180, Angle_270};
	ScatterCtrl &Rotate(Angle angle)		{rotate = angle; Refresh(); return *this;}
	Angle GetRotate()						{return (Angle)rotate;}

private:	
	template <class T>
	void Ize(T& io) { 
		ScatterDraw::Ize(io);
		io
			("defaultFileNamePlot", defaultFileNamePlot)
			("defaultDataFile", defaultDataFile)
			("saveSize", saveSize)
			("jpgQuality", jpgQuality)
			("rotate", rotate)
		;
	}

public:
	void Xmlize(XmlIO& xml) 	{Ize(xml);}
	void Jsonize(JsonIO& json) 	{Ize(json);}
	
	void Serialize(Stream& s) {
		s % defaultFileNamePlot
		  % defaultDataFile
		  % saveSize
		  % jpgQuality
		  % rotate
		;
	}
	virtual void Paint(Draw& w);
	virtual void LeftDown(Point, dword);
	virtual void LeftDouble(Point p, dword);
	virtual void LeftUp(Point, dword);
	virtual void MiddleDown(Point, dword);
	virtual void MouseMove(Point, dword);
	virtual void MiddleUp(Point, dword);
	virtual void RightDown(Point, dword);
	virtual void RightUp(Point, dword);
	virtual void MouseLeave();
	virtual void MouseWheel(Point, int zdelta, dword);
	virtual bool Key(dword key, int count);
	virtual void GotFocus();
	virtual void LostFocus();
		
private:
	bool showInfo;
	PopUpInfo popTextBegin, popTextVert, popTextHoriz, popTextEnd;
	String popTextX, popTextY, popTextY2, popTextZ;
	Point popLT, popRB;
	bool isZoomWindow;
	const Point popOffset;
	MouseAction mouseAction;
	
	int butDownX, butDownY;
	bool isScrolling, isLabelPopUp;
	
	bool showContextMenu;
	bool showPropDlg;
	bool showProcessDlg;
	bool showButtons;
	bool showLoadData, showSaveData;
	
	int lastRefresh_ms;
	dword lastRefresh0_ms;
	int maxRefresh_ms;
	
	bool highlighting;
	
	Upp::Array<MouseBehavior> mouseBehavior; 
	Upp::Array<KeyBehavior> keyBehavior;
	
	void Paint0(Draw& w, const Size &sz);
	
	void ProcessPopUp(const Point &pt);
	//void ProcessClickSeries(const Point &pt);
	
	void DoMouseAction(bool down, Point pt, ScatterAction action, int wheel);
	void DoKeyAction(ScatterAction action);
	void ProcessMouse(bool down, Point &pt, bool ctrl, bool alt, bool shift, bool left, bool middle, int middleWheel, bool right); 
	bool ProcessKey(int key); 
	void LabelPopUp(bool down, Point &pt); 
	void Scrolling(bool down, Point &pt, bool isOut = false);
	void MouseZoom(int zdelta, bool hor, bool ver);
	void ZoomWindow(bool down, Point &pt); 
	
	void ContextMenu(Bar& bar);
	void DoShowEditDlg(int itab);
	void DoShowData();
	void DoProcessing();
	
	virtual Image CursorImage(Point p, dword keyflags);
	
	template <class T>
	void SetDrawing(T& w, const Size &sz, bool ctrl = true);	
	void TimerCallback();	
	
	String defaultCSVseparator;

	String defaultFileNamePlot;	
	void OnTypeImage(FileSel *_fs);
	
	String defaultDataFile;
	void OnTypeDataFile(FileSel *_fs);
	
	Button processButton, dataButton, propertiesButton;
	
	Size saveSize;
	int jpgQuality;
	
	int rotate;
	Point &MousePointRot(Point &pt);		
	Point &MousePointUnrot(Point &pt);		
};

template <class T>
void ScatterCtrl::SetDrawing(T& w, const Size &sz, bool ctrl) {
	ScatterDraw::SetSize(sz);
	ScatterDraw::SetDrawing(w, ctrl);
	if (!IsNull(popLT) && popLT != popRB) {
		if (isZoomWindow) {
			DrawLine(w, popLT.x, popLT.y, popLT.x, popRB.y, 1, SColorHighlight());
			DrawLine(w, popRB.x, popLT.y, popRB.x, popRB.y, 1, SColorHighlight());
			DrawLine(w, popLT.x, popLT.y, popRB.x, popLT.y, 1, SColorHighlight());
			DrawLine(w, popLT.x, popRB.y, popRB.x, popRB.y, 1, SColorHighlight());
#ifdef PLATFORM_WIN32			
			Ctrl::Refresh();	
#endif
		} else {
			DrawVArrow(w, popLT.x, popLT.y, popLT.x, popRB.y, 1, 4, 15, SColorHighlight());
			DrawHArrow(w, popLT.x, popRB.y, popRB.x, popRB.y, 1, 4, 15, SColorHighlight());
#ifdef PLATFORM_WIN32			
			Ctrl::Refresh(min(popLT.x-4, popRB.x-4), min(popLT.y-4, popRB.y-4), 
						  abs(popRB.x-popLT.x) + 9, abs(popRB.y-popLT.y) + 9);
#endif
		}
	} 
}

#include "Properties.h"

#endif
