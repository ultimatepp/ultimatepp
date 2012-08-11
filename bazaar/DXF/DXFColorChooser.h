#ifndef _TimberControls_DXFColorChooser_h_
#define _TimberControls_DXFColorChooser_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class DXFColorChooser : public ParentCtrl
{
	private:
	
		// stored DXF color
		int color;

		// handle mouse clicks
		void LeftDown(Point p, dword);
	
	protected:
	
		// paint control
		void Paint(Draw &w);
	
	public:
	
		typedef DXFColorChooser CLASSNAME;
		
		// constructor
		DXFColorChooser();
		
		// getters/setters
		operator int(void) { return color; }
		int operator~(void) { return color; }
		int GetColor(void) { return color; }

		int operator=(int c) { color = c; Refresh(); return c; }
		int operator <<= (int c) { color = c; Refresh(); return c; }
		Callback operator <<= (Callback c) { return Ctrl::operator<<=(c); }
		DXFColorChooser &SetColor(int c) { color = c; Refresh(); return *this; }
};

#endif
