#include <RichEdit/RichEdit.h>

using namespace Upp;

#define LOGPOS            UPP::VppLog() << "-=> " << __FILE__ << '@' << __LINE__ << '\n',

#undef LOG
#undef DUMP
#undef DUMPC
#undef DUMPM
#undef LOGHEX
#undef DUMPHEX

#define LOG(a)           LOGPOS UPP::VppLog() << a << UPP::EOL
#define DUMP(a)          LOGPOS UPP::VppLog() << #a << " = " << (a) << UPP::EOL
#define DUMPC(c)         LOGPOS UPP::DumpContainer(UPP::VppLog() << #c << ':' << UPP::EOL, (c))
#define DUMPM(c)         LOGPOS UPP::DumpMap(VppLog() << #c << ':' << UPP::EOL, (c))
#define LOGHEX(x)        LOGPOS UPP::LogHex(x)
#define DUMPHEX(x)       LOGPOS UPP::VppLog() << #x << " = ", UPP::LogHex(x)

#undef DLOG
#undef DDUMP
#undef DDUMPC
#undef DDUMPM
#undef DLOGHEX
#undef DDUMPHEX

#define DLOG(a)           LOGPOS UPP::VppLog() << a << UPP::EOL
#define DDUMP(a)          LOGPOS UPP::VppLog() << #a << " = " << (a) << UPP::EOL
#define DDUMPC(c)         LOGPOS UPP::DumpContainer(UPP::VppLog() << #c << ':' << UPP::EOL, (c))
#define DDUMPM(c)         LOGPOS UPP::DumpMap(VppLog() << #c << ':' << UPP::EOL, (c))
#define DLOGHEX(x)        LOGPOS UPP::LogHex(x)
#define DDUMPHEX(x)       LOGPOS UPP::VppLog() << #x << " = ", UPP::LogHex(x)

#undef RLOG
#undef RDUMP
#undef RDUMPC
#undef RDUMPM
#undef RLOGHEX
#undef RDUMPHEX

#define RLOG(a)           LOGPOS UPP::VppLog() << a << UPP::EOL
#define RDUMP(a)          LOGPOS UPP::VppLog() << #a << " = " << (a) << UPP::EOL
#define RDUMPC(c)         LOGPOS UPP::DumpContainer(UPP::VppLog() << #c << ':' << UPP::EOL, (c))
#define RDUMPM(c)         LOGPOS UPP::DumpMap(VppLog() << #c << ':' << UPP::EOL, (c))
#define RLOGHEX(x)        LOGPOS UPP::LogHex(x)
#define RDUMPHEX(x)       LOGPOS UPP::VppLog() << #x << " = ", UPP::LogHex(x)

#define DO(x) void x(); x();

#define SECTION(x)        UPP::VppLog() << "=-= " << x << '\n';

void MakeTutorial();