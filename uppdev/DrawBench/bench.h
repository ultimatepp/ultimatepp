#ifndef _DrawBench_bench_h_
#define _DrawBench_bench_h_

#include <SDraw/SDraw.h>
#include <plugin/cairo/CairoDraw.h>

using namespace Upp;

void PaintLion(SDraw *sw, Cairo *ca, Draw *w);
void DrawPythagorasTree(Size sz, SDraw *sw, Cairo *ca);

#endif
