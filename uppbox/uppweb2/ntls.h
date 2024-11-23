#ifndef NTLS_H
#define NTLS_H

#include <string>
#include <ntl.h>

NTL_MOVEABLE(std::string)

template<>
inline unsigned GetHashValue(const std::string& x)
{
	return memhash(x.c_str(), x.size());
}

#ifndef NOWSTRING

NTL_MOVEABLE(std::wstring)

template<>
inline unsigned GetHashValue(const std::wstring& x)
{
	CombineHash hash;
	for(size_t i = 0; i != x.size(); ++i)
		hash = hash.Put(x[i]);
	return hash;
}

#endif

#endif
