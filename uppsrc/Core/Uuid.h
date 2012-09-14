struct Uuid : AssignValueTypeNo<Uuid, 50, Moveable<Uuid> > {
	dword a, b, c, d;

	void Serialize(Stream& s);
	void Xmlize(XmlIO& xio);
	void Jsonize(JsonIO& jio);
	bool IsNullInstance() const   { return a == 0 && b == 0 && c == 0 && d == 0; }
	void SetNull()                { a = b = c = d = 0; }

	operator Value() const        { return RichToValue(*this); }
	Uuid(const Value& q)          { *this = q.Get<Uuid>(); }
	Uuid(const Nuller&)           { SetNull(); }
	Uuid()                        {}

	unsigned GetHashValue() const { return CombineHash(a, b, c, d); }
	String   ToString() const;
	String   ToStringWithDashes() const;
	
	void     New()                { *this = Create(); }

	static Uuid Create();
};

String Format(const Uuid& id);
String FormatWithDashes(const Uuid& id);
Uuid   ScanUuid(const char *s);

inline bool  operator==(const Uuid& u, const Uuid& w) {
	return ((u.a ^ w.a) | (u.b ^ w.b) | (u.c ^ w.c) | (u.d ^ w.d)) == 0;
}

inline bool  operator!=(const Uuid& u, const Uuid& w) {
	return !(u == w);
}

template<>
inline String AsString(const Uuid& id) { return Format(id); }

ValueGen& UuidValueGen();
