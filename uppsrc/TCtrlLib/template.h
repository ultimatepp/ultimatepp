NAMESPACE_UPP

/* obsoleted by Fidler's StoreAsString
template <class T>
String SerializeOut(const T& object)
{
	StringStream stream;
	stream % const_cast<T&>(object);
	return stream;
}
*/

/* obsoleted by Fidler's ReadFromString
template <class T>
bool SerializeIn(T& object, const String& data)
{
	if(data.IsEmpty())
		return true; // no data - do not serialize but report no errors
	StringStream load(data);
	load % object;
	return !load.IsError();
}
*/

/* obsoleted by Fidler's ReadFromString (more or less)
template <class T>
T SerializeIn(const String& data, T * = 0)
{
	T temp;
	if(SerializeIn(temp, data, false))
		return temp;
	return T();
}
*/

/* obsoleted by Fidler's LoadFromFile
template <class T>
bool SerializeLoad(T& object, const char *filename)
{
	FileIn file(filename);
	if(!file.IsOpen())
		return true; // no file - no error
	file % object;
	return !file.IsError();
}
*/

/* obsoleted by Fidler's LoadFromFile (more or less)
template <class T>
T SerializeLoad(const char *filename, T * = 0)
{
	T temp;
	if(SerializeLoad(temp, filename, false))
		return temp;
	return T();
}
*/

/* obsoleted by Fidler's StoreToFile
template <class T>
bool SerializeSave(T& object, const char *filename)
{
	if(!filename || !*filename)
		return false;
	FileOut file(filename);
	if(!file.IsOpen())
		return false;
	file % object;
	file.Close();
	if(!file.IsError())
		return true;
	DeleteFile(filename);
	return false;
}
*/

template <class T>
class WithChoiceList : public T
{
public:
	WithChoiceList();

	virtual bool Key(dword key, int repcnt);

public:
	ChoiceList choices;
};

template <class T>
WithChoiceList<T>::WithChoiceList()
{
	choices.Attach(*this);
}

template <class T>
bool WithChoiceList<T>::Key(dword key, int repcnt)
{
	return choices.OnKey(key) || T::Key(key, repcnt);
}

template <class T>
class WithKeyMap : public T
{
public:
	WithKeyMap() {}

	void         AddKey(dword key, Callback callback) { map.Add(key, callback); }
	void         ClearKey(dword key);
	void         ClearKey()                           { map.Clear(); }

	virtual bool Key(dword key, int count);

public:
	VectorMap<dword, Callback> map;
};

template <class T>
void WithKeyMap<T>::ClearKey(dword key)
{
	Vector<int> found;
	for(int i = map.Find(key); i >= 0; i = map.FindNext(i))
		found.Add(i);
	while(!found.IsEmpty())
		map.Remove(found.Pop());
}

template <class T>
bool WithKeyMap<T>::Key(dword key, int count)
{
	int i = map.Find(key);
	if(i < 0)
		return T::Key(key, count);
	for(; i >= 0; i = map.FindNext(i))
		map[i]();
	return true;
}

template <class T>
class WithKeyCallback : public T
{
public:
	WithKeyCallback() : WhenKey(false) {}
	virtual bool Key(dword key, int count);

	Gate1<dword> WhenKey;
};

template <class T>
bool WithKeyCallback<T>::Key(dword key, int count)
{
	return WhenKey(key) || T::Key(key, count);
}

template <class T>
class CtrlDataHelper
{
public:
	CtrlDataHelper(Ctrl& ctrl, T dflt) : ctrl(ctrl), value(dflt) {}

	void Serialize(Stream& stream)
	{
		if(stream.IsStoring())
			value = ~ctrl;
		stream % value;
		if(stream.IsLoading())
			ctrl <<= value;
	}

	friend Stream& operator % (Stream& stream, CtrlDataHelper<T> hlp) { hlp.Serialize(stream); return stream; }

private:
	Ctrl& ctrl;
	T     value;
};

template <class T>
inline CtrlDataHelper<T>
CtrlData(Ctrl& ctrl, T dflt = Null)
{ return CtrlDataHelper<T>(ctrl, dflt); }

template <class T>
class WithDisplay : public T
{
public:
	WithDisplay();
	WithDisplay(const PaintRect& prc) : prc(prc) {}
	WithDisplay(const Display& picture, const Value& value) : prc(picture, value) {}

	virtual void     Paint(Draw& draw);

	WithDisplay&     Background(const PaintRect& _prc)  { prc = _prc; this->Refresh(); return *this; }
	WithDisplay&     Background(const Value& _value)    { prc.SetValue(_value); this->Refresh(); return *this; }

	const Display&   GetDisplay() const                 { return prc.GetDisplay(); }
	const Value&     GetValue() const                   { return prc.GetValue(); }
	const PaintRect& GetPaintRect() const               { return prc; }

protected:
	PaintRect        prc;
};

template <class T>
WithDisplay<T>::WithDisplay()
: prc(StdDisplay())
{
}

template <class T>
void WithDisplay<T>::Paint(Draw& draw)
{
	prc.Paint(draw, Rect(this->GetSize()), SColorText(), SColorFace(), 0);
}

template <class T>
class NotNullCtrl : public T
{
public:
	virtual Value  GetData() const;
};

template <class T>
Value NotNullCtrl<T>::GetData() const
{
	Value value = T::GetData();
	if(value.IsError() || !value.IsNull())
		return value;
	return ErrorValue("Hodnota nesmí být prázdná.");
}

template <class T>
class WithDropFiles : public T
{
public:
	WithDropFiles() : registered(false) {}

#ifdef PLATFORM_WIN32
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
#endif
	virtual void    DropFiles(Point pt, const Vector<String>& file_list) = 0;

private:
	bool            registered;
};

#ifdef PLATFORM_WIN32
template <class T>
LRESULT WithDropFiles<T>::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(!registered && this->GetHWND())
	{
		registered = true;
		DropFilesRegister(this->GetHWND());
	}
	if(registered && message == WM_DROPFILES)
	{
		Point pt = DropFilesGetPos(wParam);
		DropFiles(pt, DropFilesGetList(wParam));
		return 0;
	}
	return T::WindowProc(message, wParam, lParam);
}
#endif

template <class T>
class WithBar : public T
{
public:
	virtual void RightDown(Point pt, dword keyflags);
	virtual bool Key(dword key, int repcnt);

public:
	Callback1<Bar&> WhenBar;
};

template <class T>
void WithBar<T>::RightDown(Point pt, dword keyflags)
{
	MenuBar::Execute(WhenBar);
}

template <class T>
bool WithBar<T>::Key(dword key, int repcnt)
{
	return MenuBar::Scan(WhenBar, key) || T::Key(key, repcnt);
}

END_UPP_NAMESPACE
