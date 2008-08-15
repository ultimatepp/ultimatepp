#include "dbase.h"

bool _isBitSet(int _byte, short _bitno) {
	int val;
	
	val = 1 << _bitno;
	return (_byte & val) >> _bitno;
}

void _bitSet(int *ptr, short _bitno) {
	int val;
	
	val = 1 << _bitno;
	*ptr |= val;
	return;
}

void _bitReset(int *ptr, short _bitno) {
	int val;
	
	val = 1 << _bitno;
	*ptr &= ~val;
	return;
}

Vector<String> StringToToken(const String &str) {
	Vector<String> tkn;

	int p, s=0, e=0;
	for(p=0; p<=str.GetCount(); p++) {
		int h=0, b=0;
		while(((str.Mid(p, 1) != "," && str.Mid(p, 1) != " ") || h > 0 || b > 0) && p < str.GetCount()) {
			if(str.Mid(p, 1) == "\"" && h == 0) h++;
			else if(str.Mid(p, 1) == "\"" && h > 0) h--;
			else if(str.Mid(p, 1) == ")") b--;
			else if(str.Mid(p, 1) == "(") b++;
			p++;
		}
		e=p;
		if(str.Mid(s, e-s).GetCount() > 0) {
			if(tkn.GetCount() > 0) {
				if(str.Mid(s, e-s) == "=" || str.Mid(s, e-s) == "<" || str.Mid(s, e-s) == ">" || str.Mid(s, e-s) == "<=" || str.Mid(s, e-s) == ">=") {
					tkn.Top().Cat(str.Mid(s, e-s));
				}
				else if(tkn.Top().Right(1) == "=" || tkn.Top().Right(1) == "<" || tkn.Top().Right(1) == ">") {
					tkn.Top().Cat(str.Mid(s, e-s));
				}
				else {
					tkn.Add(str.Mid(s, e-s));
				}
			}	
			else {
				tkn.Add(str.Mid(s, e-s));
			}
#ifdef	_WITH_DEBUG			
RLOG("StringToToken(): " + tkn.Top());
#endif
		}
		s=e+1;
	}
	return tkn;
}

bool lmlEval(String eqt) {
	unsigned int i, l, m, s, e;
	String a, b;
	bool j = true;
	
	a = eqt;
	while(j) {
		l = 0; m = 0;
		for(i=0; i<a.GetLength(); i++) {
			if(strcmp(a.Mid(i, 1), "(") == 0) l++;
			if(strcmp(a.Mid(i, 1), ")") == 0) l--;
			if(l > m) m = l;
		}
		s = 1; e = a.GetLength()-2; l = 0;
		bool f = false;
		for(i=0; i<a.GetLength(); i++) {
			if(strcmp(a.Mid(i, 1), "(") == 0) l++;
			if(l == m && !f) { s = i+1; f = true; }
			if(strcmp(a.Mid(i, 1), ")") == 0) l--;
			if(l < m && f) { e = i-s;	f = false; }
		}
		if(m > 0) b = a.Mid(s, e);
		else b = a;
		
		if(lplEval(b) == true) {
			a.Remove(s-1, e+2);
			a.Insert(s-1, "T");
		}
		else {
			a.Remove(s-1, e+2);
			a.Insert(s-1, "F");
		}
#ifdef		_WITH_DEBUG	
		RLOG(a);
#endif		
		if(m < 1) j = false;
	}
	if(strcmp(a.Mid(0, 1), "T") == 0) return true;
	return false;
}

bool lplEval(String eqt) {
	Vector<String> token;
	unsigned int i;
	
	token = StringToToken(eqt);
#ifdef		_WITH_DEBUG	
	for(i=0; i<token.GetCount(); i++) RLOG(token.At(i));
#endif

	Array<bool> x;
	Array<String> lo;
	bool z;
	unsigned int k;
	k = 0;
	x.Add(false);
	for(i=0; i<token.GetCount(); i++)	{
		if(strcmp(token.At(i), "NOT") == 0) x.At(k) = !x.At(k);
		else if(strcmp(token.At(i), "T") == 0) x.At(k) = !x.At(k);
		else if(strcmp(token.At(i), "AND") == 0 || strcmp(token.At(i), "OR") == 0 || strcmp(token.At(i), "NAND") == 0 || strcmp(token.At(i), "NOR") == 0) {
			lo.Add(token.At(i));
			x.Add(false);
			k++;
		}
	}
#ifdef		_WITH_DEBUG	
	for(i=0; i<x.GetCount(); i++) RLOG(FormatBool(x.At(i)));
	for(i=0; i<lo.GetCount(); i++) RLOG(lo.At(i));
#endif
	z = x.At(0);
	for(i=0; i<lo.GetCount(); i++) {
		if(strcmp(lo.At(i), "AND") == 0) {
			z = z & x.At(i+1);
		}
		if(strcmp(lo.At(i), "OR") == 0) {
			z = z | x.At(i+1);
		}
		if(strcmp(lo.At(i), "NAND") == 0) {
			z = !(z & x.At(i+1));
		}		
		if(strcmp(lo.At(i), "NOR") == 0) {
			z = !(z | x.At(i+1));
		}		
	}
#ifdef		_WITH_DEBUG	
	RLOG(FormatBool(z));
#endif	
	return z;
}

