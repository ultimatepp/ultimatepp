#ifndef _Oce2Upp_Oce2Upp_h
#define _Oce2Upp_Oce2Upp_h

#include <CtrlLib/CtrlLib.h>
#include <Controls4U/Controls4U.h>

using namespace Upp;

#define LAYOUTFILE <Oce2Upp/Oce2Upp.lay>
#include <CtrlCore/lay.h>

class Oce2Upp : public WithSelectorLayout<TopWindow>
{
	private:
	
		bool canceled;
	
		// root OCE folder and various subfolders
		String oceRoot;
		
		// various needed subfolder
		String admDir, incDir, srcDir, drvDir;
		
		// root for generated packages
		String destRoot;
		
		// name of builder package (and assembly name too)
		String builderName;
		
		// name of OCE user package
		String oceName;

		// maps toolkits to packages
		VectorMap<String, Vector<String> > toolkits;
		
		// packages dependencies
		VectorMap<String, Vector<String> > dependencies;

		// Gets directory content
		Vector<String> GetDirectoryContent(const String &Path, bool Folders);
		
		// reads an UDLIST file to gather modules/toolkits mapping
		bool ReadUDLIST(void);

		// builds database
		bool BuildDb(void);
		
		// copy include files from inc and various src folders
		// in 'include' folder
		bool CopyIncludes(String const &pacRoot);
		
		// create the oce-config.h
		bool BuildConfig(String const &pacRoot);

		// create a main package file -- by now, dummy, just to avoid build error
		bool BuildMain(String const &pacRoot);

		// creates the UPP library builder  package
		bool MakeBuilderPackage(void);

		// creates the UPP OCE package
		bool MakeOCEPackage(void);

		// callbacks
		void okCb(void);
		void cancelCb(void);
		
		String libPrefix;

	protected:
	
	public:
	
		typedef Oce2Upp CLASSNAME;
	
		Oce2Upp();
		~Oce2Upp();
	
};

#endif
