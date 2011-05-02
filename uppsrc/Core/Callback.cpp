#include "Core.h"

NAMESPACE_UPP

void Callback::Execute() const
{
	if(action) action->Execute();
}

Callback& Callback::operator=(const Callback& c)
{
	c.Retain();
	Release();
	action = c.action;
	return *this;
}

Callback::Callback(const Callback& c)
{
	action = c.action;
	Retain();
}

Callback::~Callback()
{
	Release();
}

Callback callback(Callback cb1, Callback cb2)
{
	return Callback(new CallbackForkAction (cb1, cb2));
}

Callback& operator<<(Callback& a, Callback b)
{
	if(a)
		a = callback(a, b);
	else
		a = b;
	return a;
}

//

bool Gate::Execute() const
{
	return (void *)action == (void *)1 ? true : action ? action->Execute() : false;
}

Gate& Gate::operator=(const Gate& c)
{
	c.Retain();
	Release();
	action = c.action;
	return *this;
}

Gate::Gate(const Gate& c)
{
	action = c.action;
	Retain();
}

Gate::~Gate()
{
	Release();
}

Gate callback(Gate cb1, Gate cb2)
{
	return Gate(new GateForkAction (cb1, cb2));
}

Gate& operator<<(Gate& a, Gate b)
{
	if(a)
		a = callback(a, b);
	else
		a = b;
	return a;
}

END_UPP_NAMESPACE
