#include "Dispatcher.h"

template<class T>
Dispatchable<T>::Dispatchable()
{
	
}

template<class T>
Dispatchable<T>::~Dispatchable()
{
	UnregisterAll();
}

template<class T>
void Dispatchable<T>::Unregister(const Any & _src)
{
	if(_src.Is<Dispatcher<T> * >())
	{
		Dispatcher<T> * disp = _src.Get<Dispatcher<T> * >();
		disp->Unregister(*this, key.Get(GetPtrHashValue(disp)));
	}
	else
	if(_src.Is<DispatcherGen * >())
	{
		DispatcherGen * disp = _src.Get<DispatcherGen * >();
		disp->Unregister(*this, key.Get(GetPtrHashValue(disp)));
	}
}

template<class T>
void Dispatchable<T>::UnregisterAll()
{
	while(src.GetCount()>0)
	{
		Unregister(src[0]);
	}
}

template<class T>
Dispatcher<T>::Dispatcher()
{
	
}

template<class T>
Dispatcher<T>::~Dispatcher()
{
	ASSERT(dests.IsEmpty());
}

template<class T>
void Dispatcher<T>::DoDispatch(const T & o, unsigned param) const
{
	for(int i = 0; i < dests.GetCount(); i++)
	{
		Dispatchable<T> * dest = dests.operator[](i);
		dest->Dispatch(o, param);
	}
}

template<class T>
void Dispatcher<T>::Register(Dispatchable<T> & d, unsigned key)
{
	if(key == 0) key = GetPtrHashValue(&d);
	int i = dests.Find(key);
	if(i>=0) return;
	dests.Add(key) = &d;
	Any & a = d.src.Add(GetPtrHashValue(this));
	a.Create<Dispatcher<T> * >() = this;
	d.key.Add(GetPtrHashValue(this)) = key;
}

template<class T>
void Dispatcher<T>::Unregister(Dispatchable<T> & d, unsigned key)
{
	if(key == 0) key = GetPtrHashValue(&d);
	int i = dests.Find(key);
	if(i<0) return;
	dests.Remove(i);
	d.src.RemoveKey(GetPtrHashValue(this));
	d.key.RemoveKey(GetPtrHashValue(this));
}

template<class T>
Dispatchable<T> * Dispatcher<T>::GetDispatchable(unsigned key) const
{
	int i = dests.Find(key);
	if(i<0) return NULL;
	Dispatchable<T> * dest = dests.operator[](i);
	return dest;
}

template<class T>
void DispatcherGen::DoDispatch(const T & o, unsigned param) const
{
	for(int i = 0; i < dests.GetCount(); i++)
	{
		const Any & a = dests.operator[](i);
		if(!a.Is<Dispatcher<T> >()) continue;
		const Dispatcher<T> & dest = a.Get<Dispatcher<T> >();
		dest.DoDispatch(o, param);
		return;
	}
}

template<class T>
void DispatcherGen::Register(Dispatchable<T> & d, unsigned key)
{
	for(int i = 0; i < dests.GetCount(); i++)
	{
		Any & a = dests.operator[](i);
		if(!a.Is<Dispatcher<T> >()) continue;
		Dispatcher<T> & dest = a.Get<Dispatcher<T> >();
		dest.Register(d, key);
		return;
	}
	dests.Add().Create<Dispatcher<T> >().Register(d,key);
}

template<class T>
void DispatcherGen::Unregister(Dispatchable<T> & d, unsigned key)
{
	for(int i = 0; i < dests.GetCount(); i++)
	{
		Any & a = dests.operator[](i);
		if(!a.Is<Dispatcher<T> >()) continue;
		Dispatcher<T> & dest = a.Get<Dispatcher<T> >();
		dest.Unregister(d, key);
		if(dest.GetDests().GetCount()<=0)
			dests.Remove(i);
		return;
	}
}

template<class T>
Dispatcher<T> & DispatcherGen::GetDispatcher()
{
	for(int i = 0; i < dests.GetCount(); i++)
	{
		const Any & a = dests.operator[](i);
		if(!a.Is<Dispatcher<T> >()) continue;
		Dispatcher<T> & dest = a.Get<Dispatcher<T> >();
		return dest;
	}
	return dests.Add().Create<Dispatcher<T> >();
}


//Callback variant

template<class T>
DispatcherCB<T>::DispatcherCB()
{
}

template<class T>
DispatcherCB<T>::~DispatcherCB()
{
	ASSERT(dests.IsEmpty());
}

template<class T>
void DispatcherCB<T>::DoDispatch(const T & o, unsigned param) const
{
	for(int i = 0; i < dests.GetCount(); i++)
	{
		const Callback2<const T &, unsigned> & dest = dests.operator[](i);
		dest(o, param);
	}
}

template<class T>
void DispatcherCB<T>::Register(Callback2<const T &, unsigned> d, unsigned key)
{
	int i = dests.Find(key);
	if(i>=0) return;
	dests.Add(key) = d;
}

template<class T>
void DispatcherCB<T>::Unregister(unsigned key)
{
	int i = dests.Find(key);
	if(i<0) return;
	dests.Remove(i);
}

template<class T>
Callback2<const T &, unsigned> DispatcherCB<T>::GetDispatchable(unsigned key)
{
	int i = dests.Find(key);
	if(i<0) return;
	Callback2<const T &, unsigned> & dest = dests.operator[](i);
	return dest;
}

template<class T>
void DispatcherCBGen::DoDispatch(const T & o, unsigned param) const
{
	for(int i = 0; i < dests.GetCount(); i++)
	{
		const Any & a = dests.operator[](i);
		if(!a.Is<DispatcherCB<T> >()) continue;
		const DispatcherCB<T> & dest = a.Get<DispatcherCB<T> >();
		dest.DoDispatch(o, param);
		return;
	}
}

template<class T>
void DispatcherCBGen::Register(Callback2<const T &, unsigned> d, unsigned key)
{
	for(int i = 0; i < dests.GetCount(); i++)
	{
		Any & a = dests.operator[](i);
		if(!a.Is<DispatcherCB<T> >()) continue;
		DispatcherCB<T> & dest = a.Get<DispatcherCB<T> >();
		dest.Register(d, key);
		return;
	}
	dests.Add().Create<DispatcherCB<T> >().Register(d,key);
}

template<class T>
void DispatcherCBGen::Unregister(unsigned key)
{
	for(int i = 0; i < dests.GetCount(); i++)
	{
		Any & a = dests.operator[](i);
		if(!a.Is<DispatcherCB<T> >()) continue;
		DispatcherCB<T> & dest = a.Get<DispatcherCB<T> >();
		dest.Unregister(key);
		if(dest.GetDests().GetCount()<=0)
			dests.Remove(i);
		return;
	}
}

template<class T>
DispatcherCB<T> & DispatcherCBGen::GetDispatcherCB()
{
	for(int i = 0; i < dests.GetCount(); i++)
	{
		const Any & a = dests.operator[](i);
		if(!a.Is<DispatcherCB<T> >()) continue;
		DispatcherCB<T> & dest = a.Get<DispatcherCB<T> >();
		return dest;
	}
	return dests.Add().Create<DispatcherCB<T> >();
}
