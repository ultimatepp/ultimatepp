#include <Core/Core.h>

#include "../bsdiff.h"

NAMESPACE_UPP

String errMsg;

String BsGetLastError() {
	return errMsg;
}

bool Err(String str) {
	errMsg = str;
	return false;
}

END_UPP_NAMESPACE
