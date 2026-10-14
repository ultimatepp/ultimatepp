//#BLITZ_PROHIBIT

// pretend that we are linking statically even if not so that we can check that we have
// big enough buffers for xxHash states, with 100% reserve
// as we are really not doing any real code here, this should be fine

#define XXH_STATIC_LINKING_ONLY
#include <xxhash.h>

static_assert(2 * sizeof(XXH32_state_t) < 256);
static_assert(2 * sizeof(XXH64_state_t) < 256);
