#include "Painter.h"

namespace Upp {

void Dasher::Put(const Pointf& p)
{
	if(flag)
		PutLine(p);
	else
		PutMove(p);
}

void Dasher::Move(const Pointf& p)
{
	PutMove(p);
	p0 = p;
}

void Dasher::Line(const Pointf& p)
{
	if(sum == 0) {
		PutLine(p);
		return;
	}
	Pointf v = p - p0;
	double len = Length(v);
	double pos = 0;
	while(pos + rem < len) {
		pos += rem;
		Put(pos / len * v + p0);
		flag = !flag;
		rem = (*pattern)[patterni];
		patterni = (patterni + 1) % pattern->GetCount();
	}
	rem -= len - pos;
	Put(p);
	p0 = p;
}

void Dasher::Init(const Vector<double>& p, double distance)
{
	pattern = &p;
	sum = Sum(p);
	if(sum == 0)
		return;
	distance -= int(distance / sum) * sum;
	patterni = 0;
	flag = false;
	for(;;) {
		rem = (*pattern)[patterni];
		patterni = (patterni + 1) % pattern->GetCount();
		flag = !flag;
		if(rem > distance) {
			rem -= distance;
			break;
		}
		distance -= rem;
	}
	p0 = Pointf(0, 0);
}

}
