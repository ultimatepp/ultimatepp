//$ class Ctrl {
private:
	void SetImageCursor(const Image& img);
	
	friend struct MMCtrl;
	friend struct MMImp;

public:
// TODO: Temp only, remove!
	void Create(const Upp::Rect& r, const char *title, bool popup);

	static void      EndSession()              {}
	static bool      IsEndSession()            { return false; }
//$ };
