#ifndef _Functions4U_StaticPlugin_h_
#define _Functions4U_StaticPlugin_h_

#define PluginRegister(a, b, c)	 	a::Register<b>(c, typeid(a).name())
#define PluginInit(a, b)			a.Init(b, typeid(a).name())

class StaticPlugin {
public:
	StaticPlugin();
	~StaticPlugin();
	
	//bool Init(const char *name);
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
	
	bool Init(const char *_name, const char *_type) {
		type = _type;
		for (int i = 0; i < Plugins().GetCount(); ++i) {
			if (Plugins()[i].name == _name && Plugins()[i].type == type) {
				if (data)
					Plugins()[i].Delete(data);
				data = Plugins()[i].New();
				name = _name;
				instance = this;
				Plugins()[i].instance = instance;
				return true;
			}
		}
		for (int i = 0; i < Plugins().GetCount(); ++i) {
			if (Plugins()[i].name == "" && Plugins()[i].type == type) {
				if (data)
					Plugins()[i].Delete(data);
				data = Plugins()[i].New();
				name = "";
				instance = this;
				Plugins()[i].instance = instance;
				return false;
			}
		}
		return false;
	}

	String &GetType() {return name;};
	String &GetName() {return name;};

protected:
	inline void *GetData() {return data;};
	
	struct PluginData {
		String name;
		String type;
		void *instance;
		void *(*New)();
		void (*Delete)(void *);
	};
	
	static Array<PluginData>& Plugins();
	
private:
	void *data;
	String name;
	String type;
	void *instance;
	
	template <class T> static void *New() 			{return new T;};
	template <class T> static void Delete(void *p) 	{delete static_cast<T *>(p);};
};


#endif
