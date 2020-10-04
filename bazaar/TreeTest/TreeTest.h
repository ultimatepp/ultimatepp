#ifndef _TreeTest_TreeTest_h
#define _TreeTest_TreeTest_h

#include <CtrlLib/CtrlLib.h>
#include <Tree/Tree.h>

namespace Upp {

#define LAYOUTFILE <TreeTest/TreeTest.lay>
#include <CtrlCore/lay.h>

class Element
{
public:
	String name;
	Value value;

	void Xmlize(XmlIO xml) { xml("name", name)("value", value); }
	void Serialize(Stream& s) { s % name % value; }
};

///

class TreeTest : public WithLayout<TopWindow> {
public:
	TreeTest();
	
	Node<One<Element>> root;
	Node<Element> roota;

	NodeB<Element> rootb;
	NodeB<One<Element>> rootc;
	
	MapNode<String, Element> mroota;
};

}

#endif

