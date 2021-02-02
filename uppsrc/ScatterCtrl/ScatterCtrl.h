#ifndef _ScatterCtrl_ScatterCtrl_h
#define _ScatterCtrl_ScatterCtrl_h

#include <ScatterDraw/ScatterDraw.h>
#include <plugin/jpg/jpg.h>
#include <PdfDraw/PdfDraw.h>
#include <GridCtrl/GridCtrl.h>
#include <plugin/Eigen/Eigen.h>
#include <ScatterDraw/Histogram.h>

#include "PopUpText.h"

namespace Upp {
	
class EditDoubleLostFocus : public EditDouble {
public:
	Event<> WhenLostFocus;
	
	virtual void LostFocus() {
		WhenLostFocus();
		EditDouble::LostFocus();
	}
};

typedef WithSpin<double, EditDoubleLostFocus> EditDoubleLostFocusSpin;

class CtrlScroll : public StaticRect {
public:
	CtrlScroll();

	CtrlScroll &AddPane(Ctrl& c, bool scrollH = true, bool scrollV = true);
	CtrlScroll &AddPaneH(Ctrl& c) 	{return AddPane(c, true, false);}
	CtrlScroll &AddPaneV(Ctrl& c)	{return AddPane(c, false, true);}
	inline bool HasPane() const 	{return (~pane != NULL);}

	Event<> WhenScrolled;
	
private:
	virtual void Layout();
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);

	void Scroll(const Point& p);
	void OnScroll();

	ScrollBars scroll;

	Ptr<Ctrl> pane;
	bool hsizepos, vsizepos;
};

