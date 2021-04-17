class TabBarCtrl : public TabBar 
{
private:
	VectorMap<Value, Ctrl *> ctrls;
	StaticRect pane;
	
public:
	TabBarCtrl()							{ Ctrl::Add(pane); }
	virtual void Layout() 					{ TabBar::Layout(); pane.SetRect(GetClientArea()); }
	
	TabBarCtrl& AddCtrl(Ctrl &ctrl, Value key, Value value, Image icon = Null, String group = Null, bool make_active = false);
	TabBarCtrl& AddCtrl(Ctrl &ctrl, Value value, Image icon = Null, String group = Null, bool make_active = false);
	TabBarCtrl& InsertCtrl(int ix, Ctrl &ctrl, Value key, Value value, Image icon = Null, String group = Null, bool make_active = false);
	TabBarCtrl& InsertCtrl(int ix, Ctrl &ctrl, Value value, Image icon = Null, String group = Null, bool make_active = false);
	
	void    RemoveCtrl(Value key);
	void    RemoveCtrl(int ix);
	void    RemoveCtrl(Ctrl &c);
	
	Ctrl *	GetCtrl(Value key);
	Ctrl *	GetCtrl(int ix);
	Ctrl *	GetCurrentCtrl();
	int		GetCurrentIndex();
	
	void    SetCtrl(Value key);
	void    SetCtrl(int ix);
	void    SetCtrl(Ctrl &ctrl);

	Vector<Ctrl*> GetCtrlGroup(const String& group);

protected:
	virtual void CursorChanged();
	virtual void TabClosed(Value key);
};
