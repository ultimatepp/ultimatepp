#include "Dispatcher.hpp"

//Dispatcher0

void Dispatcher0::DoDispatch() const
{
	if(!R::IsEnabled()) return;
	for(int i = 0; i < dests.GetCount(); i++)
		dests[i]();
}

void Dispatcher0::Register(Callback d, unsigned key)
{
	int i = dests.Find(key);
	if(i>=0) return;
	dests.Add(key) = d;
}

void Dispatcher0::Unregister(unsigned key)
{
	int i = dests.Find(key);
	if(i<0) return;
	dests.Remove(i);
}

Callback* Dispatcher0::GetDispatchable(unsigned key)
{
	int i = dests.Find(key);
	if(i<0) return NULL;
	return &dests[i];
}

//DispatcherL0

void DispatcherL0::DoDispatch() const
{
	int c = 0;	
	const Handler *list = dests.GetPtr(), *e = list;
	while((e = e->GetNext()) != list)
	{
		e->h(); ++c;
	}
}
