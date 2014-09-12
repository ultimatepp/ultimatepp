#ifdef flagGUI

#include <CtrlLib/CtrlLib.h>

#include "Functions4U.h"
#include "Functions4U_Gui.h"

NAMESPACE_UPP

inline bool iscidplus(int c) {
	return iscib(c) || c == '.' || c >= '0' && c <= '9';
}

String CParserPlus::ReadIdPlus() throw(Error) {
	if(!IsId())
		ThrowError("missing id");
	String result;
	const char *b = term;
	const char *p = b;
	while(iscidplus(*p))
		p++;
	term = p;
	DoSpaces();
	return String(b, (int)(uintptr_t)(p - b));
}

Drawing EquationDraw::Text(String text, bool italic, int offsetX, int offsetY, double betw) {
	Font fnt;
	
	int _pos1, _pos2;
	if ((_pos1 = text.Find('_', 0)) >= 0) {
		String sub = text.Right(text.GetCount()-_pos1-1);
		if ((_pos2 = sub.Find('_', 0)) >= 0)
			sub = sub.Left(_pos2) + ',' + sub.Right(sub.GetCount()-_pos2-1);
		return SubSup(text.Left(_pos1), sub, ""); 
	}
	fnt.Face(Font::ROMAN);
	fnt.Height(400); 
	fnt.Italic(italic);
	
	FontInfo fi = fnt.Info();
	int width = 0;
	WString wtext(text);
	for (int i = 0; i < wtext.GetCount(); ++i) 
		width += fi.GetWidth(wtext[i]);
	
	width += 10;
	width = int(width*betw);
	ImageDraw idw(width, fnt.GetCy());
	idw.DrawRect(Size(width, fnt.GetCy()), White);
	idw.DrawText(offsetX, offsetY, text, fnt);

	DrawingDraw dw(width/3, fnt.GetCy()/3);
	dw.DrawImage(0, 0, width/3, fnt.GetCy()/3, idw);
	
	return dw;
}

Drawing EquationDraw::SubSup(Drawing &drwText, Drawing &drwSub, Drawing &drwSup) {
	Size szText = drwText.GetSize();
	Size szSub  = (2/3.)*drwSub.GetSize();
	Size szSup  = (2/3.)*drwSup.GetSize();
	
	int width = szText.cx + max(szSup.cx, szSub.cx);
	int deltay = szSup.cy/2;
	
	DrawingDraw dw(width, szText.cy + szSup.cy/2 + szSub.cy/2);
	
	dw.DrawDrawing(0, 		  deltay, 			  			   szText.cx, szText.cy, drwText);
	dw.DrawDrawing(szText.cx, 0, 			 	  			   szSup.cx,  szSup.cy,  drwSup);
	dw.DrawDrawing(szText.cx, deltay + szText.cy - szSub.cy/2, szSub.cx,  szSub.cy,  drwSub);
	
	return dw;
}

Drawing EquationDraw::SubSup(String text, String sub, String sup) {
	Drawing textdraw = Text(text, true);
	Drawing subdraw  = Text(sub, true);
	Drawing supdraw  = Text(sup, true);
	
	return SubSup(textdraw, subdraw, supdraw);
}

Drawing EquationDraw::SubSup(Drawing &drwText, String sub, String sup) {
	Drawing subdraw  = Text(sub, true);
	Drawing supdraw  = Text(sup, true);
	
	return SubSup(drwText, subdraw, supdraw);
}

Drawing EquationDraw::SubSupInv(Drawing &drwText, Drawing &drwSub, Drawing &drwSup)
{
	double factor = 0.6;
	
	Size szText = drwText.GetSize();
	Size szSub  = factor*drwSub.GetSize();
	Size szSup  = factor*drwSup.GetSize();
	
	int width = szText.cx + max(szSup.cx, szSub.cx);
	int deltay = szSup.cy/2;
	
	DrawingDraw dw(width, szText.cy + szSup.cy/2 + szSub.cy/2);
	
	dw.DrawDrawing(width-szText.cx, deltay, 			  			 szText.cx, szText.cy, drwText);
	dw.DrawDrawing(0, 		  		0, 			 	  			     szSup.cx,  szSup.cy,  drwSup);
	dw.DrawDrawing(0, 		  		deltay + szText.cy - szSub.cy/2, szSub.cx,  szSub.cy,  drwSub);
	
	return dw;
}

