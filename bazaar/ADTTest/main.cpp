#include <Core/Core.h>

#include <ADT/ADT.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	// SList tests.
	{
		using ADT::SListNode;
		using ADT::SList;

		struct Node : public SListNode<Node> {
			Node(int i) : i(i) {}
			const int i;
		};

		typedef SList<Node> List;
		Node n0(0), n1(1), n2(2), n3(3), n4(4), n5(5);
		List list;

		list.PushFront(&n0).PushFront(&n1).PushFront(&n2).PushFront(&n3).PushFront(&n4);
		
		for (SList<Node>::ForwardRange r = list.ForwardR(); !r.IsEmpty(); r.PopFront())
			r.Front();

		for (SList<Node>::ForwardRange r = list.ForwardR(); !r.IsEmpty(); r.PopFront())
			Cout() << r.Front().i << " ";
		Cout() << EOL;
	}
	
	// DList tests.
	{
		using ADT::DListNode;
		using ADT::DList;

		struct Node : public DListNode<Node>
		{
			Node(int i) : i(i) {}
			const int i;
		};

		typedef DList<Node> List;
		Node n0(0), n1(1), n2(2), n3(3), n4(4), n5(5);
		List list;

		list << n0 << n1 << n2 << n3 << n4;
		ASSERT(list.GetLast() == &n4);
		
		for (DList<Node>::ForwardRange r = list.ForwardR(); !r.IsEmpty(); r.PopFront())
			r.Front();
		
		n1.ReplaceThisWith(&n5);
		n0.RemoveThis();
	}
} 