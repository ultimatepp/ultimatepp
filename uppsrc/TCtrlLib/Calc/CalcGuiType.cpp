#include "TCtrlLibCalc.h"
#pragma hdrstop
#pragma BLITZ_APPROVE

NAMESPACE_UPP

void UseCalcGuiType() {}

RegisterCalcTypeName(Pointf,             t_("point"))
RegisterCalcTypeName(Rectf,              t_("rectangle"))
RegisterCalcTypeName(Color,              t_("color"))

static String GroupGeom()   { return "Geometrické objekty"; }

static double CxP(Pointf pt)               { return pt.x; }
static double CyP(Pointf pt)               { return pt.y; }
static Pointf CpointNN(double x, double y) { return Pointf(x, y); }
static String CtextP(Pointf pt)            { return AsString(pt); }

FDECLP0(x, P, &GroupGeom)
FDECLP0(y, P, &GroupGeom)
FDECLP0(point, NN, &GroupGeom)
FDECLP0(text, P, &GroupString)

static double CleftR(const Rectf& rc)         { return rc.left; }
static double CxminR(const Rectf& rc)         { return rc.left; }
static double CtopR(const Rectf& rc)          { return rc.top; }
static double CyminR(const Rectf& rc)         { return rc.top; }
static double CrightR(const Rectf& rc)        { return rc.right; }
static double CxmaxR(const Rectf& rc)         { return rc.right; }
static double CbottomR(const Rectf& rc)       { return rc.bottom; }
static double CymaxR(const Rectf& rc)         { return rc.bottom; }
static double CxcenterR(const Rectf& rc)      { return (rc.left + rc.right) / 2; }
static double CycenterR(const Rectf& rc)      { return (rc.top + rc.bottom) / 2; }
static Pointf CtopleftR(const Rectf& rc)      { return rc.TopLeft(); }
static Pointf CtopcenterR(const Rectf& rc)    { return rc.TopCenter(); }
static Pointf CtoprightR(const Rectf& rc)     { return rc.TopRight(); }
static Pointf CcenterleftR(const Rectf& rc)   { return rc.CenterLeft(); }
static Pointf CcenterR(const Rectf& rc)       { return rc.CenterPoint(); }
static Pointf CcenterrightR(const Rectf& rc)  { return rc.CenterRight(); }
static Pointf CbottomleftR(const Rectf& rc)   { return rc.BottomLeft(); }
static Pointf CbottomcenterR(const Rectf& rc) { return rc.BottomCenter(); }
static Pointf CbottomrightR(const Rectf& rc)  { return rc.BottomRight(); }
static Rectf  CrectNNNN(double l, double t, double r, double b) { return Rectf(l, t, r, b); }
static Rectf  CrectP(Pointf pt)               { return PointfRectf(pt); }
static Rectf  CrectPP(Pointf p, Pointf q)     { return SortRectf(p, q); }
static double CwidthR(const Rectf& rc)        { return rc.Width(); }
static double CheightR(const Rectf& rc)       { return rc.Height(); }
static String CtextR(const Rectf& rc)         { return AsString(rc); }

FDECLP0(left,         R,     &GroupGeom)
FDECLP0(top,          R,     &GroupGeom)
FDECLP0(right,        R,     &GroupGeom)
FDECLP0(bottom,       R,     &GroupGeom)
FDECLP0(xmin,         R,     &GroupGeom)
FDECLP0(ymin,         R,     &GroupGeom)
FDECLP0(xmax,         R,     &GroupGeom)
FDECLP0(ymax,         R,     &GroupGeom)
FDECLP0(xcenter,      R,     &GroupGeom)
FDECLP0(ycenter,      R,     &GroupGeom)
FDECLP0(topleft,      R,     &GroupGeom)
FDECLP0(topcenter,    R,     &GroupGeom)
FDECLP0(topright,     R,     &GroupGeom)
FDECLP0(centerleft,   R,     &GroupGeom)
FDECLP0(center,       R,     &GroupGeom)
FDECLP0(centerright,  R,     &GroupGeom)
FDECLP0(bottomleft,   R,     &GroupGeom)
FDECLP0(bottomcenter, R,     &GroupGeom)
FDECLP0(bottomright,  R,     &GroupGeom)
FDECLP0(width,        R,     &GroupGeom)
FDECLP0(height,       R,     &GroupGeom)
FDECLP0(rect,         NNNN,  &GroupGeom)
FDECLP0(rect,         P,     &GroupGeom)
FDECLP0(rect,         PP,    &GroupGeom)
FDECLP0(text,         R,     &GroupString)

END_UPP_NAMESPACE
