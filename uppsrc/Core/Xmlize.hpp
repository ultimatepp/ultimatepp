template <class T, class X> XmlIO XmlIO::Var(const char *tag, T& var, X item_xmlize)
{
	XmlIO n(*this, tag);
	if(IsLoading() && n.Node().GetCount() == 0 && n.Node().GetAttrCount() == 0)
		return *this;
	item_xmlize(n, var);
	return *this;
}

template <class T> XmlIO XmlIO::operator()(const char *tag, T& var) {
	return Var(tag, var, [](XmlIO& io, T& var) { Xmlize(io, var); });
}

template <class T, class X> XmlIO XmlIO::Array(const char *tag, T& var, X item_xmlize, const char *itemtag)
{
	XmlIO n(*this, tag);
	if(IsLoading() && n.Node().GetCount() == 0 && n.Node().GetAttrCount() == 0)
		return *this;
	XmlizeContainer(n, itemtag, var, item_xmlize);
	return *this;
}

template <class T> XmlIO XmlIO::List(const char *tag, const char *itemtag, T& var) {
	return Array(tag, var, [](XmlIO& io, ValueTypeOf<T>& data) { Xmlize(io, data); }, itemtag);
}

template <class T, class D> XmlIO XmlIO::operator()(const char *tag, T& var, const D& def)
{
	XmlIO n(*this, tag);
	if(IsLoading() && n.Node().GetCount() == 0 && n.Node().GetAttrCount() == 0)
		var = def;
	else
		Xmlize(n, var);
	return *this;
}

template <class T, class D> XmlIO XmlIO::List(const char *tag, const char *itemtag, T& var, const D& def)
{
	XmlIO n(*this, tag);
	if(IsLoading() && n.Node().GetCount() == 0 && n.Node().GetAttrCount() == 0)
		var = def;
	else
		Xmlize(n, itemtag, var);
	return *this;
}

template <class T, class X>
void XmlizeContainer(XmlIO& xml, const char *tag, T& data, X item_xmlize)
{
	if(xml.IsStoring())
		for(int i = 0; i < data.GetCount(); i++) {
			XmlIO io = xml.Add(tag);
			item_xmlize(io, data[i]);
		}
	else {
		data.Clear();
		for(int i = 0; i < xml->GetCount(); i++)
			if(xml->Node(i).IsTag(tag)) {
				XmlIO io = xml.At(i);
				item_xmlize(io, data.Add());
			}
	}
}

template<class T>
void XmlizeContainer(XmlIO& xml, const char *tag, T& data)
{
	XmlizeContainer(xml, tag, data, [](XmlIO& xml, ValueTypeOf<T>& data) { Xmlize(xml, data); });
}

template<class K, class V, class T>
void XmlizeMap(XmlIO& xml, const char *keytag, const char *valuetag, T& data)
{
	if(xml.IsStoring()) {
		for(int i = 0; i < data.GetCount(); i++)
			if(!data.IsUnlinked(i)) {
				XmlIO k = xml.Add(keytag);
				XmlizeStore(k, data.GetKey(i));
				XmlIO v = xml.Add(valuetag);
				XmlizeStore(v, data[i]);
			}
	}
	else {
		data.Clear();
		int i = 0;
		while(i < xml->GetCount() - 1 && xml->Node(i).IsTag(keytag) && xml->Node(i + 1).IsTag(valuetag)) {
			K key;
			XmlIO k = xml.At(i++);
			Xmlize(k, key);
			XmlIO v = xml.At(i++);
			Xmlize(v, data.Add(key));
		}
	}
}

template<class K, class V, class T>
void XmlizeSortedMap(XmlIO& xml, const char *keytag, const char *valuetag, T& data)
{
	if(xml.IsStoring()) {
		for(int i = 0; i < data.GetCount(); i++) {
			XmlIO k = xml.Add(keytag);
			XmlizeStore(k, data.GetKey(i));
			XmlIO v = xml.Add(valuetag);
			XmlizeStore(v, data[i]);
		}
	}
	else {
		data.Clear();
		int i = 0;
		while(i < xml->GetCount() - 1 && xml->Node(i).IsTag(keytag) && xml->Node(i + 1).IsTag(valuetag)) {
			K key;
			XmlIO k = xml.At(i++);
			Xmlize(k, key);
			XmlIO v = xml.At(i++);
			Xmlize(v, data.Add(key));
		}
	}
}

template<class K, class T>
void XmlizeIndex(XmlIO& xml, const char *keytag, T& data)
{
	if(xml.IsStoring()) {
		for(int i = 0; i < data.GetCount(); i++)
			if(!data.IsUnlinked(i)) {
				XmlIO io = xml.Add(keytag);
				XmlizeStore(io, data[i]);
			}
	}
	else {
		data.Clear();
		int i = 0;
		while(i < xml->GetCount() && xml->Node(i).IsTag(keytag)) {
			K k;
			XmlIO io = xml.At(i++);
			Xmlize(io, k);
			data.Add(k);
		}
	}
}
