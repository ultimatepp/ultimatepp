#ifndef _SysExec_ArgEnv_h_
#define _SysExec_ArgEnv_h_

#include <Core/Core.h>

NAMESPACE_UPP

///////////////////////////////////////////////////////////////////////////////////////////////
// parses an args line to be useable by spawnxx functions
char **BuildArgs(String command, String const &argline);

///////////////////////////////////////////////////////////////////////////////////////////////
// parses environment map and builds env array
char **BuildEnv(const VectorMap<String, String> &env);

END_UPP_NAMESPACE

#endif
