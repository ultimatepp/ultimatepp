#if defined(flagWIN32) || defined(flagOSX) || defined(flagSTATIC_PNG)

#define PNG_NO_MMX_CODE
#define PNG_USE_GLOBAL_ARRAYS

#include "lib/png.c"
#include "lib/pngerror.c"
#include "lib/pnggccrd.c"
#include "lib/pngget.c"
#include "lib/pngmem.c"
#include "lib/pngpread.c"
#include "lib/pngread.c"
#include "lib/pngrio.c"
#include "lib/pngrtran.c"
#include "lib/pngrutil.c"
#include "lib/pngset.c"
#include "lib/pngtrans.c"
#include "lib/pngvcrd.c"
#include "lib/pngwio.c"
#include "lib/pngwrite.c"
#include "lib/pngwtran.c"
#include "lib/pngwutil.c"
#include "lib/arm/arm_init.c"
#include "lib/arm/filter_neon_intrinsics.c"
#include "lib/arm/palette_neon_intrinsics.c"

#endif
