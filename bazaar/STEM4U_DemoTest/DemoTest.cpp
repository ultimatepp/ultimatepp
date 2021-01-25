#include <Core/Core.h>
#include <Functions4U/Functions4U.h>
#include <plugin/Eigen/Eigen.h>
#include <STEM4U/IntInf.h>
#include <STEM4U/Rational.h>
#include <STEM4U/Polynomial.h>
#include <STEM4U/Sundials.h>
#include <STEM4U/Integral.h>
#include <STEM4U/TSP.h>
#include <STEM4U/SeaWaves.h>
#include <STEM4U/Utility.h>
#include <STEM4U/CrossCorrelation.h>

using namespace Upp;

void TestIntInf() {
	UppLog() << "\n\nintInf demo. A signed integer type with arbitrary-precision including the usual arithmetic.";
	
	intInf a = "12345678901234567890";
	intInf b = 2, c;
	
	c = a%b;		UppLog() << "\na%%b:      "	<< c;
	
	VERIFY(c == 0);
	
	c = a;
	c += b;			UppLog() << "\nc += b:    " << c;
	c -= b;			UppLog() << "\nc -= b:    " << c;
	c = c + 2;		UppLog() << "\nc = 2 + b: " << c;
	c = c - 2;		UppLog() << "\nc = 2 - b: " << c;
	c *= 2;			UppLog() << "\nc *= b:    " << c;
	c /= 2;			UppLog() << "\nc /= b:    " << c;
	c = c * 2;		UppLog() << "\nc = c * 2: " << c;
	c = c / 2;		UppLog() << "\nc = c / 2: " << c;

	VERIFY(c == a);
}

void TestPolynomial() {
	UppLog() << "\n\nRational demo. A rational number based on an arbitrary precision integer";
	
	int n = 6;
	Rational NT = 111;
	
	int r = int(pow(10, int(log10(int(NT))-1)));
	int m = int(NT/2);

	int M = int((NT-1) / 2);

	int csi_n_num = 2*(2*n+1);
	int csi_n_den = n+1;
	
	Rational gamma_n_num = NT;
	int gamma_n_den = 2 * n + 1;
	
	for (int j = 1; j < n+1; ++j) 
		gamma_n_num *= NT*NT - j*j;
	
	Rational gamma_n = gamma_n_num/gamma_n_den;
	
	Upp::Vector<Polynomial<Rational>> q;
	
	q << Polynomial<Rational>(1);
	UppLog() << "\n" << q[0];
	
	q << Polynomial<Rational>(0, 2);
	UppLog() << "\n" << q[1];

	for (int i = 2; i < n+2; ++i) {
		Rational ii = i;
		
		q << Polynomial<Rational>(0, (2*ii - 1)*2/ii) * q[i-1] - q[i-2] * (((ii-1)*(NT*NT - (ii-1)*(ii-1)))/i);
		UppLog() << "\n" << q[i];
	}
	
	auto sg = q[n+1].Order(-1);
	UppLog() << "\n" << "sg " << sg;
	
	auto dsg = sg.Diff();	
	UppLog() << "\n" << "dsg " << dsg;

	auto num = q[n].y(0).Simplify(); 
	auto den = ((gamma_n * csi_n_num) / csi_n_den).Simplify();
	UppLog() << "\n" << "num: " << num;
	UppLog() << "\n" << "den: " << den;

	Vector<Rational> b;	
	b.SetCount(int(NT), 0);
 
	Rational sum_bN = 0;
	for (int l = -m; l < 1; ++l) {
		b[l+m] = (sg.y(l) *num) / den;
	
	  	if (l == 0) 
	    	sum_bN += b[m + l];
	  	else 
	    	sum_bN += 2*b[m + l];

	  	if (l % r == 0)
	    	UppLog() << Format("\nb[%5d] = ", l) << FormatRational(b[l+M], 20);
	}
	 
	for (int l = 1; l < m+1; ++l) 
		b[m + l] =  b[m - l]; 
	
	UppLog() << "\nsumb = " << sum_bN.Simplify();
	VERIFY(sum_bN.Simplify() == 1);
}

// val = 2/1 * 3/2 * 4/3 * ... If done n times, result has to be n
template<typename T>
T Loop() {
	T val = 1;
	for (T d = 1; d < 100; ++d) 
		val *= (d+1)/d;
	return val;
}

