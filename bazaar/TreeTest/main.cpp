#include "TreeTest.h"

TreeTest::TreeTest()
{
	CtrlLayout(*this, "Window title");

	//Tree<One<Element> >

	root.SetAsRoot();
	root.leaf.Create<Element>();
	root.leaf->name = "/";
	root.SetCount(3);
	Node<One<Element> > & child = root[2];
	ASSERT(child.GetParent() == &root);
	ASSERT(child.GetRoot() == &root);
	child.leaf.Create<Element>();
	child.leaf->name = "Hallo";
	String & s = child.GetParent()->leaf->name;
	s = "root";
	String & ss = root.leaf->name;
	ASSERT(ss.IsEqual(s));
	
	//Node<Element>

	roota.SetAsRoot();
	roota.leaf.name = "/";
	roota.SetCount(3);
	Node<Element> & childa = roota[2];
	childa.leaf.name = "Hallo";
	ASSERT(childa.GetParent() == &roota);
	ASSERT(childa.GetRoot() == &roota);
	String & sa = childa.GetParent()->leaf.name;
	sa = "root";
	String & ssa = roota.leaf.name;
	ASSERT(ssa.IsEqual(sa));
	
	int x = 123;
}

GUI_APP_MAIN
{
	TreeTest().Run();
}

