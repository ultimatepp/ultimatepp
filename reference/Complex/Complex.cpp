#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN{
	Complex a(1,2);
	Cout() << "a = " << a << "\n";
	Cout() << "Im(a) = " << a.imag() << "\n";
	Cout() << "Re(a) = " << a.real() << "\n";
	Cout() << "|a| = " << abs(a) << "\n";
	Cout() << "a+a = " << a+a << "\n";
	Cout() << "2a = " << 2.0*a << "\n";
	Cout() << "a^2 = " << a*a << "\n";
}


