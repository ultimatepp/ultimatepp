#include "Tutorial.h"

void Bidirectional()
{
	/// .Bidirectional containers

	/// `Vector` and `Array` containers allow fast adding and removing elements at the end of
	/// sequence. Sometimes, same is needed at begin of sequence too (usually to support FIFO
	/// queues). `BiVector` and `BiArray` are optimal for this scenario:
	
	BiVector<int> n;
	n.AddHead(1);
	n.AddTail(2);
	n.AddHead(3);
	n.AddTail(4);
	DUMP(n);
	
	///

	n.DropHead();
	DUMP(n);
	
	///

	n.DropTail();
	DUMP(n);
	
	///

	struct Val {
		virtual String ToString() const = 0;
		virtual ~Val() {}
	};
	
	struct Number : Val {
		int n;
		virtual String ToString() const { return AsString(n); }
	};
	
	struct Text : Val {
		String s;
		virtual String ToString() const { return s; }
	};

	BiArray<Val> num;
	num.CreateHead<Number>().n = 3;
	num.CreateTail<Text>().s = "Hello";
	num.CreateHead<Text>().s = "World";
	num.CreateTail<Number>().n = 2;

	DUMP(num);
	
	///
}