#ifndef _ScatterCtrl_ScatterCtrl_h
#define _ScatterCtrl_ScatterCtrl_h

#include "PopUpText.h"
#include <ScatterDraw/ScatterDraw.h>
#include <plugin/jpg/jpg.h>
#include <GridCtrl/GridCtrl.h>

NAMESPACE_UPP

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
		Init(data, ids, useCols, beginData, numData);
	}
	void Init(ArrayCtrl &_data, Vector<int> &_ids, bool _useCols = true, int _beginData = 0, int _numData = Null) {
		data = &_data;
		useCols = _useCols;
		
		ids.SetCount(_ids.GetCount());
		for (int i = 0; i < ids.GetCount(); ++i)
			ids[i] = _ids[i];
		beginData = _beginData;
		numData = _numData;
		if (IsNull(_numData)) {
			if (!useCols)
				numData = data->GetColumnCount() - beginData;
			else
				numData = data->GetCount() - beginData;
		}
	}
	void Init(ArrayCtrl &_data, int idY, int idX, bool _useCols = true, int _beginData = 0, int _numData = Null) {
		Vector<int> ids;
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
		numData = _numData;
		if (IsNull(_numData)) {
			if (!useCols)
				numData = data->GetColumnCount() - beginData;
			else
				numData = data->GetRowCount() - beginData;
		}		
	}
	void Init(GridCtrl &_data, int idY, int idX, bool _useCols = true, int _beginData = 0, int _numData = Null) {
		Vector<int> ids;
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

class ScatterCtrl : public StaticRect, public ScatterDraw {
public:
	typedef ScatterCtrl CLASSNAME;
	ScatterCtrl();
	
	enum MouseAction {NO_ACTION = 0, SCROLL, ZOOM_H_ENL, ZOOM_H_RED, ZOOM_V_ENL, ZOOM_V_RED, SHOW_INFO, CONTEXT_MENU};

	struct MouseBehaviour {
		bool ctrl;
		bool alt;
		bool shift;
		bool left;
		bool middle;
		int middleWheel;
		bool right;
		MouseAction action;
	};
	
	#define MAX_MOUSEBEHAVIOR 20
	
	bool SetMouseBehavior(MouseBehaviour *_mouseBehavior);
	ScatterCtrl& ShowContextMenu(bool show = true) 			{showContextMenu = show; return *this;}
	ScatterCtrl& ShowPropertiesDlg(bool show = true)		{showPropDlg = show; 	 return *this;}
	ScatterCtrl& SetPopText(const String x, const String y1, const String y2) 	
															{popTextX = x; popTextY = y1; popTextY2 = y2; return *this;}
	ScatterCtrl& SetMouseHandling(bool valx = true, bool valy = false);
	ScatterCtrl& ShowInfo(bool show = true)					{paintInfo = show;		 return *this;}
	
#ifdef PLATFORM_WIN32
	void SaveAsMetafile(const char* file);
#endif	
	void SaveToClipboard(bool saveAsMetafile = false);
	void SaveToFile(String fileName = Null);
	
	virtual void Refresh() {Ctrl::Refresh();};
	virtual Size GetSize() const {return Ctrl::GetSize();};
	
	ScatterCtrl& SetColor(const Upp::Color& _color)				{ScatterDraw::SetColor(_color);				return *this;};
	ScatterCtrl& SetGridColor(const Upp::Color& grid_color)		{ScatterDraw::SetGridColor(grid_color);		return *this;};
	ScatterCtrl& SetGridWidth(int grid_width) 					{ScatterDraw::SetGridWidth(grid_width); 	return *this;};
	ScatterCtrl& SetPlotAreaColor(const Upp::Color& p_a_color)	{ScatterDraw::SetPlotAreaColor(p_a_color); 	return *this;};
	ScatterCtrl& SetAxisColor(const Upp::Color& axis_color)		{ScatterDraw::SetAxisColor(axis_color);		return *this;};
	ScatterCtrl& SetAxisWidth(int axis_width)					{ScatterDraw::SetAxisWidth(axis_width);		return *this;};
	ScatterCtrl& SetTitle(const String& title)		 			{ScatterDraw::SetTitle(title); 				return *this;};
	ScatterCtrl& SetTitleFont(const Font& fontTitle) 			{ScatterDraw::SetTitleFont(fontTitle); 		return *this;};
	ScatterCtrl& SetTitleColor(const Upp::Color& colorTitle)	{ScatterDraw::SetTitleColor(colorTitle);	return *this;};
	ScatterCtrl& SetLabelsFont(const Font& fontLabels) 			{ScatterDraw::SetLabelsFont(fontLabels); 	return *this;};
	ScatterCtrl& SetLabelsColor(const Upp::Color& colorLabels)	{ScatterDraw::SetLabelsColor(colorLabels);	return *this;};
	ScatterCtrl& SetLabelX(const String& xLabel)				{ScatterDraw::SetLabelX(xLabel); 			return *this;};
	ScatterCtrl& SetLabelY(const String& yLabel)				{ScatterDraw::SetLabelY(yLabel); 			return *this;};
	ScatterCtrl& SetLabelY2(const String& yLabel)				{ScatterDraw::SetLabelY2(yLabel); 			return *this;};
	ScatterCtrl& SetPlotAreaMargin(int hLeft, int hRight, int vTop, int vBottom)
																{ScatterDraw::SetPlotAreaMargin(hLeft, hRight, vTop, vBottom); return *this;};
	ScatterCtrl& SetPlotAreaLeftMargin(int margin)				{ScatterDraw::SetPlotAreaLeftMargin(margin);return *this;};
	ScatterCtrl& SetPlotAreaTopMargin(int margin)				{ScatterDraw::SetPlotAreaTopMargin(margin);	return *this;};
	ScatterCtrl& SetPlotAreaRightMargin(int margin)				{ScatterDraw::SetPlotAreaRightMargin(margin);return *this;};
	ScatterCtrl& SetPlotAreaBottomMargin(int margin)			{ScatterDraw::SetPlotAreaBottomMargin(margin);return *this;};
	
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
	ScatterCtrl& SetLegendAnchor(int anchor) 					{ScatterDraw::SetLegendAnchor(anchor);		return *this;}
	int GetLegendAnchor() 										{return ScatterDraw::GetLegendAnchor();}
	ScatterCtrl& SetLegendFillColor(const Upp::Color &fill)		{ScatterDraw::SetLegendFillColor(fill);		return *this;}
	ScatterCtrl& SetLegendBorderColor(const Upp::Color &border)	{ScatterDraw::SetLegendBorderColor(border);	return *this;}
	Upp::Color& GetLegendFillColor() 							{return ScatterDraw::GetLegendFillColor();}
	Upp::Color& GetLegendBorderColor() 							{return ScatterDraw::GetLegendBorderColor();}

	using ScatterDraw::AddSeries; 
	ScatterCtrl &AddSeries(ArrayCtrl &data, bool useCols = true, int idX = 0, int idY = 1, int idZ = 2, int beginData = 0, int numData = Null);
	using ScatterDraw::InsertSeries; 
	void InsertSeries(int id, ArrayCtrl &data, bool useCols = true, int idX = 0, int idY = 1, int idZ = 2, int beginData = 0, int numData = Null);	
	
	ScatterCtrl& SetCopyRatio(int ratio)				{copyRatio = ratio; return *this;}
	int GetCopyRatio()									{return copyRatio;}
	
	ScatterCtrl& SetMaxRefreshTime(int _maxRefresh_ms) 	{maxRefresh_ms = _maxRefresh_ms; return *this;}
	int GetMaxRefreshTime() 							{return maxRefresh_ms;}
	
private:
	bool paintInfo;
	Point clickPoint;
	PopUpInfo popText;
	String popTextX, popTextY, popTextY2;
	Point popLT, popRB;
	const Point offset;
	int copyRatio;

	int butDownX, butDownY;
	bool isScrolling, isLabelPopUp;
	
	bool mouseHandlingX, mouseHandlingY;
	bool showContextMenu;
	bool showPropDlg;
	
	int lastRefresh_ms;
	dword lastRefresh0_ms;
	int maxRefresh_ms;
	
	bool highlighting;
	
	MouseBehaviour *mouseBehavior;
	
	void ProcessPopUp(const Point & pt);

	virtual void Paint(Draw& w);
	virtual void LeftDown(Point, dword);
	virtual void LeftUp(Point, dword);
	virtual void MiddleDown(Point, dword);
	virtual void MouseMove(Point, dword);
	virtual void MiddleUp(Point, dword);
	virtual void RightDown(Point, dword);
	virtual void RightUp(Point, dword);
	virtual void MouseLeave();
	virtual void MouseWheel(Point, int zdelta, dword);
	
	void DoMouseAction(bool down, Point pt, MouseAction action, int value);
	void ProcessMouse(bool down, Point &pt, bool ctrl, bool alt, bool shift, bool left, bool middle, int middleWheel, bool right); 
	void LabelPopUp(bool down, Point &pt); 
	void Scrolling(bool down, Point &pt, bool isOut = false);
	void MouseZoom(int zdelta, bool hor, bool ver);
	
	void ContextMenu(Bar& bar);
	void DoShowEditDlg();
	
	virtual Image CursorImage(Point p, dword keyflags);
	
	template <class T>
	void SetDrawing(T& w, const Size &size, int scale);	
	void TimerCallback();	
	
	FileSel fileToSave;
};

template <class T>
void ScatterCtrl::SetDrawing(T& w, const Size &size, int scale) {
	ScatterDraw::SetDrawing(w, size, scale, true);
	if (!IsNull(popLT) && popLT != popRB) {
		DrawVArrow(w, popLT.x, popLT.y, popLT.x, popRB.y, 1, 4, 15, SColorHighlight());
		DrawHArrow(w, popLT.x, popRB.y, popRB.x, popRB.y, 1, 4, 15, SColorHighlight());
		Ctrl::Refresh(min(popLT.x-4, popRB.x-4), min(popLT.y-4, popRB.y-4), 
					  abs(popRB.x-popLT.x) + 9, abs(popRB.y-popLT.y) + 9);
	}
}

END_UPP_NAMESPACE

#endif
