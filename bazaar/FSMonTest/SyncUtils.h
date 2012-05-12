#ifndef _FSMonSync_SyncUtils_h_
#define _FSMonSync_SyncUtils_h_

#include <Core/Core.h>

using namespace Upp;

// synchronize a source file with a dest one
// checks filestamp, size and attributes
bool SyncFile(String const &src, String const &dst);

// synchronizes a whole source folder with a dest one
// uses SyncFile for single files synchronization
bool SyncFolder(String const &src, String const &dst);

#endif