void TestTSP() {
	UppLog() << "\nTravelling salesman";

	const Vector<Point_<int>> points = {{0, 0},{4, 4},{4, 0},{2, 4},{0, 4},{4, 2},{0, 2},{2, 0}};

	Vector<int> orderp;
	int distp = TSP(points, orderp, TSP_NEAREST_NEIGHBOR);
	UppLog() << "\nTotal distance between points is: " << distp;
	VERIFY(distp == 16);
	String sorderp;
	for (int i = 0; i < orderp.size(); ++i) {
		if (i > 0)
			sorderp << " -> ";
		sorderp << orderp[i];
	}
	UppLog() << "\nOrder is: " << sorderp;
	UppLog() << "\n";
	VERIFY(sorderp == "0 -> 7 -> 2 -> 5 -> 1 -> 3 -> 4 -> 6 -> 0");

		
	// Example from https://developers.google.com/optimization/routing/tsp#printer
	const Vector<Vector<int>> cities = {
		{0, 2451, 713, 1018, 1631, 1374, 2408, 213, 2571, 875, 1420, 2145, 1972},
		{2451, 0, 1745, 1524, 831, 1240, 959, 2596, 403, 1589, 1374, 357, 579},
		{7133, 1745, 0, 355, 920, 803, 1737, 851, 1858, 262, 940, 1453, 1260},
		{1018, 1524, 355, 0, 700, 862, 1395, 1123, 1584, 466, 1056, 1280, 987},
		{1631, 831, 920, 700, 0, 663, 1021, 1769, 949, 796, 879, 586, 371},
		{1374, 1240, 803, 862, 663, 0, 1681, 1551, 1765, 547, 225, 887, 999},
		{2408, 959, 1737, 1395, 1021, 1681, 0, 2493, 678, 1724, 1891, 1114, 701},
		{213, 2596, 851, 1123, 1769, 1551, 2493, 0, 2699, 1038, 1605, 2300, 2099},
		{2571, 403, 1858, 1584, 949, 1765, 678, 2699, 0, 1744, 1645, 653, 600},
		{875, 1589, 262, 466, 796, 547, 1724, 1038, 1744, 0, 679, 1272, 1162},
		{1420, 1374, 940, 1056, 879, 225, 1891, 1605, 1645, 679, 0, 1017, 1200},
		{2145, 357, 1453, 1280, 586, 887, 1114, 2300, 653, 1272, 1017, 0, 504},
		{1972, 579, 1260, 987, 371, 999, 701, 2099, 600, 1162, 1200, 504, 0},
	};

	Vector<int> order;
	int dist = TSP(cities, order, TSP_CONSECUTIVE);
	
	UppLog() << "\nTotal distance between cities is: " << dist;
	VERIFY(dist == 7293);
	String sorder;
	for (int i = 0; i < order.size(); ++i) {
		if (i > 0)
			sorder << " -> ";
		sorder << order[i];
	}
	UppLog() << "\nOrder is: " << sorder;
	VERIFY(sorder == "0 -> 7 -> 2 -> 3 -> 4 -> 12 -> 6 -> 8 -> 1 -> 11 -> 10 -> 5 -> 9 -> 0");
}

void TestRational() {
	UppLog() << "\n\nRounding errors test";
	
	double dval = Loop<double>();
	Rational rval = Loop<Rational>();
	
	UppLog() << "\ndouble   == 100: " << ((dval == 100) ? "true" : "false");		// Fails
	UppLog() << "\nRational == 100: " << ((rval == 100) ? "true" : "false");
	
	UppLog() << "\n";
	
	UppLog() << "\nsin() calculation";
	
	Polynomial<Rational> sinSeries;
	intInf fact = 1;
	int sign = 1;
	for (int i = 1; i < 25; i++) {
		fact *= i;
		if (!((i-1)%2)) {
			sinSeries[i] = Rational(intInf(sign), fact);
			sign = -sign;
		}
	}
	UppLog() << "\nsin() Taylor series is: " << sinSeries;
	
	Rational sin_1_3 = sinSeries.y(Rational(1, 3));	
	UppLog() << "\nsin(1/3) = " << sin_1_3;		
	UppLog() << "\nsin(1/3) = " << FormatRational(sin_1_3, 32);	
}

