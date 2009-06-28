#include "ide.h"

Vector<String> GetAllUses(const char *path)
{
	Index<String> upp;
	return upp.PickKeys();
/*	for(FindFile ff(ConfigFile("*.var")); ff; ff.Next())
		if(ff.IsFile()) {
			
		}
			varlist.Add(GetFileTitle(ff.GetName()));*/
	
}

void CopyPackage(const char *path)
{
	
}

void RenamePackage(const char *path)
{

}
