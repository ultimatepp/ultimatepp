#include <Core/Core.h>

using namespace Upp;

#include <STEM4U/STEM4U.h>

#ifdef flagTEST

void TestIntInf() {
	intInf a = "12345678901234567890";
	intInf b = 2, c;
	
	c = a%b;		Cout() << "a%%b:      "	<< c << "\n";
	
	ASSERT(c == 0);
	
	c = a;
	c += b;			UppLog() << "c += b:    " << c << "\n";
	c -= b;			UppLog() << "c -= b:    " << c << "\n";
	c = c + 2;		UppLog() << "c = 2 + b: " << c << "\n";
	c = c - 2;		UppLog() << "c = 2 - b: " << c << "\n";
	c *= 2;			UppLog() << "c *= b:    " << c << "\n";
	c /= 2;			UppLog() << "c /= b:    " << c << "\n";
	c = c * 2;		UppLog() << "c = c * 2: " << c << "\n";
	c = c / 2;		UppLog() << "c = c / 2: " << c << "\n";

	ASSERT(c == a);
}

void TestPolynomial() {
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
	UppLog() << q[0] << "\n";
	
	q << Polynomial<Rational>(0, 2);
	UppLog() << q[1] << "\n";

	for (int i = 2; i < n+2; ++i) {
		Rational ii = i;
		
		q << Polynomial<Rational>(0, (2*ii - 1)*2/ii) * q[i-1] - q[i-2] * (((ii-1)*(NT*NT - (ii-1)*(ii-1)))/i);
		UppLog() << q[i] << "\n";
	}
	
	auto sg = q[n+1].Order(-1);
	UppLog() << "sg " << sg << "\n";
	
	auto dsg = sg.Diff();	
	UppLog() << "dsg " << dsg << "\n";

	auto num = q[n].y(0).Simplify(); 
	auto den = ((gamma_n * csi_n_num) / csi_n_den).Simplify();
	UppLog() << "num: " << num << "\n";
	UppLog() << "den: " << den << "\n";

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
	    	UppLog() << Format("b[%5d] = ", l) << FormatRational(b[l+M], 20) << "\n";
	}
	 
	for (int l = 1; l < m+1; ++l) 
		b[m + l] =  b[m - l]; 
	
	UppLog() << "sumb = " << sum_bN.Simplify() << "\n";
	ASSERT(sum_bN.Simplify() == 1);
}


CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
    
    TestIntInf();
    TestPolynomial();
}

#else

// val = 2/1 * 3/2 * 4/3 * ... If done n times, result has to be n
template<typename T>
T Loop() {
	T val = 1;
	for (T d = 1; d < 100; ++d) 
		val *= (d+1)/d;
	return val;
}
	
CONSOLE_APP_MAIN 
{
	Cout() << "To test rounding errors\n";
	
	double dval = Loop<double>();
	Rational rval = Loop<Rational>();
	
	Cout() << "double   == 100: " << ((dval == 100) ? "true" : "false") << "\n";		// Fails
	Cout() << "Rational == 100: " << ((rval == 100) ? "true" : "false") << "\n";
	
	Cout() << "\n";
	
	Cout() << "sin() calculation\n";
	
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
	Cout() << "sin() Taylor series is: " << sinSeries << "\n";
	
	Rational sin_1_3 = sinSeries.y(Rational(1, 3));	
	Cout() << "sin(1/3) = " << sin_1_3 << "\n";		
	Cout() << "sin(1/3) = " << FormatRational(sin_1_3, 32) << "\n";	
}

#endif