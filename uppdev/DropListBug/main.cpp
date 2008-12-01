#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <DropListBug/DropListBug.lay>
#include <CtrlCore/lay.h>

class DropListBug : public WithDropListBugLayout<TopWindow> 
{
private:
	char chr;
	
public:
	typedef DropListBug CLASSNAME;
	
	DropListBug()
	{
		chr = 'a';
		CtrlLayout(*this, "Window title");
		addButton <<= THISBACK(AddButtonClicked);
		removeButton <<= THISBACK(RemoveButtonClicked);
		dropList.Add("A");
		dropList.Add("B");
		dropList.Add("C");
		dropList.Add("D");
		dropList.Add("E");
	}
	
	void AddButtonClicked()
	{
		dropList.Add(String().Cat() << chr++);
		dropList.SetIndex(dropList.GetCount() - 1);
	}
	
	void RemoveButtonClicked()
	{
		if (dropList.GetCount() == 0)
			return;
		
		dropList.Remove(dropList.GetIndex());
	}
};

GUI_APP_MAIN
{
	DropListBug bug;
	bug.Open();
	bool quit = false;
	while(!quit) {
		Sleep(1000);
		Ctrl::ProcessEvents(&quit);
	}
}
