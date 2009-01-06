#ifdef flagWIN32

#undef CAIRO_VERSION_H
#undef CAIRO_VERSION_MAJOR
#undef CAIRO_VERSION_MINOR
#undef CAIRO_VERSION_MICRO
#include "lib/cairo-version.c"

#include "lib/cairo-analysis-surface.c"
#include "lib/cairo-arc.c"
#include "lib/cairo-array.c"
#include "lib/cairo-atomic.c"
#include "lib/cairo-base85-stream.c"
#include "lib/cairo-bentley-ottmann.c"
#include "lib/cairo-cache.c"

#undef _BSD_SOURCE
#include "lib/cairo-cff-subset.c"

#include "lib/cairo-clip.c"
#include "lib/cairo-color.c"
//#include "lib/cairo-debug.c" 
#include "lib/cairo-deflate-stream.c"
#include "lib/cairo-fixed.c"

#undef _BSD_SOURCE
#include "lib/cairo-font-face.c"

#include "lib/cairo-font-face-twin.c"
#include "lib/cairo-font-face-twin-data.c"
#include "lib/cairo-font-options.c"
#include "lib/cairo-freelist.c"
#include "lib/cairo-gstate.c"
#include "lib/cairo-hash.c"
#include "lib/cairo-hull.c"
#include "lib/cairo-image-surface.c"
#include "lib/cairo-lzw.c"
#include "lib/cairo-matrix.c"
#include "lib/cairo-meta-surface.c"
#include "lib/cairo-misc.c"
#include "lib/cairo-mutex.c"

#undef _BSD_SOURCE
#include "lib/cairo-output-stream.c"

#include "lib/cairo-paginated-surface.c"
#include "lib/cairo-path-bounds.c"
#include "lib/cairo-path-fill.c"
#include "lib/cairo-path-fixed.c"
#include "lib/cairo-path-stroke.c"
#include "lib/cairo-path.c"
#include "lib/cairo-pattern.c"
#include "lib/cairo-pdf-operators.c"

#undef _BSD_SOURCE
#include "lib/cairo-pdf-surface.c"

#include "lib/cairo-pen.c"
#include "lib/cairo-polygon.c"
#include "lib/cairo-png.c"
#include "lib/cairo-rectangle.c"
#include "lib/cairo-region.c"
#include "lib/cairo-scaled-font-subsets.c"
#include "lib/cairo-scaled-font.c"
#include "lib/cairo-skiplist.c"
#include "lib/cairo-slope.c"
#include "lib/cairo-spline.c"
#include "lib/cairo-stroke-style.c"
#include "lib/cairo-surface-fallback.c"
#include "lib/cairo-surface.c"
#include "lib/cairo-svg-surface.c"
#include "lib/cairo-traps.c"
#include "lib/cairo-truetype-subset.c"
#include "lib/cairo-type1-fallback.c"
#include "lib/cairo-type3-glyph-surface.c"
#include "lib/cairo-unicode.c"
#include "lib/cairo-user-font.c"
#include "lib/cairo-wideint.c"
#include "lib/cairo.c"

// files that would be needed if you want to compile for linux.
// currently we just use the cairo library from the system
/*
#include "lib/cairo-ft-font.c"
#include "lib/cairo-xlib-screen.c"
#include "lib/cairo-xlib-surface.c"
#include "lib/cairo-xlib-visual.c"
#include "lib/cairo-type1-subset.c"
#include "lib/cairo-xlib-display.c"
*/

#endif
