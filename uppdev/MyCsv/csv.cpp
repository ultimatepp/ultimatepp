#include "csv.h"

Vector<Value> CSVStringToToken(const String &s) {
	int i, e=0, t=0;
	Vector<Value> item;
	
	bool d = false;
	for(i=0; i<=s.GetCount(); i++) {
		if(s.Mid(i, 1) == "\"" && s.Mid(i, 2) != "\"\"" && s.Mid(i-1, 2) != "\"\"") {
			d ? d=false : d=true;
		}
		if(!d) {
			if(s.Mid(i, 1) == ";" || i==s.GetCount()) {
				e=i;
				if(IsQuoted(s.Mid(t, e-t))) item.Add(CSVDequote(s.Mid(t, e-t)));
				//else item.Add(MyScanDouble(s.Mid(t, e-t)));
				else item.Add(s.Mid(t, e-t));
				t = e+1;
			}
		}
	}
	return item;
}

String CSVQuote(const String &s) {
	String str(s);
	int i;

	for(i=0; i<str.GetCount(); i++) {
		if(str.Mid(i, 1) == "\"") {
			str.Insert(i, '\"');
			i++;
		}
	}
	if(!IsQuoted(str)) {
		str.Insert(0, '\"');
		str.Cat('\"');
	}
	return str;
}

String CSVDequote(const String &s) {
	//String str(s);
	String str;
	int i;

	if(s.Left(1) == "\"" && s.Right(1) == "\"") {
		if(s.GetCount() == 2) str = "";
		else str = s.Mid(1, s.GetLength()-2);	
	}
	
	for(i=0; i<str.GetCount(); i++) {
		if(str.Mid(i, 2) == "\"\"") {
			str.Remove(i);
		}
	}
	return str;
}
