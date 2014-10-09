void DropEdge_Write(Value);

class PopUpTable : public ArrayCtrl {
public:
	virtual void LeftUp(Point p, dword keyflags);
	virtual bool Key(dword key, int);

protected:
	void PopupDeactivate();
	void PopupCancelMode();

	struct Popup : Ctrl {
		PopUpTable *table;
		
		virtual void Deactivate() { table->PopupDeactivate(); }
		virtual void CancelMode() { table->PopupCancelMode(); }
	};

	int          droplines;
	int          inpopup;
	bool         open;
	One<Popup>   popup;

	void         DoClose();

public:
	void         PopUp(Ctrl *owner, int x, int top, int bottom, int width);
	void         PopUp(Ctrl *owner, int width);
	void         PopUp(Ctrl *owner);

	Callback     WhenCancel;
	Callback     WhenSelect;

	PopUpTable&  SetDropLines(int _droplines)          { droplines = _droplines; return *this; }

	void         Normal();

	PopUpTable();
	virtual ~PopUpTable();
};

class DropList : public MultiButton, public Convert {
public:
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
	virtual bool  Key(dword key, int);
	virtual void  SetData(const Value& data);
	virtual Value GetData() const;

	virtual Value Format(const Value& q) const;

private:
	PopUpTable         list;
	Index<Value>       key;
	Value              value;
	int                dropwidth;
	const Convert     *valueconvert;
	const Display     *valuedisplay;
	bool               displayall;
	bool               dropfocus;
	bool               notnull;
	bool               alwaysdrop;
	bool               usewheel;

	void          Select();
	void          Cancel();
	void          Change(int q);
	void          EnableDrop(bool b = true)         { MainButton().Enable(b || alwaysdrop); }
	void          Sync();

	typedef       DropList CLASSNAME;

public:
	typedef MultiButton::Style Style;

	Callback      WhenDrop;

	DropList&     Add(const Value& key, const Value& value);
	DropList&     Add(const Value& value)         { return Add(value, value); }
	void          Remove(int i);
	void          ClearList();
	void          Clear();
	
	DropList&     AddSeparator();

	void          Drop();

	const Value& operator=(const Value& v)        { SetData(v); return v; }
	operator Value() const                        { return GetData(); }

	void          SetIndex(int i)                 { SetData(GetKey(i)); }
	int           GetIndex() const                { return FindKey(value); }
	void          GoBegin()                       { if(GetCount()) SetIndex(0); }
	void          GoEnd()                         { if(GetCount()) SetIndex(GetCount() - 1); }

	bool          HasKey(const Value& k) const    { return FindKey(k) >= 0; }
	int           FindKey(const Value& k) const;
	int           Find(const Value& k) const      { return FindKey(k); }
	int           FindValue(const Value& v) const { return list.Find(v); }

	int           GetCount() const                { return key.GetCount(); }
	void          Trim(int n);
	const Value&  GetKey(int i) const             { return key[i]; }

	Value         GetValue(int i) const           { return list.Get(i, 0); }
	Value         GetValue() const;
	void          SetValue(int i, const Value& v);
	void          SetValue(const Value& v);
	Value         operator[](int i) const         { return GetValue(i); }

	void          Adjust();
	void          Adjust(const Value& k);

	const PopUpTable& GetList() const                   { return list; }
	PopUpTable&   ListObject()                          { return list; }

	DropList&     SetDropLines(int d)                   { list.SetDropLines(d); return *this; }
	DropList&     SetValueConvert(const Convert& cv);
	DropList&     SetConvert(const Convert& cv);
	DropList&     SetDisplay(int i, const Display& d);
	DropList&     SetDisplay(const Display& d);
	DropList&     SetLineCy(int lcy)                    { list.SetLineCy(lcy); return *this; }
	DropList&     SetDisplay(const Display& d, int lcy);
	DropList&     ValueDisplay(const Display& d);
	DropList&     DisplayAll(bool b = true)             { displayall = b; return *this; }
	DropList&     DropFocus(bool b = true)              { dropfocus = b; return *this; }
	DropList&     NoDropFocus()                         { return DropFocus(false); }
	DropList&     AlwaysDrop(bool e = true);
	DropList&     SetStyle(const Style& s)              { MultiButton::SetStyle(s); return *this; }
	DropList&     NotNull(bool b = true)                { notnull = b; return *this; }
	DropList&     DropWidth(int w)                      { dropwidth = w; return *this; }
	DropList&     DropWidthZ(int w)                     { dropwidth = HorzLayoutZoom(w); return *this; }
	DropList&     Wheel(bool b = true)                  { usewheel = b; return *this; }
	DropList&     NoWheel()                             { return Wheel(false); }

