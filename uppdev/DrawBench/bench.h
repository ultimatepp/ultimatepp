#ifndef _DrawBench_bench_h_
#define _DrawBench_bench_h_

#include <Painter/Painter.h>
#include <plugin/cairo/CairoDraw.h>

using namespace Upp;

void PaintLion(Painter *sw, Cairo *ca, Draw *w);
void DrawPythagorasTree(Size sz, Painter*sw, Cairo *ca);

#endif
