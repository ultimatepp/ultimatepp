#include "Designers.h"

void IdePngDes::Save()
{
	if(GetCount())
		SaveChangedFileFinish(filename, PNGEncoder().SaveString(GetImage(0)));
	filetime = FileGetTime(filename);
}

void IdePngDes::Load(const char *_filename)
{
	Clear();
	filename = _filename;
	filetime = FileGetTime(filename);
	Image m = StreamRaster::LoadFileAny(filename);
	AddImage(filename, m, false);
	SingleMode();
}

void IdePngDes::Create(const char *_filename)
{
	Clear();
	filename = _filename;
	filetime = GetSysTime();
	Image m = CreateImage(Size(16, 16), Null);
	AddImage(filename, m, false);
	SingleMode();
}
