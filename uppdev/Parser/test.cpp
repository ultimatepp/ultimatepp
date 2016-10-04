#include <RichEdit/RichEdit.h>

struct alpha {
	int beta;
};

struct oper2 {
	int oper2m;
};

struct oper3 {
	int oper3m;
};

struct X {
	alpha a;
	
	alpha operator[](int i);
};

struct Y : X{
	X b;
	int a;
	
	struct H {
		int x;
	};
	
	H x;
};

template <class T>
struct Tm {
	template <class V>
	struct H {
		T Test();
		V Test2();
	};
	
	T x;
};


void Test(int a, long b)
{
	X().;
	Y x;
	x.;
	x.a. ; // should not work...
	x.X::a. ;
	x.b.a. ;
	x[]. ;
	x(). ;
	x. ;
	x-> ;
	y. ;
	Tm<X> tm;
	Tm<  X > :: H <alpha > tt;
	tt. ;
	tt.Test(). ;
	tt.Test2(). ;
	X:: ;
	;
	
}

// ------------------------------------------------------------------------------------------

void Foo()
{
	
}

CppItemInfoDisplay ItemList::display;

class ItemList : public ColumnList {
	CppItemInfoDisplay display;

	friend struct ItemDisplay;

	int    GetTopic(Point p, String& key);
	String Item(int i);

public:
	bool active_topics;

	void Clear();

	ItemList();
};

void ItemList::Clear()
{
}

void ItemList::Clear(int)
{
}

	void Clear();

typedef short unsigned     word;

typedef unsigned char      byte;
typedef signed char        int8;
typedef unsigned char      uint8;

typedef short int          int16;
typedef short unsigned     uint16;

struct FindBrokenRefIterator : RichTxt::Iterator {
	int cursor;
	Uuid itemstyle;
	
	void Test();
	void Dnes();

	virtual bool operator()(int pos, const RichPara& para)
	{
		
		this-> ;
		THISBACK(;
		if(pos >= cursor) {
			if(para.format.label != "noref") {
				if(IsNull(para.format.label))
					return true;
				String nest;
				String key;
				if(!SplitNestKey(para.format.label, nest, key))
					return true;
				int q = BrowserBase().Find(nest);
				if(q < 0 || BrowserBase()[q].key.Find(key) < 0)
					return true;
			}
			cursor = pos;
		}
		return false;
	}

	FindBrokenRefIterator() {
	}
};

void FindBrokenRefIterator::Test()
{
}

void Foo()
{
	String x;
	x.Right(). ;
	String(xxx). ;
	String x;
	x.;
	x->; // <<
	AttrText("Hello!"). ;
	Vector<String> vx;
	vx. ;
	vx[0]. ;
	vx.Add(). ;
	vx[654].Right(). ;
	vx[654]->Right(). ;
	Vector<String>(). ;
	x > x.;
	EditorBar bar;
	bar. ;
	EditString es;
	es. ;
	es.NullText(). ;
	
	String x[5];
	x[1]. ; // << here!
	String y;
	y[0]. ; // << here!
	Rect r;
	r. ;
	Vector<String>:: ;
	Array<String> ax;
	ax.At(1). ;
	Array<String> tst;
	tst[1]. ;
	Ctrl::Dsize;
	Ctrl::LogPos::x;
}

static WString ss[12];

struct XXX {
	String s[10];
	String t;
};


void isptr() {
	String p[2];
	Vector<String> pp;
	XXX xx;
	xx.t. ;
	xx.s[2]. ;
	ss[1]. ;
	p[1]. ;
	pp[1]. ;
}

class Test {
	void Priv();

public:
	void Public() { Priv(); /* test if it can see it */ };
};

int  TabCtrl::GetTab(Point p) const
{
	if(p.y >= 0 && p.y < style->tabheight)
		for(int i = 0; i < tab.GetCount(); i++)
			if(p.x < tab[i].Right()) // << Here!
				return i;
	tab[i]. ; // <<
	return -1;
	TabCtrl::
}

struct FwTest;

struct Dlg : WithEditStringLayout<TopWindow> {
	Dlg() {
		Ctrl:: ;
		Dlg x;
		x. ;
		this. ;
		this-> ;
		text. ;
	}
	Pusher::GetRect(). ;
	
};

struct FwTest {
	void Test();
};