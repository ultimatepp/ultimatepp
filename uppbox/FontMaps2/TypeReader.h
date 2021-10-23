#ifndef _FontMaps2_TypeReader_h_
#define _FontMaps2_TypeReader_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class FontTypeReader {
	String font;

	struct Table : Moveable<Table> {
		int offset;
		int length;
	};
	VectorMap<String, Table> table;

	struct Fail {};
	static int Error() { throw Fail(); return 0; }

	int    Peek8(const char *s);
	int    Peek16(const char *s);
	int    Peek32(const char *s);
	int    Peek8(const char *s, int i);
	int    Peek16(const char *s, int i);
	int    Peek32(const char *s, int i);
	int    Read8(const char *&s);
	int    Read16(const char *&s);
	int    Read32(const char *&s);

	const char *Seek(const char *tab, int& len);
	const char *Seek(const char *tab);
//	void        Seek(const char *tab, TTFStreamIn& s);
	String      GetTable(const char *tab);

public:
	String ps_name;
	
	Array<Tuple<int, int>> ranges;

	bool   Open(const String& fnt, bool symbol = false, bool justcheck = false);
};

#endif
