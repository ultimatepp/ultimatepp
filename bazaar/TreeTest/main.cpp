#include "TreeTest.h"

TreeTest::TreeTest()
{
	CtrlLayout(*this, "Window title");

	//Node<One<Element> >

	root.SetAsRoot();
	root.leaf.Create();
	root.leaf->name = "/";
	root.SetCount(3);
	Node<One<Element> > & child = root[2];
	ASSERT(child.GetParent() == &root);
	ASSERT(child.GetRoot() == &root);
	child.leaf.Create();
	child.leaf->name = "Hallo";
	String & s = child.GetParent()->leaf->name;
	s = "root";
	String & ss = root.leaf->name;
	ASSERT(ss.IsEqual(s));
	
	//Node<Element>

	roota.SetAsRoot();
	roota.leaf.name = "/";
	roota.leaf.value = "Hallo";
	roota.SetCount(3);
	Node<Element> & childa = roota[2];
	childa.leaf.name = "Servus";
	childa.leaf.value = 123;
	ASSERT(childa.GetParent() == &roota);
	ASSERT(childa.GetRoot() == &roota);
	String & sa = childa.GetParent()->leaf.name;
	sa = "root";
	String & ssa = roota.leaf.name;
	ASSERT(ssa.IsEqual(sa));

#if 1
	StoreAsXMLFile(roota, "MyTree", "check.xml");
	Node<Element> roota_;
	LoadFromXMLFile(roota_, "check.xml");
	StoreAsXMLFile(roota_, "MyTree", "check1.xml");
#endif

#if 1
	StoreToFile(roota, "check.cfg");
	Node<Element> roota__;
	LoadFromFile(roota__, "check.cfg");
	StoreToFile(roota__, "check1.cfg");
#endif

	//Leaf<Element>

	rootb.SetAsRoot();
	rootb.name = "/";
	rootb.value = "Hallo";
	rootb.SetCount(3);
	Leaf<Element> & childb = rootb[2];
	childb.name = "Servus";
	childb.value = 123;
	ASSERT(childb.GetParent() == &rootb);
	ASSERT(childb.GetRoot() == &rootb);
	String & sb = childb.GetParent()->name;
	sb = "root";
	String & ssb = rootb.name;
	ASSERT(ssb.IsEqual(sb));

#if 0
	StoreAsXMLFile(rootb, "CHECK");
	Leaf<Element> rootb_;
	LoadFromXMLFile(rootb_);
	StoreAsXMLFile(rootb_, "CHECK1");
#endif

#if 0
	StoreToFile(rootb);
	Node<Element> rootb__;
	LoadFromFile(rootb__);
	StoreToFile(rootb__);
#endif

	//Leaf<One<Element> >

	rootc.SetAsRoot();
	rootc.One<Element>::Create();
	rootc->name = "/";
	rootc.SetCount(3);
	Leaf<One<Element> > & childc = rootc[2];
	ASSERT(childc.GetParent() == &rootc);
	ASSERT(childc.GetRoot() == &rootc);
	childc.One<Element>::Create();
	childc->name = "Hallo";
	String & sc = childc.GetParent()->operator*().name;
	sc = "root";
	String & ssc = rootc->name;
	ASSERT(ssc.IsEqual(sc));

	int x = 123;
}

GUI_APP_MAIN
{
	TreeTest().Run();
}

