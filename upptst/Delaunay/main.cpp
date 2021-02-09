#include <CtrlLib/CtrlLib.h>
#include <Geom/Geom.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void Paint(Draw& w)
	{
		Array<Pointf> poly;
		poly << Pointf(18.951605,	0.8) << Pointf(19.714577,	0.6168874) << Pointf(20.14996, 0) << Pointf(19.714577, -0.6168874) << Pointf(18.951605, -0.8) << Pointf(17.753117, 0) << Pointf(18.188618, -0.6) << Pointf(18.188618, 0.6);
			
		Delaunay del;
		del.Build(poly);

		auto Get = [&](int i) {
			Pointf fp = poly[i];
			fp.x -= 19;
			return (Point)(fp * 200 + GetSize() / 2);
		};
		
		w.DrawRect(GetSize(), White());
		for(int i = 0; i < poly.GetCount(); i++) {
			Point p = Get(i);
			w.DrawRect(p.x - 3, p.y - 3, 5, 5, LtBlue());
		}
		
		for(int i = 0; i < del.GetCount(); i++) {
			const Delaunay::Triangle& t = del[i];
			if(t.IsProper()) {
				w.DrawLine(Get(t[0]), Get(t[1]));
				w.DrawLine(Get(t[1]), Get(t[2]));
				w.DrawLine(Get(t[2]), Get(t[0]));
			}
		}
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
