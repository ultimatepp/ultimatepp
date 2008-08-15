#ifndef _Gen2_Gen2_h_
#define _Gen2_Gen2_h_


#include <CtrlLib/CtrlLib.h>

using namespace Upp;

enum {
	EMPTY, HARDTRAP = 30, TRAP = 60, ROCK = 100, GRASS = 150, 
};

enum {
	PMSK = 1023,
	WSZ = PMSK + 1
};

struct Creature;

struct World : public TopWindow {
	struct Spot {
		byte      kind;
		byte      energy;
		Creature *creature;
	};

	Spot            (*spot)[WSZ];
	int             step;
	Array<Creature> creature;
	int             born, died;
	
	virtual void Paint(Draw& w);
	
	void Step();
	void Die(int i, const char *cause);
	
	typedef World CLASSNAME;
	
	World();
	~World();
};

struct Gen {
	byte   op;
	word   op1;
	word   op2;
	int    value;
};

struct Creature {
	Color color;
	Gen   gen[8192];
	int   energy;
	int   collected;
	int   generation;
	Point pos;
	Point ppos;
	
	int   Op1(const Gen& g)              { return gen[g.op1 & 8191].value; }
	int   Op2(const Gen& g)              { return gen[g.op2 & 8191].value; }

	void Step();
	void Random();
	
	Creature() { collected = 0; generation = 0; }
};


#endif
