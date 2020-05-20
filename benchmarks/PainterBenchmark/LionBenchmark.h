#ifndef _LionBenchmark_LionBenchmark_h
#define _LionBenchmark_LionBenchmark_h

#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <PainterBenchmark/test.iml>
#include <Draw/iml_header.h>

void PaintLion(Painter& sw);
void PaintLionLarge(Painter& sw);
void PythagorasTree(Painter& sw);
void Demo(Painter& sw);
void ClippingExample(Painter& sw);
void PolyPolygon(Painter& sw);
void TextOnPath(Painter& sw);
void TextRotated(Painter& sw);
void Dash(Painter& sw);
void SomeRocks(Painter& sw);

#endif