Drawing EquationDraw::SubSupInv(String text, String sub, String sup) {
	Drawing textdraw = Text(text, true);
	Drawing subdraw  = Text(sub, true);
	Drawing supdraw  = Text(sup, true);
	
	return SubSupInv(textdraw, subdraw, supdraw);
}

Drawing EquationDraw::SubSupInv(Drawing &drwText, String sub, String sup) {
	Drawing subdraw  = Text(sub, true);
	Drawing supdraw  = Text(sup, true);
	
	return SubSupInv(drwText, subdraw, supdraw);
}

Drawing EquationDraw::JoinCenter(Drawing &left, Drawing &right) {
	Size szLeft = left.GetSize();
	Size szRight = right.GetSize();
	
	int width = szLeft.cx + szRight.cx;
	int height = max(szLeft.cy, szRight.cy);
	
	DrawingDraw dw(width, height);
	
	dw.DrawDrawing(0, 		  height == szLeft.cy?  0: (height-szLeft.cy)/2,  szLeft.cx,  szLeft.cy, left);
	dw.DrawDrawing(szLeft.cx, height == szRight.cy? 0: (height-szRight.cy)/2, szRight.cx, szRight.cy, right);
	
	return dw;
}

Drawing EquationDraw::JoinFlex(Drawing &left, double betw1, Drawing &right, double betw2) {
	Size szLeft = left.GetSize();
	Size szRight = right.GetSize();
	
	int width = int(szLeft.cx*betw1 + szRight.cx*betw2);
	int height = max(szLeft.cy, szRight.cy);
	
	DrawingDraw dw(width, height);
	
	dw.DrawDrawing(0, 		        0, szLeft.cx,  height, left);
	dw.DrawDrawing(int(szLeft.cx*betw1), 0, szRight.cx, height, right);
	
	return dw;
}

Drawing EquationDraw::NumDenom(Drawing &num, Drawing &denom) {
	Size szNum = num.GetSize();
	Size szDenom = denom.GetSize();
	
	int width = max(szNum.cx, szDenom.cx);
	int height = szNum.cy + szDenom.cy; 
	
	DrawingDraw dw(width, height);
	
	dw.DrawDrawing(width == szNum.cx?   0: (width-szNum.cx)/2, 	 0, 		szNum.cx,  szNum.cy, num);
	dw.DrawDrawing(width == szDenom.cx? 0: (width-szDenom.cx)/2, szNum.cy, szDenom.cx, szDenom.cy, denom);
	dw.DrawLine(0, szNum.cy, width, szNum.cy, 1);
	
	return dw;
}

Drawing EquationDraw::Bracket(Drawing &data) {
	Drawing pizq = Text("(", true, 0, 0, 1.3);
	Drawing pder = Text(")");
	Drawing parizq = JoinFlex(pizq, 1, data, 1);
	
	return JoinFlex(parizq, 1, pder, 1);
}

Drawing EquationDraw::Sqrt(Drawing &right) {
	Drawing left = Text("√");
	
	Size szLeft = left.GetSize();
	Size szRight = right.GetSize();
	
	int width = 2*szLeft.cx + szRight.cx;
	int height = max(szLeft.cy, szRight.cy);
	
	DrawingDraw dw(width, height);
	
	dw.DrawDrawing(0, 		  0, szLeft.cx,  height, left);
	dw.DrawDrawing(szLeft.cx, 1, szRight.cx, height-1, right);
	dw.DrawLine(szLeft.cx,    1, width-szLeft.cx, 1, 1);
	
	return dw;	
}

Drawing EquationDraw::Exponent(Drawing &right) {
	Drawing left = Text("e");
	
	return EquationDraw::Exp(left, right);
}

Drawing EquationDraw::Der(Drawing &data) {
	return EquationDraw::Function("d", data);
}

Drawing EquationDraw::Abs(Drawing &data) {
	Drawing pizq = Text("|", true, 0, 0, 1.3);
	Drawing pder = Text("|");
	Drawing parizq = JoinFlex(pizq, 1, data, 1);
	
	return JoinFlex(parizq, 1, pder, 1);
}

Drawing EquationDraw::Integral(Drawing &data, Drawing &sub, Drawing &sup) {
	Drawing left = Text("∫");
	Drawing right = SubSupInv(data, sub, sup);
	
	Size szLeft = left.GetSize();
	Size szRight = right.GetSize();
	
	int sqWidth = int(szLeft.cx*1.5);
	int width = sqWidth + szRight.cx;
	int height = szRight.cy;
	
	DrawingDraw dw(width, height);
	
	dw.DrawDrawing(0, 		0, (int)(szLeft.cx*1.5),  height, left);
	dw.DrawDrawing(sqWidth, 0, szRight.cx, 			  height, right);
	
	return dw;	
}

