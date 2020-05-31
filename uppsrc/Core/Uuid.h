struct Uuid : AssignValueTypeNo<Uuid, 50, Moveable<Uuid> > {
	uint64 v[2];

	void Serialize(Stream& s);
	void Xmlize(XmlIO& xio);
	void Jsonize(JsonIO& jio);
	bool IsNullInstance() const   { return (v[0] | v[1]) == 0; }
	void SetNull()                { v[0] = v[1] = 0; }

	operator Value() const        { return RichToValue(*this); }
	Uuid(const Value& q)          { *this = q.Get<Uuid>(); }
	Uuid(const Nuller&)           { SetNull(); }
	Uuid()                        {}

	hash_t   GetHashValue() const { return CombineHash(v[0], v[1]); }
	String   ToString() const;
	String   ToStringWithDashes() const;
	
	void     New();

	static Uuid Create()          { Uuid uuid; uuid.New(); return uuid; }
};

String Format(const Uuid& id);
String FormatWithDashes(const Uuid& id);
Uuid   ScanUuid(const char *s);

inline bool  operator==(const Uuid& u, const Uuid& w) {
	return ((u.v[0] ^ w.v[0]) | (u.v[1] ^ w.v[1])) == 0;
}

inline bool  operator!=(const Uuid& u, const Uuid& w) {
	return !(u == w);
}

template<>
inline String AsString(const Uuid& id) { return Format(id); }

ValueGen& UuidValueGen();
