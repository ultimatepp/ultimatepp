#include "IterTest.h"

void CommonHandler(Iter<int>& ii)
{
	while(ii.Next())
		ii.Get() = Random();
}

IterTest::IterTest()
{
	CtrlLayout(*this, "Window title");

	//generally, an iter interface needs its underlying container to live as long as interface is present.
	//no checks are performed to keep code small
	//an Iter interface will not modify the underlying container.
	//if container is changed while an Iter interface is bound to it, it may become undefined in behaviour (Vector i.e)
	//an Iter interface is always created on heap, so use means to delete it after usage, best is One<Iter<int > > foo, destroyed when scope is left

	//==========================================================================================	

	//Vector or any common linear container	

	Vector<int> vi;
	vi.SetCount(10);

	//use the explicit interface to do things
	One<Iter<int> > ii = IterCreator::GetIter(vi);
	while(ii->Next())
		ii->Get() = Random();

	//an iterator can be copied, without knowing its underlying type
	//and can be reinitiated
	One<Iter<int> > ii2 = ii->PartialCopy();
	while(ii2->Next())
		LOG(ii2->Get());

	//the const variant of Iter, ConstIter
	One<ConstIter<int> > ii3 = IterCreator::GetIter((const Vector<int>&)vi);
	while(ii3->Next())
		LOG(ii3->Get());

	FOREACH(int, e, vi)
		LOG(e);

	//and the const version
	FOREACHC(int, e, (const Vector<int>&)vi)
		LOG(e);

	//helpers to define the scope safe iterators
	ITER(int) _ii = IterCreator::GetIter(vi);
	ITERC(int) _cii = IterCreator::GetIter((const Vector<int>&)vi);

	//a macro for usual containers only without using Iter interface
	//can speed up things because no virtual stuff involved
	FOREACHCONT(int, e, vi)
		LOG(e);

	//the const variant
	FOREACHCONTC(int, e, (const Vector<int>&)vi)
		LOG(e);

	//other 'containers' also yield a Iter interface

	//usual pointer

	int in = 123;
	int* inp = &in;

	FOREACH(int, e, inp)
		LOG(e);

	FOREACHC(int, e, (const int*)inp)
		LOG(e);

	//Ptr

	EditInt ei;
	Ptr<Ctrl> eip(&ei);

	FOREACH(Ctrl, e, eip)
		e.SetData(123); 

	FOREACHC(Ctrl, e, (const Ptr<Ctrl>&)eip)
		e.GetData(); 

	//One

	One<Ctrl> oei;
	oei.Create<EditInt>();

	FOREACH(Ctrl, e, oei)
		e.SetData(123); 

	FOREACHC(Ctrl, e, (const One<Ctrl>&)oei)
		e.GetData(); 

	//Any

	Any a;
	a.Create<int>() = 345;
	One<Iter<int> > ia = IterCreator::GetIter<int>(a);
	
	FOREACH(int, e, a)
		LOG(e); 

	FOREACHC(int, e, (const Any&)a)
		LOG(e); 

	//Value

	Value v = 789;
	One<Iter<int> > iv = IterCreator::GetIter<int>(v);

	FOREACH(int, e, v)
		LOG(e);
	
	FOREACHC(int, e, (const Value&)v)
		LOG(e);

	//Link

	LinkOwner<Foo> lf;
	for(int i = 0; i < 10; i++)
		lf.InsertPrev()->d = i;

	FOREACH(Foo, e, (Link<Foo>&)lf)
		LOG(e.d);

	FOREACHC(Foo, e, (const Link<Foo>&)lf)
		LOG(e.d);

	//all containers can be handled in a common interface	
	ITER(int) iii;

	iii = IterCreator::GetIter(vi);
	CommonHandler(*iii);

	Array<int> ai;
	iii = IterCreator::GetIter(ai);
	CommonHandler(*iii);	
}

GUI_APP_MAIN
{
	IterTest().Run();
}