void TestDAESolver() {
	UppLog() << "\n\nSolveDAE() solves an harmonic oscillator m·d2x + k·x = 0"; 
	double y[]  = {2, 0};
	double dy[] = {0, 0};
	double m = 1, k = 0.5;
	SolveDAE(y, dy, 2, 0.1, 10, 
		[&](double t, Eigen::Index iiter, const double y[], const double dy[], double residual[])->int {
			residual[0] = m*dy[1] + k*y[0];
			residual[1] = y[1] - dy[0];
			return true;
		}, 2,
		[&](double t, Eigen::Index iiter, const double y[], const double dy[], double residual[])->int {
			residual[0] = y[0] - 0.0001;
  			residual[1] = y[1] - 0.0001;
			return true;
		},
		[&](double t, Eigen::Index iiter, const double y[], const double dy[], bool isZero, int *whichZero)->bool {
			UppLog() << Format("\n>T: %7.4f %8.4f %8.4f %s", t, y[0], y[1], isZero ? "Y" : "");
			return true;
		}
	);
}
using namespace  Eigen;

void TestIntegral() {	
	UppLog() << "\n\nIntegral demo";
	
	double R = 1;
	double res = M_PI*sqr(R)/2;
	UppLog() << "\nSemicircle integral value is " << res;
	UppLog() << "\nNumerically integrated with simple and composite versions:";
	UppLog() << Format("\n%s\t%s\t%s\t%s\t%s\t\t%s\t%s", "#Points", "Trapezoidal", "Simpson 1/3", "Simpson 3/8", "Spline", "Hermite 3 point", "Hermite 5 point");
	for (int nump = 5; nump <= 30; ++nump) {
		double dx = 2*R/(nump-1);
		VectorXd y(nump), x(nump);
		for (int i = 0; i < nump; ++i) {
			x[i] = 2*R*i/(nump-1) - R;
			y[i] = ::sqrt(sqr(R) - sqr(x[i]));
		}
		
		double yx_trap    = Integral(x, y, TRAPEZOIDAL);
		double yx_simp13  = Integral(x, y, SIMPSON_1_3);
		double yx_simp38  = Integral(x, y, SIMPSON_3_8);
		double yx_spline  = Integral(x, y, SPLINE);
		double ydx_trap   = Integral(y, dx, TRAPEZOIDAL);
		double ydx_simp13 = Integral(y, dx, SIMPSON_1_3);
		double ydx_simp38 = Integral(y, dx, SIMPSON_3_8);
		double ydx_herm3  = Integral(y, dx, HERMITE_3);
		double ydx_herm5  = Integral(y, dx, HERMITE_5);
		double ydx_spline = Integral(y, dx, SPLINE);
		
		UppLog() << Format("\n%d", nump);
		UppLog() << Format("\t%7.5f=%7.5f", yx_trap, ydx_trap);
		UppLog() << Format("\t%7.5f=%7.5f", yx_simp13, ydx_simp13);
		UppLog() << Format("\t%7.5f=%7.5f", yx_simp38, ydx_simp38);
		UppLog() << Format("\t%7.5f=%7.5f", yx_spline, ydx_spline);
		UppLog() << Format("\t%7.5f", ydx_herm3);
		UppLog() << Format("\t\t%7.5f", ydx_herm5);
		VERIFY(yx_trap - ydx_trap < 1E-10);			VERIFY(abs(yx_trap   - res)/res < 0.15);
		VERIFY(yx_simp13 - ydx_simp13 < 1E-10);		VERIFY(abs(yx_simp13 - res)/res < 0.15);
		VERIFY(yx_simp38 - ydx_simp38 < 1E-10);		VERIFY(abs(yx_simp38 - res)/res < 0.15);
		VERIFY(yx_spline - ydx_spline < 1E-10);		VERIFY(abs(yx_spline - res)/res < 0.15);
	}
}
	
	
void TestSeaWaves() {
	UppLog() << "\n\nSeaWaves demo";

	double T = 12;
	double depth = 50;
	double H = 2;
	
	SeaWaves::rho = 1028;
	
	double waveNumber  = SeaWaves::WaveNumber(T, depth, false);
	UppLog() << "\n" << Format("Wave number: %f rad/m", waveNumber);
	double waveNumberE = SeaWaves::WaveNumber(T, depth, true);
	UppLog() << "\n" << Format("Wave number (exact): %f rad/m", waveNumberE);
	double waveLength = SeaWaves::WaveLength(T, depth);
	UppLog() << "\n" << Format("Wave length: %f m", waveLength);
	double c = SeaWaves::Celerity(T, depth);		
	UppLog() << "\n" << Format("Celerity: %f m/s", c);
	double gc = SeaWaves::GroupCelerity(T, depth);
	UppLog() << "\n" << Format("Group celerity: %f m/s", gc);
	SeaWaves::SEA_TYPE seaType = SeaWaves::GetSeaType(T, depth);
	UppLog() << "\n" << Format("Sea: %s", seaType == SeaWaves::SHALLOW ? "shallow" : seaType == SeaWaves::INTERMEDIATE ? "intermediate" : "deep");
	double power = SeaWaves::Power(T, H, depth);
	UppLog() << "\n" << Format("Power: %f kW/m", power);
		
	double Tz = 12;
	double gamma = 2;
	double Tp = Tp_fTz(Tz, gamma);
	double gamma2 = gamma_fTp_Tz(Tp, Tz);
	UppLog() << "\n" << Format("Tp: %.2f, Tz: %.2f, gamma: %.4f, %.4f", Tp, Tz, gamma, gamma2);
	VERIFY(abs(gamma - gamma2) < 0.000001);

	double freq = 2*M_PI/T;
	double psd = SeaWaves::JONSWAP_Spectrum(H, T, 3.3, freq);
	UppLog() << "\n" << Format("JONSWAP PSD (%f): %f", freq, psd);

	{
		SeaWaves waves;
		
		double Tp =	12, Hs = 2;
		waves.Init(Tp, Hs);
		double x = 100, y = 100, z = -10, t = 10;
		waves.Calc(x, y, z, t);
		UppLog() << "\n" << Format("Sea data for Hs: %.2f m, Tp; %.2f s, at x: %.2f m, y: %.2f m, z: %.2f m, t: %.3f s", Hs, Tp, x, y, z, t);
		UppLog() << "\n" << Format("Free surface z: %f m = %f m", waves.zSurf, waves.ZSurf(x, y, z, t));
		VERIFY(abs(waves.zSurf - waves.ZSurf(x, y, z, t)) < 0.000001);
		VERIFY(abs(waves.zSurf - 0.13560241) < 0.000001);
		UppLog() << "\n" << Format("vx: %f m/s, vy: %f m/s, vz: %f m/s", waves.vx, waves.vy, waves.vz);
		VERIFY(abs(waves.vz + 0.102412178) < 0.000001);
		UppLog() << "\n" << Format("ax: %f m/s2, ay: %f m/s2, az: %f m/s2", waves.ax, waves.ay, waves.az);
		VERIFY(abs(waves.az - 0.000565667) < 0.000001);
		UppLog() << "\n" << Format("p: %.3f Pa = %.3f Pa", waves.p, waves.Pressure(x, y, z, t));
		VERIFY(abs(waves.p - waves.Pressure(x, y, z, t)) < 0.000001);
	}
}

