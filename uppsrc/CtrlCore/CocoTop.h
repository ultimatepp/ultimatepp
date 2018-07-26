//$ class TopWindow {
protected:
	dword GetMMStyle() const;

	virtual void MMClose() { DLOG("Top::MMClose"); WhenClose(); }
//$ };