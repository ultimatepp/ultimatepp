
struct ExifReader {
	String app1;
	bool   le;
	
	const byte *Begin() const   { return (const byte *)~app1 + 6; }
	const byte *End() const     { return (const byte *)app1.End(); }
	operator bool() const       { return app1.GetCount(); }
	
	uint32 Peek32(const byte *ptr) const { return (le ? Peek32le : Peek32be)(ptr); }
	uint16 Peek16(const byte *ptr) const { return (le ? Peek16le : Peek16be)(ptr); }
	
	String GetThumbnail() const;
	
	ExifReader(const String& app1);
};

ExifReader::ExifReader(const String& q)
{
	app1 = q;
	if(app1.GetCount() > 30 && memcmp(~app1, "ExifReader", 4) == 0)
		if(app1[6] == 'I')
			le = true;
		else
		if(app1[6] == 'M')
			le = false;
		else
			app1.Clear();
	else
		app1.Clear();
}

String ExifReader::GetThumbnail() const
{
	if(!app1.GetCount())
		return Null;
	const byte *begin = Begin();
	const byte *end = End();
	const byte *p = begin + Peek32(begin + 4);
	if(p + 2 >= end) return Null;
	p += Peek16(p) * 12 + 2;
	if(p + 4 >= end) return Null;
	p = begin + Peek32(p);
	if(p <= begin || p + 2 >= end) return Null;
	word count = Peek16(p);
	p += 2;
	dword offset = 0;
	dword len = 0;
	for(int n = 0; n < count; n++) {
		if(p + 12 >= end) Null;
		if(Peek32(p + 4) == 1) {
			dword val = 0;
			switch(Peek16(p + 2)) {
			case 4:
			case 9:
				val = Peek32(p + 8);
				break;
			case 3:
			case 8:
				val = Peek16(p + 8);
				break;
			}
			if(val)
				switch(Peek16(p)) {
				case 0x201:
					offset = val;
					break;
				case 0x202:
					len = val;
					break;
				}
		}
		p += 12;
	}
	return offset && len && begin + offset + len < end ? String(begin + offset, len) : String();
}
