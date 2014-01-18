//#BLITZ_PROHIBIT

// This file is explicitly removed from BLITZ to avoid inline optimizations

#ifndef flagNONAMESPACE
namespace Upp {
#endif

void Set__(volatile bool& b)
{
	//WriteMemoryBarrier should be here
	b = true;
}

#ifndef flagNONAMESPACE
};
#endif
