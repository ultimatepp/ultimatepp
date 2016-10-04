#ifndef guard_DllTest_DllTest_h
#define guard_DllTest_DllTest_h

#define DllImport extern "C" __declspec(dllimport)
#define DllExport extern "C" __declspec(dllexport)

#ifdef flagDLL
	#define DLLIMPORT __declspec(dllexport)
#else
	#define DLLIMPORT __declspec(dllimport)
#endif

#endif
