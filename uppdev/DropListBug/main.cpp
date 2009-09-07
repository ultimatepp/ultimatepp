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

		dropList.AddSeparator();
		for(int i=0;i<1000;i++){
			dropList.Add(i,Format("%d",i));
			if(((i+1)%3)==0) dropList.AddSeparator();
		}
		dropList.AddSeparator();
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
	bug.Run();
}
