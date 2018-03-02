#include <Core/Core.h>

#include "../bsdiff.h"

//NAMESPACE_UPP
using namespace Upp;

String errMsg;

String BsGetLastError() {
	return errMsg;
}

bool Err(String str) {
	errMsg = str;
	return false;
}

