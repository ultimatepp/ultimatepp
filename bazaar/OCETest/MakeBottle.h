#ifndef _MakeBottle_h_
#define _MakeBottle_h_

#include <OCE/OCE.h>

#ifdef Convex
#undef Convex
#endif

#include <TopoDS_Shape.hxx>

TopoDS_Shape MakeBottle(const Standard_Real myWidth, const Standard_Real myHeight, const Standard_Real myThickness);

#endif
