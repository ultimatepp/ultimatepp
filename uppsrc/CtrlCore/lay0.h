//#BLITZ_APPROVE

#define LAYOUT(name, x, y)       struct name##__layid {};
#define UNTYPED(variable, param)
#define ITEM(classname, var, param)
#define END_LAYOUT

#include LAYOUTFILE

#undef  LAYOUT
#undef  UNTYPED
#undef  ITEM
#undef  END_LAYOUT

#define LAYOUT(name, x, y)       template<class T> \
	                             struct With##name : public T, public name##__layid { \
										static Upp::Size GetLayoutSize() { return Upp::Ctrl::LayoutZoom(x, y); }
#define UNTYPED(variable, param)
#define ITEM(classname, var, param)     classname var;
#define END_LAYOUT               };

#include LAYOUTFILE

#undef  LAYOUT
#undef  UNTYPED
#undef  ITEM
#undef  END_LAYOUT

#define LAYOUT(nm, x, y)       template<class T> inline void SetLayout_##nm(T& parent, bool add = false, bool show = false) { \
                                  SetLayout_Size(parent, Upp::Zx(x), Upp::Zy(y));
#define UNTYPED(var, param)       parent.var.param; if(add) parent.Add(parent.var); if(show) parent.var.Show();
#define ITEM(clss, var, param)    UNTYPED(var, param);
#define END_LAYOUT             }

#include LAYOUTFILE

#undef  LAYOUT
#undef  UNTYPED
#undef  ITEM
#undef  END_LAYOUT

#define LAYOUT(nm, x, y)       template<class T, class D> inline void SetLayout_##nm(T& ctrl, D& parent, bool add = false, bool show = false) { \
                                  SetLayout_Size(ctrl, Upp::Zx(x), Upp::Zy(y));
#define UNTYPED(var, param)       parent.var.param; if(add) ctrl.Add(parent.var); if(show) parent.var.Show();
#define ITEM(clss, var, param)    UNTYPED(var, param);
#define END_LAYOUT             }

#include LAYOUTFILE

#undef  LAYOUT
#undef  UNTYPED
#undef  ITEM
#undef  END_LAYOUT

#define LAYOUT(nm, x, y)       template <class L, class D> \
                               void InitLayout(UPP::Ctrl& parent, L& layout, D& uts, nm##__layid&) { \
                                  parent.LayoutIdLiteral(#nm);
#define UNTYPED(var, param)       uts.var.param; uts.var.LayoutIdLiteral(#var); parent.Add(uts.var);
#define ITEM(clss, var, param)    layout.var.param; layout.var.LayoutIdLiteral(#var); parent.Add(layout.var);
#define END_LAYOUT             }

#include LAYOUTFILE

#undef  LAYOUT
#undef  UNTYPED
#undef  ITEM
#undef  END_LAYOUT

#undef  LAYOUTFILE
