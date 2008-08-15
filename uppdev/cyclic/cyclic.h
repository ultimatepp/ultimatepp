#ifndef _cyclic_cyclic_h
#define _cyclic_cyclic_h
#include <time.h>
#include <stdlib.h>

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <cyclic/cyclic.lay>
#include <CtrlCore/lay.h>

class cyclic_space : public TopWindow {
public:
	typedef cyclic_space CLASSNAME;
	cyclic_space();
	virtual void Paint(Draw& w);
	virtual void LeftDown(Point, dword);
	virtual bool Key(dword key, int count) {
		if (key == K_SPACE) {
			for(int i=0;i<1000;i++)
			{
		    	step();	
			}	
			return true;	
		}
		return false;
	}
	void step();
	void reset(int cn);
	void change(int num, Color ncs[16]);
	void calculate();
	int fd[640][480];
	Color col[16];
	int n;
};

class cyclic : public WithcyclicLayout<TopWindow> {
public:
	typedef cyclic CLASSNAME;
	cyclic();
	void Click();
	void Setcols(Color pc[16]);
private:
	ColorPusher cols[16];
	Color dc[16];
	Label lbl[16]; 	
};
cyclic_space par;

#endif

