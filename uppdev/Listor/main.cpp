#include <Core/Core.h>

class ListVector {
	struct Node {
		int prev;
		int next;
	};

	int head;
	int tail;
	int free;

	Vector<Node> node;

public:
	int  GetHead() const   { return head; }
	int  GetTail() const   { return tail; }

	int  AddHead();
	int  AddTail();

	void Unlink(int q);
}

int ListVector::AddHead()
{
	if(free < 0) {
		int q = node.GetCount();
		node.Add();
		node.Top().prev =
	}
}

int ListVector::AddTail()
{
}

void ListVector::Unlink(int q)
{
}


struct ImageMaker {
	virtual String GetKey();
	virtual Image  Render();

	virtual ~CachedImage();
};


struct ScaleRasterMaker {
	String data;
	int64  id;
	int    cx;
	int    cy;

	virtual String GetKey();
	virtual Image  Render();
};

String GetKey() {
	String s;
	s.Cat((const char *)&id, sizeof(id));
}

Image ScaleRasterMaker::Render()
{

}
