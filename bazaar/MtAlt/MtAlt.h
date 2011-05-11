#ifndef _CORE_alt_MTalt_h_
#define _CORE_alt_MTalt_h_

#ifndef flagMT
	#error Please add 'MT' to build flags
#endif

#include <Core/Core.h>
using namespace Upp;

dword rdtsc();

class CallbackQueue : private NoCopy
{
public:
	CallbackQueue(int _maxQueueLength = 100000, int _id = 0) 
		:shuttingDown(false)
		,started(false)
		,id(_id)
		,maxQueueLength(_maxQueueLength)
	{
		Vector<CallbackQueue *> &q = queues.GetAdd(id);
		q << this;
	}
	
	virtual ~CallbackQueue()
	{
		qMutex.Enter();
		queue.Clear();
		qMutex.Leave();
		
		int qi = queues.Find(id);
		if (qi >= 0)
		{
			Vector<CallbackQueue *> &q = queues[qi];
			for (int i=0; i<q.GetCount(); ++i)
			{
				if (q[i] == this)
				{
					q.Remove(i);
					break;
				}
			}
		}
	}

	#define LOCK_ADD_UNLOCK \
		bool addSem = true; \
		qMutex.Enter(); \
		if (queue.GetCount() >= maxQueueLength) \
		{ \
			queue.DropHead(); \
			addSem = false; \
			LOG(Format("MtAlt: queue is too big (this:%s)", Format64Hex((uint64)this))); \
		} \
		queue.AddTail(e); \
		qMutex.Leave(); \
		if (addSem) \
			qSemaphore.Release(); 

	void ClearQueue()
	{
		qMutex.Enter();
		queue.Clear();
		qMutex.Leave();
	}
	
	template<class OBJECT>
	void Request(void (OBJECT::*m)())
	{
		Element0<OBJECT> *e = new Element0<OBJECT>;
		e->caller = (OBJECT *) this;
		e->m  = m;
		LOCK_ADD_UNLOCK;
	}
	
	template<class OBJECT, class P1>
	void Request(void (OBJECT::*m)(pick_ P1 &), pick_ P1 &p1)
	{
		Element1<OBJECT, P1> *e = new Element1<OBJECT, P1>;
		e->caller = (OBJECT *) this;
		e->m  = m;
		e->p1 = p1;
		LOCK_ADD_UNLOCK;
	}
	
	template<class OBJECT, class P1>
	void Request(void (OBJECT::*m)(P1), P1 p1)
	{
		Element1c<OBJECT, P1> *e = new Element1c<OBJECT, P1>;
		e->caller = (OBJECT *) this;
		e->m  = m;
		e->p1 = p1;
		LOCK_ADD_UNLOCK;
	}
	
	template<class OBJECT, class P1, class P2>
	void Request(void (OBJECT::*m)(pick_ P1 &,pick_ P2 &), pick_ P1 &p1, pick_ P2 &p2)
	{
		Element2<OBJECT, P1, P2> *e = new Element2<OBJECT, P1, P2>;
		e->caller = (OBJECT *) this;
		e->m  = m;
		e->p1 = p1;
		e->p2 = p2;
		LOCK_ADD_UNLOCK;
	}
	
	template<class OBJECT, class P1, class P2>
	void Request(void (OBJECT::*m)(P1,P2), P1 p1, P2 p2)
	{
		Element2c<OBJECT, P1, P2> *e = new Element2c<OBJECT, P1, P2>;
		e->caller = (OBJECT *) this;
		e->m  = m;
		e->p1 = p1;
		e->p2 = p2;
		LOCK_ADD_UNLOCK;
	}
	
	template<class OBJECT, class P1, class P2, class P3>
	void Request(void (OBJECT::*m)(pick_ P1 &, pick_ P2 &, pick_ P3 &), pick_ P1 &p1, pick_ P2 &p2, pick_ P3 &p3)
	{
		Element3<OBJECT, P1, P2, P3> *e = new Element3<OBJECT, P1, P2, P3>;
		e->caller = (OBJECT *) this;
		e->m  = m;
		e->p1 = p1;
		e->p2 = p2;
		e->p3 = p3;
		LOCK_ADD_UNLOCK;
	}
	
	template<class OBJECT, class P1, class P2, class P3>
	void Request(void (OBJECT::*m)(P1,P2,P3), P1 p1, P2 p2, P3 p3)
	{
		Element3c<OBJECT, P1, P2, P3> *e = new Element3c<OBJECT, P1, P2, P3>;
		e->caller = (OBJECT *) this;
		e->m  = m;
		e->p1 = p1;
		e->p2 = p2;
		e->p3 = p3;
		LOCK_ADD_UNLOCK;
	}
	
