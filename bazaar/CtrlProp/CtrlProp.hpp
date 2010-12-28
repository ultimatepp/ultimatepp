#ifndef _CtrlProp_CtrlProp_hpp
#define _CtrlProp_CtrlProp_hpp

#include "CtrlProp.h"

//type specific maps for the handlers
template<class T> ArrayMap<String, Gate2<T&, const Value&> >& wpmap() { static ArrayMap<String, Gate2<T&, const Value&> > _; return _; }
template<class T> ArrayMap<String, Callback2<const T&, Value&> >& rpmap() { static ArrayMap<String, Callback2<const T&, Value&> > _; return _; }

#endif
