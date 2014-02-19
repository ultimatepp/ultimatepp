#ifndef _ide_Debuggers_TypeSimplify_h_
#define _ide_Debuggers_TypeSimplify_h_

#include "Debuggers.h"
#include <ide/ide.h>

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
// parameters:
//	gdb			handler to debugger object -- used mostly to inspect deeper
//	expRoot		gdb evaluable expression of 'val' expression root. The one used to get the 'val'.
typedef bool (*TYPE_SIMPLIFIER_HANDLER)(Gdb_MI2 &gdb, MIValue &val, bool deep);

void RegisterSimplifier(const char *pattern, TYPE_SIMPLIFIER_HANDLER handler);

TYPE_SIMPLIFIER_HANDLER GetSimplifier(String const &pattern);

// next index when stepping through containers values
int SIMPLIFIER_NEXT_INDEX(MIValue &val, int total);

#define REGISTERSIMPLIFIER(pattern, handler) \
	INITBLOCK { \
		RegisterSimplifier(pattern, handler); \
}

#endif
