#include "zip.h"

namespace Upp {

bool FileUnZip::Create(const char *name)
{
	if(!zip.Open(name))
		return false;
	UnZip::Create(zip);
	return true;
}

void MemUnZip::Create(const void *ptr, int count)
{
	zip.Create(ptr, count);
	UnZip::Create(zip);
}

void StringUnZip::Create(const String& s)
{
	zip.Open(s);
	UnZip::Create(zip);
}

}
