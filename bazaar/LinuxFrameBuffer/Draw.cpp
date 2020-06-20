#include "LinuxFrameBuffer.h"

using namespace Upp;

SystemDraw& LinuxFrameBuffer::BeginDraw()
{
    return screen.GetSystemDraw();
}


void LinuxFrameBuffer::CommitDraw()
{
    memcpy(screen.GetBuffer(), ~screen.GetImageBuffer(), screen.GetImageBuffer().GetLength() * sizeof(RGBA));
}
