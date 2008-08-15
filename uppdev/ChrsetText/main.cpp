#include <Core/Core.h>

void Dumps(const String& s)
{
	for(int i = 0; i < s.GetLength(); i++)
		VppLog() << Format("%02X ", s[i]);
	VppLog() << "\n";
}

void Dumps(const WString& s)
{
	for(int i = 0; i < s.GetLength(); i++)
		VppLog() << Format("%04X ", s[i]);
	VppLog() << "\n";
}

#define lng_(a, b, c, d)   ( (((a - 'A' + 1) & 31) << 15) | (((b - 'A' + 1) & 31) << 10) | \
                             (((c - 'A' + 1) & 31) << 5) | (((d - 'A' + 1) & 31) << 0) )

#define lngc_(a, b, c, d, cs) ( (((a - 'A' + 1) & 31) << 15) | (((b - 'A' + 1) & 31) << 10) | \
                                (((c - 'A' + 1) & 31) << 5) | (((d - 'A' + 1) & 31) << 0) | \
                                ((cs & 255) << 20) )

void Main2()
{
	DUMP(lngAsText(lng_('C', 'S', 'C', 'Z')));
	DUMP(lngAsText(lngc_('C', 'S', 'C', 'Z', CHARSET_WIN1250)));

	DUMP(lngAsText(lngFromText("CS-CZ windows-1250")));
	DUMP(lngAsText(lngFromText("CS-CZ windows-1250")));

	DUMP(usehex << lng_('E', 'N', 'U', 'S'));
	DUMP(usehex << lngc_('E', 'N', 'U', 'S', 0));

	DUMP(usehex << lng_('C', 'S', 'C', 'Z'));
	DUMP(usehex << lngc_('C', 'S', 'C', 'Z', CHARSET_WIN1250));


	Dumps(ToUtf8(WString(0x80, 1)));
	Dumps(ToUtf8(WString(0x7ff, 1)));
	Dumps(ToUtf8(WString(0x800, 1)));
	Dumps(ToUtf8(WString(0xffff, 1)));

	LOG("--------");
	for(int i = 0; i < 0xffff; i++) {
		wchar w = i;
		Dumps(ToUtf8(&w, 1));
	}

	int q = 0;
	for(;;) {
		if(q++ % 100000 == 0)
			printf("%d\n", q);
		int len = rand() & 63;
		String data;
		for(int q = 0; q < len; q++)
			data.Cat(rand());
		WString wd = FromUtf8(data);
		String data2 = ToUtf8(wd);
/*		DUMP(len);
		Dumps(data);
		Dumps(wd);
		Dumps(data2);
		LOG("--------------------");*/
		ASSERT(data == data2);
	}
}
