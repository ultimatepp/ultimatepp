#ifndef FORM_VIEW_HPP
#define FORM_VIEW_HPP

#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
using namespace Upp;

#include <Form/FormLayout.hpp>

#define IMAGECLASS FormViewImg
#define IMAGEFILE <FormEditor/FormView.iml>
#include <Draw/iml_header.h>

class ScrollContainer;

class FormView : public Ctrl, public XMLConfig
{
public:
	FormView();
	void NullCallback() {}

///////////////////////////////////////////////////////////////////////////////////////////
// Draw methods / Прорисовка
///////////////////////////////////////////////////////////////////////////////////////////

public:
	virtual void Paint(Draw& w);

	void DrawRect(Draw& w, const Point& p, const Size& sz, int width, Color clr);
	void DrawRect(Draw& w, const Rect& r, int width, Color clr);
	void DrawGrid(Draw& w);
	void DrawObject(Draw& w, int id, const Color& c = LtGray(), bool springs = true);
	void DrawSprings(Draw& w, const Rect& r, dword align, dword valign,
		bool drawLeft = true, bool drawTop = true, bool drawRight = true,
			bool drawBottom = true, bool centered = true);
	void DrawGroupTools(Draw& w, const Rect& r);

///////////////////////////////////////////////////////////////////////////////////////////
// Events / События мыши / клавиатуры
///////////////////////////////////////////////////////////////////////////////////////////

public:
	virtual void LeftDouble(Point p, dword keyflags);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void LeftDrag(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);

