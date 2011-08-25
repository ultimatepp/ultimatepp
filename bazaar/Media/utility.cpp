#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <Media/ffmpeg_base.h>
//#include <Functions4U/Functions4U.h>
#include "utility.h"


#ifdef PLATFORM_POSIX
#include <sys/time.h>
#include <time.h>

int64 GetUSec() {
	struct timeval tv;
	struct timezone tz;
	memset(&tz, 0, sizeof(tz));
	gettimeofday(&tv, &tz);
	return int64(tv.tv_sec)*1000000 + tv.tv_usec;
}

#else

int64 GetUSec() {
 	LARGE_INTEGER ticksPerSec;
  	LARGE_INTEGER tick;   

  	QueryPerformanceFrequency(&ticksPerSec);
  	QueryPerformanceCounter(&tick);
  	return tick.QuadPart/(ticksPerSec.QuadPart/1000000);
}

#endif