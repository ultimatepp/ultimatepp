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
		static VectorMap<String, String> &descMap() { static VectorMap<String, String> dMap; return dMap; }
		static VectorMap<String, int> &indexMap() { static VectorMap<String, int> iMap; return iMap; }
		static VectorMap<String, Image> &imageMap() { static VectorMap<String, Image> imgMap; return imgMap; }
		template<class D> static One<T> __Create(void) { return One<T>((T *)new D); }
	public:
	
		template<class D> static void Register(const String &name, const String &desc = "", int idx = 0, Image const &img = Null)
		{
			classMap().Add(name, __Create<D>);
			typeMap().Add(typeid(D).name(), name);
			descMap().Add(name, desc);
			indexMap().Add(name, idx);
			imageMap().Add(name, img);
		}
		static One<T> CreateInstance(const String &className) { return classMap().Get(className)(); }
		static T *CreatePtr(String const &className) { return classMap().Get(className)().Detach(); }
		static Vector<String> const &Classes(void) { return classMap().GetKeys(); }
		static String const &GetClassDescription(const String &className) { return descMap().Get(className); }
		static int const &GetClassIndex(const String &className) { return indexMap().Get(className); }
		static Image const &GetClassImage(const String &className) { return imageMap().Get(className); }
		String const &IsA(void) { return typeMap().Get(typeid(*this).name()); }
		virtual ~WithFactory() {}
};

#ifdef COMPILER_MSC
#define REGISTERCLASS(type, ...) \
	INITBLOCK { \
		type::Register<type>(#type, __VA_ARGS__); \
}
#else
#define REGISTERCLASS(type, ...) \
	INITBLOCK { \
		type::Register<type>(#type, ##__VA_ARGS__); \
}
#endif

END_UPP_NAMESPACE;

#endif
