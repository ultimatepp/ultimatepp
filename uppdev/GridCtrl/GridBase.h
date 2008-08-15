#ifndef _GridCtrl2_GridBase_h_
#define _GridCtrl2_GridBase_h_
#include "GridDisplay.h"

class GridCtrl;
class GridHeader;
class ItemRect;

inline static void BitSet(dword &k, dword v, bool s)
{
	if(s) k |= v; else  k &= ~v;
}

/*--------------------------------------------------------------------------------*/
struct Item : Moveable<Item>
{
	friend class ItemRect;
	friend class GridCtrl;
	friend class GridHeader;

	Item()
	{
		ctrl    = NULL;
		convert = NULL;
		display = NULL;

		fs = fe = 0;
		style = 0;
		toremove = false;
	}

	private:

		bool IsSelect() { return style & GD::SELECT; }
		bool IsLive()   { return style & GD::LIVE;   }
		bool IsCursor() { return style & GD::CURSOR; }
		bool IsFound()  { return style & GD::FOUND;  }

		void Select(bool s) { BitSet(style, GD::SELECT, s); }
		void Cursor(bool s) { BitSet(style, GD::CURSOR, s); }
		void Live(bool s)   { BitSet(style, GD::LIVE, s);   }
		void Found(bool s)  { BitSet(style, GD::FOUND, s);  }

		int fs, fe;
		dword style;
		bool toremove;

		Ptr<Ctrl> ctrl;

		Convert     *convert;
		GridDisplay *display;

		void SetCtrl(Ctrl * newCtrl);
		void DropCtrl();

	public:
		Value val;

};

typedef Vector< Vector<Item> > Items;

/*--------------------------------------------------------------------------------*/

class ItemRect : public Moveable<ItemRect>
{
	friend class GridCtrl;
	friend class GridHeader;

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
			fixedsize = false;
			hidden = false;
			index = false;
			marked = false;
			convertion = true;
			editable = true;
			style = 0;

			fnt = StdFont();
			align = GD::LEFT | GD::VCENTER;
			wrap = false;
			fg = Null;
			bg = Null;
		}

	private:

		Items *items;
		Ctrl *parent;

//		GridDisplay *display;

		static VectorMap<Id, int> *aliases;

		double pos, size, prop;
		int npos, nsize;
		int min, max;
		int id, uid, n;

		double tpos, tsize, tprop;
		int tnpos, tnsize;

		bool fixedsize;
		bool hidden;
		bool index;
		bool marked;
		bool convertion;
		bool editable;
		Value defval;

		dword style;

		bool ismin;

		/* operatory typu friend moga odwolywac sie tylko do zmiennych statycznych klasy */

		static int sortCol;
		static int sortMode;

		int sortcol;
		int sortmode;

		int align;
		bool wrap;
		Font fnt;
		Color fg, bg;

		double Left(int scroll = 0)    { return pos - scroll;          }
		double Right(int scroll = 0)   { return pos + size - scroll;   }
		double Top(int scroll = 0)     { return pos - scroll;          }
		double Bottom(int scroll = 0)  { return pos + size - scroll;   }
		double Width()                 { return size; 				   }
		double Height()                { return size;				   }

		int    nLeft(int scroll = 0)   { return npos - scroll;         }
		int    nRight(int scroll = 0)  { return npos + nsize - scroll; }
		int    nTop(int scroll = 0)    { return npos - scroll;         }
		int    nBottom(int scroll = 0) { return npos + nsize - scroll; }
		int    nWidth()                { return nsize; 				   }
		int    nHeight()               { return nsize;				   }

		bool IsMin() { return fceil(size) <= min; }
		bool IsMax() { return fceil(size) >= max; }

		bool IsSelect()    { return style & GD::SELECT;    }
		bool IsLive()      { return style & GD::LIVE;      }
		bool IsCursor()    { return style & GD::CURSOR;    }
		bool IsHighlight() { return style & GD::HIGHLIGHT; }

		void Select(bool s)    { BitSet(style, GD::SELECT, s);    }
		void Cursor(bool s)    { BitSet(style, GD::CURSOR, s);    }
		void Live(bool s)      { BitSet(style, GD::LIVE, s);      }
		void Highlight(bool s) { BitSet(style, GD::HIGHLIGHT, s); }

		void ChangeSortMode(bool idsort = true);

		friend bool operator<(const ItemRect& a, const ItemRect& b)
		{
			if(sortMode)
//				return (StdValueCompare((*items)[a.id][sortCol].val, (*items)[b.id][sortCol].val, 0) < 0);
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
		ItemRect& Fixed(bool set = true);
		ItemRect& Edit(Ctrl &newCtrl);
		ItemRect& SetConvert(Convert &c);
		ItemRect& SetFormat(const char *fmt);
		ItemRect& SetDisplay(GridDisplay &gd);
		ItemRect& NoConvertion();
		ItemRect& Default(Value v);

		ItemRect& AlignTop()          { align = GD::TOP;     return *this; }
		ItemRect& AlignBottom()       { align = GD::BOTTOM;  return *this; }
		ItemRect& AlignLeft()         { align = GD::LEFT;    return *this; }
		ItemRect& AlignRight()        { align = GD::RIGHT;   return *this; }
		ItemRect& AlignHorzCenter()   { align = GD::HCENTER; return *this; }
		ItemRect& AlignVertCenter()   { align = GD::VCENTER; return *this; }
		ItemRect& AlignCenter()       { align = GD::HCENTER | GD::VCENTER; return *this; }
		ItemRect& AlignTopLeft()      { align = GD::LEFT    | GD::TOP;     return *this; }
		ItemRect& AlignTopRight()     { align = GD::RIGHT   | GD::TOP;     return *this; }
		ItemRect& AlignTopCenter()    { align = GD::HCENTER | GD::TOP;     return *this; }
		ItemRect& AlignBottomLeft()   { align = GD::LEFT    | GD::BOTTOM;  return *this; }
		ItemRect& AlignBottomRight()  { align = GD::RIGHT   | GD::BOTTOM;  return *this; }
		ItemRect& AlignBottomCenter() { align = GD::HCENTER | GD::BOTTOM;  return *this; }

		ItemRect& WrapText()          { wrap = true; return *this; }

		ItemRect& SetFont(Font &f)    { fnt = f; return *this; }
		ItemRect& Bg(Color c)         { bg = c;  return *this; }
		ItemRect& Fg(Color c)         { fg = c;  return *this; }

		ItemRect& Editable(bool b)    { editable = b; return *this; }

		bool IsConvertion() { return convertion; }

		void Serialize(Stream &s);
		void Clear();
};

typedef Vector<ItemRect> RectItems;
typedef Vector<ItemRect> VItems;
typedef Vector<ItemRect> HItems;
/*--------------------------------------------------------------------------------*/

#endif
