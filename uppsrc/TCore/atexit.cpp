#include "TCore.h"

#ifdef PLATFORM_WIN32
#if defined(COMPILER_MSC6) && !defined(flagSO)

typedef void (__cdecl *_PVFV)(void);
//typedef int  (__cdecl * _onexit_t)(void);

extern "C" _PVFV *__onexitbegin;
extern "C" _PVFV *__onexitend;

enum { BLOCKSIZE = 1022 };

struct OnExitBlock
{
	int          count;
	OnExitBlock *prev_block;
	_PVFV        pointers[BLOCKSIZE];
};

static OnExitBlock *onexitlist = NULL;
static bool         lockexit = false;

inline static OnExitBlock *AtExitAlloc()
{
	return (OnExitBlock *)GlobalAlloc(GMEM_FIXED, sizeof(OnExitBlock));
}

inline static void AtExitFree(OnExitBlock *block)
{
	GlobalFree((HGLOBAL)block);
}

static void __cdecl AtExitDestroy()
{
//	ASSERT(lockexit);
#ifdef _DEBUG
	int total = 0;
	for(OnExitBlock *p = onexitlist; p; p = p -> prev_block)
		total += p -> count;
	LOG("AtExitDestroy: " << total << " atexit functions");
#endif//_DEBUG
	while(onexitlist)
	{
		while(onexitlist -> count > 0)
			onexitlist -> pointers[--onexitlist -> count]();
		OnExitBlock *prev = onexitlist -> prev_block;
		AtExitFree(onexitlist);
		onexitlist = prev;
	}
}

void lockatexit()
{
	lockexit = true;
}

extern "C" int __cdecl atexit(_PVFV func)
{
	if(!func)
		return 0; // silently ignore NULL pointers
	ASSERT(!lockexit);
	if(!onexitlist || onexitlist -> count >= BLOCKSIZE)
	{
		if(!onexitlist)
		{
			ASSERT(__onexitbegin == NULL);
			static _PVFV exitlist[1] = { &AtExitDestroy };
			__onexitend = (__onexitbegin = exitlist) + 1;
		}
		OnExitBlock *new_block = AtExitAlloc();
		ASSERT(new_block);
		new_block -> count = 0;
		new_block -> prev_block = onexitlist;
		onexitlist = new_block;
	}
	onexitlist -> pointers[onexitlist -> count++] = func;
	return 0;
}

#endif
#endif
