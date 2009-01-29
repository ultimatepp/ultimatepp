#include "ScanLine.h"

ScanLine And0(const ScanLine& a, const ScanLine& b)
{
	ScanLine r;
	r.len = 0;
	r.x = b.x;
	byte aval = 0;
	int  acount = 0;
	byte bval = 0;
	int  ai = 0;
	int  bi = 0;
	int  bcount = b.x - a.x;
	while(bcount) {
		if(ai >= a.data.GetCount())
			return r;
		aval = a.data[ai++];
		if(aval > 128) {
			acount = aval - 128;
			aval = a.data[ai++];
			int n = min(acount, bcount);
			acount -= n;
			bcount -= n;
		}
		else
			bcount--;
	}
	while(ai < a.data.GetCount() && bi < b.data.GetCount()) {
		if(acount == 0) {
			aval = a.data[ai++];
			if(aval > 128) {
				acount = aval - 128;
				aval = a.data[ai++];
			}
		}
		if(bcount == 0) {
			bval = b.data[bi++];
			if(bval > 128) {
				bcount = bval - 128;
				bval = b.data[bi++];
			}
		}
		if(acount && bcount) {
			int l = min(acount, bcount);
			acount -= l;
			bcount -= l;
			r.len += l;
			while(l) {
				int n = min(l, 127);
				r.data.Cat(n + 128);
				r.data.Cat((aval * bval) >> 7);
				l -= n;
			}
		}
		else {
			r.data.Cat((aval * bval) >> 7);
			r.len++;
			if(acount) acount--;
			if(bcount) bcount--;
		}
	}
	return r;
}

ScanLine And(const ScanLine& a, const ScanLine& b)
{
	return a.x < b.x ? And0(a, b) : And0(b, a);
}

ScanLine Pack(int x, const byte *data, int len)
{
	ScanLine r;
	int i = 0;
	r.x = x;
	r.len = 0;
	while(i < len) {
		int n = i;
		byte val = data[i++];
		while(i < len && data[i] == val)
			i++;
		n = i - n;
		if(n > 1) {
			r.len += n;
			while(n) {
				int q = min(n, 127);
				r.data.Cat(q + 128);
				r.data.Cat(val);
				n -= q;
			}
		}
		else {
			r.data.Cat(val);
			r.len++;
		}
	}
	return r;
}

String ScanLine::ToString() const
{
	String s;
	s << "x = " << x << ", len = " << len << ", right = " << len + x << ": ";
	for(int i = 0; i < data.GetCount(); i++) {
		byte val = data[i];
		if(val > 128) {
			int n = val - 128;
			val = data[++i];
			while(n--)
				s << (int)val << ", ";
		}
		else
			s << (int)val << ", ";
	}
	s << "datalen: " << data.GetCount();
	return s;
}

void Apply(RGBA *t, int len, const RGBA& color, const ScanLine& s)
{
	RGBA *e = t + len;
	const char *q = ~s.data;
	const char *qe = s.data.End();
	t += s.x;
	while(t < e && q < qe) {
		byte val = *q++;
		if(val > 128) {
			RGBA *e1 = min(e, t + val - 128);
			val = *q++;
			while(t < e1)
				AlphaBlendCover7(*t++, color, val);
		}
		else
			AlphaBlendCover7(*t++, color, val);
//		DDUMP(t - e);
	}
}
