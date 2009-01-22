#ifndef _PainterExamples_Examples_h_
#define _PainterExamples_Examples_h_

#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <PainterExamples/Test.iml>
#include <Draw/iml_header.h>

#define LAYOUTFILE <PainterExamples/Examples.lay>
#include <CtrlCore/lay.h>

void RegisterExample(const char *name, void (*ex)(Painter& pw));

#endif
