#include "GridCtrlTest.h"

void TestFocusLost::Init()
{
	Add(spl.SizePos());
	spl.Horz(grid0, grid1);
	
	grid0.AddColumn("Key");
	grid0.AddColumn("Value");
	
	grid1.AddColumn("Key");
	grid1.AddColumn("Value");
}
