#ifndef _SkylarkGuiExample_WebServer_h_
#define _SkylarkGuiExample_WebServer_h_

#include <Skylark/Skylark.h>

namespace Upp {

template <class Type>
class MTObj{
private:
	Mutex m;
	Type data;
public:
	Type Get()								{m.Enter(); Type tmp = data; m.Leave(); return tmp;}
	void Set(const Type & _data)			{m.Enter(); data = _data; m.Leave();}
};


class WebInterface : public SkylarkApp {
	Thread thr;
	void WorkThread();

public:
	typedef WebInterface CLASSNAME;

	WebInterface();
	~WebInterface();

	void StopServer();

	//Shared objects
	static MTObj<String>	SharedVariable;
	static Callback1<String> MessageFromWeb;

};


}

#endif
