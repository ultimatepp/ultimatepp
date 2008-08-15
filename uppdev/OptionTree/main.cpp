#include <CtrlLib/CtrlLib.h>

class AWindow : public TopWindow
{
public:
	typedef AWindow CLASSNAME;
	Option _optA, _optAA, _optAAA, _optAAB, _optAAC;
	OptionTree _tree;
	Button _btn;

	AWindow()
	{
		_tree.SetRoot(_optA, "Root");
		_tree.Add(0, _optAA, "Node");
		_tree.Add(1, _optAAA, "Leaf 1");
		_tree.Add(1, _optAAB, "Leaf 2");
		_tree.Add(1, _optAAC, "Leaf 3");


		_tree.LeftPosZ(0, 140).TopPosZ(0, 164);
		_btn.LeftPosZ(0, 140).TopPosZ(164, 20);
		_btn <<= THISBACK(ButtonPush);

		Add(_tree);
		Add(_btn);

		_tree.Set(3, 1); // Set through the TreeCtrl::Set - no call to SetOption
	}

	void ButtonPush()
	{
		_tree.Set(3, 1); // Set through the TreeCtrl::Set - no call to SetOption
	}
};

GUI_APP_MAIN
{
	AWindow w;

	w.Run();
}
