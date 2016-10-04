#ifndef _ErrorThread_displays_h_
#define _ErrorThread_displays_h_

using namespace Upp;

struct ProgressDisplayMy : Display
{
	Color color;
	int x;

	virtual void Paint(Draw& w, const Rect& r, const Value& q,
	Color ink, Color paper, dword style) const
	{
		w.DrawRect(x, 0, 100, r.GetHeight(), Blue);
	}
};

#endif
