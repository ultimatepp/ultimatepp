//$ class TopWindow {
protected:
	Ptr<Ctrl> owner;

	dword GetMMStyle() const;

	virtual void MMClose() { DLOG("Top::MMClose"); WhenClose(); }
//$ };