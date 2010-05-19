#ifndef MACRO_HPP
#define MACRO_HPP

#define PARAMETER(Type, Name) \
	public: \
		CLASSNAME& Set##Name(Type p##Name) { _##Name = p##Name; return *this; } \
		const Type& Get##Name() const { return _##Name; } \
		Type& Get##Name() { return _##Name; } \
	private: \
		Type _##Name;

#endif // .. MACRO_HPP
