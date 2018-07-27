//$ class TopWindow {
protected:
	dword GetMMStyle() const;

	virtual void MMClose() { WhenClose(); }
//$ };