#include "CtrlCore.h"

#ifdef GUI_X11

namespace Upp {

Vector<Rect> RectRgn(const Rect& r)
{
	Vector<Rect> q;
	q.Add(r);
	return q;
}

Vector<Rect> Intersect(const Vector<Rect>& r1, const Vector<Rect>& r2)
{
	Vector<Rect> q;
	for(int i = 0; i < r1.GetCount(); i++)
		for(int j = 0; j < r2.GetCount(); j++) {
			Rect r = r1[i] & r2[j];
			if(!r.IsEmpty())
				q.Add(r);
		}
	return q;
}

Vector<Rect> Subtract(const Vector<Rect>& r1, const Vector<Rect>& r2)
{
	Vector<Rect> q;
	bool dummy;
	q <<= r1;
	for(int i = 0; i < r2.GetCount(); i++)
		q = Subtract(q, r2[i], dummy);
	return q;
}

Vector<Rect> Xor(const Vector<Rect>& r1, const Vector<Rect>& r2)
{
	Vector<Rect> is = Intersect(r1, r2);
	Vector<Rect> q = Subtract(r1, is);
	q.Append(Subtract(r2, is));
	return q;
}

}

#endif
