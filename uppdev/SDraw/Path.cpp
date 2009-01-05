#include "SDraw.h"

NAMESPACE_UPP

Pointf SDraw::ReadPoint(CParser& p, bool rel)
{
	Pointf t;
	t.x = p.IsDouble() ? p.ReadDouble() : 0;
	t.y = p.IsDouble() ? p.ReadDouble() : 0;
	return rel ? t + current : t;
}

SDraw& SDraw::Path(CParser& p)
{
	while(!p.IsEof()) {
		int c = p.GetChar();
		bool rel = IsLower(c);
		Pointf t, t1, t2;
		switch(ToUpper(c)) {
		case 'M':
			t = ReadPoint(p, rel);
			Move(t.x, t.y);
		case 'L':
			while(p.IsDouble()) {
				t = ReadPoint(p, rel);
				Move(t.x, t.y);
			}
			break;
		case 'Z':
			Close();
			break;
		case 'H':
			while(p.IsDouble())
				Line(p.ReadDouble() + rel * current.x, current.y);
			break;
		case 'V':
			while(p.IsDouble())
				Line(p.ReadDouble() + rel * current.x, current.y);
			break;
		case 'C':
			while(p.IsDouble()) {
				t1 = ReadPoint(p, rel);
				t2 = ReadPoint(p, rel);
				t = ReadPoint(p, rel);
				Cubic(t1.x, t1.y, t2.x, t2.y, t.x, t.y);
			}
			break;
		case 'S':
			while(p.IsDouble()) {
				t2 = ReadPoint(p, rel);
				t = ReadPoint(p, rel);
				Cubic(t2.x, t2.y, t.x, t.y);
			}
			break;
		case 'Q':
			while(p.IsDouble()) {
				t1 = ReadPoint(p, rel);
				t = ReadPoint(p, rel);
				Quadratic(t1.x, t1.y, t.x, t.y);
			}
			break;
		case 'T':
			while(p.IsDouble()) {
				t = ReadPoint(p, rel);
				Quadratic(t.x, t.y);
			}
			break;
		default:
			return *this;
		}
	}
	return *this;
}

SDraw& SDraw::Path(const char *path)
{
	CParser p(path);
	Path(p);
	return *this;
}

END_UPP_NAMESPACE