Drawing EquationDraw::Summat(Drawing &data, Drawing &sub, Drawing &sup) {
	Drawing left = Text("∑", true, 0, -20);
	Drawing right = SubSupInv(data, sub, sup);
	
	Size szLeft = left.GetSize();
	Size szRight = right.GetSize();
	
	int sqWidth = int(szLeft.cx*1.3);
	int width = sqWidth + szRight.cx;
	int height = szRight.cy;
	
	DrawingDraw dw(width, height);
	
	dw.DrawDrawing(0, 0, (int)(szLeft.cx*1.1),  height, left);
	dw.DrawDrawing(sqWidth, 0, szRight.cx, height, right);
	
	return dw;
}
Drawing EquationDraw::Exp(Drawing &data, Drawing &exp) {
	Drawing foo = Text("");
	
	return SubSup(data, foo, exp);
}

Drawing EquationDraw::Function(String function, Drawing &content) {
	Drawing fundraw = Text(function);
	Drawing pardraw = Bracket(content);
	
	return JoinCenter(fundraw, pardraw);
}

Drawing EquationDraw::Equal(Drawing &left, Drawing &right) {
	Drawing equal = Text(" = ");
	Drawing leftdraw = JoinCenter(left, equal);
	
	return JoinCenter(leftdraw, right);
}

String EquationDraw::ReplaceSymbols(String var) {
	for (int i = 0; i < symbols.GetCount(); ++i) {
		String letter = ToLower(symbols.GetKey(i));
		if (var.Find(letter) >= 0)
			var.Replace(letter, symbols[i].Mid(2));
		letter = ToUpper(symbols.GetKey(i));
		if (var.Find(letter) >= 0)
			var.Replace(letter, symbols[i].Mid(0, 2));		
	}
	return var;	    
}

String EquationDraw::TermTrig(CParserPlus& p) {
	if (p.Id("sin"))	return "sin";
	if (p.Id("cos"))	return "cos";
	if (p.Id("tan"))	return "tan";
	if (p.Id("sinh"))	return "sinh";
	if (p.Id("cosh"))	return "cosh";
	if (p.Id("tanh"))	return "tanh";
	if (p.Id("asin"))	return "asin";
	if (p.Id("acos"))	return "acos";
	if (p.Id("atan"))	return "atan";
	return Null;
}

Drawing EquationDraw::Term(CParserPlus& p, bool noBracket) {
	if(p.Id("integral")) {
		p.PassChar('(');
		Drawing data = Exp(p);
		p.PassChar(',');
		Drawing sub = Exp(p);
		p.PassChar(',');
		Drawing sup = Exp(p);
		p.PassChar(')');
		return EquationDraw::Integral(data, sub, sup);
	}
	if(p.Id("summation")) {
		p.PassChar('(');
		Drawing data = Exp(p);
		p.PassChar(',');
		Drawing sub = Exp(p);
		p.PassChar(',');
		Drawing sup = Exp(p);
		p.PassChar(')');
		return EquationDraw::Summat(data, sub, sup);
	}
	if(p.Id("sqrt")) {
		p.PassChar('(');
		Drawing x = Exp(p);
		p.PassChar(')');
		return EquationDraw::Sqrt(x);
	}
	if(p.Id("exp")) {
		p.PassChar('(');
		Drawing x = Exp(p);
		p.PassChar(')');
		return EquationDraw::Exponent(x);
	}
	if(p.Id("der")) {
		p.PassChar('(');
		Drawing x = Exp(p);
		p.PassChar(')');
		return EquationDraw::Der(x);
	}
	if(p.Id("abs")) {
		p.PassChar('(');
		Drawing x = Exp(p);
		p.PassChar(')');
		return EquationDraw::Abs(x);
	}
	String trig = TermTrig(p);
	if (!IsNull(trig)) {
		p.PassChar('(');
		Drawing x = Exp(p);
		p.PassChar(')');
		return EquationDraw::Function(trig, x);
	}
	if(p.IsId()) 
		return EquationDraw::Text(ReplaceSymbols(p.ReadId()));
	if(p.Char('(')) {
		Drawing x = Exp(p);
		p.PassChar(')');
		if (noBracket) 
			return x;
		else
			return EquationDraw::Bracket(x);
	}
	if(p.Char('-')) {
		Drawing left = Text("-");
		Drawing term = EquationDraw::Term(p);
		return EquationDraw::JoinCenter(left, term);
	}
	return EquationDraw::Text(FormatDouble(p.ReadDouble()));
}
	
