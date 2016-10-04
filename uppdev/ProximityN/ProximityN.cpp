#include <Core/Core.h>

using namespace Upp;

const int N = 1000 * 1000;

int max_sims = 5;

struct Sim : Moveable<Sim> {
	int subject;
	int points;
};

inline int sPw2(int x)
{
	return x * x;
}

struct Subject {
	Point       data;
	Vector<Sim> sim;
	
	int Points(const Subject& b)
	{
		return sPw2(data.x - b.data.x) + sPw2(data.y - b.data.y);
	}
	
	void Insim(int subject, int points) {
		for(int i = 0; i < sim.GetCount(); i++)
			if(points < sim[i].points) {
				Sim& s = sim.Insert(i);
				s.points = points;
				s.subject = subject;
				if(sim.GetCount() > max_sims)
					sim.Trim(max_sims);
				return;
			}
		if(sim.GetCount() >= max_sims)
			return;
		Sim& s = sim.Add();
		s.points = points;
		s.subject = subject;
	}
};

void DoWork(Array<Subject>& s)
{
	for(int i = 0; i < s.GetCount(); i++) {
		if(i % 100 == 0)
			Cout() << i << '\n';
		Subject& a = s[i];
		for(int j = 0; j < s.GetCount(); j++)
			if(i != j)
				a.Insim(j, a.Points(s[j]));
	}
}

void DoWorkOpt(Array<Subject>& s)
{
	for(int m = 0; m < s.GetCount(); m += 32) {
		if(m % 64 == 0) 
			Cout() << m << '\n';
		for(int i = 0; i < s.GetCount(); i++) {
			Subject& a = s[i];
			int h = min(m + 32, s.GetCount());
			for(int j = m; j < h; j++)
				if(i != j)
					a.Insim(j, a.Points(s[j]));
		}
	}
}

const int SIMSTEP = 32;

void DoWorkMtl(Array<Subject> *ps, int m)
{
	Array<Subject>& s = *ps;
	for(int i = 0; i < s.GetCount(); i++) {
		Subject& a = s[i];
		int h = min(m + SIMSTEP, s.GetCount());
		for(int j = m; j < h; j++)
			if(i != j)
				a.Insim(j, a.Points(s[j]));
	}
}

void DoWorkMt(Array<Subject>& s)
{
	CoWork co;
	for(int m = 0; m < s.GetCount(); m += SIMSTEP) {
		if(m % (10 * SIMSTEP) == 0)
			Cout() << m << '\n';
		co & callback2(DoWorkMtl, &s, m);
	}
}

void Print(Array<Subject>& s, int ii = -1)
{
	if(ii < 0) return;
	for(int i = 0; i < s.GetCount(); i++) {
		if(ii < 0 || i == ii) {
			Subject& a = s[i];
			VppLog() << i << "[" << a.data << "]";
			for(int i = 0; i < a.sim.GetCount(); i++)
				VppLog() << ' ' << a.sim[i].subject << ':' << s[a.sim[i].subject].data;
			VppLog() << '\n';
		}
	}
}

void BubbleWork(Array<Subject>& s)
{
	for(int i = 0; i < s.GetCount(); i++) {
		Index<int> x;
		for(int j = 0; j < s[i].sim.GetCount(); j++)
			x.FindAdd(s[i].sim[j].subject);
		for(int q = x.GetCount(); --q >= 0;) {
			Subject& ss = s[x[q]];
			for(int j = 0; j < ss.sim.GetCount(); j++)
				x.FindAdd(ss.sim[j].subject);
		}
		for(int j = 0; j < max_sims; j++)
			x.FindAdd(MAKELONG(rand(), rand()) % s.GetCount());
		Subject& a = s[i];
		a.sim.Clear();
		for(int j = 0; j < x.GetCount(); j++)
			if(i != x[j])
				a.Insim(x[j], a.Points(s[x[j]]));
	}
}

CONSOLE_APP_MAIN
{
	Array<Subject> a;
	for(int i = 0; i < N; i++) {
		Subject& s = a.Add();
		s.data = Point(rand(), rand());
//		s.data = Point(1000 * sin(i * M_2PI / N), 1000 * cos(i * M_2PI / N));
	}
	if(0) {
		RTIMING("Normal");
		DoWork(a);
	}
//	Print(a);
//	Print(a, 0);
	LOG("------------------------------------");
	for(int i = 0; i < N; i++) {
		a[i].sim.Clear();
		a[i].sim.Shrink();
	}
	for(int i = 0; i < 20; i++) {
		Cout() << "Pass: " << i << '\n';
		RTIMING("BubbleWork");
		BubbleWork(a);
		Print(a, 0);
	}
	if(0) {
		RTIMING("MT");
		DoWorkMt(a);
	}
	LOG("------------------------------------");
	Print(a);
}
