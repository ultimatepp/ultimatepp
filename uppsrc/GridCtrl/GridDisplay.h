#ifndef _GridCtrl_GridDisplay_h_
#define _GridCtrl_GridDisplay_h_

#define  IMAGEFILE <GridCtrl/GridCtrl.iml>
#define  IMAGECLASS GridImg
#include <Draw/iml_header.h>

#define BIT(x) (1 << x)
#define BIT_ALL (0xFFFFFFFF)

class GridCtrl;

namespace GD
{
	enum
	{
		CURSOR    = BIT(0),
		SELECT    = BIT(1),
		LIVE      = BIT(2),
		FOCUS     = BIT(3),
		READONLY  = BIT(4),
		FOUND     = BIT(5),
		MARKED    = CURSOR | SELECT | LIVE | READONLY | FOUND,
		HIGHLIGHT = BIT(6),
		EVEN      = BIT(7),
		ODD       = BIT(8),
		LEFT      = BIT(9),
		RIGHT     = BIT(10),
		TOP       = BIT(11),
		BOTTOM    = BIT(12),
		HCENTER   = BIT(13),
		VCENTER   = BIT(14),
		HPOS      = BIT(15),
		VPOS      = BIT(16),
		WRAP      = BIT(17),
		CHAMELEON = BIT(18),
		NOTEXT    = BIT(19),
		BOLDKEY   = BIT(20),
		BOLDVALUE = BIT(21),
		HALIGN    = LEFT | RIGHT | HCENTER,
		VALIGN    = TOP | BOTTOM | VCENTER,
		ALIGN     = HALIGN | VALIGN
	};
}

class GridDisplay
{
	public:

		Image	leftImg;
		Image	rightImg;
		Image   centerImg;
		Image	bgImg;
		Image   hdrhigh;
		
		bool	reverse_sort_icon;
		
		int 	align;
		int		lm, rm, tm, bm;
		int     theme;

		int 	col, row;

		GridCtrl *parent;

		GridDisplay()
		{
			SetDefault();
		}
		
		Size real_size;

		void SetDefault();

		virtual ~GridDisplay() {};

		void SetLeftImage(const Image &img)			    { leftImg = img;               }
		void SetRightImage(const Image &img)		    { rightImg = img;              }
		void SetCenterImage(const Image &img)		    { centerImg = img;  	       }
		void SetBgImage(Image &img)					    { bgImg = img;    	           }
		void SetTextAlign(int al = GD::TOP | GD::LEFT)  { align = al;                  }
		void SetHorzMargin(int left = 4, int right = 4) { lm = left; rm = right;  	   }
		void SetVertMargin(int top = 0, int bottom = 0) { tm = top;  bm = bottom;      }
		void ReverseSortIcon(bool b = true)				{ reverse_sort_icon = b;       }
		void SetTheme(int th = 6);
		int  GetThemeCount()                            { return 7;                    }
		bool IsFixedRow()                               { return row < 0;              }
		bool IsFixedCol()                               { return col < 0;              }

		WString GetStdConvertedValue(const Value &v) const;
		int GetLinesCount(int cx, const wchar* s, const Font& font, bool wrap);

		void DrawText(Draw &w, int mx, int x, int y, int cx, int cy, int align,
					  const wchar *s, const Font &font, const Color &fg, const Color &bg,
					  bool found = false, int fs = 0, int fe = 0, bool wrap = false);

		virtual void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
				           Color &fg, Color &bg, Font &fnt, bool found = false, int fs = 0, int fe = 0);

		virtual void PaintFixed(Draw &w, bool firstx, bool firsty, int x, int y, int cx, int cy, const Value &val, dword style, Font &fnt,
								bool indicator = false, bool moved = false,
								int sortmode = 0, int sortcol = -1, int sortcnt = 0, bool horizontal = true);

};

struct GridDisplayCtrl : GridDisplay, StaticRect
{};

GridDisplay& StdGridDisplay();

#endif
