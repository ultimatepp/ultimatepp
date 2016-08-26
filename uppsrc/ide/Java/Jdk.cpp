#include "Java.h"

namespace Upp {

String Jdk::GetDownloadUrl()
{
	return "http://www.oracle.com/technetwork/java/javase/downloads/index.html";
}

Jdk::Jdk()
{
	
}

Jdk::Jdk(const String& path)
{
	this->path = path;
}

Jdk::~Jdk()
{
	
}

bool Jdk::Validate() const
{
	if(!FileExists(GetJavacPath())) return false;
	
	return true;
}

}
