//$ class Upp::TopWindow {
protected:
	friend struct MMImp;
	
	bool placefocus;

	dword GetMMStyle() const;

	virtual void MMClose() { WhenClose(); }
	
	static TopWindow *GetMenuTopWindow();
	static void SyncMainMenu(bool force);
	
	void *menubar = NULL;
	Event<Bar&> MainMenu;

public:
	virtual bool HotKey(dword key);

	void SetMainMenu(Event<Bar&> menu);
//$ };