Drawing EquationDraw::Mul(CParserPlus& p) {
	Drawing x = Term(p);
	for(;;) {	
		if(p.Char('*')) {
			Drawing mult = EquationDraw::Text(" ");
			Drawing left = EquationDraw::JoinCenter(x, mult);
			Drawing y = Term(p, false);
			x = EquationDraw::JoinCenter(left, y);
		} else if(p.Char('/')) {
			Drawing y = Term(p, true);
			x = EquationDraw::NumDenom(x, y);
		} else if(p.Char('^')) {
			Drawing y = Term(p, false);
			x = EquationDraw::Exp(x, y);
		} else
			return x;
	}
}
	
Drawing EquationDraw::Exp(CParserPlus& p) {
	Drawing x = Mul(p);
	for(;;) {
		if(p.Char('+')) {
			Drawing plus = EquationDraw::Text("+");
			Drawing left = EquationDraw::JoinCenter(x, plus);
			Drawing y = Mul(p);				
			x = EquationDraw::JoinCenter(left, y);
		} else if(p.Char('-')) {
			Drawing minus = EquationDraw::Text("-");
			Drawing left = EquationDraw::JoinCenter(x, minus);
			Drawing y = Mul(p);				
			x = EquationDraw::JoinCenter(left, y);
		} else if(p.Char('=')) {
			Drawing y = Mul(p);
			x = EquationDraw::Equal(x, y);
		} else
			return x;
	}
}

EquationDraw::EquationDraw() {
	symbols.GetAdd("alpha") = "Αα"; 	
	symbols.GetAdd("nu")    = "Νν";
	symbols.GetAdd("beta")  = "Ββ"; 	
	symbols.GetAdd("xi")    = "Ξξ"; 	
	symbols.GetAdd("gamma") = "Γγ"; 	 	
	symbols.GetAdd("omicron") = "Οο"; 	
	symbols.GetAdd("delta") = "Δδ"; 	 	
	symbols.GetAdd("pi") 	= "Ππ"; 	
	symbols.GetAdd("epsilon") = "Εε"; 	
	symbols.GetAdd("rho")   = "Ρρ"; 	
	symbols.GetAdd("zeta") 	= "Ζζ"; 	 	
	symbols.GetAdd("sigma") = "Σσ"; 	
	symbols.GetAdd("eta") 	= " Ηη"; 		
	symbols.GetAdd("tau") 	= "Ττ"; 	
	symbols.GetAdd("theta") = "Θθ"; 	 	
	symbols.GetAdd("upsilon") = "Υυ"; 	
	symbols.GetAdd("iota") 	= "Ιι"; 	 	
	symbols.GetAdd("phi") 	= "Φφ"; 	
	symbols.GetAdd("kappa") = "Κκ"; 	 	
	symbols.GetAdd("chi") 	= "Χχ"; 	 	
	symbols.GetAdd("lambda") = "Λλ"; 	
	symbols.GetAdd("psi") 	= "Ψψ"; 	
	symbols.GetAdd("mu") 	= "Μμ"; 	 	
	symbols.GetAdd("omega") = "Ωω";
}

Drawing DrawEquation(const String &str) {
	EquationDraw equation;
	
	CParserPlus p(str);
	try {
		if(p.IsId()) {
			String id;
			CParser::Pos pos = p.GetPos();
			id = p.ReadId();
			p.SetPos(pos);
			return equation.Exp(p);
		} else
			return equation.Exp(p);
	}
	catch(CParser::Error e) {
		String res;
		res << "ERROR: " << e;
		return EquationDraw::Text(res);
	}
}

QtfRichObject QtfEquation(const String &str) {
	Drawing drw = DrawEquation(str);
	
	Size sz = drw.GetSize();
	DrawingDraw dw(sz);
	dw.DrawDrawing(0, 0, sz.cx, sz.cy, drw);
	
	return QtfRichObject(CreateDrawingObject(dw.GetResult(), sz, sz));
}

END_UPP_NAMESPACE

#endif

