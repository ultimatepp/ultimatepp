#ifndef _plugin_tessel_tessel_h_
#define _plugin_tessel_tessel_h_

#include <Core/Core.h>

namespace Upp {

void Tesselate(Vector<Vector<Pointf>>& shape, Vector<Pointf>& vertex, Vector<Tuple<int, int, int>>& triangle, bool evenodd = false);
void Tesselate(Vector<Vector<Pointf>>& shape, Vector<Pointf>& vertex, Vector<int>& index, bool evenodd = false);

};

#endif
