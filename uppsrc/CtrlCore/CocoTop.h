//$ class TopWindow {
protected:
	friend struct MMImp;
	
	bool placefocus;

	dword GetMMStyle() const;

	virtual void MMClose() { WhenClose(); }

	static TopWindow *GetMenuTopWindow();
	static void SyncMainMenu(bool force);
	
	void *menubar = NULL;

public:
	void SetMainMenu(Event<Bar&> menu);

//$ };