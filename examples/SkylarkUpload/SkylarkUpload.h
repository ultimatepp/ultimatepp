#ifndef _SkylarkUpload_SkylarkUpload_h_
#define _SkylarkUpload_SkylarkUpload_h_

#include <Skylark/Skylark.h>

using namespace Upp;

class SkylarkUpload : public SkylarkApp
{
	private:

		// root path for saving files
		String rootPath;
	
	protected:
	
	public:
	
		// constructor
		SkylarkUpload(String const &rPath);
		
		// get root path
		String const &GetRootPath(void) { return rootPath; }
		
};

SkylarkUpload &Upload();

#endif
