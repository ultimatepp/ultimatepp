#include "Core.h"

NAMESPACE_UPP

INITBLOCK {
	Point p;
	RichValue<Point>::Register();
	RichValue<Point64>::Register();
	RichValue<Pointf>::Register();
	RichValue<Size>::Register();
	RichValue<Size64>::Register();
	RichValue<Sizef>::Register();
	RichValue<Rect>::Register();
	RichValue<Rect64>::Register();
	RichValue<Rectf>::Register();
}

//template <>
//void Rect_<double>::Union(const Rect_<double>& r) {
void Rect_double_Union(Rect_<double>& self, const Rect_<double>& r) {
	if(IsNull(r)) return;
	if(IsNull(self)) {
		self.Set(r);
		return;
	}
	if(r.left   < self.left)   self.left   = r.left;
	if(r.top    < self.top)    self.top    = r.top;
	if(r.right  > self.right)  self.right  = r.right;
	if(r.bottom > self.bottom) self.bottom = r.bottom;
}

//bool Rect_double_Contains(const Rect_<double>& self, const Point_<double>& p) {
//	return p.x >= self.left && p.x <= self.right && p.y >= self.top && p.y <= self.bottom;
//}

//template <>
//bool Rect_<double>::Intersects(const Rect_<double>& r) const {
bool Rect_double_Intersects(const Rect_<double>& self, const Rect_<double>& r) {
	if(IsNull(self) || IsNull(r)) return false;
	return r.right >= self.left && r.bottom >= self.top && r.left <= self.right && r.top <= self.bottom;
}

//template <>
//Point_<double> Rect_<double>::Bind(Point_<double> pt) const
Point_<double> Rect_double_Bind(const Rect_<double>& self, Point_<double> pt) {
	return Point_<double>(pt.x < self.left ? self.left : pt.x > self.right ? self.right : pt.x,
		pt.y < self.top ? self.top : pt.y > self.bottom ? self.bottom : pt.y);
}

Size iscale(Size a, int b, int c)
{
	return Size(iscale(a.cx, b, c), iscale(a.cy, b, c));
}

Size iscalefloor(Size a, int b, int c)
{
	return Size(iscalefloor(a.cx, b, c), iscalefloor(a.cy, b, c));
}

Size iscaleceil(Size a, int b, int c)
{
	return Size(iscaleceil(a.cx, b, c), iscaleceil(a.cy, b, c));
}

Size idivfloor(Size a, int b)
{
	return Size(idivfloor(a.cx, b), idivfloor(a.cy, b));
}

Size idivceil(Size a, int b)
{
	return Size(idivceil(a.cx, b), idivceil(a.cy, b));
}

Size iscale(Size a, Size b, Size c)
{
	return Size(iscale(a.cx, b.cx, c.cx), iscale(a.cy, b.cy, c.cy));
}

Size iscalefloor(Size a, Size b, Size c)
{
	return Size(iscalefloor(a.cx, b.cx, c.cx), iscalefloor(a.cy, b.cy, c.cy));
}

Size iscaleceil(Size a, Size b, Size c)
{
	return Size(iscaleceil(a.cx, b.cx, c.cx), iscaleceil(a.cy, b.cy, c.cy));
}

Size idivfloor(Size a, Size b)
{
	return Size(idivfloor(a.cx, b.cx), idivfloor(a.cy, b.cy));
}

Size idivceil(Size a, Size b)
{
	return Size(idivceil(a.cx, b.cx), idivceil(a.cy, b.cy));
}

Size  GetRatioSize(Size sz, int cx, int cy)
{
	return cx == 0 ? cy == 0 ? sz : sz.cy ? Size(sz.cx * cy / sz.cy, cy) : Size(0, 0)
		       : cy == 0 ? sz.cx ? Size(cx, sz.cy * cx / sz.cx) : Size(0, 0)
				         : Size(cx, cy);
}

Size  GetFitSize(Size sz, int cx, int cy)
{
	if(cx <= 0 || cy <= 0 || sz.cx <= 0 || sz.cy <= 0)
		return Size(0, 0);
	if(cx * sz.cy >= cy * sz.cx) // too high
		return iscale(sz, cy, sz.cy);
	else
		return iscale(sz, cx, sz.cx);
}

END_UPP_NAMESPACE
