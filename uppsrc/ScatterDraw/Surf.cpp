#include "ScatterDraw.h"


ScatterDraw& ScatterDraw::AddSurf(DataSourceSurf &surf) {
	this->surf = &surf;
	if (IsNull(surfMinZ))
		surfMinZ = surf.MinZ();
	if (IsNull(surfMaxZ))
		surfMaxZ = surf.MaxZ();
	return *this;
}

inline static double Smooth01(double val) {
	double val2 = val*val;
	return 3*val2 - 2*val2*val;
}

inline static double Smooth01Left(double val)  {return 2*Smooth01(val/2);}
inline static double Smooth01Right(double val) {return 2*(Smooth01(0.5 + val/2)-0.5);}

inline static double Filter01(double frac) {
	if (frac >= 1)
		return 1;
	else if (frac < 0)
		return 0;
	return frac;
}

static Color InterpolateColor(double frac, const Color &from, const Color &to) {
	return Color(int(from.GetR() + frac*(to.GetR() - from.GetR())), 
				 int(from.GetG() + frac*(to.GetG() - from.GetG())),
				 int(from.GetB() + frac*(to.GetB() - from.GetB())));
}

Color GetRainbowColor(double frac, RAINBOW rainbow, int numScales) {
	frac = Filter01(frac);
	switch (rainbow) { 
	case BLUE_YELLOW_RED:	frac = 1 - frac;
	case RED_YELLOW_BLUE:	return GetRainbowColor(frac, Color(255,0,0), Color(255,255,0), Color(0,0,255), numScales);
	case GREEN_YELLOW_RED:	frac = 1 - frac;
	case RED_YELLOW_GREEN:	return GetRainbowColor(frac, Color(255,0,0), Color(255,255,0), Color(0,255,0), numScales);
	case WHITE_BLACK:		frac = 1 - frac;
	case BLACK_WHITE:		return GetRainbowColor(frac, Color(0,0,0), Color(255,255,255), numScales);
	case BLUE_WHITE_RED:	frac = 1 - frac;
	case RED_WHITE_BLUE:	return GetRainbowColor(frac, Color(255,0,0), Color(255,255,255), Color(0,0,255), numScales);
	}
	return Null;
}

Color GetRainbowColor(double frac, const Color &from, const Color &to, int numScales) {
	if (numScales > 0)
		frac = (int(frac*numScales)/double(numScales))/(1 - 1/numScales);
	frac = Filter01(frac);
	return InterpolateColor(frac, from, to);
}

Color GetRainbowColor(double frac, const Color &col0, const Color &col1, const Color &col2, int numScales) {
	if (IsNull(col2)) 
		return GetRainbowColor(frac, col0, col1, numScales);
	if (numScales > 0)
		frac = int(frac*numScales)/double(numScales - 1);
	frac = Filter01(frac);
	if (frac < 0.5) 
		return InterpolateColor((numScales > 0) ? 2*frac : Smooth01Right(2*frac), col0, col1);
	else
		return InterpolateColor((numScales > 0) ? 2*(frac - 0.5) : Smooth01Left(2*(frac - 0.5)), col1, col2);
}
