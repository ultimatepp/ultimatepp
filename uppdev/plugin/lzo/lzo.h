#ifndef _lzo_lzo_h
#define _lzo_lzo_h

#include <Core/Core.h>
#include "lib/minilzo.h"

namespace Upp {

String LZOCompress(const void *data, int len);
String LZOCompress(const String& s);
String LZODecompress(const void *data, int len);
String LZODecompress(const String& s);

};

#endif
