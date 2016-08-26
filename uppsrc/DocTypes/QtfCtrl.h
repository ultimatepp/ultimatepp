// OBSOLETE!!! Use RichTextView instead!

namespace Upp {

class QtfCtrl : public Ctrl {
public:
	virtual void  Paint(Draw& w);
	virtual void  Layout();
	virtual bool  Key(dword key, int);
	virtual Image CursorImage(Point p, dword keyflags);
	virtual void  LeftUp(Point p, dword keyflags);
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);

protected:
	Document          doc;
	mutable Document::Cache cache;
	Color             color;
	int               tm, lm, rm, bm;
	int               zoom;
	ScrollBar         sb;
	bool              vcenter;
	Vector<ValueRect> valuerect;

	int      GetHeight() const                    { return GetHeight(GetSize().cx); }
	void     Scroll();
	Value    FindValue(Point p) const;

public:
	Callback1<const Value&> WhenLink;

	QtfCtrl&  SetLabel(const char *text);

	void     Set(const char *qtf);
	void     Set(const Document& _doc);
	void     Pick(pick_ Document& _doc);

	int      GetHeight(int cx) const;
	int      GetWidth() const                      { return doc.GetWidth(zoom); }

	const Vector<ValueRect>& GetValueRects() const { return valuerect; }
	int      GetSb() const                         { return sb; }
	void     SetSb(int i)                          { sb = i; }

	const Document& GetDocument() const            { return doc; }

	QtfCtrl& Background(Color _color)  { Transparent(IsNull(color = _color)); Refresh(); return *this; }
//	QtfCtrl& Zoom(int _zoom)           { zoom = _zoom; RefreshLayout(); Refresh(); return *this; }
	QtfCtrl& VCenter(bool b = true)    { vcenter = b; return *this; }
	QtfCtrl& NoVCenter()               { return VCenter(false); }
	QtfCtrl& Lm(int _lm)               { lm = _lm; Refresh(); return *this; }
	QtfCtrl& Rm(int _rm)               { rm = _rm; Refresh(); return *this; }
	QtfCtrl& Tm(int _tm)               { tm = _tm; Refresh(); return *this; }
	QtfCtrl& Bm(int _bm)               { bm = _bm; Refresh(); return *this; }
	QtfCtrl& HMargins(int a)           { lm = rm = a; Refresh(); return *this; }
	QtfCtrl& VMargins(int a)           { tm = bm = a; Refresh(); return *this; }
	QtfCtrl& Margins(int a)            { lm = rm = tm = bm = a; Refresh(); return *this; }
//	QtfCtrl& WithSb()                  { AddFrame(sb); return *this; }

	int      GetLm() const             { return lm; }
	int      GetTm() const             { return tm; }
	int      GetRm() const             { return rm; }
	int      GetBm() const             { return bm; }

	void     operator=(const char *s)          { Set(s); }
	void     operator=(Document& _doc)         { Pick(_doc); }
	void     operator<<=(const Document& _doc) { Set(_doc); }

	void     Clear()                           { Set(""); }

	QtfCtrl();
	virtual ~QtfCtrl();
};

}
