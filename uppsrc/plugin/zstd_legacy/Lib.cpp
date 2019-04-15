#include <stdlib.h>     /* malloc, free, qsort */
#include <string.h>     /* memcpy, memset */
#include <stdio.h>      /* printf (debug) */

namespace Upp {
	
namespace Legacy {

#include "lib/entropy_common.c"
#include "lib/fse_compress.c"
#include "lib/fse_decompress.c"
#include "lib/huf_compress.c"
#include "lib/huf_decompress.c"
#include "lib/zbuff_compress.c"
#include "lib/zbuff_decompress.c"
#include "lib/zstd_common.c"
#include "lib/zstd_compress.c"
#include "lib/zstd_decompress.c"

};

};
