#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(sz, White());
		static const int d[] = { PEN_NULL, PEN_SOLID, PEN_DASH, PEN_DOT, PEN_DASHDOT, PEN_DASHDOTDOT, 1, 2, 3, 10 };
		for(int i = 0; i < __countof(d); i++) {
			w.DrawLine(10, i * 10, 200, i * 10, d[i], Red());
			w.DrawLine(300, i * 10, 400, i * 10 + 15, d[i], Red());
			w.DrawLine(450 + i * 10, 10, 450 + i * 10, 200, d[i], Red());
		}

		Point points[12][2][4];
		int subpolygon_counts[12][2];
		int disjunct_counts[12];
		Point lpoints[12][5];
		int lcounts[12][2];
		Pointf center(300, 300);
		static const double out_delta = 2 * M_PI / 24;
		static const double in_delta = 2 * M_PI / 48;
		for(int i = 0; i < 12; i++) {
			double angle = i * 2 * M_PI / 12;
			points[i][0][0] = Polar(center, 200, angle);
			points[i][0][1] = Polar(center, 120, angle - out_delta);
			points[i][0][2] = Polar(center, 40, angle);
			points[i][0][3] = Polar(center, 120, angle + out_delta);
			points[i][1][0] = Polar(center, 160, angle);
			points[i][1][1] = Polar(center, 120, angle + in_delta);
			points[i][1][2] = Polar(center, 80, angle);
			points[i][1][3] = Polar(center, 120, angle - in_delta);
			subpolygon_counts[i][0] = 4;
			subpolygon_counts[i][1] = 4;
			disjunct_counts[i] = subpolygon_counts[i][0] + subpolygon_counts[i][1];
			
			lpoints[i][0] = Polar(center, 260, angle + in_delta);
			lpoints[i][1] = Polar(center, 230, angle);
			lpoints[i][2] = Polar(center, 260, angle - in_delta);
			lcounts[i][0] = 3;
			lpoints[i][3] = Polar(center, 280, angle + in_delta);
			lpoints[i][4] = Polar(center, 280, angle - in_delta);
			lcounts[i][1] = 2;
		}
		w.DrawPolyPolyPolygon(points[0][0], sizeof(points) / sizeof(Point),
			subpolygon_counts[0], sizeof(subpolygon_counts) / sizeof(int),
			disjunct_counts, sizeof(disjunct_counts) / sizeof(int),
			LtBlue(), 4, LtRed(), I64(0xF0F0F0F00F0F0F0F));
		w.DrawPolyPolyline(lpoints[0], sizeof(lpoints) / sizeof(Point),
			lcounts[0], sizeof(lcounts) / sizeof(int),
			4, Color(0, 192, 0));
		
		w.DrawRect(sz.cx - 200, sz.cy - 100, 200, 100, Yellow());
		w.DrawEllipse(sz.cx - 200, sz.cy - 100, 200, 100, Gray(), 3, LtBlue());
		w.DrawEllipse(0, sz.cy - 100, 150, 100, Gray(), PEN_DASHDOTDOT, LtBlue());

		{
			Pointf center(sz.cx - 320, 300);
			for(int i = 0; i < 13; i++) {
				double angle = (i - 3) * M_PI / 6;
				Pointf box_center = Polar(center, 250, angle);
				Size radius(60, 20);
				Rect box(Point(box_center), Size(1, 1));
				box.Inflate(radius);
				w.DrawRect(box, Yellow());
				for(int j = 0; j < 12; j++) {
					double ang2 = j * M_PI / 6;
					Sizef hand = Polar(Pointf(0, 0), 1, ang2);
					Point h1(box_center + Sizef(radius) * hand);
					Point h2(box_center + Sizef(radius) * 0.7 * hand);
					w.DrawLine(h1, h2, 1, LtBlue());
				}
				Sizef hand = Polar(Pointf(0, 0), 1, angle);
				w.DrawArc(box, Point(box_center + radius * hand), box.TopCenter(), PEN_DASHDOT, LtRed());
			}
		}
	}

	typedef MyApp CLASSNAME;

	MyApp() {
	}
};

GUI_APP_MAIN
{
	MyApp().Sizeable().Run();
}