bool IsSQLWord(String &_w) {
	bool r;
	
	r = false;
	if(_w == "SELECT" || _w == "FROM" || _w == "WHERE" || _w == "GROUP" || _w == "ORDER" || _w == "BY" || _w == "LIMIT" || _w == "HAVING" || _w == "UPDATE" || _w == "INSERT") r = true;
	
	return r;
}

bool EvalWhere(dbRecord &rec1, const String &whr) {
	int f, a, j;

	// creo la lista delle condizioni where
	String fd, opr, val;
	String eqt, temp;
	unsigned int s, len;
	bool fg = false, seg;
	bool validRecord = false;
	if(whr.GetLength() > 0) { // se c'è condizione WHERE
		// copio la condizione where
		eqt = whr;
		f = 0;
		// inizio a convertire i songoli token di confronto
		while(f<eqt.GetLength()) {
			if(*((char*)~eqt.Mid(0, 1)) == '(') {
				for(f; f<eqt.GetLength(); f++) {
					seg = false;
					char c = *((char*)~eqt.Mid(f, 1));
					if(c != '(' && c != ')' && c != ' ') {
						if(!fg) {
							s = f;
							fg = true;
						}
					}
					else if(fg) {
						fg = false;
						len = f-s;
						seg = true;
						break;
					}
				}
			}
			else {
				seg = true;
				f = eqt.GetLength();
				s = 0;
				len = f;
			}
			if(seg) {
				temp = eqt.Mid(s, len);
				if((strcmp((char*)~temp, "T") != 0) && (strcmp((char*)~temp, "F") != 0) && (strcmp((char*)~temp, "AND") != 0) && (strcmp((char*)~temp, "OR") != 0) && (strcmp((char*)~temp, "NOT") != 0) && (strcmp((char*)~temp, "NAND") != 0) && (strcmp((char*)~temp, "NOR") != 0)) {
					a = strcspn((char*)~temp, "=<>");
					fd = temp.Left(a);
					j = strspn((char*)~temp.Mid(a), "=<>");
					opr = temp.Mid(a, j);
					val = RemoveQuote(temp.Mid(j+a));

					if(strcmp((char*)~opr, "=") == 0) {
						if(IsEqual(rec1.GetValue(fd), val)) {
							temp = "T";
						}
						else temp = "F";
					}
					else if(strcmp((char*)~opr, "<") == 0) {
						if(IsSmaller(rec1.GetValue(fd), val)) {
							temp = "T";
						}
						else temp = "F";
					}
					else if(strcmp((char*)~opr, ">") == 0) {
						if(IsBigger(rec1.GetValue(fd), val)) {
							temp = "T";
						}
						else temp = "F";
					}
					else if(strcmp((char*)~opr, "<=") == 0) {
						if(IsSmallerEqual(rec1.GetValue(fd), val)) {
							temp = "T";
						}
						else temp = "F";
					}
					else if(strcmp((char*)~opr, ">=") == 0) {
						if(IsBiggerEqual(rec1.GetValue(fd), val)) {
							temp = "T";
						}
						else temp = "F";
					}
					else temp = "F";
						
					eqt.Remove(s, len);
					eqt.Insert(s, temp);
					++f -= len;
				}
			}
		}
		// token di confronto convertiti, ora la stringa e' in formato (T AND F) ...
		// valuto il risultato della condizione
		if(lmlEval(eqt)) {
			validRecord = true;
		}
	}
	else { // altrimenti senza WHERE
		validRecord = true;
	}
	
	return validRecord;
}

VectorMap<int, Vector<Value> > StringToVector(const String &s) {
		
	StringStream ss(s);
	VectorMap<int, Vector<Value> > m;
	ss.SetLoading();
	m.Serialize(ss);
	//LoadFromString(m, s);
	return m;
}

String VectorToString(VectorMap<int, Vector<Value> > &v) {
	
	StringStream ss;
	//ss.Create();
	ss.SetStoring();
	v.Serialize(ss);
	return ss.GetResult();
	//return StoreAsString(v);
}
