#include "Img.h"

void IdePngDes::Save()
{
}

void IdePngDes::Load(const char *_filename)
{
	Clear();
	filename = _filename;
	filetime = FileGetTime(filename);
	Image m = StreamRaster::LoadFileAny(filename);	
	AddImage(filename, m, false);
	HideList();
}
