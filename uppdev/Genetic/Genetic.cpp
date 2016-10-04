#include <Core/Core.h>

enum {
	MAXSTACK = 10000
};

struct Op {
	unsigned op:17;
	unsigned nextn:5;
	unsigned nextz:5;
	unsigned nextp:5;
};

enum OpCode {
	MAIN,
	DUP,
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	DROP,
	SWAP,
	OUTPUT,
	STORE,
	LOAD,

	PUSH = 0x10000,
};

struct Gen : Moveable<Gen> {
	Op op[32];
};

struct Invalid {};

void Interpret(Vector<int>& stack, Vector<Gen>& genom, int geni,
               int& maxsteps, int& maxdepth, String& out, int *memory)
{
	if(--maxdepth < 0)
		throw Invalid();
	Op *gen = genom[geni].op;
	int i = 0;
	for(;;) {
		if(stack.GetCount() > MAXSTACK || --maxsteps <= 0)
			throw Invalid();
		Op code = gen[i];
		switch(code.op) {
		case DUP:
			if(stack.GetCount()) {
				int q = stack.Top();
				stack.Add(q);
			}
			break;
		case ADD:
			if(stack.GetCount() > 1) {
				int q = stack.Pop();
				stack.Top() = stack.Top() + q;
			}
			break;
		case SUB:
			if(stack.GetCount() > 1) {
				int q = stack.Pop();
				stack.Top() = stack.Top() - q;
			}
			break;
		case MUL:
			if(stack.GetCount() > 1) {
				int q = stack.Pop();
				stack.Top() = stack.Top() * q;
			}
			break;
		case DIV:
			if(stack.GetCount() > 1) {
				int q = stack.Pop();
				stack.Top() = stack.Top() / q;
			}
			break;
		case MOD:
			if(stack.GetCount() > 1) {
				int q = stack.Pop();
				stack.Top() = stack.Top() % q;
			}
			break;
		case DROP:
			stack.Drop();
			break;
		case SWAP:
			if(stack.GetCount() > 1)
				Swap(stack.Top(), stack[stack.GetCount() - 2]);
			break;
		case STORE:
			if(stack.GetCount() > 1) {
				int adr = stack.Pop();
				int data = stack.Pop();
				memory[adr] = stack[data];
			}
			break;
		case LOAD:
			if(stack.GetCount()) {
				int adr = stack.Pop();
				stack.Add(memory[adr]);
			}
			break;
		case OUTPUT:
			if(stack.GetCount())
				out << (char)stack.Pop();
			break;
		default:
			if(code.op < PUSH) {
				if((int)code.op >= genom.GetCount())
					throw Invalid();
				Interpret(stack, genom, code.op, maxsteps, maxdepth, out, memory);
			}
			else
				stack.Add((int16)code.op);
		}
		i = code.nextz;
		if(stack.GetCount()) {
			int q = stack.Top();
			if(q < 0)
				i = code.nextn;
			if(q > 0)
				i = code.nextp;
		}
		if(i == 0)
			return;
	}
}

String Interpret(Vector<Gen>& genom) {
	String out;
	try {
		int maxsteps = 10000;
		int maxdepth = 100;
		Vector<int> stack;
		int memory[65536];
		memset(memory, 0, 65536);
		Interpret(stack, genom, 0, maxsteps, maxdepth, out, memory);
	}
	catch(...) {}
	return out;
}

struct MakeGen : Gen {
	int i;
	int Op(int code, int n, int z, int p) {
		ASSERT(i < 32);
		::Op pp;
		pp.op = code;
		pp.nextn = n;
		pp.nextz = z;
		pp.nextp = p;
		op[i++] = pp;
		return i - 1;
	}
	int Op(int code) {
		return Op(code, i + 1, i + 1, i + 1);
	}
	int Push(int data) {
		return Op(PUSH|data);
	}

	MakeGen() { i = 0; }
};


CONSOLE_APP_MAIN
{
	MakeGen g;
	g.Push('A');
	g.Op(OUTPUT);
	g.Push('H');
	g.Op(OUTPUT);
	g.Push('O');
	g.Op(OUTPUT);
	g.Push('J');
	g.Op(OUTPUT);
	g.Push(' ');
	g.Op(OUTPUT);
	g.Op(20);
	int lbl = g.Push('x');
	g.Op(OUTPUT);
	g.Push(1);
	g.Op(SUB, 0, 0, lbl);

	Vector<Gen> genom;
	genom.Add(g);

	DUMP(Interpret(genom));
}
