#ifndef _GridCtrl_GridDisplay_h_
#define _GridCtrl_GridDisplay_h_

#define  IMAGEFILE <GridCtrl/GridDisplay.iml>
#define  IMAGECLASS GridDispImg
#include <Draw/iml_header.h>

#define BIT(x) (1 << x)

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
		ALIGN     = LEFT | RIGHT | TOP | BOTTOM	| VCENTER | HCENTER
	};
}

class GridDisplay
{
	public:

		Image	leftImg;
		Image	rightImg;
		Image   centerImg;
		Image	bgImg;
		Font	font;
		int 	align;
		int		lm, rm, tm, bm;
		int     theme;
		bool	hgrid;
		bool	vgrid;

		int 	col, row;

		GridCtrl *parent;

		GridDisplay() : font(StdFont())
		{
			align = GD::TOP | GD::LEFT;
			lm = rm = 4;
			tm = bm = 0;
			theme = 5;
		}

		~GridDisplay() {};

		void SetLeftImage(const Image &img)			 { leftImg  = img;  	       }
		void SetRightImage(const Image &img)		 { rightImg = img;  	       }
		void SetCenterImage(const Image &img)		 { centerImg = img;  	       }
		void SetBgImage(Image &img)					 { bgImg    = img;    	       }
		void SetFont(Font &fnt)						 { font     = fnt;    	       }
		void SetTextAlign(int al)		 	 		 { align = al; 				   }
		void SetHMargin(int left, int right)		 { lm = left; rm = right;  	   }
		void SetVMargin(int top, int bottom)		 { tm = top;  bm = bottom;     }
		void SetTheme(int th)                        { theme = th;                 }
		int  GetThemeCount()                         { return 6;                   }


		void DrawText(Draw &w, int mx, int x, int y, int cx, int cy, int align,
					  const wchar *s, const Font &font, const Color &fg, const Color &bg,
					  bool found, int fs, int fe, bool wrap);

		virtual void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
				           Color &fg, Color &bg, Font &fnt, bool found = false, int fs = 0, int fe = 0);

		virtual void PaintFixed(Draw &w, bool firstx, bool firsty, int x, int y, int cx, int cy, const Value &val, dword style,
								bool indicator = false, bool moved = false,
								int sortmode = 0, int sortcol = -1, int sortcnt = 0, bool horizontal = true);

};


extern GridDisplay StdGridDisplay;

#endif
