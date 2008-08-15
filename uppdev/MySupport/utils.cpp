#include <MySupport/support.h>

String pathToString(const char *p, bool last_sep) {
	String r;	

	while(*p) {
		if(*p == '\\') {
			r.Cat('\\');
			r.Cat('\\');
		}
		else
			r.Cat(*p);
		
		p++;
	}
	
	if(!last_sep)
		r.Remove(r.GetLength()-2, 2);

	return r;
}

int StringSearch(const String &string, const String& key) {
	for(int i=0; i<(string.GetLength()-key.GetLength()); i++) {
		if(string.Mid(i, key.GetLength()) == key) {
			return i;
		}
	}
	return -1;
}

int CountRecurrence(const String &s, const String &k) {
	int r=0;
	
	for(int i=0; i<(s.GetLength()-(k.GetLength()-1)); i++) {
		if(s.Mid(i, k.GetLength()) == k) {
			i += (k.GetLength()-1);
			r++;
		}
	}
	return r;
}

String RemoveQuote(const String &str) {

	if(str.Left(1) == "\"" && str.Right(1) == "\"") {
		if(str.GetCount() == 2) return String("");
		return str.Mid(1, str.GetLength()-2);	
	}
	return str;
}

bool IsQuoted(const String &s) {
	
	if(s.Left(1) == "\"" && s.Right(1) == "\"") {
		return true;	
	}
	return false;	
}

bool isBitSet(int _byte, int _bitno) {
	int val;
	
	val = 1 << _bitno;
	return (_byte & val) >> _bitno;
}

void bitSet(int *ptr, int _bitno, bool b) {
	int val = 0;
	
	if(b) {
		val = (1 << _bitno);
#ifdef _WITH_DEBUG
RLOG(Format("bitSet():val = %s", FormatUnsigned(val)));
#endif
	*ptr |= val;
	}
	else {
		val = (1 << _bitno);
#ifdef _WITH_DEBUG
RLOG(Format("bitSet():val = %s; ~val = %s", FormatUnsigned(val), FormatUnsigned(~val)));
#endif
		*ptr &= ~val;
	}
	return;
}

String GetVolumeSn(const String &vol, int len) {
	dword sn;
	
	// Win API
	if(!GetVolumeInformation(vol, NULL, 0, &sn, NULL, NULL, NULL, 0)) sn = 71511731;
#ifdef _WITH_DEBUG
RLOG("GetVolumeSn():sn = " + AsString(sn));
#endif	
	
	return String(AsString(sn)).Right(len);
}

unsigned int GetLockNumber(const String& pcID) {
	int n[] = {3, 5, 7, 11, 13, 17};
	int p;
	unsigned int r = 0;

	p = 0;
	for(int i=0; i<pcID.GetLength(); i++) {
		r += ((atoi((char*)~pcID.Mid(i, 1)) * n[p]) * 709);
		p++;
		if(p>5) p=0;
	}
	return r * 2707;
}

String MyKeyReplace(const String& raw, const String& key, const String& value) {
	int i;
	String result = raw;
	
	if((i = StringSearch(result, (String)"<<" + key + ">>"))	>= 0) {
		result.Remove(i, key.GetLength()+4);
		result.Insert(i, value);
	}
	return result;
}