	DropList&     SetScrollBarStyle(const ScrollBar::Style& s) { list.SetScrollBarStyle(s); return *this; }

	DropList();
	virtual ~DropList();
};

void Append(DropList& list, const VectorMap<Value, Value>& values);
void Append(DropList& list, const VectorMap<int, String>& values);
void Append(MapConvert& convert, const VectorMap<Value, Value>& values);
void Append(MapConvert& convert, const VectorMap<int, String>& values);
void Append(DropList& list, const MapConvert& convert);

void operator*=(DropList& list, const VectorMap<Value, Value>& values);
void operator*=(DropList& list, const VectorMap<int, String>& values);
void operator*=(MapConvert& convert, const VectorMap<Value, Value>& values);
void operator*=(MapConvert& convert, const VectorMap<int, String>& values);
void operator*=(DropList& list, const MapConvert& convert);

class DropChoice : public MultiButtonFrame {
public:
	virtual void       Serialize(Stream& s); //empty

protected:
	PopUpTable         list;
	Ctrl              *owner;
	bool               appending;
	bool               dropfocus;
	bool               always_drop;
	bool               updownkeys;
	bool               rodrop;

	void        Select();
	void        Drop();
	void        EnableDrop(bool b);
	void        PseudoPush();
	int         dropwidth;

	typedef DropChoice CLASSNAME;

public:
	Callback    WhenDrop;
	Callback    WhenSelect;

	bool        DoKey(dword key);
	void        DoWheel(int zdelta);

	void        Clear();
	void        Add(const Value& data);
	void        SerializeList(Stream& s);
	
	int         GetCount() const                      { return list.GetCount(); }
	Value       Get(int i) const                      { return list.Get(i, 0); }

	void        AddHistory(const Value& data, int max = 12);

	void        AddTo(Ctrl& _owner)                   { MultiButtonFrame::AddTo(_owner); owner = &_owner; }
	bool        IsActive() const                      { return IsOpen(); }

	Value       Get() const;
	int         GetIndex() const;

	DropChoice& SetDisplay(int i, const Display& d)   { list.SetDisplay(i, 0, d); return *this; }
	DropChoice& SetDisplay(const Display& d)          { list.ColumnAt(0).SetDisplay(d); return *this; }
	DropChoice& SetLineCy(int lcy)                    { list.SetLineCy(lcy); return *this; }
	DropChoice& SetDisplay(const Display& d, int lcy) { SetDisplay(d); SetLineCy(lcy); return *this; }
	DropChoice& SetConvert(const Convert& d)          { list.ColumnAt(0).SetConvert(d); return *this; }
	DropChoice& SetDropLines(int n)                   { list.SetDropLines(n); return *this; }
	DropChoice& Appending()                           { appending = true; return *this; }
	DropChoice& AlwaysDrop(bool e = true);
	DropChoice& RdOnlyDrop(bool e = true)             { rodrop = e; return *this; }
	DropChoice& NoDropFocus()                         { dropfocus = false; return *this; }

	DropChoice& DropWidth(int w)                      { dropwidth = w; return *this; }
	DropChoice& DropWidthZ(int w)                     { dropwidth = HorzLayoutZoom(w); return *this; }
	DropChoice& UpDownKeys(bool b = true)             { updownkeys = b; return *this; }

	DropChoice& SetScrollBarStyle(const ScrollBar::Style& s) { list.SetScrollBarStyle(s); return *this; }

	DropChoice();

	static bool DataSelect(Ctrl& owner, DropChoice& drop, const String& appends);
};

template <class T>
class WithDropChoice : public T {
public:
	virtual bool   Key(dword key, int repcnt);
	virtual void   MouseEnter(Point p, dword keyflags);
	virtual void   MouseLeave();
	virtual void   MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void   GotFocus();
	virtual void   LostFocus();


protected:
	DropChoice      select;
	String          appends;
	bool            withwheel;

