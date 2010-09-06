#include "GridCtrlTest.h"

void TestPropertyGrid::Init()
{
	
	grid.AddColumn("Property");
	grid.AddColumn("Value");
	
	grid.Add("Name", "Window");
	grid.SetCtrl(1, new EditString());
	grid.Add("Title", "User account");
	grid.SetCtrl(1, new EditString());
	grid.Add("Width", 500);
	grid.SetCtrl(1, new EditInt());
	grid.Add("Height", 600);
	grid.SetCtrl(1, new EditInt());
	grid.Add("Editable", "false");
	grid.Add("Background", Red());
	grid.SetCtrl(1, new ColorPusher());
	grid.Add("Hint", "This window is big");
	grid.SetCtrl(1, new EditString());
	
	grid.DrawFocus();
	
	
}
