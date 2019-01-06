#include "LinuxFrameBuffer.h"

using namespace Upp;

SystemDraw& LinuxFrameBuffer::BeginDraw()
{
	return screen.sysdraw;
}


void LinuxFrameBuffer::CommitDraw()
{
	memcpy(screen.buffer, ~screen.image, screen.image.GetLength() * sizeof(RGBA));
}
