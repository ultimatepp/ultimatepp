#include <Core/Core.h>

using namespace Upp;

bool IsPrime(unsigned q)
{
	for(int i = 2; i * i < q; i++)
		if((q % i) == 0)
			return false;
	return true;
}

CONSOLE_APP_MAIN
{
	int q = 32;
	while(q) {
		for(int w = 1; w < 256; w += w)
			if(IsPrime(w + q - 1)) {
				LOG(q << "+" << w - 1 << "=" << w + q - 1);
				break;
			}
		for(int w = 1; w < 256; w += w)
			if(IsPrime(w + q - 1)) {
				LOG(q << "+" << w - 1 << "=" << w + q - 1);
				break;
			}
		q = 2 * q;
	}
}

