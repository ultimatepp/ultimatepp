 #ifndef _DateTimeCtrl_DateTimeCtrl_h_
#define _DateTimeCtrl_DateTimeCtrl_h_

#include <CtrlLib/CtrlLib.h>

class FlatButton : public Pusher
{
	public:
		Image img;
		Color fg, bg, hl;
		bool left;
		bool drawedge;

		FlatButton();

		void DrawFrame(Draw &w, const Rect &r, Color lc, Color tc, Color rc, Color bc);
		virtual void Paint(Draw &w);
		virtual void MouseEnter(Point p, dword kflags)
		{
			Refresh();
			Pusher::MouseEnter(p, kflags);
		}
		virtual void MouseLeave()
		{
			Refresh();
			Pusher::MouseLeave();
		}

		FlatButton& SetImage(const Image &_img)
		{
			img = _img;
			Refresh();
			return *this;
		}
		FlatButton& SetLeft() 			{left = true;	return *this;}
		FlatButton& SetRight() 			{left = false;	return *this;}
		FlatButton& DrawEdge(bool b) 	{drawedge = b;	return *this;}

};

class DateTimePopUp : public Ctrl
{
	public:
		typedef DateTimePopUp CLASSNAME;

		FlatButton mleft;
		FlatButton mright;

		FlatButton yleft;
		FlatButton yright;

		static const int cols = 7;
		static const int rows = 6;

		static const Point nullday;
		int bs; //border size
		int ls; //line space
		int lx; //horizontal line distance from left edge
		int ly; //vertical line distance from bottom edge of header
		int hs; //header vertical size
		int ts; //today vertical size
		int colw;
		int rowh;

		int col;
		int row;
		int lastrow;

		bool selall;

		Color colHeader;
		Color colBg;
		Color colFg;
		Color colCurDayBg;
		Color colCurDayFg;
		Color colCurDayBr;
		Color colSelDayBg;
		Color colSelDayFg;
		Color colSelDayBr;
		Color colGrayDay;
		Color colCurDate;
		Color colToday;
		Color colSelToday;
		Color colSelDay;
		Color colHLine;
		Color colVLine;
		Color colDayNames;
		Color colWeek;

		int days[rows][cols];

		Point newday;
		Point oldday;

		String today;
		Size sztoday;
		bool istoday;
		bool wastoday;

		EditDate *date;

		int day, month, year;
		int tday, tmonth, tyear;
		int cday, cmonth, cyear;
		int selday;
		int selmonth;

		bool userdate;
		bool open;
		bool clickall;

		DateTimePopUp();

		void Reset();

		void OnMonthLeft();
		void OnMonthRight();
		void OnYearLeft();
		void OnYearRight();

		bool IsLeapYear(int year) { return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0); }

		int	DayOfWeek(int day, int month, int year, int zelleroffset = 2);
		int WeekOfYear(int day, int month, int year);

		virtual void LeftDown(Point p, dword keyflags);
		virtual void MouseMove(Point p, dword keyflags);
		virtual Image CursorImage(Point p, dword keyflags);

		bool MouseOnToday(Point p);

		Point GetDay(Point p);

		virtual void Paint(Draw &w);
		virtual void Deactivate();
		virtual bool Key(dword key, int count);

		void PopUp(Ctrl *owner, Rect &rt);

		int& Day(int x, int y) { return days[y][x]; }
		int& Day(Point p) 	   { return days[p.y][p.x]; }

		void RefreshDay(Point p);
};


class DateTimeCtrl : public EditDate
{
	FrameRight<Button> drop;
	DateTimePopUp      calendar;
	void OnDrop();

	int width, height;

	static const int defWidth = 215;
	static const int defHeight = 150;

	public:
		typedef DateTimeCtrl CLASSNAME;

	    DateTimeCtrl();


		DateTimeCtrl& SetDate(int d, int m, int y);
		DateTimeCtrl& SetSize(int w = defWidth, int h = defHeight);

		DateTimeCtrl& SetBorderSize(int size) { calendar.bs = size;  return *this; }
		DateTimeCtrl& SelectAll(bool b) 	  { calendar.selall = b; return *this; }
};

#endif
