#include "ppm.h"

namespace Upp {

int PPMEncoder::GetPaletteCount()
{
	return 0;
}

void PPMEncoder::Start(Size sz)
{
	size = sz;
	format.Set24be(0xff0000, 0x00ff00, 0x0000ff);
	GetStream() << "P6\n" << AsString(sz.cx) << ' ' << AsString(sz.cy) << "\n" << 255 << "\n";
}

void PPMEncoder::WriteLineRaw(const byte *data)
{
	GetStream().Put(data, size.cx * 3);
}

}