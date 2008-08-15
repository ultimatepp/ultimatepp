#include "bsb.h"

CONSOLE_APP_MAIN
{
	for(int q = 5; q < 1000; q = 5 * q / 4) {
		{
			TimeStop tm;
			for(int i = 0; i < 1000000; i++)
				BenchStringBuffer(q);
			Cout() << q << ' ' << tm << " - ";
		}
		{
			TimeStop tm;
			for(int i = 0; i < 1000000; i++)
				BenchString(q);
			Cout() << tm << "\r\n";
		}
	}
}

