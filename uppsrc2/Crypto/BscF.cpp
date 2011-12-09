#include "Crypto.h"

NAMESPACE_UPP

template <int N>
class Birc {
	byte  r[N];
	int   origin;
public:
	byte& operator[](int i)       { return r[(origin + i) % N]; }
	void  Restart()               { origin = 0; }
	void  Move()                  { origin = (origin + 1) % N; }

	Birc()                        { origin = 0; }
};

struct BscF {
	static byte T1[256];
	static const int Krok;

	Birc<81> R81, Q81;
	byte IV[10], Wk[10];   /*  MAC, Klic na zpr.  */

	void MAC_init();
	void R81_init();
	void MAC_step();
	byte R81_step();
	void R81_start();
	void MACDummy();
	void MACGen(const char *s, const char *lim);
	void Start();
};

const int BscF::Krok = 101;

byte BscF::T1[256] = {
	32, 124, 29, 247, 230, 199, 244, 5, 86, 19, 125, 149, 108, 74, 237, 176,
	165, 82, 240, 236, 144, 154, 107, 131, 147, 66, 118, 116, 126, 250, 127, 23,
	232, 223, 243, 45, 83, 67, 216, 202, 33, 145, 62, 114, 61, 34, 205, 109,
	14, 6, 193, 238, 55, 196, 163, 182, 140, 8, 115, 222, 217, 191, 112, 73,
	142, 75, 214, 1, 59, 46, 239, 72, 164, 68, 180, 119, 209, 84, 152, 159,
	184, 36, 47, 99, 235, 212, 42, 38, 123, 26, 17, 97, 155, 22, 190, 175,
	64, 25, 94, 166, 183, 69, 234, 65, 195, 197, 77, 173, 20, 104, 63, 201,
	249, 15, 134, 122, 57, 204, 139, 0, 210, 174, 3, 2, 80, 135, 106, 213,
	56, 221, 172, 12, 162, 103, 188, 128, 9, 138, 137, 133, 54, 169, 100, 132,
	60, 28, 113, 141, 44, 255, 181, 111, 10, 117, 254, 160, 90, 218, 186, 30,
	48, 246, 79, 187, 37, 35, 53, 179, 129, 157, 146, 203, 177, 71, 27, 189,
	252, 229, 143, 130, 11, 78, 226, 105, 88, 101, 251, 24, 170, 7, 76, 41,
	198, 245, 208, 31, 21, 242, 89, 98, 225, 81, 219, 58, 96, 91, 215, 110,
	227, 93, 39, 220, 52, 211, 253, 85, 151, 70, 228, 233, 171, 148, 206, 121,
	248, 156, 241, 102, 207, 161, 192, 185, 120, 153, 4, 40, 136, 194, 50, 95,
	87, 168, 178, 13, 18, 200, 49, 167, 150, 92, 16, 51, 231, 224, 43, 158
};

void BscF::MAC_init() {
	for(int i = 0; i < 81; i++)
		Q81[i] = T1[i];
}


void BscF::R81_init() {
	for(int i = 80; i >= 0; i--)
		R81[i] = T1[i];
}

void BscF::MAC_step() {
	byte zvq81, neq81;
	neq81 = T1[Q81[80]];
	zvq81 = Q81[0];
	Q81.Move();
	Q81[80] = zvq81 ^ neq81;
	Q81[54] = Q81[54] ^ zvq81;
	Q81[49] = Q81[49] ^ zvq81;
	Q81[16] = Q81[16] ^ zvq81;
}

byte BscF::R81_step() {
	byte zvr81, ner81;
	ner81 = T1[R81[80]];
	zvr81 = R81[0];
	R81.Move();
	R81[80] = zvr81 ^ ner81;
	R81[79] = R81[79] ^ zvr81;
	R81[48] = R81[48] ^ zvr81;
	R81[16] = R81[16] ^ zvr81;
	return ner81;
}

void BscF::R81_start() {
	int i;
	for(i = 0; i < 81; i++) {
		R81_step();
		R81[80] = R81[80] ^ Wk[i % 10];
	}
	for(i = 0; i < Krok; i++)
		R81_step();
}

void BscF::MACDummy() {
	for(int k = 0; k < Krok; k++)
		MAC_step();
}

void BscF::MACGen(const char *s, const char *lim) {
	int i;
	MAC_init();
	while(s < lim) {
		MAC_step();
		Q81[80] = Q81[80] ^ *s++;
	}
	MACDummy();
	for(i = 0; i < 10; i++)
		IV[i] = Q81[80 - i];
}

void BscF::Start() {
	R81_init();
	R81_start();
}

String EncodeBscF(const String& src) {
	String result;
	BscF f;
	const char *lim = src.End();
	f.MACGen(src, lim);
	memcpy(f.Wk, f.IV, 10);
	f.Start();
	result.Cat((const char *)f.IV, 10);
	for(const char *s = src; s < lim; s++)
		result.Cat(*s ^ f.R81_step());
	for(int i = 0; i < 10; i++)
		result.Cat(f.IV[i] ^ f.R81_step());
	return result;
}

String DecodeBscF(const String& src) {
	if(src.GetLength() <= 20)
		return String::GetVoid();
	BscF f;
	String result;
	const char *s = ~src + 10;
	const char *lim = src.End() - 10;
	f.MAC_init();
	memcpy(f.Wk, src, 10);
	f.Start();
	while(s < lim) {
		byte b = *s++ ^ f.R81_step();
		result.Cat(b);
		f.MAC_step();
		f.Q81[80] ^= b;
	}
	f.MACDummy();
	for(int i = 0; i < 10; i++)
		if((byte)*s++ != (f.R81_step() ^ f.Q81[80 - i])) return String::GetVoid();
	return result;
}

END_UPP_NAMESPACE
