#include "Painter.h"

NAMESPACE_UPP

Pointf Painter::ReadPoint(CParser& p)
{
	Pointf t;
	t.x = p.IsDouble() ? p.ReadDouble() : 0;
	p.Char(',');
	t.y = p.IsDouble() ? p.ReadDouble() : 0;
	return t;
}

Painter& Painter::Path(CParser& p)
{
	while(!p.IsEof()) {
		int c = p.GetChar();
		p.Spaces();
		bool rel = IsLower(c);
		Pointf t, t1, t2;
		switch(ToUpper(c)) {
		case 'M':
			t = ReadPoint(p);
			Move(t.x, t.y);
		case 'L':
			while(p.IsDouble()) {
				t = ReadPoint(p);
				Line(t.x, t.y, rel);
			}
			break;
		case 'Z':
			Close();
			break;
		case 'H':
			while(p.IsDouble())
				Line(p.ReadDouble(), Null, rel);
			break;
		case 'V':
			while(p.IsDouble())
				Line(Null, p.ReadDouble(), rel);
			break;
		case 'C':
			while(p.IsDouble()) {
				t1 = ReadPoint(p);
				t2 = ReadPoint(p);
				t = ReadPoint(p);
				Cubic(t1.x, t1.y, t2.x, t2.y, t.x, t.y, rel);
			}
			break;
		case 'S':
			while(p.IsDouble()) {
				t2 = ReadPoint(p);
				t = ReadPoint(p);
				Cubic(t2.x, t2.y, t.x, t.y, rel);
			}
			break;
		case 'Q':
			while(p.IsDouble()) {
				t1 = ReadPoint(p);
				t = ReadPoint(p);
				Quadratic(t1.x, t1.y, t.x, t.y, rel);
			}
			break;
		case 'T':
			while(p.IsDouble()) {
				t = ReadPoint(p);
				Quadratic(t.x, t.y, rel);
			}
			break;
		default:
			return *this;
		}
	}
	return *this;
}

Painter& Painter::Path(const char *path)
{
	CParser p(path);
	Path(p);
	return *this;
}

END_UPP_NAMESPACE
