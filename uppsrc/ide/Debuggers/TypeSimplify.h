#ifndef _ide_Debuggers_TypeSimplify_h_
#define _ide_Debuggers_TypeSimplify_h_

#include "Debuggers.h"
#include "VarItem.h"

#define SIMPLIFY_EXPR		"<!EXPR>"
#define SIMPLIFY_VALUE		"<!VALUE>"
#define SIMPLIFY_TEMPVAL	"<!TEMPVAL>"
#define SIMPLIFY_HINT		"<!HINT>"
#define SIMPLIFY_START		"<!START>"
#define SIMPLIFY_COUNT		"<!COUNT>"

#define SIMPLIFY_STEP		"<!STEP>"
#define SIMPLIFY_STEPVAL	"<!STEPVAL>"


#define SIMPLIFY_SIMPLE		"<!SIMPLE>"
#define SIMPLIFY_ARRAY		"<!ARRAY>"
#define SIMPLIFY_MAP		"<!MAP>"

// Simplifier handler
// step is the simplifying step, used for arrays and maps
// step 0 -- base simplify, no deep evaluation of containers
// step i -- deep evaluation of element 'i' of container
// returns number of needed steps to complete optimization
// all this stuff is needed to allow gui to have priority on data display
typedef int (*TYPE_SIMPLIFIER_HANDLER)(VarItem &varItem, int step);

void RegisterSimplifier(const char *pattern, TYPE_SIMPLIFIER_HANDLER handler);

TYPE_SIMPLIFIER_HANDLER GetSimplifier(String const &pattern);

// next index when stepping through containers values
int SIMPLIFIER_NEXT_INDEX(MIValue &val, int total);

#define REGISTERSIMPLIFIER(pattern, handler) \
	INITBLOCK { \
		RegisterSimplifier(pattern, handler); \
}

#endif