	template<class OBJECT, class P1, class P2, class P3, class P4>
	void Request(void (OBJECT::*m)(pick_ P1 &, pick_ P2 &, pick_ P3 &, pick_ P4 &), pick_ P1 &p1, pick_ P2 &p2, pick_ P3 &p3, pick_ P4 &p4)
	{
		Element4<OBJECT, P1, P2, P3, P4> *e = new Element4<OBJECT, P1, P2, P3, P4>;
		e->caller = (OBJECT *) this;
		e->m  = m;
		e->p1 = p1;
		e->p2 = p2;
		e->p3 = p3;
		e->p4 = p4;
		LOCK_ADD_UNLOCK;
	}
	
	template<class OBJECT, class P1, class P2, class P3, class P4>
	void Request(void (OBJECT::*m)(P1,P2,P3,P4), P1 p1, P2 p2, P3 p3, P4 p4)
	{
		Element4c<OBJECT, P1, P2, P3, P4> *e = new Element4c<OBJECT, P1, P2, P3, P4>;
		e->caller = (OBJECT *) this;
		e->m  = m;
		e->p1 = p1;
		e->p2 = p2;
		e->p3 = p3;
		e->p4 = p4;
		LOCK_ADD_UNLOCK;
	}
	
	template<class OBJECT, class P1, class P2, class P3, class P4, class P5>
	void Request(void (OBJECT::*m)(pick_ P1 &, pick_ P2 &, pick_ P3 &, pick_ P4 &, pick_ P5 &), pick_ P1 &p1, pick_ P2 &p2, pick_ P3 &p3, pick_ P4 &p4, pick_ P5 &p5)
	{
		Element5<OBJECT, P1, P2, P3, P4, P5> *e = new Element5<OBJECT, P1, P2, P3, P4, P5>;
		e->caller = (OBJECT *) this;
		e->m  = m;
		e->p1 = p1;
		e->p2 = p2;
		e->p3 = p3;
		e->p4 = p4;
		e->p5 = p5;
		LOCK_ADD_UNLOCK;
	}
	
	template<class OBJECT, class P1, class P2, class P3, class P4, class P5>
	void Request(void (OBJECT::*m)(P1,P2,P3,P4,P5), P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		Element5c<OBJECT, P1, P2, P3, P4, P5> *e = new Element5c<OBJECT, P1, P2, P3, P4, P5>;
		e->caller = (OBJECT *) this;
		e->m  = m;
		e->p1 = p1;
		e->p2 = p2;
		e->p3 = p3;
		e->p4 = p4;
		e->p5 = p5;
		LOCK_ADD_UNLOCK;
	}
	
	void DoTasks()
	{
		while (!IsShutdown())
			if (!Execute())
				return;
	}
	
	virtual void OnBeforeTask() {}
	virtual void OnAfterTask()  {}
	void WaitDoTasksInf()
	{
		while (!IsShutdown())
		{
			qSemaphore.Wait();
			OnBeforeTask();
			Execute();
			OnAfterTask();	
		}
	}

	void SetMaxQueueLength(int mql) {qMutex.Enter(); maxQueueLength = mql; while (queue.GetCount() >= mql) queue.DropHead(); qMutex.Leave();}
	int GetTasksCount() {qMutex.Enter(); int out=queue.GetCount(); qMutex.Leave(); return out;}

	bool IsShutdown() {return shuttingDown;}
	bool IsStarted() {return started;}
	
	static void StartAll(int id = 0)
	{
		Vector<CallbackQueue *> &q = queues.GetAdd(id);
		for (int i=0; i<q.GetCount(); ++i)
				q[i]->Start();
	}
	
	static void ShutdownAll(int id = 0)
	{
		Vector<CallbackQueue *> &q = queues.GetAdd(id);
		for (int i=0; i<q.GetCount(); ++i)
				q[i]->Shutdown();
	}

	virtual void Start() {started = true;}
	virtual void Shutdown()
	{
		if (!IsStarted() || IsShutdown())
			return;
		shuttingDown = true;
		Request(&CallbackQueue::DoNothingJustAwake);
	}
	
private:
	static VectorMap<int, Vector<CallbackQueue *> > queues;
	bool Execute()
	{
		if (queue.IsEmpty())
			return false;
		
		qMutex.Enter();
		Element *e = queue.DetachHead();
		qMutex.Leave();
		e->Execute();
		delete e;
		return true;
	}
	
	struct Element
	{
		virtual ~Element() {}
		virtual void Execute() = 0;
		void *caller;
	};
	
	template<class OBJECT>
	struct Element0 : public Element
	{
		virtual ~Element0() {}
		virtual void Execute() {((static_cast<OBJECT *>(caller))->*m)();}
		void (OBJECT::*m)();
	};
	
