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
	s << "datalen: " << data.GetCount() << "x = " << x << ", len = " << len << ", right = " << len + x << ": ";
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
	return s;
}

void Apply(RGBA *t, int len, const RGBA& c, const ScanLine& s)
{
	PAINTER_TIMING("Apply");
	RGBA *e = t + len;
	const char *q = ~s.data;
	const char *qe = s.data.End();
	t += s.x;
	while(t < e && q < qe) {
		byte val = *q++;
		if(val > 128) {
			RGBA *e1 = min(e, t + val - 128);
			byte val = *q++;
			if(val == 128 && c.a == 255) {
				int l = e1 - t;
				while(l >= 16) {
					t[0] = c; t[1] = c; t[2] = c; t[3] = c;
					t[4] = c; t[5] = c; t[6] = c; t[7] = c;
					t[8] = c; t[9] = c; t[10] = c; t[11] = c;
					t[12] = c; t[13] = c; t[14] = c; t[15] = c;
					t += 16;
					l -= 16;
				}
				switch(l) {
				case 15: t[14] = c;
				case 14: t[13] = c;
				case 13: t[12] = c;
				case 12: t[11] = c;
				case 11: t[10] = c;
				case 10: t[9] = c;
				case 9: t[8] = c;
				case 8: t[7] = c;
				case 7: t[6] = c;
				case 6: t[5] = c;
				case 5: t[4] = c;
				case 4: t[3] = c;
				case 3: t[2] = c;
				case 2: t[1] = c;
				case 1: t[0] = c;
				}
				t = e1;
			}
			else {
				RGBA c1;
				if(val != 128)
					c1 = Mul7(c, val);
				else
					c1 = c;
				while(t < e1)
					AlphaBlend(*t++, c1);
			}
		}
		else
			AlphaBlendCover7(*t++, c, val);
//		DDUMP(t - e);
	}
}

struct SolidFiller {
	RGBA *t;
	RGBA c;
	
	void Start(int x) { t += x; }
	void Render(byte val) { AlphaBlendCover7(*t++, c, val); } 
	void Render(byte val, int len);
};

void SolidFiller::Render(byte val, int len)
{
	if(((val - 128) | (c.a - 255)) == 0) {
		while(len >= 16) {
			t[0] = c; t[1] = c; t[2] = c; t[3] = c;
			t[4] = c; t[5] = c; t[6] = c; t[7] = c;
			t[8] = c; t[9] = c; t[10] = c; t[11] = c;
			t[12] = c; t[13] = c; t[14] = c; t[15] = c;
			t += 16;
			len -= 16;
		}
		switch(len) {
		case 15: t[14] = c;
		case 14: t[13] = c;
		case 13: t[12] = c;
		case 12: t[11] = c;
		case 11: t[10] = c;
		case 10: t[9] = c;
		case 9: t[8] = c;
		case 8: t[7] = c;
		case 7: t[6] = c;
		case 6: t[5] = c;
		case 5: t[4] = c;
		case 4: t[3] = c;
		case 3: t[2] = c;
		case 2: t[1] = c;
		case 1: t[0] = c;
		}
		t += len;
	}
	else {
		RGBA c1;
		if(val != 128)
			c1 = Mul7(c, val);
		else
			c1 = c;
		RGBA *e = t + len;
		while(t < e)
			AlphaBlend(*t++, c1);
	}
}

void Render(ImageBuffer& ib, Rasterizer& r, const RGBA& color)
{
	Size sz = ib.GetSize();
	SolidFiller f;
	f.c = color;
	for(int y = r.MinY(); y <= r.MaxY(); y++) {
		f.t = ib[y];
		r.Render(y, f);
	}
}
