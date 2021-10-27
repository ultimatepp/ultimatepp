#ifndef _FontMaps2_TypeReader_h_
#define _FontMaps2_TypeReader_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class FontTypeReader {
	String data;

	struct Table : Moveable<Table> {
		int offset;
		int length;
	};
	VectorMap<String, Table> table;

	struct Fail {};
	static int Error();

	int    Peek8(const char *s);
	int    Peek16(const char *s);
	int    Peek32(const char *s);
	int    Peek8(const char *s, int i);
	int    Peek16(const char *s, int i);
	int    Peek32(const char *s, int i);
	int    Read8(const char *&s);
	int    Read16(const char *&s);
	int    Read32(const char *&s);

public:
	String ps_name;
	
	Array<Tuple<int, int>> ranges;
	
	String panose;

	bool  Open(Font font, bool symbol = false, bool justcheck = false);
};

#endif
