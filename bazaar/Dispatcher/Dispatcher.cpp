#include "Dispatcher.hpp"

DispatcherGen::DispatcherGen()
{
}

DispatcherGen::~DispatcherGen()
{
	//ASSERT(dests.IsEmpty()); //FIXME, DispatcherGen does keep the Dispatcher<T> instances, cause Dispatchable<T> are Register()ed there (forwarded) and do Unregister() only there as well.
}


//Callback variant

DispatcherCBGen::DispatcherCBGen()
{
}

DispatcherCBGen::~DispatcherCBGen()
{
	//ASSERT(dests.IsEmpty()); //FIXME, DispatcherGen does keep the Dispatcher<T> instances, cause Dispatchable<T> are Register()ed there (forwarded) and do Unregister() only there as well.
}

//Dispatcher0

Dispatcher0::Dispatcher0()
{
}

Dispatcher0::~Dispatcher0()
{
	ASSERT(dests.IsEmpty());
}

void Dispatcher0::DoDispatch() const
{
	if(!R::IsEnabled()) return;
	for(int i = 0; i < dests.GetCount(); i++)
	{
		const Callback & dest = dests.operator[](i);
		dest();
	}
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
	Callback & dest = dests.operator[](i);
	return &dest;
}

#if 0 //for compile / debug only
template class Dispatchable<int>;
template class Dispatcher<int>;

template void DispatcherGen::DoDispatch<int>(const int & o, unsigned param) const;
template void DispatcherGen::Register<int>(Dispatchable<int> & d, unsigned key);
template void DispatcherGen::Unregister<int>(Dispatchable<int> & d, unsigned key);

DispatcherGen gen;
DispatcherCBGen gencb;
#endif