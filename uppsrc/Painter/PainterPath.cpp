#include "Painter.h"

namespace Upp {

Painter& Painter::Path(CParser& p)
{
	Pointf current(0, 0);
	bool done = false;
	int previousCmd = 0;
	while(!p.IsEof()) {
		while(p.Char(','));
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
		auto ReadPoint = [&] { return ReadPointP(current, rel); };
		auto IsDouble = [&] { while(p.Char(',')) {} return p.IsDouble2(); };
		switch(ToUpper(c)) {
		case 'M':
			current = ReadPoint();
			Move(current);
		case 'L':
			while(IsDouble()) {
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
			while(IsDouble()) {
				current.x = p.ReadDouble() + rel * current.x;
				Line(current);
				done = true;
			}
			break;
		case 'V':
			while(IsDouble()) {
				current.y = p.ReadDouble() + rel * current.y;
				Line(current);
				done = true;
			}
			break;
		case 'C':
			while(IsDouble()) {
				t1 = ReadPoint();
				t2 = ReadPoint();
				current = ReadPoint();
				Cubic(t1, t2, current);
				done = true;
			}
			break;
		case 'S':
			while(IsDouble()) {
				Pointf now = current;
				t2 = ReadPoint();
				current = ReadPoint();
				if(previousCmd != 'C' && previousCmd != 'S')
					Cubic(now, t2, current);
				else
					Cubic(t2, current);
				while(p.Char(','));
				done = true;
			}
			break;
		case 'Q':
			while(IsDouble()) {
				t1 = ReadPoint();
				current = ReadPoint();
				Quadratic(t1, current);
				done = true;
			}
			break;
		case 'T':
			while(IsDouble()) {
				Pointf now = current;
				current = ReadPoint();
				if(previousCmd != 'Q' && previousCmd != 'T')
					Quadratic(now, current);
				else
					Quadratic(current);
				done = true;
			}
			break;
		case 'A':
			while(IsDouble()) {
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
		
		previousCmd = ToUpper(c);
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
