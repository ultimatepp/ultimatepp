#ifndef t_h
#define t_h

#ifdef t_
#undef t_
#endif

#ifdef tt_
#undef tt_
#endif

inline const char *t_(const char *s)  { return t_GetLngString(s); }
inline const char *tt_(const char *s) { return s; }

// #define t_(x)          t_GetLngString(x)
// #define tt_(x)         x
#endif