	void            DoWhenSelect();
	void            DoWhenDrop()                          { WhenDrop(); }

public:
	Callback        WhenDrop;
	Callback        WhenSelect;

	void            ClearList()                           { select.Clear(); }
	void            AddList(const Value& data)            { select.Add(data); }
	void            SerializeList(Stream& s)              { select.SerializeList(s); }

	int             GetCount() const                      { return select.GetCount(); }
	Value           Get(int i) const                      { return select.Get(i); }

	void            AddHistory(int max = 12)              { select.AddHistory(this->GetData(), max); }

	MultiButton::SubButton& AddButton()                   { return select.AddButton(); }
	int                     GetButtonCount() const        { return select.GetButtonCount(); }
	MultiButton::SubButton& GetButton(int i)              { return select.GetButton(i); }
	Rect                    GetPushScreenRect() const     { return select.GetPushScreenRect(); }

	const MultiButton::Style& StyleDefault()              { return select.StyleFrame(); }
	WithDropChoice& SetStyle(const MultiButton::Style& s) { select.SetStyle(s); return *this; }

	WithDropChoice& Dropping(bool b = true)               { select.MainButton().Show(b); return *this; }
	WithDropChoice& NoDropping()                          { return Dropping(false); }
	WithDropChoice& NoDropFocus()                         { select.NoDropFocus(); return *this; }
	WithDropChoice& Appending(const String& s = ", ")     { appends = s; select.Appending(); return *this; }
	WithDropChoice& SetDropLines(int n)                   { select.SetDropLines(n); return *this; }
	WithDropChoice& SetDisplay(int i, const Display& d)   { select.SetDisplay(i, d); return *this; }
	WithDropChoice& SetDisplay(const Display& d)          { select.SetDisplay(d); return *this; }
	WithDropChoice& SetLineCy(int lcy)                    { select.SetLineCy(lcy); return *this; }
	WithDropChoice& SetDisplay(const Display& d, int lcy) { select.SetDisplay(d, lcy); return *this; }
	WithDropChoice& SetConvert(const Convert& d)          { select.SetConvert(d); return *this; }
	WithDropChoice& AlwaysDrop(bool b = true)             { select.AlwaysDrop(b); return *this; }
	WithDropChoice& RdOnlyDrop(bool b = true)             { select.RdOnlyDrop(b); return *this; }
	WithDropChoice& WithWheel(bool b = true)              { withwheel = b; return *this; }
	WithDropChoice& NoWithWheel()                         { return WithWheel(false); }
	WithDropChoice& DropWidth(int w)                      { select.DropWidth(w); return *this; }
	WithDropChoice& DropWidthZ(int w)                     { select.DropWidthZ(w); return *this; }
	WithDropChoice& UpDownKeys(bool b = true)             { select.UpDownKeys(b); return *this; }
	WithDropChoice& NoUpDownKeys()                        { return UpDownKeys(false); }

	WithDropChoice();
};

template <class T>
WithDropChoice<T>::WithDropChoice() {
	select.AddTo(*this);
	select.WhenDrop = callback(this, &WithDropChoice::DoWhenDrop);
	select.WhenSelect = callback(this, &WithDropChoice::DoWhenSelect);
	appends = String::GetVoid();
	withwheel = true;
	SetStyle(StyleDefault());
}

template <class T>
bool WithDropChoice<T>::Key(dword key, int repcnt) {
	return select.DoKey(key) || T::Key(key, repcnt);
}

template <class T>
void WithDropChoice<T>::MouseWheel(Point p, int zdelta, dword keyflags)
{
	if(withwheel)
		select.DoWheel(zdelta);
}

template <class T>
void WithDropChoice<T>::MouseEnter(Point p, dword keyflags)
{
	select.Refresh();
	T::MouseEnter(p, keyflags);
}

template <class T>
void WithDropChoice<T>::MouseLeave()
{
	select.Refresh();
	T::MouseLeave();
}

template <class T>
void WithDropChoice<T>::GotFocus()
{
	select.Refresh();
	T::GotFocus();
}

template <class T>
void WithDropChoice<T>::LostFocus()
{
	select.Refresh();
	T::LostFocus();
}

template <class T>
void WithDropChoice<T>::DoWhenSelect() {
	if(DropChoice::DataSelect(*this, select, appends)) {
		this->SetFocus();
		WhenSelect();
	}
}
