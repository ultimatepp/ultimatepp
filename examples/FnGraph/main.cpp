#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct ExpressionEvaluator {
	CParser p; // this will hold our lexical context
	double  x; // 'variable'

	double Factor();
	double Exponent();
	double Term();
	double Expression();
};

double ExpressionEvaluator::Expression()
{ // resolve + -
	double y = Term(); // at least one term
	for(;;)
		if(p.Char('+'))
			y = y + Term(); // add another term
		else
		if(p.Char('-'))
			y = y - Term(); // subtract another term
		else
			return y; // no more + - operators
}

double ExpressionEvaluator::Term()
{ // resolve * /
	double y = Exponent(); // at least one member
	for(;;)
		if(p.Char('*'))
			y = y * Exponent(); // multiply by another member
		else
		if(p.Char('/'))
			y = y / Exponent(); // divide by another member
		else
			return y; // no more * / operators
}

double ExpressionEvaluator::Exponent()
{ // resolve power ^
	double y = Factor(); // at least one factor
	for(;;)
		if(p.Char('^'))
			y = pow(y, Factor()); // power by another factor
		else
			return y; // no more power ^ operators
}

double ExpressionEvaluator::Factor()
{
	if(p.Char('-')) // unary -
		return -Factor();
	if(p.Id("abs")) // some functions...
		return fabs(Factor());
	if(p.Id("sqrt"))
		return sqrt(Factor());
	if(p.Id("exp"))
		return exp(Factor());
	if(p.Id("exp2"))
		return exp2(Factor());
	if(p.Id("ln"))
		return log(Factor());
	if(p.Id("log"))
		return log10(Factor());
	if(p.Id("log2"))
		return log2(Factor());
	if(p.Id("sin"))
		return sin(Factor());
	if(p.Id("cos"))
		return cos(Factor());
	if(p.Id("tan"))
		return tan(Factor());
	if(p.Id("asin"))
		return asin(Factor());
	if(p.Id("acos"))
		return acos(Factor());
	if(p.Id("atan"))
		return atan(Factor());
	if(p.Id("sinh"))
		return sinh(Factor());
	if(p.Id("cosh"))
		return cosh(Factor());
	if(p.Id("tanh"))
		return tanh(Factor());
	if(p.Id("asinh"))
		return asinh(Factor());
	if(p.Id("acosh"))
		return acosh(Factor());
	if(p.Id("atanh"))
		return atanh(Factor());
	if(p.Id("x")) // our variable
		return x;
	if(p.Id("e")) // e constant
		return M_E;
	if(p.Id("pi")) // pi constant
		return M_PI;
	double x;
	if(p.Char('(')) { // resolve parenthesis - recurse back to Sum (+ - operators)
		x = Expression();
		p.PassChar(')'); // make sure there is closing parenthesis
	}
	else
		x = p.ReadDouble(); // last possibility is that we are at number...
	if(p.Char('!')) { // compute factorial
		if(x >= 0 && x < 120) {
			int n = (int)x;
			if(n == x) {
				x = 1;
				for(int q = 1; q <= n; q++)
					x *= q;
				return x;
			}
		}
		p.ThrowError("invalid argument");
	}
	return x;
}

double Evaluate(const char *s, double x)
{ // evaluate expression for given variable, return Null on any failure or out-of-bounds result (NaN)
	ExpressionEvaluator v;
	v.x = x;
	v.p.Set(s);
	try {
		double y = v.Expression();
		return y >= -1e200 && y < 1e200 ? y : Null;
	}
	catch(CParser::Error) {}
	return Null;
}

struct FnGraph : public TopWindow {
	virtual void Paint(Draw& w);
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);

	int        zoom = 9;
	EditString expression; // function to display
	
	FnGraph();
};

FnGraph::FnGraph()
{
	Title("Graph of a function");
	Add(expression.TopPos(0).HSizePos()); // place widget to the top, horizontally fill the window
	expression << [=] { Refresh(); }; // when expression changes, repaint the graph
	Sizeable().Zoomable(); // make the window resizable
}

void FnGraph::MouseWheel(Point, int zdelta, dword keyflags)
{
	zoom = clamp(zoom + -sgn(zdelta), 1, 20);
	Refresh();
}

void FnGraph::Paint(Draw& w_)
{
	Size sz = GetSize();
	DrawPainter w(w_, sz); // Use Painter for smooth sw rendering
	w.Clear(White()); // clear the background
	int ecy = expression.GetSize().cy; // query the height of widget
	w.Offset(0, ecy); // move coordinates out of widget
	sz.cy -= ecy; // and reduce the size
	if(sz.cy < 1) // if too small, do nothing (avoid div by zero)
		return;
	sz = sz / 2 * 2 - 1; // this trick will force axes to .5, results in sharper AA rendering
	double pixels_per_unit = sz.cy / zoom; // we want to display y range -4.5 .. 4.5
	double xaxis = sz.cy / 2.0; // vertical position of x axis
	double yaxis = sz.cx / 2.0; // horizontal position of y axis
	w.Move(0, xaxis).Line(sz.cx, xaxis).Stroke(1, Blue()); // draw x axis
	w.Move(yaxis, 0).Line(yaxis, sz.cy).Stroke(1, Blue()); // draw y axis
	Font font = Serif(15);
	if(pixels_per_unit > 20) // if big enough, paint some axis markers and numbers...
		for(int i = 1; i < 2 * sz.cx / pixels_per_unit; i++)
			for(int sgn = -1; sgn < 2; sgn += 2) {
				String n = AsString(sgn * i);
				Size tsz = GetTextSize(n, font);

				double x = yaxis + sgn * i * pixels_per_unit;
				w.Move(x, xaxis - 5).Line(x, xaxis + 5).Stroke(1, Blue());
				w.Text(int(x - tsz.cx / 2.0), int(xaxis + 6), n, font).Fill(Blue());

				double y = xaxis - sgn * i * pixels_per_unit;
				w.Move(yaxis - 5, y).Line(yaxis + 5, y).Stroke(1, Blue());
				w.Text(int(yaxis + 6), int(y - tsz.cy / 2.0), n, font).Fill(Blue());
			}
	Vector<String> xp = Split(~~expression, ';'); // get individual functions
	for(int ii = 0; ii < xp.GetCount(); ii++) {
		double y0 = Null; // store previous value
		for(int i = 0; i < sz.cx; i++) { // now iterate through all x pointes and draw the graph
			double x = (i - sz.cx / 2.0) / pixels_per_unit;
			double y = Evaluate(xp[ii], x);
			if(!IsNull(y)) {
				double gy = sz.cy / 2.0 - y * pixels_per_unit;
				if(IsNull(y0)) // previous value was defined
					w.Move(i, gy);
				else
					w.Line(i, gy);
			}
			y0 = y;
		}
		w.Stroke(1, Color(!!(ii & 1) * 150, !!(ii & 2) * 150, !!(ii & 4) * 150)); // finally paint the graph line
	}
}

GUI_APP_MAIN
{
	FnGraph().Run();
}
