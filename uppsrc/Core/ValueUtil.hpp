inline
void ValueMap::Add(const Value& key, const Value& value) {
	Data& d = UnShare();
	d.key.Add(key);
	d.value.Add(value);
}

#ifdef DEPRECATED
template <class T>
struct RawRef : public RefManager {
	virtual void  SetValue(void *p, const Value& v)       { *(T *) p = RawValue<T>::Extract(v); }
	virtual Value GetValue(const void *p)                 { return RawValue<T>(*(const T *) p); }
	virtual int   GetType()                               { return GetValueTypeNo<T>(); }
	virtual ~RawRef() {}
};

template <class T>
Ref RawAsRef(T& x) {
	return Ref(&x, &Single< RawRef<T> >());
}

template <class T>
struct RichRef : public RawRef<T> {
	virtual Value GetValue(const void *p)                 { return RichToValue(*(T *) p); }
	virtual bool  IsNull(const void *p)                   { return UPP::IsNull(*(T *) p); }
	virtual void  SetValue(void *p, const Value& v)       { *(T *) p = T(v); }
	virtual void  SetNull(void *p)                        { UPP::SetNull(*(T *)p); }
};

template <class T>
Ref RichAsRef(T& x) {
	return Ref(&x, &Single< RichRef<T> >());
}
#endif