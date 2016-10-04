#ifndef _AddOn_AddOn_h_
#define _AddOn_AddOn_h_

#include <Core/Core.h>

using namespace Upp;

#if defined PLATFORM_WIN32 || defined PLATFORM_WINCE
#define ADDONEXPORT extern "C" __declspec(dllexport)
#else
#define ADDONEXPORT
#endif

class IAddOn
{
public:
	virtual String GetName() = 0;
	virtual int GetMajorRevision();
	virtual int GetMinorRevision();
};

#endif