class ArrayCtrlSource : public DataSource {
private:
	ArrayCtrl *data;
	bool useCols;
	Vector<int> ids;
	int beginData;
	int64 numData;

public:
	ArrayCtrlSource() : data(0), useCols(true), beginData(0), numData(Null) {ids << 0 << 1;}
	ArrayCtrlSource(ArrayCtrl &_data, bool _useCols = true, int _idX = 0, int _idY = 1, int _beginData = 0, int _numData = Null) {
		Init(_data, _idY, _idX, _useCols, _beginData, _numData);
	}
	void Init(ArrayCtrl &_data, Vector<int> &_ids, bool _useCols = true, int _beginData = 0, int _numData = Null) {
		data = &_data;
		useCols = _useCols;
		
		ids = clone(_ids);
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
	virtual inline double y(int64 id) 	{return useCols ? data->Get(beginData + int(id), ids[0]) : data->Get(ids[0], beginData + int(id));};
	virtual inline double x(int64 id)  {
		if (IsNull(ids[1]))
			return double(id);
		else
			return useCols ? data->Get(beginData + int(id), ids[1]) : data->Get(ids[1], beginData + int(id));
	}
	virtual inline double xn(int n, int64 id)	{return useCols ? data->Get(beginData + int(id), ids[n]) : data->Get(ids[n], beginData + int(id));}
	virtual inline int64 GetCount() const		{return numData;};
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
	GridCtrlSource(GridCtrl &_data, bool _useCols = true, int idX = 0, int idY = 1, int _beginData = 0, int _numData = Null) {
		ids << idY << idX;
		Init(_data, ids, _useCols, _beginData, _numData);
	}
	GridCtrlSource(GridCtrl &_data, Vector<int> &_ids, bool _useCols = true, int _beginData = 0, int _numData = Null) {
		Init(_data, _ids, _useCols, _beginData, _numData);
	}
	void Init(GridCtrl &_data, Vector<int> &_ids, bool _useCols = true, int _beginData = 0, int _numData = Null) {
		data = &_data;
		useCols = _useCols;

		ids = clone(_ids);
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
		Vector<int> _ids;
		_ids << idY << idX;
		Init(_data, _ids, _useCols, _beginData, _numData);
	}	
	double GetVal(int64 id, int idx) const {
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
	virtual inline double y(int64 id)  {return GetVal(id, 0);}
	virtual inline double x(int64 id)  {return GetVal(id, 1);}
	virtual inline double xn(int n, int64 id)	{return useCols ? data->Get(beginData + int(id), ids[n]) : data->Get(ids[n], beginData + int(id));}
	virtual inline int64 GetCount() const		{return numData;};
};

class ScatterCtrl : public Ctrl, public ScatterDraw {
public:
	ScatterCtrl();
	virtual ~ScatterCtrl() noexcept {RemoveInstance(this);};
	
	enum ScatterAction {NO_ACTION = 0, SCROLL, ZOOM_H_ENL, ZOOM_H_RED, ZOOM_V_ENL, ZOOM_V_RED, 
						SHOW_COORDINATES, CONTEXT_MENU, ZOOM_WINDOW, 
					  	SCROLL_LEFT, SCROLL_RIGHT, SCROLL_UP, SCROLL_DOWN, ZOOM_FIT};
	#define SHOW_INFO SHOW_COORDINATES
	
	struct MouseBehavior {
		MouseBehavior(bool _ctrl, bool _alt, bool _shift, bool _left, bool _middle, int _middleWheel, bool _right, ScatterAction _action) : 
			ctrl(_ctrl), alt(_alt), shift(_shift), left(_left), middle(_middle), middleWheel(_middleWheel), right(_right), action(_action) {}
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
	Function<void(Point, dword, MouseAction)> WhenMouseClick;
	
	struct KeyBehavior {		
		KeyBehavior(bool _ctrl, bool _alt, bool _shift, int _key, bool _isVirtualKey, ScatterAction _action) : 
			ctrl(_ctrl), alt(_alt), shift(_shift), key(_key), isVirtualKey(_isVirtualKey), action(_action) {}
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
	
	ScatterCtrl& ShowLoadData(bool _showLoadData) {
		this->showLoadData = _showLoadData;
		return *this;
	}
	ScatterCtrl& ShowSaveData(bool _showSaveData) {
		this->showSaveData = _showSaveData;
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
	bool SaveToFile(String fileName = Null);
	
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
	ScatterCtrl& SetTitle(const String& _title)		 			{ScatterDraw::SetTitle(_title); 			return *this;};
	ScatterCtrl& SetTitleFont(const Upp::Font& fontTitle) 		{ScatterDraw::SetTitleFont(fontTitle); 		return *this;};
	ScatterCtrl& SetTitleColor(const Upp::Color& colorTitle)	{ScatterDraw::SetTitleColor(colorTitle);	return *this;};
	ScatterCtrl& SetLabelsFont(const Upp::Font& fontLabels) 	{ScatterDraw::SetLabelsFont(fontLabels); 	return *this;};
	ScatterCtrl& SetLabelsColor(const Upp::Color& colorLabels)	{ScatterDraw::SetLabelsColor(colorLabels);	return *this;};
	ScatterCtrl& SetLabelX(const String& _xLabel)				{ScatterDraw::SetLabelX(_xLabel); 			return *this;};
	ScatterCtrl& SetLabelY(const String& _yLabel)				{ScatterDraw::SetLabelY(_yLabel); 			return *this;};
	ScatterCtrl& SetLabelY2(const String& _yLabel)				{ScatterDraw::SetLabelY2(_yLabel); 			return *this;};
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
	ScatterCtrl& SetLegendPos(const Point &_pos) 				{ScatterDraw::SetLegendPos(_pos);			return *this;}
	ScatterCtrl& SetLegendPosX(int x)			 				{ScatterDraw::SetLegendPosX(x);				return *this;}
	ScatterCtrl& SetLegendPosY(int y) 							{ScatterDraw::SetLegendPosY(y);				return *this;}
	Point& GetLegendPos() 										{return ScatterDraw::GetLegendPos();}
	ScatterCtrl& SetLegendNumCols(int num) 						{ScatterDraw::SetLegendNumCols(num);		return *this;}
	int GetLegendNumCols() 										{return ScatterDraw::GetLegendNumCols();}
	ScatterCtrl& SetLegendRowSpacing(int num) 					{ScatterDraw::SetLegendRowSpacing(num);		return *this;}
	int GetLegendRowSpacing() 									{return ScatterDraw::GetLegendRowSpacing();}
	ScatterCtrl& SetLegendAnchor(int anchor) 					{ScatterDraw::SetLegendAnchor(static_cast<LEGEND_POS>(anchor));	return *this;}
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
	
	ScatterCtrl& RemoveAllSeries() {
		GuiLock __;
		ScatterDraw::RemoveAllSeries();
		return *this;
	}
		
	ScatterCtrl& SetMaxRefreshTime(int _maxRefresh_ms) 	{maxRefresh_ms = _maxRefresh_ms; return *this;}
	int GetMaxRefreshTime() 							{return maxRefresh_ms;}
	
	ScatterCtrl& SetDefaultCSVSeparator(String sep) 	{defaultCSVseparator = sep;	return *this;}
	String GetDefaultCSVSeparator() 					{return defaultCSVseparator;}
	
	ScatterCtrl &SetMouseHandling(bool valx = true, bool valy = false)			{ScatterDraw::SetMouseHandling(valx, valy);			return *this;} 
	ScatterCtrl &SetMouseHandlingLinked(bool valx = true, bool valy = false) 	{ScatterDraw::SetMouseHandlingLinked(valx, valy);	return *this;}
	
	ScatterCtrl &SetSaveSize(Size &_size) 	{saveSize = _size; return *this;}
	Size &GetSaveSize() 					{return saveSize;}
	ScatterCtrl &SetSaveSizeX(int cx) 		{saveSize.cx = cx; return *this;}
	ScatterCtrl &SetSaveSizeY(int cy) 		{saveSize.cy = cy; return *this;}
	ScatterCtrl &SetJPGQuality(int quality) {jpgQuality = quality; return *this;}
	int GetJPGQuality() 					{return jpgQuality;}

	enum Angle {Angle_0, Angle_90, Angle_180, Angle_270};
	ScatterCtrl &Rotate(Angle angle)		{rotate = angle; Refresh(); return *this;}
	Angle GetRotate()						{return static_cast<Angle>(rotate);}

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
	
	static int GetInstancesCount()			{return instances.GetCount();}	
	static ScatterCtrl &GetInstance(int i)	{return *(instances[i]);}	
	
	void SetPopUp(bool _pop)				{pop = _pop;}
	
private:
	bool showInfo = false;
	PopUpInfo popInfoBegin, popInfoVert, popInfoHoriz, popInfoEnd;
	PopUpText popTextBegin, popTextVert, popTextHoriz, popTextEnd;
	String popTextX  = t_("x"), 
		   popTextY  = t_("y"), 
		   popTextY2 = t_("y right"), 
		   popTextZ  = t_("z");
	Point popLT = Null, popRB = Null;
	bool pop;
	bool isZoomWindow = false;
	const Point popOffset = Point(10, 12);
	MouseAction mouseAction = NONE;
	
	int butDownX, butDownY;
	bool isScrolling = false, 
		 isLabelPopUp = false;
	
	bool showContextMenu = false,
		 showPropDlg = false,
		 showProcessDlg = false,
		 showButtons = false,
		 showLoadData = false, showSaveData = false;
	
	int lastRefresh_ms = Null;
	dword lastRefresh0_ms;
	int maxRefresh_ms = 500;
	
	bool highlighting = false;
	
	Upp::Array<MouseBehavior> mouseBehavior; 
	Upp::Array<KeyBehavior> keyBehavior;
	
	void Paint0(Draw& w, const Size &sz);
	
	void ProcessPopUp(Point &pt);
	
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
	
	void Closest(double &x, double &y, double &y2);
		
	virtual Image CursorImage(Point p, dword keyflags);
	
	template <class T>
	void SetDrawing(T& w, const Size &sz, bool ctrl = true);	
	void TimerCallback();	
	
	String defaultCSVseparator = ";";

	String defaultFileNamePlot;	
	void OnTypeImage(FileSel *_fs);
	
	String defaultDataFile;
	void OnTypeDataFile(FileSel *_fs);
	
	Button processButton, dataButton, propertiesButton;
	
	Size saveSize = Size(1000, 600);
	int jpgQuality = 90;
	
	int rotate = Angle_0;
	Point &MousePointRot(Point &pt);		
	Point &MousePointUnrot(Point &pt);	
	
	static Vector<ScatterCtrl *> instances;	
	static void AddInstance(ScatterCtrl *instance) {instances << instance;}
	static void RemoveInstance(ScatterCtrl *instance) {
		instances.RemoveIf([&](int i) {return instance == instances[i];});
	}
};

template <class T>
void ScatterCtrl::SetDrawing(T& w, const Size &sz, bool ctrl) {
	ScatterDraw::SetSize(sz);
	ScatterDraw::SetDrawing(w, ctrl);
	if (!IsNull(popLT)) {
	 	if (popLT != popRB) {
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
				if (!pop) {
					popTextBegin.DoPaint(w);
					popTextVert.DoPaint(w);
					popTextHoriz.DoPaint(w);
					popTextEnd.DoPaint(w);
				}
#ifdef PLATFORM_WIN32			
				Ctrl::Refresh(min(popLT.x-4, popRB.x-4), min(popLT.y-4, popRB.y-4), 
							  abs(popRB.x-popLT.x) + 9, abs(popRB.y-popLT.y) + 9);
#endif
			}
	 	} else {
	 		if (!pop)
	 			popTextBegin.DoPaint(w);
#ifdef PLATFORM_WIN32			
			Ctrl::Refresh();	
#endif 		
	 	}
	} 
}

class ScatterWindow : public TopWindow {
public:
	ScatterWindow() {
		Sizeable().Zoomable();
		Add(scatter.SizePos());
		scatter.ShowAllMenus();
		pos.Clear();
	}
	ScatterCtrl &operator()()		{return scatter;}
	
	ScatterWindow &OpenMain(bool dataInternal = true, bool zoomToFit = true) {
		if (scatter.ThereArePrimaryY()) 
			scatter.SetPlotAreaLeftMargin(100).SetDrawYReticle(true).SetDrawYReticleNumbers(true);
		else 
			scatter.SetPlotAreaLeftMargin(10).SetDrawYReticle(false).SetDrawYReticleNumbers(false);
		if (scatter.ThereAreSecondaryY()) 
			scatter.SetPlotAreaRightMargin(100).SetDrawY2Reticle(true).SetDrawY2ReticleNumbers(true);
		else 
			scatter.SetPlotAreaRightMargin(10).SetDrawY2Reticle(false).SetDrawY2ReticleNumbers(false);

		if (dataInternal)
			scatter.SetDataSourceInternal();
		if (zoomToFit)
			scatter.ZoomToFit(true, true);
		TopWindow::SetRect(0, 0, width, height);
		TopWindow::OpenMain();
		Ctrl::ProcessEvents();
		return *this;
	}

	void Refresh() {
		scatter.Refresh();
		Ctrl::ProcessEvents();
	}
	
private:	
	ScatterCtrl scatter;	
	int width = 800, height = 500;
	Point pos;
};

class ScatterWindowPool {
public:
	static ScatterWindow &Get()	{return pool.Add();}

private:
	static Array<ScatterWindow> pool;
};

}

#include "Properties.h"


#endif
