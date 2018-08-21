//$ class TopWindow {
protected:
	friend struct MMImp;

	dword GetMMStyle() const;

	virtual void MMClose() { WhenClose(); }
//$ };