struct LngEntry__ {
	int         lang;
	const char *text;
};

void    AddModule(const LngEntry__ *entry, const char *modulename = NULL);


const char *t_GetLngString(const char *id);

#define     LNG_enUS LNG_('E','N','U','S')

const char *GetENUS(const char *id);

// t_ and tt_ would cause translation files sync to report error, so that
// has to be included from file that is not part of package...
#include "t_.h"

/* t_.h:
#ifdef t_
#undef t_
#endif

#ifdef tt_
#undef tt_
#endif

#define t_(x)          t_GetLngString(x)
#define tt_(x)         x
*/

/* lt_.h:
#ifdef t_
#undef t_
#endif

#ifdef tt_
#undef tt_
#endif

#define t_(x)          x
#define tt_(x)         x
*/

String     GetLngString(const char *id);

String     GetLngString(int lang, const char *id);

Index<int> GetLngSet();
void       SaveLngFile(FileOut& out, int lang);
bool       LoadLngFile(const char *file);

void       SetCurrentLanguage(int lang);

void       LngSetAdd(const char *id, int lang, const char *txt, bool addid);
