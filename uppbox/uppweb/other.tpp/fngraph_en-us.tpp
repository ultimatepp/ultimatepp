topic "Introduction";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0; Introduction&]
[s0; &]
[s0; Descend parsing a well known simple approach to evaluating complex 
syntaxes. In this article we shall create a simple descend parser 
capable of evaluating mathematical expressions, using U`+`+ CParser 
class for lexical analysis. And to have some fun, we shall add 
an interactive level to render a graph of function.&]
[s0; &]
[s0; Descend parsing basics&]
[s0; &]
[s0; Descend parsing usually works by providing a function for the 
each level of syntax. If evaluating expressions, our main concern 
usually is to maintain the priority of operators. In descent 
parser, each priority level is represented by single function. 
Usually, expressions can represented as addition/subtraction 
of terms (descend level), terms are formed by multiplying or 
dividing exponentials (descent level), exponentials combine factors 
(descend level). Factors usually represent the final descend 
level. Expression in parenthesis is considered a factor and resolved 
using recursion.&]
[s0; Usually, language definitions provide some formal syntax graphs 
or definitions, however in this simple example, shall avoid this, 
as you will see properly coded descent parser is very self`-explaining 
to the point of serving as syntax definition.&]
[s0; &]
[s0; Lexical parsing with CParser&]
[s0; &]
[s0; Most syntax parsing systems employ `'two`-level`' parsing architecture, 
with lower level `'lexical`' parser used to resolve item like 
numbers or identifiers. U`+`+ framework provides a handy lexical 
parser class CParser for parsing syntax that has similar lexical 
rules of identifiers and numbers to C language. CParser operates 
on a stream of zero`-terminated characters (const char `*). Fundamental 
functionality are tests on existence of various lexical elements, 
usually also in `"advance if true`" variant. For example, Char 
method returns true if input position is on specified character 
and advances position to the next element. Whitespaces are automatically 
skipped in default mode. Some of methods throw CParser`::Exc exception 
in case of failure.&]
[s0; &]
[s0; Expression descend parser&]
[s0; &]
[s0; We shall use a helper processing class&]
[s0; &]
[s0; struct ExpressionEvaluator `{&]
[s0; -|CParser p; // this will hold our lexical context&]
[s0; -|double  x; // `'variable`'&]
[s0; &]
[s0; -|double Factor();&]
[s0; -|double Exponent();&]
[s0; -|double Term();&]
[s0; -|double Expression();&]
[s0; `};&]
[s0; &]
[s0; so that we do not have to pass CParser and our function variable 
x into each method as parameters. We shall start with Expression 
descend level:&]
[s0; &]
[s0; double ExpressionEvaluator`::Expression()&]
[s0; `{ // resolve `+ `-&]
[s0; -|double x `= Term(); // at least one term&]
[s0; -|for(;;)&]
[s0; -|-|if(p.Char(`'`+`'))&]
[s0; -|-|-|x `= x `+ Term(); // add another term&]
[s0; -|-|else&]
[s0; -|-|if(p.Char(`'`-`'))&]
[s0; -|-|-|x `= x `- Term(); // subtract another term&]
[s0; -|-|else&]
[s0; -|-|-|return x; // no more `+ `- operators&]
[s0; `}&]
[s0; &]
[s0; what we say here is quite simple: Expression is a list of at 
least one term, separated by operators `+ or `-. Of course, as 
our aim is to evaluate the value of expression, we perform required 
calculations (adding or subtracting) are needed.&]
[s0; &]
[s0; Next two levels (Term, Exponent) are quite similar:&]
[s0; &]
[s0; double ExpressionEvaluator`::Term()&]
[s0; `{ // resolve `* /&]
[s0; -|double x `= Exponent(); // at least one member&]
[s0; -|for(;;)&]
[s0; -|-|if(p.Char(`'`*`'))&]
[s0; -|-|-|x `= x `* Exponent(); // multiply by another member&]
[s0; -|-|else&]
[s0; -|-|if(p.Char(`'/`'))&]
[s0; -|-|-|x `= x / Exponent(); // divide by another member&]
[s0; -|-|else&]
[s0; -|-|-|return x; // no more `* / operators&]
[s0; `}&]
[s0; &]
[s0; double ExpressionEvaluator`::Exponent()&]
[s0; `{ // resolve power `^&]
[s0; -|double x `= Factor(); // at least one factor&]
[s0; -|for(;;)&]
[s0; -|-|if(p.Char(`'`^`'))&]
[s0; -|-|-|x `= pow(x, Factor()); // power by another factor&]
[s0; -|-|else&]
[s0; -|-|-|return x; // no more power `^ operators&]
[s0; `}&]
[s0; &]
[s0; Final level has to resolve numeric constants, unary `-, variable 
x and functions:&]
[s0; &]
[s0; double ExpressionEvaluator`::Factor()&]
[s0; `{&]
[s0; -|if(p.Char(`'`-`')) // unary `-&]
[s0; -|-|return `-Factor();&]
[s0; -|if(p.Id(`"abs`")) // some functions...&]
[s0; -|-|return fabs(Factor());&]
[s0; -|if(p.Id(`"sqrt`"))&]
[s0; -|-|return sqrt(Factor());&]
[s0; -|if(p.Id(`"ln`"))&]
[s0; -|-|return log(Factor());&]
[s0; -|if(p.Id(`"log`"))&]
[s0; -|-|return log10(Factor());&]
[s0; -|if(p.Id(`"log2`"))&]
[s0; -|-|return log2(Factor());&]
[s0; -|if(p.Id(`"sin`"))&]
[s0; -|-|return sin(Factor());&]
[s0; -|if(p.Id(`"cos`"))&]
[s0; -|-|return cos(Factor());&]
[s0; -|if(p.Id(`"tan`"))&]
[s0; -|-|return tan(Factor());&]
[s0; -|if(p.Id(`"asin`"))&]
[s0; -|-|return asin(Factor());&]
[s0; -|if(p.Id(`"acos`"))&]
[s0; -|-|return acos(Factor());&]
[s0; -|if(p.Id(`"atan`"))&]
[s0; -|-|return atan(Factor());&]
[s0; -|if(p.Id(`"x`")) // our variable&]
[s0; -|-|return x;&]
[s0; -|if(p.Id(`"e`")) // e constant&]
[s0; -|-|return M`_E;&]
[s0; -|if(p.Id(`"pi`")) // pi constant&]
[s0; -|-|return M`_PI;&]
[s0; -|if(p.Char(`'(`')) `{ // resolve parenthesis `- recurse back 
to Sum (`+ `- operators)&]
[s0; -|-|double x `= Expression();&]
[s0; -|-|p.PassChar(`')`'); // make sure there is closing parenthesis&]
[s0; -|-|return x;&]
[s0; -|`}&]
[s0; -|return p.ReadDouble(); // last possibility is that we are at 
number...&]
[s0; `}&]
[s0; &]
[s0; Interesting part here is recursion used to resolve parenthesis 
(and also arguments to functions and unary `-). PassChar method 
throws exception if required character is missing. ReadDouble 
throws exception input cannot be interpreted as floating point 
number.&]
[s0; Note: The list of functions here is tedious and it would be better 
to use a map. We are using this variant because signature of 
target functions can be different on various platforms (double 
cdecl sin(double) vs double fastcall sin(double)) and we would 
not know the correct value type for map. In production code, 
this could be resolved by adding interface functions with fixed 
signature but that is too long for the example.&]
[s0; &]
[s0; Working directly with ExpressionEvaluator would not be comfortable, 
so we add convenience function:&]
[s0; &]
[s0; double Evaluate(const char `*s, double x)&]
[s0; `{ // evaluate expression for given variable, return Null on 
any failure or out`-of`-bounds result (NaN)&]
[s0; -|ExpressionEvaluator v;&]
[s0; -|v.x `= x;&]
[s0; -|v.p.Set(s);&]
[s0; -|try `{&]
[s0; -|-|double x `= v.Expression();&]
[s0; -|-|return x >`= `-1e200 `&`& x < 1e200 ? x : Null;&]
[s0; -|`}&]
[s0; -|catch(CParser`::Error) `{`}&]
[s0; -|return Null;&]
[s0; `}&]
[s0; &]
[s0; Null is U`+`+ concept that designates null value. It is in fact 
defined as very high negative number that is by definition excluded 
from valid double value (same concept is true for int). We use 
it here to signal that either the syntax is wrong or result is 
not a valid number.&]
[s0; &]
[s0; Drawing the Graph of a function&]
[s0; &]
[s0; Now we can evaluate text expression for x, let us add some GUI. 
Window will be trivial, just single input text field and area 
to draw the graph. We shell setup the widget position manually:&]
[s0; &]
[s0; struct FnGraph : public TopWindow `{&]
[s0; -|virtual void Paint(Draw`& w);&]
[s0; &]
[s0; -|EditString expression; // function to display&]
[s0; &]
[s0; -|FnGraph();&]
[s0; `};&]
[s0; &]
[s0; FnGraph`::FnGraph()&]
[s0; `{&]
[s0; -|Title(`"Graph of a function`");&]
[s0; -|Add(expression.TopPos(0).HSizePos()); // place widget to the 
top, horizontally fill the window&]
[s0; -|expression << `[`&`] `{ Refresh(); `}; // when expression changes, 
repaint the graph&]
[s0; -|Sizeable().Zoomable(); // make the window resizable&]
[s0; `}&]
[s0; &]
[s0; The only moderately complex thing to do is to paint the graph:&]
[s0; &]
[s0; void FnGraph`::Paint(Draw`& w`_)&]
[s0; `{&]
[s0; -|Size sz `= GetSize();&]
[s0; -|DrawPainter w(w`_, sz); // Use Painter for smooth sw rendering&]
[s0; -|w.Clear(White()); // clear the background&]
[s0; -|int ecy `= expression.GetSize().cy; // query the height of 
widget&]
[s0; -|w.Offset(0, ecy); // move coordinates out of widget&]
[s0; -|sz.cy `-`= ecy; // and reduce the size&]
[s0; -|if(sz.cy < 1) // if too small, do nothing (avoid div by zero)&]
[s0; -|-|return;&]
[s0; -|sz `= sz / 2 `* 2 `- 1; // this trick will force axes to .5, 
results in sharper AA rendering&]
[s0; -|double pixels`_per`_unit `= sz.cy / 9; // we want to display 
y range `-4.5 .. 4.5&]
[s0; -|double xaxis `= sz.cy / 2.0; // vertical position of x axis&]
[s0; -|double yaxis `= sz.cx / 2.0; // horizontal position of y axis&]
[s0; -|w.Move(0, xaxis).Line(sz.cx, xaxis).Stroke(1, Blue()); // draw 
x axis&]
[s0; -|w.Move(yaxis, 0).Line(yaxis, sz.cy).Stroke(1, Blue()); // draw 
y axis&]
[s0; -|Font font `= Serif(15);&]
[s0; -|if(pixels`_per`_unit > 20) // if big enough, paint some axis 
markers and numbers...&]
[s0; -|-|for(int i `= 1; i < 2 `* sz.cx / pixels`_per`_unit; i`+`+)&]
[s0; -|-|-|for(int sgn `= `-1; sgn < 2; sgn `+`= 2) `{&]
[s0; -|-|-|-|String n `= AsString(sgn `* i);&]
[s0; -|-|-|-|Size tsz `= GetTextSize(n, font);&]
[s0; &]
[s0; -|-|-|-|double x `= yaxis `+ sgn `* i `* pixels`_per`_unit;&]
[s0; -|-|-|-|w.Move(x, xaxis `- 5).Line(x, xaxis `+ 5).Stroke(1, Blue());&]
[s0; -|-|-|-|w.Text(int(x `- tsz.cx / 2.0), int(xaxis `+ 6), n, font).Fill(Blue());&]
[s0; &]
[s0; -|-|-|-|double y `= xaxis `- sgn `* i `* pixels`_per`_unit;&]
[s0; -|-|-|-|w.Move(yaxis `- 5, y).Line(yaxis `+ 5, y).Stroke(1, Blue());&]
[s0; -|-|-|-|w.Text(int(yaxis `+ 6), int(y `- tsz.cy / 2.0), n, font).Fill(Blue());&]
[s0; -|-|-|`}&]
[s0; -|double y0 `= Null; // store previous value&]
[s0; -|for(int i `= 0; i < sz.cx; i`+`+) `{ // now iterate through 
all x pointes and draw the graph&]
[s0; -|-|double x `= (i `- sz.cx / 2.0) / pixels`_per`_unit;&]
[s0; -|-|double y `= Evaluate(`~`~expression, x);&]
[s0; -|-|if(!IsNull(y)) `{&]
[s0; -|-|-|double gy `= sz.cy / 2.0 `- y `* pixels`_per`_unit;&]
[s0; -|-|-|if(IsNull(y0)) // previous value was defined&]
[s0; -|-|-|-|w.Move(i, gy);&]
[s0; -|-|-|else&]
[s0; -|-|-|-|w.Line(i, gy);&]
[s0; -|-|`}&]
[s0; -|-|y0 `= y;&]
[s0; -|`}&]
[s0; -|w.Stroke(1, Black()); // finally paint the graph line&]
[s0; `}&]
[s0; &]
[s0; We are using Painter package to provide smooth anti`-aliased 
rendering. About the most complex part of painting the graph 
is to draw axes, the rest is simple. As this is more or less 
illustration example, we do not bother about GUI for scaling 
the and moving the origin, just draw `+/`-4.5 in y and place 
the origin into the center.&]
[s0; &]
[s0; Conclusion&]
[s0; &]
[s0; Example demonstrates how simple and clean can descend parsing 
be made if using well thought lexical layer. Similar approach 
can be applied to the most syntaxes encountered in general computing.]]