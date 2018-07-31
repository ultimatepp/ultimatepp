#include "Core.h"

namespace Upp {

/* Faster, but consuming more memory....
PteBase::Prec *PteBase::PtrAdd()
{
	AtomicInc(prec->n);
	return prec;
}

void PteBase::PtrRelease(Prec *prec)
{
	if(prec && AtomicDec(prec->n) == 0)
		delete prec;
}

PteBase::PteBase()
{
	prec = new Prec;
	prec->n = 1;
	prec->ptr = this;
}
*/

static StaticMutex sPteLock;

PteBase::Prec *PteBase::PtrAdd()
{
	Mutex::Lock __(sPteLock);
	if(prec)
		++prec->n;
	else {
		prec = tiny_new<Prec>();
		prec->n = 1;
		prec->ptr = this;
	}
	return const_cast<Prec *>(prec);
}

void PteBase::PtrRelease(Prec *prec)
{
	Mutex::Lock __(sPteLock);
	if(prec && --prec->n == 0) {
		if(prec->ptr)
			prec->ptr->prec = NULL;
		tiny_delete(prec);
	}
}

PteBase::PteBase()
{
	prec = NULL;
}

PteBase::~PteBase()
{
	Mutex::Lock __(sPteLock);
	if(prec)
		prec->ptr = NULL;
}

void PtrBase::Release()
{
	PteBase::PtrRelease(prec);
}

void PtrBase::Set(PteBase *p)
{
	prec = p ? p->PtrAdd() : NULL;
}

void PtrBase::Assign(PteBase *p)
{
	Release();
	Set(p);
}

PtrBase::~PtrBase()
{
	Release();
}

}
