#include "XmlRpc.h"

#define STRUCT(id)    void id::Map(ValueMapper& m) {
#define VAR(type, id) m(#id, id);
#define INT(id)       m(#id, id);
#define BOOL(id)      m(#id, id);
#define STRING(id)    m(#id, id);
#define DOUBLE(id)    m(#id, id);
#define DATE(id)      m(#id, id);
#define TIME(id)      m(#id, id);
#define END_STRUCT    };

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
