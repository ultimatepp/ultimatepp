#ifndef MAP_COMMON__I_MAP_ITEM_H
#define MAP_COMMON__I_MAP_ITEM_H

#include "IMapEvent.h"
#include <Painter/Painter.h>

enum
{
	STATE_SHOWN,
	STATE_SELECTED,
	STATE_FOUND,
	STATE_EDIT,

	PERFORM_INIT,
	PERFORM_UPDATE,
	PERFORM_RELEASE,
	PERFORM_SEARCH
};

class IMapRender;
class IMapItem : public Pte<IMapItem>, public IMapEvent
{
public:
	IMapItem(IMapRender* parent);
	virtual ~IMapItem() { Clear(); }

	virtual void  Render(Painter* w) { Render((Draw*)w); }
	virtual void  Render(Draw* w) = 0;

	virtual bool  Contains(const Point& p) { return GetRect().Contains(p); }
	virtual Point GetTopLeft() { return GetRect().TopLeft(); }
	virtual Point GetCenter() { return GetRect().CenterPoint(); }
	virtual Rect  GetRect() = 0;
	virtual void  Clear() {}
	virtual Rect  GetRenderRect();

	virtual void  StatePerformed (dword state, const String& param = String()) {}
	virtual void  StateChanged   (dword state) {}
	virtual void  StateOff       (dword state);
	virtual void  StateOn        (dword state);
	virtual bool  IsState        (dword state);
	virtual void  PerformState   (dword state, const String& param = String());

	static  void  StatesPerformed(dword state, const String& param = String());
	static  void  StatesChanged  (dword state);
	static  void  StatesOff      (dword state);
	static  void  StatesOn       (dword state);
	static  bool  IsAnyState     (dword state);
	static  void  PerformStates  (dword state, const String& param = String());

public:
	bool IsShown() { return IsState(STATE_SHOWN); }
	bool IsFound() { return IsState(STATE_FOUND); }
	bool IsSelected() { return IsState(STATE_SELECTED); }

	void        SetParent(Ptr<IMapRender> parent) { _parent = parent; }
	IMapRender* GetParent() { return _parent; }

	void        SetTopRender(Ptr<IMapRender> render) { _topRender = render; }
	IMapRender* GetTopRender() { return _topRender; }

	virtual String GetItemType() = 0;

	virtual double GetOpacity();
	virtual dword  GetQuality();
	virtual Color  GetClearColor();
	virtual dword  GetRenderType();

	virtual Vector<Ptr<IMapItem> >& GetItems() { return _globalItemList; }
	const virtual Vector<Ptr<IMapItem> >& GetItems() const { return _globalItemList; }

protected:
	static  void  RegisterInGlobalList(Ptr<IMapItem> item);
	static  void  UnregisterFromGlobalList(Ptr<IMapItem> item);

private:
	IMapRender* _topRender;
	IMapRender* _parent;

	Vector<dword> _states;
	static Vector<Ptr<IMapItem> > _globalItemList;
};

#endif
