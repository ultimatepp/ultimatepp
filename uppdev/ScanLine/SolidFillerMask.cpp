#include "ScanLine.h"

/*
struct SolidFillerWithMask : Rasterizer::Target {
	SolidFiller f;
	byte       *mask;
	int         empty;
	int         full;
	
	void Start(int minx, int maxx)      { t += x; }
	void Render(int val);
	void Render(int val, int len);
};

void SolidFillerWithMask::Render(byte val)
{
	for(;;) {
		if(empty) {
			f.t++;
			empty--;
			return;
		}
		if(full) {
			f.Render(val);
			full--;
			return;
		}
		byte m = *mask++;
		if(m) {
			f.Render(val * m >> 8);
			return;
		}
		else {
			if(m < 128)
				empty = m + 1;
			else
				full = m - 128 + 1;
		}
	}
}

void SolidFillerWithMask::Render(byte val, int len)
{
	while(len) {
		if(empty) {
			int n = min(len, empty);
			f.Skip(n);
			empty -= n;
		}
		else
		if(full) {
			int n = min(len, full);
			f.Render(val, n);
			full -= n;
		}
		else {
			byte m = *mask++;
			if(m) {
				f.Render(val * m >> 8);
				len--;
			}
			else {
				if(m < 128)
					empty = m + 1;
				else
					full = m - 128 + 1;
			}
		}
	}
}
*/
