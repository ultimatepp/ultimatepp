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

#if 0 //for compile / debug only
template class Dispatchable<int>;
template class Dispatcher<int>;

template void DispatcherGen::DoDispatch<int>(const int & o, unsigned param) const;
template void DispatcherGen::Register<int>(Dispatchable<int> & d, unsigned key);
template void DispatcherGen::Unregister<int>(Dispatchable<int> & d, unsigned key);

DispatcherGen gen;
DispatcherCBGen gencb;
#endif