	virtual void RightDown(Point p, dword keyflags);
	virtual void RightDrag(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void MouseLeave();

	virtual void Layout();

	bool ProcessLeftDrag(Point p, dword keyflags);
	void ProcessRightDrag(Point p, dword keyflags);

///////////////////////////////////////////////////////////////////////////////////////////
// Objects and lays / Работа с объектами / слоями
///////////////////////////////////////////////////////////////////////////////////////////

public:
	void CreateObject(Point p, const char* type);
	void CreateObjectMenu(Bar& bar, int id);

	void AlignObjectMenu(Bar& bar);
	void AddObjectMenu(Bar& bar, Point p);

	void ToggleOutlineDraw(int obj);
	bool IsOutlineDraw(int obj);

	int GetObjectCount() { return IsLayout() ? GetCurrentLayout()->GetObjects().GetCount() : 0; }
	Vector<FormObject>* GetObjects() { return IsLayout() ? &(GetCurrentLayout()->GetObjects()) : NULL; }

	void New();
	int GetLayoutCount() const { return _Layouts.GetCount(); }
	FormLayout* GetLayout(int id) { return IsLayout(id) ? &_Layouts[id] : NULL; }
	bool IsLayout(int id) const { return id >= 0 && id < GetLayoutCount(); }
	bool IsLayout() const { return IsLayout(_Layout); }
	FormLayout* GetCurrentLayout() { return IsLayout() ? (&(_Layouts[_Layout])) : NULL; }
	const FormLayout* GetCurrentLayout() const { return IsLayout() ? (&(_Layouts[_Layout])) : NULL; }

	void AddLayout(const String& name) { _Layouts.Add().Set("Form.Name", name);
		_Layout = _Layouts.GetCount() - 1; Refresh(); }
	void SelectLayout(int index)
	{
		if (!IsLayout(index)) return;
		_Layout = index;
		SetFormSize(_Layouts[index].GetFormSize());
		Refresh();
	}

	int HasLayout(const String& layout);

	void UpdateLayoutName(int index, const String& name) { if (!IsLayout(index)) return;
		_Layouts[index].Set("Form.Name", name); }

	bool RemoveLayout(int index)
	{
		if (!IsLayout(index))
			return false;

		_Layouts.Remove(index);
		int count = GetLayoutCount();
		if (count > 0 && _Layout >= count) SelectLayout(count - 1);
		if (count ==  0) _Layout = -1;
		return true;
	}

	void MoveUpObject(int id);
	void MoveDownObject(int id);
	void MoveToTopObject(int id);
	void MoveToBottomObject(int id);

	void AlignTopSelection();
	void AlignLeftSelection();
	void AlignRightSelection();
	void AlignBottomSelection();

	void InvertSelection();
	void RemoveSelection();
	Vector<int> GetSelected();
	int GetSelectedCount();
	FormObject* GetObject(int id);

	void ToggleSelection(int id);
	void ClearSelection();
	void AddToSelection(int id);	

///////////////////////////////////////////////////////////////////////////////////////////
// Methods for FormEdit / Взаимодействие с редактором
///////////////////////////////////////////////////////////////////////////////////////////

public:
	void SetContainer(ScrollContainer& cont) { _container = &cont; }

	void SetSprings(dword hAlign, dword vAlign);

	void ShowFrames() { _showInfo = 2; Refresh(); }
	void ShowInfo()   { _showInfo = 1; Refresh(); }
	void HideInfo()   { _showInfo = 0; Refresh(); }

	void UpdateTools() { WhenMenuBar.Execute(_tools); }
	void OpenObjectProperties(const Vector<int>& indexes) { WhenObjectProperties.Execute(indexes); }

///////////////////////////////////////////////////////////////////////////////////////////
// Filesystem / Работа с файловой системой
///////////////////////////////////////////////////////////////////////////////////////////

public:
	void Xmlize(XmlIO xml);
	bool SaveAll(const String& file, bool compression);
	bool LoadAll(const String& file, bool compression);

///////////////////////////////////////////////////////////////////////////////////////////
// Coordinates / Позиционирование элементов / система координат
///////////////////////////////////////////////////////////////////////////////////////////

public:
	int X(int x) { return GetPageRect().TopLeft().x + x; }
	int Y(int y) { return GetPageRect().TopLeft().y + y; }

	int ZoomX(int x) { return HorzLayoutZoom(x); }
	int ZoomY(int y) { return VertLayoutZoom(y); }

	int DeZoomX(int x) { return (int)(1000.0 / HorzLayoutZoom(1000) * x); }
	int DeZoomY(int y) { return (int)(1000.0 / VertLayoutZoom(1000) * y); }

	Point Zoom(Point p) { return Point(ZoomX(p.x), ZoomY(p.y)); }
	Size  Zoom(Size sz) { return LayoutZoom(sz); }
	Rect  Zoom(Rect r)  { return Rect( Zoom(r.TopLeft()), Zoom(r.GetSize()) ); }

	Point DeZoom(Point p) { return Point(DeZoomX(p.x), DeZoomY(p.y)); }
	Size  DeZoom(Size sz) { return Size(DeZoomX(sz.cx), DeZoomY(sz.cy)); }
	Rect  DeZoom(Rect r)  { return Rect( DeZoom(r.TopLeft()), DeZoom(r.GetSize()) ); }

	void SetFormSize(const Size& sz);
	Size GetFormSize() { return GetPageRect().GetSize(); }
	Size GetGridSize() { return Size( GetNumber("Grid.CX", 10), GetNumber("Grid.CY", 10) ); }

	void SetPageRect(const Rect& r) { if (IsLayout()) GetCurrentLayout()->SetPageRect(r); }
	Rect GetPageRect() const { return IsLayout() ? GetCurrentLayout()->GetPageRect() : Rect(); }

	Rect Offseted(const Rect& r) { return r.Offseted(GetPageRect().TopLeft().x, GetPageRect().TopLeft().y); }
	Rect Deoffseted(const Rect& r) { return r.Offseted(-GetPageRect().TopLeft().x, -GetPageRect().TopLeft().y); }

	Point Offseted(Point& r) { return Point(X(r.x), Y(r.y)); }

	void PointToGrid(Point& p);
	void RectToGrid(Rect& r);

	int XToGrid(int pos, bool use = true);
	int YToGrid(int pos, bool use = true);

	void SelectAllInRect(Rect r, bool add = false, bool toggle = false);
	bool IsObjectButton(Point p);
	dword IsGroupButton(Point p);
	bool IsSelected(int id);
	int ObjectFromPt(Point p);
	
	Rect GetObjectsRect();
	Rect GetSelectionRect();


///////////////////////////////////////////////////////////////////////////////////////////
// Variables / Переменные и типы
///////////////////////////////////////////////////////////////////////////////////////////

public:
	enum { TOOL_NONE, TOOL_LEFT, TOOL_TOP, TOOL_RIGHT, TOOL_BOTTOM };

	Callback1<const Vector<int>& > WhenObjectProperties;
	Callback1<const Vector<int>& > WhenChildSelected;
	Callback1<Vector<int> > WhenChildPos;
	Callback1<int> WhenChildCount;
	Callback1<Bar&> WhenMenuBar;
	Callback WhenUpdateLayouts;
	Callback WhenChildAllPos;
	Callback WhenChildZ;
	Callback WhenUpdate;
	Callback WhenUpdateTabs;

private:
	typedef FormView CLASSNAME;
	PARAMETER(int, Layout);
	PARAMETER(Font, Font);
	PARAMETER(dword, HAlign);
	PARAMETER(dword, VAlign);
	PARAMETER(Vector<FormLayout>, Layouts);

	Image _cursor;

	int _leftCur;
	int _topCur;
	int _rightCur;
	int _bottomCur;
	Image _toolLeft[3];
	Image _toolTop[3];
	Image _toolRight[3];
	Image _toolBottom[3];

	Rect _groupRect;

	bool _objectResize;
	bool _frameResize;
	int _showInfo;

	ScrollContainer* _container;
	
	Vector<Color> _colors;
	StaticRect _popup;
	ToolBar _tools;
};

#endif // .. FORM_VIEW_HPP
