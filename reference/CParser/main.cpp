#include <Core/Core.h>

using namespace Upp;

VectorMap<String, double> var;

double Exp(CParser& p);

double Term(CParser& p)
{
	if(p.Id("abs")) {
		p.PassChar('(');
		double x = Exp(p);
		p.PassChar(')');
		return fabs(x);
	}
	if(p.IsId())
		return var.Get(p.ReadId(), 0);
	if(p.Char('(')) {
		double x = Exp(p);
		p.PassChar(')');
		return x;
	}
	return p.ReadDouble();
}

double Mul(CParser& p)
{
	double x = Term(p);
	for(;;)
		if(p.Char('*'))
			x = x * Term(p);
		else
		if(p.Char('/')) {
			double y = Term(p);
			if(y == 0)
				p.ThrowError("Divide by zero");
			x = x / y;
		}
		else
			return x;
}

double Exp(CParser& p)
{
	double x = Mul(p);
	for(;;)
		if(p.Char('+'))
			x = x + Mul(p);
		else
		if(p.Char('-'))
			x = x - Mul(p);
		else
			return x;
}

CONSOLE_APP_MAIN
{
	for(;;) {
		Cout() << "Expresion: ";
		String l = ReadStdIn();
		if(l.IsEmpty())
			break;
		CParser p(l);
		try {
			String id;
			if(p.IsId()) {
				CParser::Pos pos = p.GetPos();
				id = p.ReadId();
				if(p.Char('=')) {
					double x = Exp(p);
					Cout() << id << " <- " << x << '\n';
					var.GetAdd(id) = x;
					goto done;
				}
				p.SetPos(pos);
			}
			Cout() << Exp(p) << '\n';
		done:;
		}
		catch(CParser::Error e) {
			Cout() << "ERROR: " << e << '\n';
		}
	}
}
