//$ class Ctrl {
private:
	void SetImageCursor(const Image& img);

public:
// defeat private: to connect ObjectiveC with U++
	void CocoDispatchMouse__(int event, Point p, int zdelta) { DispatchMouse(event, p, zdelta); }
	void NewRect__(const Rect& r);
	

// TODO: Temp only, remove!
	void Create(const Upp::Rect& r, const char *title);

	static void      EndSession()              {}
	static bool      IsEndSession()            { return false; }
//$ };
