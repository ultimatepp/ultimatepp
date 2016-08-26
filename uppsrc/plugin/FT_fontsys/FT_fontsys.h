#ifndef _plugin_FT_fontsys_FT_fontsys_h_
#define _plugin_FT_fontsys_FT_fontsys_h_

#include <Draw/Draw.h>

namespace Upp {

enum FtStyle {
	FtBOLD = 1,
	FtITALIC = 2
};

void SetFileFont(int face, const char *path, dword style = 0);
void SetMemoryFont(int face, const byte *data, int size, dword style = 0);

}

#endif
