//$ class TopWindow {
protected:
	friend struct MMImp;
	
	bool placefocus;

	dword GetMMStyle() const;

	virtual void MMClose() { WhenClose(); }
//$ };