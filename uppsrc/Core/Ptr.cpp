#include "Core.h"

NAMESPACE_UPP

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

static StaticCriticalSection sPteLock;

PteBase::Prec *PteBase::PtrAdd()
{
	sPteLock.Enter();
	if(prec) {
		++prec->n;
		sPteLock.Leave();
	}
	else {
		sPteLock.Leave();
		prec = new Prec;
		prec->n = 1;
		prec->ptr = this;
	}
	return const_cast<Prec *>(prec);
}

void PteBase::PtrRelease(Prec *prec)
{
	CriticalSection::Lock __(sPteLock);
	if(prec && --prec->n == 0) {
		if(prec->ptr)
			prec->ptr->prec = NULL;
		delete prec;
	}
}

PteBase::PteBase()
{
	prec = NULL;
}

PteBase::~PteBase()
{
	CriticalSection::Lock __(sPteLock);
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

END_UPP_NAMESPACE