	template<class OBJECT, class P1>
	struct Element1 : public Element
	{
		virtual ~Element1() {}
		virtual void Execute() {((static_cast<OBJECT *>(caller))->*m)(p1);}
		void (OBJECT::*m)(pick_ P1 &p1);
		P1 p1;
	};
	
	template<class OBJECT, class P1>
	struct Element1c : public Element
	{
		virtual ~Element1c() {}
		virtual void Execute() {((static_cast<OBJECT *>(caller))->*m)(p1);}
		void (OBJECT::*m)(P1 p1);
		P1 p1;
	};
	
	template<class OBJECT, class P1, class P2>
	struct Element2 : public Element
	{
		virtual ~Element2() {}
		virtual void Execute() {((static_cast<OBJECT *>(caller))->*m)(p1,p2);}
		void (OBJECT::*m)(pick_ P1 &p1, pick_ P2 &p2);
		P1 p1;
		P2 p2;
	};
	
	template<class OBJECT, class P1, class P2>
	struct Element2c : public Element
	{
		virtual ~Element2c() {}
		virtual void Execute() {((static_cast<OBJECT *>(caller))->*m)(p1,p2);}
		void (OBJECT::*m)(P1 p1, P2 p2);
		P1 p1;
		P2 p2;
	};
	
	template<class OBJECT, class P1, class P2, class P3>
	struct Element3 : public Element
	{
		virtual ~Element3() {}
		virtual void Execute() {((static_cast<OBJECT *>(caller))->*m)(p1,p2,p3);}
		void (OBJECT::*m)(pick_ P1 &p1, pick_ P2 &p2, pick_ P3 &p3);
		P1 p1;
		P2 p2;
		P3 p3;
	};
	
	template<class OBJECT, class P1, class P2, class P3>
	struct Element3c : public Element
	{
		virtual ~Element3c() {}
		virtual void Execute() {((static_cast<OBJECT *>(caller))->*m)(p1,p2,p3);}
		void (OBJECT::*m)(P1 p1, P2 p2, P3 p3);
		P1 p1;
		P2 p2;
		P3 p3;
	};
	
	template<class OBJECT, class P1, class P2, class P3, class P4>
	struct Element4 : public Element
	{
		virtual ~Element4() {}
		virtual void Execute() {((static_cast<OBJECT *>(caller))->*m)(p1,p2,p3,p4);}
		void (OBJECT::*m)(pick_ P1 &p1, pick_ P2 &p2, pick_ P3 &p3, pick_ P4 &p4);
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
	};
	
	template<class OBJECT, class P1, class P2, class P3, class P4>
	struct Element4c : public Element
	{
		virtual ~Element4c() {}
		virtual void Execute() {((static_cast<OBJECT *>(caller))->*m)(p1,p2,p3,p4);}
		void (OBJECT::*m)(P1 p1, P2 p2, P3 p3, P4 p4);
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
	};
	
	template<class OBJECT, class P1, class P2, class P3, class P4, class P5>
	struct Element5 : public Element
	{
		virtual ~Element5() {}
		virtual void Execute() {((static_cast<OBJECT *>(caller))->*m)(p1,p2,p3,p4,p5);}
		void (OBJECT::*m)(pick_ P1 &p1, pick_ P2 &p2, pick_ P3 &p3, pick_ P4 &p4, pick_ P5 &p5);
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
		P5 p5;
	};
	
	template<class OBJECT, class P1, class P2, class P3, class P4, class P5>
	struct Element5c : public Element
	{
		virtual ~Element5c() {}
		virtual void Execute() {((static_cast<OBJECT *>(caller))->*m)(p1,p2,p3,p4,p5);}
		void (OBJECT::*m)(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5);
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
		P5 p5;
	};
	
	void DoNothingJustAwake(){}

	BiArray<Element> queue;
	Mutex            qMutex;
	Semaphore        qSemaphore;
	bool             shuttingDown;
	bool			 started;
	int              id;
	int              maxQueueLength;
};

class CallbackThread : public CallbackQueue, protected Thread
{
public:
	CallbackThread(int maxQueueLength = 100000, int id = 0)  :CallbackQueue(maxQueueLength, id) {}
	virtual ~CallbackThread() {Shutdown();}

	virtual void Start()
	{
		CallbackQueue::Start();
		Thread::Run(callback(this, &CallbackThread::Run));
	}
	
	virtual void Shutdown()
	{
		if (!IsStarted() || IsShutdown())
			return;
		CallbackQueue::Shutdown();
		Wait();
	}

private:	
	void Run() {WaitDoTasksInf();}
};

#endif
