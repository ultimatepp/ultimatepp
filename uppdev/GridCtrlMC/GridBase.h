#ifndef _GridCtrl_GridBase_h_
#define _GridCtrl_GridBase_h_
#include "GridDisplay.h"

class GridCtrl;
class GridHeader;
class ItemRect;

inline static bool IsSet(int s, int b)
{
	return s & b;
}

inline static void BitSet(dword &k, dword v, bool s)
{
	if(s) k |= v; else  k &= ~v;
}

inline static bool BitInverse(dword &k, dword v)
{
	bool s = k & v;
	BitSet(k, v, !s);
	return s;
}

struct Edit : Moveable<Edit>
{
	Ptr<Ctrl> ctrl;
	Convert * convert;
	Callback1<One<Ctrl>&> factory;

	Edit()
	{
		ctrl = NULL;
		convert = NULL;
	}
};

/*--------------------------------------------------------------------------------*/
struct Item : Moveable<Item>
{
	friend class ItemRect;
	friend class GridCtrl;

	Item()
	{
		ctrl    = NULL;
		convert = NULL;
		display = NULL;

		fs = fe = 0;
		style = 0;
		editable = true;
		clickable = true;
		idx = 0;
		idy = 0;
		cx = 0;
		cy = 0;
		group = -1;
		isjoined = false;
		sync_flag = -1;
		paint_flag = -1;
	}
	~Item()
	{
		delete ctrl;
	}

	private:

		bool InvertSelect() { return BitInverse(style, GD::SELECT); }
		bool IsSelect() { return style & GD::SELECT; }
		bool IsLive()   { return style & GD::LIVE;   }
		bool IsCursor() { return style & GD::CURSOR; }
		bool IsFound()  { return style & GD::FOUND;  }

		void Select(bool s) { BitSet(style, GD::SELECT, s); }
		void Cursor(bool s) { BitSet(style, GD::CURSOR, s); }
		void Live(bool s)   { BitSet(style, GD::LIVE, s);   }
		void Found(bool s)  { BitSet(style, GD::FOUND, s);  }

		bool IsJoined() { return isjoined; }

		int idx, idy, cx, cy, group;
		bool isjoined:1;
		int fs, fe;
		dword style;
		bool editable:1;
		bool clickable:1;

		int sync_flag;
		int paint_flag;

		Ptr<Ctrl> ctrl;

		Convert     *convert;
		GridDisplay *display;

		void SetCtrl(Ctrl * newCtrl);
		void DropCtrl();

	public:
		Value val;

		Item& Editable(bool b)   { editable = b;     return *this; }
		Item& NoEditable()       { editable = false; return *this; }

		Item& Clickable(bool b)  { clickable = b;     return *this; }
		Item& NoClickable()      { clickable = false; return *this; }

};

typedef Vector< Vector<Item> > Items;
typedef Vector< Edit > Edits;
/*--------------------------------------------------------------------------------*/

class ItemRect : public Moveable<ItemRect>
{
	friend class GridCtrl;

	public:
		ItemRect()
		{
			min = 5; max = 1000000;
			pos = npos = 0;
			id  = uid = n = 0;
			prop = 1;
			size = nsize = 0;
			tsize = tnsize = 0;
			sortmode = 0;
			hidden = false;
			index = false;
			convertion = true;
			editable = true;
			sortable = true;
			clickable = true;
			style = 0;

			calign = 0;
			sl = sr = st = sb = 0;
			sx = sy = 0;
			join = 0;

			ismin = false;
			ismax = false;

			fnt = StdFont();
			align = GD::LEFT | GD::VCENTER;
			wrap = false;
			fg = Null;
			bg = Null;

			ctrl    = NULL;
			convert = NULL;
			display = NULL;

		}

	private:

		Items *items;
		Ctrl *parent;
		Edits *edits;

		Ptr<Ctrl>   ctrl;
		Convert     *convert;
		GridDisplay *display;

		Callback1<One<Ctrl>&> factory;

//		GridDisplay *display;

		static VectorMap<Id, int> *aliases;

		double pos, size, prop;
		int npos, nsize;
		int min, max;
		int id, uid, n;
		int sl, sr, st, sb, sx, sy;

		double tpos, tsize, tprop;
		int tnpos, tnsize;

		int  join;

		bool hidden;
		bool index;
		bool convertion;
		bool editable;
		bool sortable;
		bool clickable;

		Value defval;

		dword style;

		bool ismin;
		bool ismax;

		/* operatory typu friend moga odwolywac sie tylko do zmiennych statycznych klasy */

		static int sortCol;
		static int sortMode;

		int sortcol;
		int sortmode;

		int align, calign;
		bool wrap;
		Font fnt;
		Color fg, bg;

		double Left(int scroll = 0)    { return pos - scroll;          }
		double Right(int scroll = 0)   { return pos + size - scroll;   }
		double Top(int scroll = 0)     { return pos - scroll;          }
		double Bottom(int scroll = 0)  { return pos + size - scroll;   }
		double Width()                 { return size; 				   }
		double Height()                { return size;				   }

		bool   InvertSelect()          { return BitInverse(style, GD::SELECT); }
		bool   IsSelect()              { return style & GD::SELECT;    }
		bool   IsLive()                { return style & GD::LIVE;      }
		bool   IsCursor()              { return style & GD::CURSOR;    }
		bool   IsHighlight()           { return style & GD::HIGHLIGHT; }

		void   Select(bool s)          { BitSet(style, GD::SELECT, s);    }
		void   Cursor(bool s)          { BitSet(style, GD::CURSOR, s);    }
		void   Live(bool s)            { BitSet(style, GD::LIVE, s);      }
		void   Highlight(bool s)       { BitSet(style, GD::HIGHLIGHT, s); }

