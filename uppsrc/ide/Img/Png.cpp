#include "Img.h"

void IdePngDes::Save()
{
	if(GetCount())
		SaveChangedFile(filename, PNGEncoder().SaveString(GetImage(0)), true);
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
