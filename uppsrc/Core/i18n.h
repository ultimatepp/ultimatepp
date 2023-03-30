struct LngEntry__ {
	int         lang;
	const char *text;
};

void    AddModule(const LngEntry__ *entry, const char *modulename = NULL);


const char *t_GetLngString(const char *id);  // Only works with string literals because of caching !

#define     LNG_enUS LNG_('E','N','U','S')

const char *GetENUS(const char *id);

// t_ and tt_ would cause translation files sync to report error, therefore it
// has to be included from file that is not part of package...
#include "t_.h"

/* t_.h content:
#ifdef t_
#undef t_
#endif

#ifdef tt_
#undef tt_
#endif

#ifndef t_h
#define t_h

inline const char *t_(const char *s)  { return t_GetLngString(s); }
inline const char *tt_(const char *s) { return s; }

#endif
*/

String     GetLngString(const char *id); // Only works with string literals because of caching !

String     GetLngString(int lang, const char *id);

Index<int> GetLngSet();
Index<int> GetLngSet(const String& module);
void       SaveLngFile(FileOut& out, int lang, int lang2 = 0);
bool       LoadLngFile(const char *file);

void       SetCurrentLanguage(int lang);
int        GetCurrentLanguage();

void       LngSetAdd(const char *id, int lang, const char *txt, bool addid);
