#include "LayDes.h"

static inline int sMatchLen(const char *a, const char *al, const char *b, const char *bl)
{
	const char *q = a;
	if(al - a < bl - b)
		while(a < al && *a == *b) {
			a++;
			b++;
		}
	else
		while(b < bl && *a == *b) {
			a++;
			b++;
		}
	return int(a - q);
}

unsigned HashPos(byte *s)
{
	return s[0] + (s[1] << 8) + (s[2] << 16) + (s[3] << 24);
}

void DiffPacker::SetMaster(const String& m)
{
	master = m;
	mapn = master.GetLength() | 5;
	prev.Alloc(master.GetLength(), -1);
	map.Alloc(mapn, -1);
	for(int i = master.GetLength() - 4; i >= 0; --i) {
		int hp = HashPos((byte *)~master + i) % mapn;
		prev[i] = map[hp];
		map[hp] = i;
	}
}

String DiffPacker::Pack(const String& data)
{
	String result;
	const char *masterend = master.End();
	const char *dataend = data.End();
	const char *s = data;
	while(s < dataend) {
		int pos = 0;
		int ml = 0;
		int p = map[HashPos((byte *)s) % mapn];
		int ni = INT_MAX;
		while(p >= 0 && ml < ni) {
			int l = sMatchLen(s, dataend, ~master + p, masterend);
			if(l > ml) {
				pos = p;
				ml = l;
			}
			p = prev[p];
			ni >>= 1;
		}
		if(ml > 65535)
			ml = 65535;
		if(pos <= 255 && ml <= 255 && ml > 3) {
			result.Cat(0xfb);
			result.Cat(pos);
			result.Cat(ml);
			s += ml;
		}
		else
		if(pos <= 65535 && ml <= 255 && ml > 4) {
			result.Cat(0xfc);
			result.Cat(LOBYTE(pos));
			result.Cat(HIBYTE(pos));
			result.Cat(ml);
			s += ml;
		}
		else
		if(pos <= 65535 && ml > 5) {
			result.Cat(0xfd);
			result.Cat(LOBYTE(pos));
			result.Cat(HIBYTE(pos));
			result.Cat(LOBYTE(ml));
			result.Cat(HIBYTE(ml));
			s += ml;
		}
		else
		if(ml > 7) {
			result.Cat(0xfe);
			result.Cat(LOBYTE(LOWORD(pos)));
			result.Cat(HIBYTE(LOWORD(pos)));
			result.Cat(LOBYTE(HIWORD(pos)));
			result.Cat(HIBYTE(HIWORD(pos)));
			result.Cat(LOBYTE(ml));
			result.Cat(HIBYTE(ml));
			s += ml;
		}
		else {
			if((byte)*s >= 0xfb)
				result.Cat(0xff);
			result.Cat(*s++);
		}
	}
	result.Shrink();
	return result;
}

String DiffPack(const String& master, const String& data)
{
	DiffPacker p;
	p.SetMaster(master);
	return p.Pack(data);
}

String DiffUnpack(const String& master, const String& pack)
{
	String result;
	const char *s = pack;
	while(s < pack.End()) {
		int c = (byte)*s++;
		if(c == 0xfb) {
			ASSERT(s <= pack.End() - 2);
			byte p = (byte)*s++;
			byte l = (byte)*s++;
			result.Cat(~master + p, l);
		}
		else
		if(c == 0xfc) {
			ASSERT(s <= pack.End() - 3);
			byte pl = (byte)*s++;
			byte ph = (byte)*s++;
			byte ln = (byte)*s++;
			result.Cat(~master + MAKEWORD(pl, ph), ln);
		}
		else
		if(c == 0xfd) {
			ASSERT(s <= pack.End() - 4);
			byte pl = (byte)*s++;
			byte ph = (byte)*s++;
			byte ll = (byte)*s++;
			byte lh = (byte)*s++;
			result.Cat(~master + MAKEWORD(pl, ph), MAKEWORD(ll, lh));
		}
		else
		if(c == 0xfe) {
			ASSERT(s <= pack.End() - 5);
			byte pll = (byte)*s++;
			byte plh = (byte)*s++;
			byte phl = (byte)*s++;
			byte phh = (byte)*s++;
			byte ll = (byte)*s++;
			byte lh = (byte)*s++;
			result.Cat(~master + MAKELONG(MAKEWORD(pll, plh), MAKEWORD(phl, phh)),
			           MAKEWORD(ll, lh));
		}
		else
		if(c == 0xff)
			result.Cat(*s++);
		else
			result.Cat(c);
	}
	return result;
}
