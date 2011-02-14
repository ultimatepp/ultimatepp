#ifndef _Functions4U_StaticPlugin_h_
#define _Functions4U_StaticPlugin_h_


class StaticPlugin {
public:
	StaticPlugin();
	~StaticPlugin();
	bool Init(const char *name);
	void End();

	template <class T>
	static void Register(const char *name) {
		PluginData& x = Plugins().Add();
		x.name = name;
		x.New = New<T>;
		x.Delete = Delete<T>;
	}
	String &GetType() {return name;};
	String &GetName() {return name;};

protected:
	inline void *GetData() {return data;};
	
	struct PluginData {
		String name;
		void *(*New)();
		void (*Delete)(void *);
	};
	
	static Array<PluginData>& Plugins();
	
private:
	void *data;
	String name;
	
	template <class T> static void *New() 			{return new T;};
	template <class T> static void Delete(void *p) 	{delete static_cast<T *>(p);};
};


#endif
