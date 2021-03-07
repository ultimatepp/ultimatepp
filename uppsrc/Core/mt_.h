// This for layouts, to avoid adding Upp:: to each t_

#ifdef t_
#undef t_
#endif

#ifdef tt_
#undef tt_
#endif

#define t_(x)          Upp::t_GetLngString(x)
#define tt_(x)         x
