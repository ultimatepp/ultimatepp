#include "Circles.h"

void CircleDoc::Update(const Rect& r)
{
	bound.Clear();
	for(int i = 0; i < data.GetCount(); i++)
		bound.Union(data[i].GetRect());
	for(int i = 0; i < view.GetCount(); i++)
		view[i].UpdateView(r);
}

void CircleDoc::UpdateFull()
{
	Rect r(-INT_MAX, -INT_MAX, INT_MAX, INT_MAX);
	Update(r);
}

void CircleDoc::AddCircle(Point p, int radius)
{
	redo.Clear();
	CircleUndo& u = undo.Add();
	u.insert = false;
	u.index = data.GetCount();
	CircleShape& c = data.Add();
	c.point = p;
	c.radius = radius;
	Update(c.GetRect());
}

void CircleDoc::RemoveCircle(int i)
{
	redo.Clear();
	CircleUndo& u = undo.Add();
	u.insert = true;
	u.index = i;
	u.shape = data[i];
	data.Remove(i);
	Update(u.shape.GetRect());
}

void CircleDoc::UndoRedo(Array<CircleUndo>& src, Array<CircleUndo>& dst)
{
	if(src.GetCount() == 0)
		return;
	CircleUndo& u = src.Top();
	CircleUndo& r = dst.Add();
	r.index = u.index;
	Rect ur;
	if(u.insert) {
		r.insert = false;
		data.Insert(u.index, u.shape);
		ur = u.shape.GetRect();
	}
	else {
		r.insert = true;
		r.shape = data[u.index];
		data.Remove(u.index);
		ur = r.shape.GetRect();
	}
	src.Drop();
	Update(ur);
}

void CircleDoc::NewView()
{
	view.Add(new CircleView(*this)).OpenMain();
}

void CircleDoc::RemoveView(CircleView *v)
{
	view.Remove(view.GetIndex(*v));
	if(view.GetCount() == 0)
		delete this;
}

void CircleDoc::Clear()
{
	data.Clear();
	undo.Clear();
	redo.Clear();
	UpdateFull();
	AddCircle(Point(100, 20), 10 );
	AddCircle(Point(20, 120), 10 );
	AddCircle(Point(160, 120), 10 );
}

bool   CircleDoc::LoadText(const char *text)
{
	CParser p(text);
	Clear();
	try {
		p.PassId("circle");
		CircleShape& c = data.Add();
		c.point.x = p.ReadNumber();
		p.PassChar(',');
		c.point.y = p.ReadNumber();
		p.PassChar(',');
		c.radius = p.ReadNumber();
		p.PassChar(';');
	}
	catch(...) {
		Clear();
		return false;
	}
	return true;
}

String CircleDoc::SaveText() const
{
	String r;
	for(int i = 0; i < data.GetCount(); i++) {
		const CircleShape& c = data[i];
		r << "circle " << c.point.x << ", " << c.point.y << ", " << c.radius << ";";
	}
	return r;
}

void   CircleDoc::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	for(int i = 0; i < data.GetCount(); i++) {
		CircleShape& c = data[i];
		s % c.point.x % c.point.y % c.radius;
	}
}

bool   CircleDoc::LoadBinary(const String& data)
{
	StringStream ss(data);
	if(Load(*this, ss))
		return true;
	Clear();
	return false;
}

String CircleDoc::SaveBinary() const
{
	StringStream ss;
	Store(*this, ss);
	return ss.GetResult();
}
