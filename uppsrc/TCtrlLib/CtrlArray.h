#ifndef __TCtrlLib_CtrlArray__
#define __TCtrlLib_CtrlArray__

NAMESPACE_UPP

class CtrlArray : public Ctrl
{
public:
	class Item : public Ctrl
	{
		friend class CtrlArray;

	public:
		Item();
		virtual ~Item();

		bool           IsCursor() const { return array->IsCursor(this); }
		void           SetCursor()      { array->SetCursor(this); }
		int            GetIndex() const { return array->GetIndex(this); }

		virtual void   Paint(Draw& draw);

		virtual void   LeftDown(Point pt, dword keyflags);
		virtual void   RightDown(Point pt, dword keyflags);

	protected:
		CtrlArray     *array;
	};

	typedef CtrlArray CLASSNAME;
	CtrlArray();
	virtual ~CtrlArray();

//	virtual Value     GetData() const;
//	virtual void      SetData(const Value& v);

	void              Clear();
	bool              IsEmpty() const           { return items.IsEmpty(); }
	int               GetCount() const          { return items.GetCount(); }
	Item&             GetCtrl(int i)            { return items[i]; }
	const Item&       GetCtrl(int i) const      { return items[i]; }
	Value             Get(int i) const          { return ~items[i]; }
	Value             operator [] (int i) const { return ~items[i]; }
	void              Set(int i, Value v)       { items[i] <<= v; }
	Item&             Add();
	void              SetCount(int c);

	CtrlArray&        Inserting(bool i = true)  { inserting = i; return *this; }
	bool              IsInserting() const       { return inserting; }

	CtrlArray&        Removing(bool r = true)   { removing = r; return *this; }
	bool              IsRemoving() const        { return removing; }

	CtrlArray&        Duplicating(bool d = true) { duplicating = d; return *this; }
	bool              IsDuplicating() const     { return duplicating; }

	CtrlArray&        Moving(bool d = true)     { moving = d; return *this; }
	bool              IsMoving() const          { return moving; }

	int               GetIndex(const Item *item) const;
	bool              IsCursor(const Item *item) const;
	void              SetCursor(const Item *item);

	bool              IsCursor() const          { return cursor >= 0; }
	int               GetCursor() const         { return cursor; }
	void              SetCursor(int at);
	void              KillCursor()              { SetCursor(-1); }

	void              ScrollInto(int item);

	virtual bool      Accept();

	virtual void      Layout();
	virtual void      Paint(Draw& draw);
	virtual void      RightDown(Point pt, dword keyflags);
	virtual void      ChildGotFocus();
	virtual bool      Key(dword key, int repcnt);
	virtual void      MouseWheel(Point pt, int zdelta, dword keyflags);

	virtual void      GotFocus();
	virtual void      LostFocus();

	void              ToolLocal(Bar& bar);
	void              ToolInsertAbove(Bar& bar);
	void              ToolInsertBelow(Bar& bar);
	void              ToolDuplicate(Bar& bar);
	void              ToolRemove(Bar& bar);
	void              ToolUp(Bar& bar);
	void              ToolDown(Bar& bar);

	void              OnInsert(int index);
	void              OnDuplicate(int index);
	void              OnRemove(int index);
	void              OnUp(int index);
	void              OnDown(int index);

public:
	Callback1< One<Item>& > WhenCreate;
	Callback1<Bar&>   WhenBar;

private:
	int               GetYPos(int i) const;

	void              OnScroll();
	void              RefreshCursor();

private:
	Array<Item>       items;
	StaticRect        scrollview;
	VScrollBar        vscroll;
	int               cursor;
	int               height;
	bool              is_closing;
	bool              inserting;
	bool              removing;
	bool              duplicating;
	bool              moving;
};

END_UPP_NAMESPACE

#endif//__TCtrlLib_CtrlArray__
