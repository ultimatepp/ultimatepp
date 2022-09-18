protected:
	friend struct MMImp;
	
	bool placefocus;

	dword GetMMStyle() const;

	virtual void MMClose() { WhenClose(); }
	
	static void SyncMainMenu(bool force);
	
	void *menubar = NULL;
	Event<Bar&> MainMenu;

public: // really private:
	static TopWindow *GetMenuTopWindow(bool dock);

public:
	virtual bool HotKey(dword key);
	
	Event<Bar&> WhenDockMenu;

	void SetMainMenu(Event<Bar&> menu);
