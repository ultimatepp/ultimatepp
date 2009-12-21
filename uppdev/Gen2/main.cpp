#include "Gen2.h"

void Creature::Step()
{
	for(int i = 0; i < 8192; i++) {
		Gen& g = gen[i];
		switch(gen[i].op & 15) {
		case 1:
			g.value = Op1(g) + Op2(g);
			break;
		case 2:
			g.value = Op1(g) * Op2(g);
			break;
		case 3:
			g.value = Op1(g) ^ Op2(g);
			break;
		case 4:
			g.value = ~Op1(g);
			break;
		case 5:
			g.value = Op1(g) << Op2(g);
			break;
		case 6:
			g.value = Op1(g) >> Op2(g);
			break;
		case 7:
			g.value = !Op1(g);
			break;
		case 8:
			g.value = g.op1;
			break;
		case 9:
			g.value = Op1(g) & Op2(g);
			break;
		case 10:
			g.value = Op1(g) | Op2(g);
			break;
		case 11:
			g.value = Op1(g) < Op2(g);
			break;
		default:
			g.value = Op1(g);
			break;
		}
	}
}

void Creature::Random()
{
	color = Color(rand() & 255, rand() & 255, rand() & 255);
	energy = 20;
	pos.x = rand() & PMSK;
	pos.y = rand() & PMSK;
	ppos = pos;
	for(int i = 0; i < 8192; i++) {
		Gen& g = gen[i];
		g.op = rand();
		g.op1 = rand();
		g.op2 = rand();
	}
}

void World::Paint(Draw& w)
{
	ImageBuffer ib(WSZ, WSZ);
	RGBA *t = ~ib;
	for(int y = 0; y < WSZ; y++) {
		for(int x = 0; x < WSZ; x++) {
			Spot& p = spot[y][x];
			RGBA c;
			if(!p.creature) {
				c.a = 255;
				c.r = c.g = c.b = 0;
				switch(p.kind) {
				case ROCK:
					c.b = 255;
					break;
				case TRAP:
					c.r = 255;
					break;
				case GRASS:
					c.g = p.energy;
					break;
				}
			}
			*t++ = c;
		}
	}
	for(int i = 0; i < creature.GetCount(); i++) {
		Point p = creature[i].pos;
		RGBA c;
		c.a = 255;
		c.b = c.r = c.g = minmax(creature[i].energy, 0, 255);
		ib[p.y][p.x] = c;
	}
	ib.SetKind(IMAGE_OPAQUE);
	w.DrawImage(0, 0, ib);
	w.DrawRect(WSZ + 1, 0, 99999, 99999, SColorFace());
	w.DrawRect(0, WSZ + 1, 99999, 99999, SColorFace());
}

void World::Die(int i, const char *cause)
{
	LOG(cause);
	creature.Remove(i);
	died++;
}

