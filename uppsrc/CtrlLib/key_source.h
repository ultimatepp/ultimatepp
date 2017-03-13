//#BLITZ_APPROVE

#ifndef KEYGROUPNAME
#ifdef  KEYNAMESPACE
#define KEYGROUPNAME #KEYNAMESPACE
#else
#define KEYGROUPNAME ""
#endif
#endif

#include <CtrlLib/akt_.h>

#ifdef KEYNAMESPACE
namespace KEYNAMESPACE {
#endif

#define KEY(id, name, def)         KeyInfo& COMBINE(AK_, id)() { static KeyInfo x = { name, { def } }; return x; }
#define KEY2(id, name, def1, def2) KeyInfo& COMBINE(AK_, id)() { static KeyInfo x = { name, { def1, def2 } }; return x; }
#include KEYFILE
#undef  KEY
#undef  KEY2

#define KEY(id, name, def)         RegisterKeyBinding(KEYGROUPNAME, #id, COMBINE(AK_, id));
#define KEY2(id, name, def1, def2) RegisterKeyBinding(KEYGROUPNAME, #id, COMBINE(AK_, id));
INITBLOCK {
#include KEYFILE
}
#undef KEY
#undef KEY2

#ifdef KEYNAMESPACE
}
#endif

#include <Core/t_.h>

#ifdef KEYNAMESPACE
#undef KEYNAMESPACE
#endif

#undef KEYGROUPNAME
#undef KEYFILE