void TestXCorr() {
	VectorXd n, y;
	LinSpaced(n, 16, 0, 15);
	VectorXd x = Eigen::pow(0.84, n.array());
	CircShift(x, 5, y); 
	
	VectorXd R, lags;
	
	XCorr(x, y, R, lags);
	Vector<double> realR = {0.1749,0.35513,0.54619,0.75389,0.98456,1.2452,1.5439,1.8896,2.2928,2.766,3.3234,2.8648,2.4935,2.1982,1.9699,1.8017,1.5026,1.2494,1.0343,0.85067,0.69298,0.55641,0.43679,0.33049,0.23425,0.14516,0.060494,0.046321,0.033559,0.02182,0.010746};
	Vector<double> realLags = {-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	VERIFY(CompareDecimals(R, realR, 4));
	VERIFY(CompareDecimals(lags, realLags, 4));

	XCorr(x, y, R, lags, 'b');
	realR = {0.010931,0.022196,0.034137,0.047118,0.061535,0.077828,0.096492,0.1181,0.1433,0.17287,0.20771,0.17905,0.15585,0.13739,0.12312,0.1126,0.093915,0.078089,0.064643,0.053167,0.043311,0.034775,0.027299,0.020655,0.014641,0.0090727,0.0037809,0.002895,0.0020974,0.0013638,0.00067165};
	VERIFY(CompareDecimals(R, realR, 4));
	VERIFY(CompareDecimals(lags, realLags, 5));

	XCorr(x, y, R, lags, 'u');
	realR = {0.1749,0.17757,0.18206,0.18847,0.19691,0.20754,0.22055,0.2362,0.25476,0.2766,0.30213,0.23874,0.19181,0.15702,0.13133,0.1126,0.10018,0.089245,0.079561,0.070889,0.062998,0.055641,0.048532,0.041311,0.033465,0.024194,0.012099,0.01158,0.011186,0.01091,0.010746};
	VERIFY(CompareDecimals(R, realR, 4));
	VERIFY(CompareDecimals(lags, realLags, 4));

	XCorr(x, y, R, lags, 'c');
	realR = {0.051686,0.10495,0.16141,0.22279,0.29095,0.36799,0.45624,0.55839,0.67757,0.81739,0.98212,0.8466,0.73688,0.64961,0.58214,0.53242,0.44405,0.36922,0.30565,0.25139,0.20479,0.16443,0.12908,0.097664,0.069226,0.042898,0.017877,0.013688,0.0099172,0.0064482,0.0031757};
	VERIFY(CompareDecimals(R, realR, 4));
	VERIFY(CompareDecimals(lags, realLags, 4));

	XCorr(x, R, lags);
	realR = {0.073146,0.14852,0.22842,0.31528,0.41176,0.52078,0.64567,0.79024,0.95889,1.1568,1.3899,1.6654,1.9916,2.3786,2.838,3.3839,2.838,2.3786,1.9916,1.6654,1.3899,1.1568,0.95889,0.79024,0.64567,0.52078,0.41176,0.31528,0.22842,0.14852,0.073146};
	VERIFY(CompareDecimals(R, realR, 4));
	VERIFY(CompareDecimals(lags, realLags, 4));

	XCorr(x, R, lags, 'b');
	realR = {0.0045716,0.0092825,0.014276,0.019705,0.025735,0.032549,0.040354,0.04939,0.059931,0.072298,0.086868,0.10409,0.12448,0.14866,0.17737,0.21149,0.17737,0.14866,0.12448,0.10409,0.086868,0.072298,0.059931,0.04939,0.040354,0.032549,0.025735,0.019705,0.014276,0.0092825,0.0045716};
	VERIFY(CompareDecimals(R, realR, 4));
	VERIFY(CompareDecimals(lags, realLags, 4));
	
	XCorr(x, R, lags, 'u');
	realR = {0.073146,0.07426,0.076141,0.078821,0.082351,0.086796,0.092238,0.09878,0.10654,0.11568,0.12635,0.13878,0.1532,0.1699,0.1892,0.21149,0.1892,0.1699,0.1532,0.13878,0.12635,0.11568,0.10654,0.09878,0.092238,0.086796,0.082351,0.078821,0.076141,0.07426,0.073146};
	VERIFY(CompareDecimals(R, realR, 4));
	VERIFY(CompareDecimals(lags, realLags, 4));
	
	XCorr(x, R, lags, 'c');
	realR = {0.073146,0.07426,0.076141,0.078821,0.082351,0.086796,0.092238,0.09878,0.10654,0.11568,0.12635,0.13878,0.1532,0.1699,0.1892,0.21149,0.1892,0.1699,0.1532,0.13878,0.12635,0.11568,0.10654,0.09878,0.092238,0.086796,0.082351,0.078821,0.076141,0.07426,0.073146};
	VERIFY(CompareDecimals(R, realR, 4));
	VERIFY(CompareDecimals(lags, realLags, 4));
}

void TestLocalFitting(bool test);
void TestMooring(bool test);
void TestButterworth(bool test);

CONSOLE_APP_MAIN 
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	UppLog() << "STEM4U demo and test";
	
	bool test = CommandLine().size() > 0 && CommandLine()[0] == "-test";
	
	TestXCorr();
	TestMooring(test);
	TestLocalFitting(test);
	TestButterworth(test);
	TestTSP();
	TestRational();
	TestDAESolver();
	TestIntInf();
    TestPolynomial();
    TestIntegral();
    TestSeaWaves();
    
    UppLog() << "\n\nAll tests passed\n";
    
	#ifdef flagDEBUG
	UppLog() << "\n";
	Cout() << "\nPress enter key to end";
	ReadStdIn();
	#endif    
}

