//#BLITZ_APPROVE

#ifndef KEYGROUPNAME
#ifdef  KEYNAMESPACE
#define KEYGROUPNAME #KEYNAMESPACE
#else
#define KEYGROUPNAME ""
#endif
#endif

#define KEY(id, name, def)        KeyInfo& COMBINE(AK_, id)();
#define KEY2(id, name, def, def2) KeyInfo& COMBINE(AK_, id)();

#ifdef KEYNAMESPACE
namespace KEYNAMESPACE {
#endif

#include KEYFILE

#ifdef KEYNAMESPACE
};
#endif

#ifdef KEYNAMESPACE
#undef KEYNAMESPACE
#endif

#undef KEYGROUPNAME
#undef KEY
#undef KEY2
#undef KEYFILE
