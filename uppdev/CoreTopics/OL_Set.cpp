//#BLITZ_PROHIBIT

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
