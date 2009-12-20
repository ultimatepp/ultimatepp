#include "XmlRpc.h"

#define STRUCT(id)    struct id {
#define VAR(type, id) type id;
#define INT(id)       int id;
#define BOOL(id)      bool id;
#define STRING(id)    String id;
#define DOUBLE(id)    double id;
#define DATE(id)      Date id;
#define TIME(id)      Time id;
#define END_STRUCT    void Map(ValueMapper& m); };

#include XRSFILE

#undef STRUCT
#undef VAR
#undef INT
#undef BOOL
#undef STRING
#undef DOUBLE
#undef DATE
#undef TIME
#undef END_STRUCT

#ifndef XRS_KEEP
#undef  XRSFILE
#endif