void World::Step()
{
	step++;
	int gen = 0;
	int best = 0;
	int n = creature.GetCount();
	if((step & 31) == 0 && creature.GetCount() < 1200)
		for(int y = 0; y < WSZ; y++)
			for(int x = 0; x < WSZ; x++) {
				Spot& p = spot[y][x];
				if(p.kind == GRASS && p.energy < 255)
					p.energy++;
			}
	if(n > 500) {
		Spot& p = spot[(rand() & 255) + 400][(rand() & 255) + 400];
		if(p.kind == EMPTY) {
			p.kind = TRAP;
			p.energy = 0;
		}
		Spot& tp = spot[rand() & 1023][rand() & 1023];
		if(tp.kind == EMPTY) {
			tp.kind = TRAP;
			tp.energy = 0;
		}
	}
	for(int i = 0; i < creature.GetCount(); i++) {
		Creature& c = creature[i];
		int ie = 7500;
		int ik = 7000;
		for(int x = -6; x <= 6; x++)
			for(int y = -6; y <= 6; y++) {
				Spot& p = spot[(c.pos.y + y) & PMSK][(c.pos.x + x) & PMSK];
				c.gen[ie++].value = p.kind;
				c.gen[ik++].value = p.energy;
			}
	}
	{
		CoWork co;
		for(int i = 0; i < creature.GetCount(); i++)
			co & callback(&creature[i], &Creature::Step);
	}
	for(int i = 0; i < creature.GetCount(); i++) {
		Creature& c = creature[i];
		if(c.generation > gen)
			gen = c.generation;
		if(c.collected > best)
			best = c.collected;
		Point np = c.pos;
		if(c.gen[8190].value < 0x7000) {
			c.gen[8190].value = 0x8000;
			np.x = (np.x - 1) & PMSK;
		}
		if(c.gen[8190].value > 0x9000) {
			c.gen[8190].value = 0x8000;
			np.x = (np.x + 1) & PMSK;
		}
		if(c.gen[8191].value < 0x7000) {
			c.gen[8191].value = 0x8000;
			np.y = (np.y - 1) & PMSK;
		}
		if(c.gen[8191].value > 0x9000) {
			c.gen[8191].value = 0x8000;
			np.y = (np.y + 1) & PMSK;
		}
		Spot& p = spot[np.y][np.x];
		if(np != c.pos)
			c.energy--;
		switch(p.kind) {
		case GRASS:
			LOG("Eaten " << (int)p.energy);
			for(int i = 5200; i < 5300; i++)
				c.gen[i].value += p.energy;
			c.energy += p.energy;
			c.collected += 16 * p.energy;
			p.energy = 0;
			p.kind = EMPTY;
			for(;;) {
				Spot& p = spot[rand() & PMSK][rand() & PMSK];
				if(p.kind == EMPTY) {
					p.kind = GRASS;
					p.energy = 0;
					break;
				}
			}
			break;
		case ROCK:
			np = c.pos;
			for(int i = 5000; i < 5100; i++)
				c.gen[i].value = 8000;
			break;
		}
		for(int q = 5400; q < 5500; q++)
			c.gen[q].value = c.energy;
		spot[c.pos.y][c.pos.x].creature = NULL;
		if(c.pos != np) {
			c.ppos = c.pos;
			c.pos = np;
		}
		spot[c.pos.y][c.pos.x].creature = &c;
		c.energy--;
		if(p.kind == TRAP) {
			if(!(c.pos.x == 0 || c.pos.y == 0 || c.pos.x == PMSK || c.pos.y == PMSK))
				p.kind = EMPTY;
			Die(i, "Fallen in trap");
		}
		else
		if(c.energy < 0)
			Die(i, "Starved to death");
		else
		if(c.energy > 400) {
			int mi = 0;
			int mind = INT_MAX;
			for(int i = 1; i < creature.GetCount(); i++) {
				Point p = creature[i].pos;
				int d = (np.x - p.x) * (np.x - p.x) + (np.y - p.y) * (np.y - p.y);
				if(d < mind) {
					mi = i;
					mind = d;
				}
			}
			Creature& pc = creature[mi];
			Creature& nc = creature.Add();
			nc.pos = c.ppos;
			nc.energy = 200;
			nc.generation = c.generation + 1;
			c.energy -= 200;
			for(int i = 0; i < 8192; i++)
				nc.gen[i] = (rand() & 1) ? c.gen[i] : pc.gen[i];
			while((rand() & 5) == 0) {
				nc.color = Color(127 + (((rand() & 15) + c.color.GetR() - 7) & 127),
				                 127 + (((rand() & 15) + c.color.GetG() - 7) & 127),
				                 127 + (((rand() & 15) + c.color.GetB() - 7) & 127));
				for(int p = rand() & 255; p < 8192; p++) {
					if((rand() & 3) == 0)
						nc.gen[p].op = rand();
					if((rand() & 3) == 0)
						nc.gen[p].op1 = rand();
					if((rand() & 3) == 0)
						nc.gen[p].op2 = rand();
					p += rand() & 255;
				}
			}
			born++;
		}
	}
/*	for(int i = 0; i < 1024; i++) {
		spot[i][0].kind = TRAP;
		spot[i][PMSK].kind = TRAP;
		spot[0][i].kind = TRAP;
		spot[PMSK][i].kind = TRAP;
	}*/
	Title(String().Cat() << "Step: " << step << " Gen: " << gen << ", pop: " << creature.GetCount()
	      << ", born: " << born << ", died: " << died << " best: " << best);
	Refresh();
}

World::World()
{
	spot = new Spot[WSZ][WSZ];
	for(int y = 0; y < WSZ; y++)
		for(int x = 0; x < WSZ; x++) {
			Spot& p = spot[x][y];
			p.kind = 0;
			if((rand() & 15) == 0)
				p.kind = ROCK;
			if((rand() & 127) == 0)
				p.kind = TRAP;
			if(rand() % 20 == 0) {
				p.kind = GRASS;
				p.energy = rand();
			}
//			if(x == 0 || y == 0 || x == PMSK || y == PMSK) {
//				p.kind = TRAP;
//				p.energy = 0;
//			}
			p.creature = NULL;
		}
	if(PromptYesNo("Restore creatures from the previous run?")) {
		FileIn in(ConfigFile("creatures"));
		for(;;) {
			Creature r;
			if(!in.GetAll(&r, sizeof(Creature)))
				break;
			creature.Add(r);
		}
	}
	else
		for(int i = 0; i < 15000; i++)
			creature.Add().Random();
	SetTimeCallback(-2, THISBACK(Step));
	step = 0;
	born = died = 0;
	Sizeable().Zoomable();
}

World::~World()
{
	if(PromptYesNo("Store creatures?")) {
		FileOut out(ConfigFile("creatures"));
		for(int i = 0; i < creature.GetCount(); i++)
			out.Put(&creature[i], sizeof(Creature));
	}
	delete[] spot;
}

GUI_APP_MAIN {
	World().Run();
}
