#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class Gradient : public TopWindow {
public:
    typedef Gradient CLASSNAME;
    Gradient();

public:
    virtual void Paint(Draw& draw);
    virtual void MouseMove(Point pt, dword keyflags);

public:
    enum { RADIUS = 100 };
    Point focus_pt;
};

Gradient::Gradient()
{
    Sizeable().Zoomable();
    focus_pt = Point(0, 0);
}

void Gradient::Paint(Draw& draw)
{
    draw.DrawRect(draw.GetClip(), White());
    Point mid = GetSize() >> 1;
    Rect rc(mid, Size(1, 1));
    Point f = focus_pt - mid;
    Color c1 = Black();
    Color c2 = White();
    for(int y = -RADIUS; y <= RADIUS; y++) {
        int xlim = fround(sqrt((double)(RADIUS * RADIUS - y * y)));
        for(int x = -xlim; x <= xlim; x++) {
            Point pt(x, y);
            Point d = Point(x, y) - f;

            double A = (d.x * d.x + d.y * d.y);
            double B = 2 * (d.x * f.x + d.y * f.y);
            double C = f.x * f.x + f.y * f.y - RADIUS * RADIUS;
            double t = (-B + sqrt(B * B - 4 * A * C)) / (2 * A);
            int ratio1 = fround(256 * 1 / max(t, 1e-10));

            double dpt = ((double)(d.x * d.x + d.y * d.y));
            double dpe = ((double)tabs(RADIUS * RADIUS - x * x - y * y));

            int ratio2 = fround(256 * dpt / max(dpt + dpe, 1e-10));
            int ratio3 = fround(256 * sqrt(dpt) / max(sqrt(dpt) + sqrt(dpe), 1e-10));

			ratio1 &= ~0x3f;
			ratio2 &= ~0x3f;
			ratio3 &= ~0x3f;
			
			Color color1 = GradientColor(c1, c2, ratio1, 256);
            Color color2 = GradientColor(c1, c2, ratio2, 256);
            Color color3 = GradientColor(c1, c2, ratio3, 256);
//            Color color(
//                c1.GetR() + (((c2.GetR() - c1.GetR()) * ratio) >> 8),
//                c1.GetG() + (((c2.GetG() - c1.GetG()) * ratio) >> 8),
//                c1.GetB() + (((c2.GetB() - c1.GetB()) * ratio) >> 8));
            draw.DrawRect(x + mid.x, y + mid.y, 1, 1, color1);
            draw.DrawRect(x + mid.x - 2 * RADIUS, y + mid.y, 1, 1, color2);
            draw.DrawRect(x + mid.x + 2 * RADIUS, y + mid.y, 1, 1, color3);
        }
    }
    draw.DrawText(mid.x - 2 * RADIUS, mid.y + RADIUS, "Rylek bez sqrt");
    draw.DrawText(mid.x, mid.y + RADIUS, "Fidler");
    draw.DrawText(mid.x + 2 * RADIUS, mid.y + RADIUS, "Rylek s sqrt");
//    rc.Inflate(RADIUS);
//    draw.DrawEllipse(rc, Null, 0, Black());
}

void Gradient::MouseMove(Point pt, dword keyflags)
{
    focus_pt = pt;
    Refresh();
}

GUI_APP_MAIN
{
    Gradient().Run();
}
