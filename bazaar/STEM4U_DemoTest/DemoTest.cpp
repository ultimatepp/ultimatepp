#include <Core/Core.h>
#include <Functions4U/Functions4U.h>
#include <plugin/Eigen/Eigen.h>
#include <STEM4U/IntInf.h>
#include <STEM4U/Rational.h>
#include <STEM4U/Polynomial.h>
#include <STEM4U/Sundials.h>
#include <STEM4U/Integral.h>
#include <STEM4U/TSP.h>

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
	UppLog() << "\nSemicircle integral value is " << M_PI*sqr(R)/2;
	UppLog() << "\nNumerically integrated with simple and composite versions:";
	UppLog() << Format("\n%s\t%s\t\t%s\t\t%s\t\t%s\t%s", "#Points", "Trapezoidal", "Simpson 1/3", "Simpson 3/8", "Hermite 3 point", "Hermite 5 point");
	for (int nump = 5; nump <= 30; ++nump) {
		double dx = 2*R/(nump-1);
		VectorXd y(nump), x(nump);
		for (int i = 0; i < nump; ++i) {
			x[i] = 2*R*i/(nump-1) - R;
			y[i] = ::sqrt(sqr(R) - sqr(x[i]));
		}
		
		double yx_trap = Integral<VectorXd, double>(y, x, TRAPEZOIDAL);
		double yx_simp13 = Integral<VectorXd, double>(y, x, SIMPSON_1_3);
		double yx_simp38 = Integral<VectorXd, double>(y, x, SIMPSON_3_8);
		double ydx_trap = Integral(y, dx, TRAPEZOIDAL);
		double ydx_simp13 = Integral(y, dx, SIMPSON_1_3);
		double ydx_simp38 = Integral(y, dx, SIMPSON_3_8);
		double ydx_herm3 = Integral(y, dx, HERMITE_3);
		double ydx_herm5 = Integral(y, dx, HERMITE_5);
		
		UppLog() << Format("\n%d", nump);
		UppLog() << Format("\t%7.5f = %7.5f", yx_trap, ydx_trap);
		UppLog() << Format("\t%7.5f = %7.5f", yx_simp13, ydx_simp13);
		UppLog() << Format("\t%7.5f = %7.5f", yx_simp38, ydx_simp38);
		UppLog() << Format("\t%7.5f", ydx_herm3);
		UppLog() << Format("\t\t%7.5f", ydx_herm5);
		VERIFY(yx_trap - ydx_trap < 1E-10);
		VERIFY(yx_simp13 - ydx_simp13 < 1E-10);
		VERIFY(yx_simp38 - ydx_simp38 < 1E-10);
	}
}
	
CONSOLE_APP_MAIN 
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	UppLog() << "STEM4U demo and test";
	
	TestTSP();
	TestRational();
	TestDAESolver();
	TestIntInf();
    TestPolynomial();
    TestIntegral();
    
	#ifdef flagDEBUG
	UppLog() << "\n";
	Cout() << "\nPress enter key to end";
	ReadStdIn();
	#endif    
}

