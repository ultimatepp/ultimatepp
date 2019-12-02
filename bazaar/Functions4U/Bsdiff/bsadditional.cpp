#include <Core/Core.h>

#include "../bsdiff.h"

namespace Upp {

String errMsg;

String BsGetLastError() {
	return errMsg;
}

bool Err(String str) {
	errMsg = str;
	return false;
}

}
