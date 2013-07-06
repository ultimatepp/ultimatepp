#ifndef _ProtectClientDemo_ProtectClientDemo_h
#define _ProtectClientDemo_ProtectClientDemo_h

#include <CtrlLib/CtrlLib.h>

#include <ProtectClient/ProtectClient.h>

using namespace Upp;

#define LAYOUTFILE <ProtectClientDemo/ProtectClientDemo.lay>
#include <CtrlCore/lay.h>

class ProtectClientDemo : public WithProtectClientDemoLayout<TopWindow>
{
	private:
		ProtectClient client;
	
	protected:
	
		virtual void onAction(int reason);
		
	public:
		typedef ProtectClientDemo CLASSNAME;
		
		// constructor
		ProtectClientDemo();
		
		// destructor
		~ProtectClientDemo();
};

#endif
