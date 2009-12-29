#include <Core/Core.h>

using namespace Upp;

String errMsg;

#include "../bsdiffwrapper.h"

String BsGetLastError() {
	return errMsg;
}

bool Err(String str) {
	errMsg = str;
	return false;
}


