#include <CtrlLib/CtrlLib.h>



GUI_APP_MAIN
{
	int diver[256];
	String s;
	for(int a = 1; a <= 255; a++) {
		diver[a] = 129 * 255 / a;
		DUMP(129 * 255 / a);
		s << diver[a] << ", ";
	}
	DUMP(s);
	LOG("------");
	int n = 0, cn = 0;
	double maxdelta = 0, cmaxdelta = 0;
	int out = 0, cout = 0;
	int step = 15;
	for(int Fa = 0; Fa <= 255; Fa += step)
		for(int Ba = 0; Ba <= 255; Ba += step) {
			double A = (Fa / 255.0 + Ba / 255.0 - Fa / 255.0 * Ba / 255.0);
			//int Da = Fa + Ba - Fa * Ba / (255 * 255);
//			int Da = Fa + (((Ba + (Ba >> 7)) * (255 - Fa) + Fa) >> 8);
			double dDa = 255 * A;
			int Da = minmax((int)(255 * A + 0.5), 0, 255);
			double d = abs(dDa - Da);
			bool b = Da > 255 || Da < 0;
			if(b) out++;
			if(d >= 1 || b) {
//				LOG("Fa: " << Fa << ", Ba: " << Ba << "|  " << dDa << "  " << dDA << " delta: " << a - A);
				n++;
				if(d > maxdelta)
					maxdelta = d;
			}
//			int m = Da ? int(129 * 255 / dDa) : 0;
			int m = A > 0.001 ? int(256 * (Fa / 255.0) / A + 0.5) : 0;
			DUMP(m);
			for(int F = 0; F <= 255; F += step)
				for(int B = 0; B <= 255; B += step) {
//					int D = B + (m * Fa * (F - B) >> 15);
					int D = B + (m * (F - B) >> 8);
					double dD = B + Fa / dDa * (F - B);
					double d = abs(D - dD);
					bool b = D > 255 || d < 0;
					if(b) cout++;
					if(d > cmaxdelta) {
						cmaxdelta = d;
					}
					if(d >= 1 || b) {
//						LOG("[" << Fa << ", " << F <<
//						    "] over [" << Ba << ",  " << B <<
//						    "]: [" << Da << ", " << D <<
//						    "] -=- [" << dDa << ", " << dD << ']');
						cn++;
					}
				}
		}
	DUMP(n);
	DUMP(maxdelta);
	DUMP(out);
	DUMP(cn);
	DUMP(cmaxdelta);
	DUMP(cout);
}
