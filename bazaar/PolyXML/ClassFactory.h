#ifndef _ClassFactory_h_
#define _ClassFactory_h_

#include <Core/Core.h>

NAMESPACE_UPP;

template<class T> class WithFactory
{
	private:
		typedef One<T> (*CreateFunc)();
		typedef VectorMap<String, CreateFunc>mapType;
		static mapType &classMap() { static mapType cMap; return cMap; }
		static VectorMap<String, String> &typeMap() { static VectorMap<String, String> tMap; return tMap; }
		template<class D> static One<T> __Create(void) { return One<T>((T *)new D); }
	public:
	
		template<class D> static void Register(const String &name)
		{
			classMap().Add(name, __Create<D>);
			typeMap().Add(typeid(D).name(), name);
		}
		static One<T> Create(const String &className) { return classMap().Get(className)(); }
		static T *CreatePtr(String const &className) { return classMap().Get(className)().Detach(); }
		static Vector<String> const &Classes(void) { return classMap().GetKeys(); }
		String const &IsA(void) { return typeMap().Get(typeid(*this).name()); }
		virtual ~WithFactory() {}
};

#define REGISTERCLASS(type) \
	INITBLOCK { \
	type::Register<type>(#type); \
}

END_UPP_NAMESPACE;

#endif
