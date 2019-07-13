#include "Painter.h"

namespace Upp {

Painter& Painter::Path(CParser& p)
{
	Pointf current(0, 0);
	bool done = false;
	while(!p.IsEof()) {
		int c = p.GetChar();
		p.Spaces();
		bool rel = IsLower(c);
		Pointf t, t1, t2;
		auto ReadDouble = [&] {
			while(p.Char(','));
			return p.IsDouble2() ? p.ReadDouble() : 0;
		};
		auto ReadPointP = [&](Pointf current, bool rel) {
			Pointf t;
			t.x = ReadDouble();
			t.y = ReadDouble();
			if(rel)
				t += current;
			return t;
		};
		auto ReadPoint = [&]() { return ReadPointP(current, rel); };
		switch(ToUpper(c)) {
		case 'M':
			current = ReadPoint();
			Move(current);
		case 'L':
			while(p.IsDouble2()) {
				current = ReadPoint();
				Line(current);
			}
			done = true;
			break;
		case 'Z':
			Close();
			done = true;
			break;
		case 'H':
			while(p.IsDouble2()) {
				current.x = p.ReadDouble() + rel * current.x;
				Line(current);
				done = true;
			}
			break;
		case 'V':
			while(p.IsDouble2()) {
				current.y = p.ReadDouble() + rel * current.y;
				Line(current);
				done = true;
			}
			break;
		case 'C':
			while(p.IsDouble2()) {
				t1 = ReadPoint();
				t2 = ReadPoint();
				current = ReadPoint();
				Cubic(t1, t2, current);
				done = true;
			}
			break;
		case 'S':
			while(p.IsDouble2()) {
				t2 = ReadPoint();
				current = ReadPoint();
				Cubic(t2, current);
				done = true;
			}
			break;
		case 'Q':
			while(p.IsDouble2()) {
				t1 = ReadPoint();
				current = ReadPoint();
				Quadratic(t1, current);
				done = true;
			}
			break;
		case 'T':
			while(p.IsDouble2()) {
				current = ReadPoint();
				Quadratic(current);
				done = true;
			}
			break;
		case 'A':
			while(p.IsDouble2()) {
				t1 = ReadPointP(Pointf(0, 0), false);
				double xangle = ReadDouble();
				auto ReadBool = [&] {
					while(p.Char(','));
					if(p.Char('1')) return true;
					p.Char('0');
					return false;
				};
				bool large = ReadBool();
				bool sweep = ReadBool();
				current = ReadPoint();
				SvgArc(t1, xangle * M_PI / 180.0, large, sweep, current);
				done = true;
			}
			break;
		default:
			if(!done)
				Move(0, 0); // to clear previous path
			return *this;
		}
	}
	if(!done)
		Move(0, 0); // to clear previous path
	return *this;
}

Painter& Painter::Path(const char *path)
{
	try {
		CParser p(path);
		Path(p);
	}
	catch(CParser::Error) {}
	return *this;
}

}
