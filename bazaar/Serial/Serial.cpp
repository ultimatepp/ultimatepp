#include "Serial.h"

NAMESPACE_UPP

// get a list of standard baud rates
Index<dword> const &Serial::GetStandardBaudRates(void)
{
	static Index<dword> v;
	if(v.IsEmpty())
	{
		for(int i = 0; i < stdBaudsCount; i++)
			v << stdBauds[i];
	}
	return v;
}

END_UPP_NAMESPACE;