		int    nLeft(int scroll = 0)   { return npos - scroll;         }
		int    nRight(int scroll = 0)  { return npos + nsize - scroll; }
		int    nTop(int scroll = 0)    { return npos - scroll;         }
		int    nBottom(int scroll = 0) { return npos + nsize - scroll; }
		int    nWidth()                { return nsize; 				   }
		int    nHeight()               { return nsize;				   }

		ItemRect& Size(int n);

		bool IsMin() { return ismin; }
		bool IsMax() { return ismax; }

		void ChangeSortMode(bool idsort = true);

		friend bool operator<(const ItemRect& a, const ItemRect& b)
		{
			if(sortMode)
				return (StdValueCompare((*a.items)[a.id][sortCol].val, (*b.items)[b.id][sortCol].val, 0) < 0);
			else
				return a.id < b.id;
		}
		friend bool operator>(const ItemRect& a, const ItemRect& b)
		{
			if(sortMode)
				return (StdValueCompare((*a.items)[a.id][sortCol].val, (*b.items)[b.id][sortCol].val, 0) > 0);
			else
	 			return a.id > b.id;
		}
	public:

		ItemRect& Alias(Id id);
		ItemRect& Alias(const char * s);
		ItemRect& Name(String &s);
		ItemRect& Name(const char * s);
		ItemRect& Width(int n);
		ItemRect& Height(int n);
		ItemRect& Min(int n);
		ItemRect& Max(int n);
		ItemRect& Fixed(int n);
		ItemRect& Edit(Ctrl &newCtrl);
		ItemRect& SetConvert(Convert &c);
		ItemRect& SetFormat(const char *fmt);
		ItemRect& SetDisplay(GridDisplay &gd);
		ItemRect& NoConvertion();
		ItemRect& Default(Value v);

		ItemRect& AlignTop()                             { align = GD::TOP;                      return *this; }
		ItemRect& AlignBottom()                          { align = GD::BOTTOM;                   return *this; }
		ItemRect& AlignLeft()                            { align = GD::LEFT;                     return *this; }
		ItemRect& AlignRight()                           { align = GD::RIGHT;                    return *this; }
		ItemRect& AlignHorzCenter()                      { align = GD::HCENTER;                  return *this; }
		ItemRect& AlignVertCenter()                      { align = GD::VCENTER;                  return *this; }
		ItemRect& AlignCenter()                          { align = GD::HCENTER | GD::VCENTER;    return *this; }
		ItemRect& AlignTopLeft()                         { align = GD::LEFT    | GD::TOP;        return *this; }
		ItemRect& AlignTopRight()                        { align = GD::RIGHT   | GD::TOP;        return *this; }
		ItemRect& AlignTopCenter()                       { align = GD::HCENTER | GD::TOP;        return *this; }
		ItemRect& AlignBottomLeft()                      { align = GD::LEFT    | GD::BOTTOM;     return *this; }
		ItemRect& AlignBottomRight()                     { align = GD::RIGHT   | GD::BOTTOM;     return *this; }
		ItemRect& AlignBottomCenter()                    { align = GD::HCENTER | GD::BOTTOM;     return *this; }


		ItemRect& CtrlAlignTop(int t = 0, int s = 0)     { calign |= GD::TOP;     st = t; sy = s; return *this; }
		ItemRect& CtrlAlignBottom(int b = 0, int s = 0)  { calign |= GD::BOTTOM;  sb = b; sy = s; return *this; }
		ItemRect& CtrlAlignLeft(int l = 0, int s = 0)    { calign |= GD::LEFT;    sl = l; sx = s; return *this; }
		ItemRect& CtrlAlignRight(int r = 0, int s = 0)   { calign |= GD::RIGHT;   sr = r; sx = s; return *this; }
		ItemRect& CtrlAlignHorzCenter(int s = 0)         { calign |= GD::HCENTER; sx = s;         return *this; }
		ItemRect& CtrlAlignVertCenter(int s = 0)         { calign |= GD::VCENTER; sy = s;         return *this; }
		ItemRect& CtrlAlignHorzPos(int l = 0, int r = 0) { calign |= GD::HPOS;    sl = l; sr = r; sx = 1; return *this; }
		ItemRect& CtrlAlignVertPos(int t = 0, int b = 0) { calign |= GD::VPOS;    st = t; sb = b; sy = 1; return *this; }

		ItemRect& WrapText()                             { wrap = true; return *this; }

		ItemRect& SetFont(Font &f)                       { fnt = f; return *this; }
		ItemRect& Bg(Color c)                            { bg = c;  return *this; }
		ItemRect& Fg(Color c)                            { fg = c;  return *this; }

		ItemRect& Ctrls(Callback1<One<Ctrl>&> _factory)	 { (*edits)[id].factory = _factory; return *this; }
		ItemRect& Ctrls(void (*factory)(One<Ctrl>&))     { return Ctrls(callback(factory)); }
		template<class T> ItemRect&  Ctrls()             { return Ctrls(DefaultCtrlFactory<T>()); }
		ItemRect& Option();

		ItemRect& Editable(bool b)                       { editable = b;     return *this; }
		ItemRect& NoEditable()                           { editable = false; return *this; }

		ItemRect& Sortable(bool b)                       { sortable = b;     return *this; }
		ItemRect& NoSortable()                           { sortable = false; return *this; }

		ItemRect& Clickable(bool b)                      { clickable = b;     return *this; }
		ItemRect& NoClickable()                          { clickable = false; return *this; }

		bool IsConvertion()                              { return convertion; }

		void Serialize(Stream &s);
		void Clear();
};

typedef Vector<ItemRect> RectItems;
typedef Vector<ItemRect> VItems;
typedef Vector<ItemRect> HItems;
/*--------------------------------------------------------------------------------*/

#endif
