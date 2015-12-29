#include <Core/Core.h>

using namespace Upp;

#include <arrayfire.h>

using namespace af; 

void MatMult_Bench();
void Pi_Bench();
void Vectorize_Bench();
void Demo();

CONSOLE_APP_MAIN
{
    try {
		int device = 0;
        setDevice(device);
        info();

		Pi_Bench();
		MatMult_Bench();
		Vectorize_Bench();
		Demo();
    } catch (af::exception& e) {
        printf("\nError: %s\n", e.what());
    } catch (...) {
        printf("\nUnknown error\n");
    }
    printf("\nEnd");
	#ifdef WIN32 // pause in Windows
        printf(". Hit enter...");
        fflush(stdout);
        getchar();
    #endif
}