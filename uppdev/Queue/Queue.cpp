#include <Core/Core.h>

using namespace Upp;

// Note that it is totally MT broken.... (can only be used in single thread)

typedef unsigned char byte;

class Queue {
	struct Qitem {
		byte next;
		byte value[15];
	};
	static Qitem buffer[128];
	static int   freelist;
	
	byte head_q;
	byte head_i; 
	byte tail_q;
	byte tail_i;
	
	enum { NIL = 255 };
	
	void Nil()                       { head_q = tail_i = NIL; }
	static void Free(int q);
	
public:
	bool  IsEmpty() const            { return head_q == NIL; }
	void  Put(byte value);
	byte  Get();

	Queue();
	~Queue();
};

Queue::Qitem Queue::buffer[128];
int          Queue::freelist = -1;

void Queue::Free(int q)
{
	buffer[q].next = freelist;
	freelist = q;
}

Queue::Queue()
{
	if(freelist < 0) {
		buffer[0].next = NIL;
		for(int i = 1; i < 128; i++)
			buffer[i].next = i - 1;
		freelist = 127;
	}
	Nil();
}

Queue::~Queue()
{
	int q = head_q;
	while(q != NIL) {
		int w = q;
		q = buffer[q].next;
		Free(w);
	}
}

void Queue::Put(byte value)
{
	Qitem *m;
	if(tail_i >= 15) {
		if(freelist == NIL)
			Panic("Out of memory");
		int q = freelist;
		freelist = buffer[freelist].next;
		if(IsEmpty()) {
			head_q = q;
			head_i = 0;
		}
		else
			buffer[tail_q].next = q;
		tail_q = q;
		tail_i = 0;
		m = &buffer[q];
		m->next = NIL;
	}
	else
		m = &buffer[tail_q];
	m->value[tail_i++] = value;
}

byte Queue::Get()
{
	ASSERT(!IsEmpty());
	if(head_i >= 15) {
		int q = head_q;
		head_q = buffer[head_q].next;
		head_i = 0;
		Free(q);
	}
	int val = buffer[head_q].value[head_i++];
	if(head_q == tail_q && head_i == tail_i) {
		Free(head_q);
		Nil();
	}
	return val;
}

CONSOLE_APP_MAIN
{
	{
		BiVector<byte> ref[16];
		Queue q[16];
		int total = 0;
		for(int i = 0; i < 100000000; i++) {
			if(i % 1000000 == 0)
				Cout() << i << '\n';
			int ii = Random() & 15;
			if((Random() & 65535) < 32900 && total < 128 * 10) {
				total++;
				byte v = (byte)Random();
				q[ii].Put(v);
				ref[ii].AddTail(v);
				DDUMP(total);
			}
			else
				if(!q[ii].IsEmpty()) {
					total--;
					ASSERT(q[ii].Get() == ref[ii].Head());
					ref[ii].DropHead();
					if(q[ii].IsEmpty())
						LOG("Empty!");
				}
		}
	}
	
	
	{
		Queue a, b;
		for(int i = 0; i < 500; i++) {
			a.Put(i);
			for(int j = 0; j < 500; j++)
				b.Put(j);
			for(int j = 0; j < 500; j++)
				ASSERT((byte)j == b.Get());
		}
		for(int i = 0; i < 500; i++)
			ASSERT(a.Get() == (byte)i);
	}
	Queue a1;
	for(int i = 0; i < 128 * 15; i++)
		a1.Put(i);
	for(int i = 0; i < 128 * 15; i++)
		DUMP((int)a1.Get());
	for(int i = 0; i < 128 * 15; i++)
		a1.Put(i);
	for(int i = 0; i < 128 * 15; i++)
		DUMP((int)a1.Get());
}

