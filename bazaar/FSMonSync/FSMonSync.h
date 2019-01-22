#ifndef _FSMonTest_FSMonSync_h
#define _FSMonTest_FSMonSync_h

#include <FSMon/FSMon.h>

using namespace Upp;

#define LAYOUTFILE <FSMonSync/FSMonSync.lay>
#include <CtrlCore/lay.h>



class FSMonSync : public WithFSMonSyncLayout<TopWindow>
{
	private:
		void Log(String const &s);
		
		void startCb(void);
		void stopCb(void);
		void quitCb(void);
		
		FSMon fsmMon;
		
		void monitorCb(void);
		
		String srcFolder, dstFolder;
		
		// gets destination path given source one
		String GetDestPath(String const &srcPath);
		
		bool started;
	
	protected:
	public:
		typedef FSMonSync CLASSNAME;

		FSMonSync();
};

#endif
