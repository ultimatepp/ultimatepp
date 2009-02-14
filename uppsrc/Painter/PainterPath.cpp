#include "Painter.h"

NAMESPACE_UPP

bool Painter::ReadBool(CParser& p)
{
	while(p.Char(','));
	if(p.Char('1')) return true;
	p.Char('0');
	return false;
}

double Painter::ReadDouble(CParser& p)
{
	while(p.Char(','));
	return p.IsDouble() ? p.ReadDouble() : 0;
}

Pointf Painter::ReadPoint(CParser& p)
{
	Pointf t;
	t.x = ReadDouble(p);
	t.y = ReadDouble(p);
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
			Move(t, rel);
		case 'L':
			while(p.IsDouble()) {
				t = ReadPoint(p);
				Line(t, rel);
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
				Cubic(t1, t2, t, rel);
			}
			break;
		case 'S':
			while(p.IsDouble()) {
				t2 = ReadPoint(p);
				t = ReadPoint(p);
				Cubic(t2, t, rel);
			}
			break;
		case 'Q':
			while(p.IsDouble()) {
				t1 = ReadPoint(p);
				t = ReadPoint(p);
				Quadratic(t1, t, rel);
			}
			break;
		case 'T':
			while(p.IsDouble()) {
				t = ReadPoint(p);
				Quadratic(t, rel);
			}
			break;
		case 'A':
			while(p.IsDouble()) {
				t1 = ReadPoint(p);
				double xangle = ReadDouble(p);
				bool large = ReadBool(p);
				bool sweep = ReadBool(p);
				t = ReadPoint(p);
				SvgArc(t1, xangle * M_PI / 180.0, large, sweep, t, rel);
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
