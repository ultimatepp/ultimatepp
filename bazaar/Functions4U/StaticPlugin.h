#ifndef _Functions4U_StaticPlugin_h_
#define _Functions4U_StaticPlugin_h_


namespace Upp {

#define PluginRegister(a, b, c)	 	a::Register<b>(c, typeid(a).name())
#define PluginInit(a, b)			(a)._Init(b, typeid(a).name())

class StaticPlugin {
private:
	void *data;
	String name;
	String type;
	void *instance;
	
	template <class T> static void *New() 			{return new T;}
	template <class T> static void Delete(void *p) 	{delete static_cast<T *>(p);}

protected:
	inline void *GetData() {return data;};
	
	struct PluginData {
		String name;
		String type;
		void *instance;
		void *(*New)();
		void (*Delete)(void *);
	};
	
	static Upp::Array<PluginData>& Plugins();
	
public:
	StaticPlugin();
	virtual ~StaticPlugin();
	
	void End();

	template <class T>
	static void Register(const char *name, const char *_type) {
		PluginData& x = Plugins().Add();
		x.type = _type;
		x.name = name;
		x.instance = 0;
		x.New = New<T>;
		x.Delete = Delete<T>;
	}
	bool _Init(const char *_name, const char *_type);
	
	String &GetType() {return name;};
	String &GetName() {return name;};
};

}

#endif
