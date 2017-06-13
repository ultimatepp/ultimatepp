#ifndef _PolyXML_h_
#define _PolyXML_h_

#include <ClassFactory/ClassFactory.h>

NAMESPACE_UPP

template<class T> class WithPolyXML : public WithFactory<T>
{
	public:
		// Xmlizer
		virtual void Xmlize(XmlIO &xml) {};
		
		// Check if object is marked as erased so the array don't store it on xml
		// useful if you don't remove objects from array but just mark them as erased
		// to be redefined if you need this feature !
		virtual bool IsErased(void) { return false; }
};

template<class T> class PolyXMLUnknown : public T
{
	private:
		String tag;
		String rawXML;
	public:
		PolyXMLUnknown(String const &_tag, String const &xml)
		{
			tag = _tag;
			rawXML = xml;
		}
		
		virtual String const &IsA(void) { return CLASSFACTORY_UNKNOWN; }
		String const &GetUnknownClassName(void) { return tag; }
		
		virtual void Xmlize(XmlIO &xml)
		{
			if(xml.IsStoring())
			{
				XmlNode node = ParseXML(rawXML);
				xml.Add();
				xml.Node() = pick(node);
			}
		}
};

////////////////////////////////////////////////////////////////////////////////////////////////
// PolyXMLArray class -- add support for streaming polymorphic arrays of objects
template<class T> class PolyXMLArray : public Array<T>
{
	public:
		// Xmlizer
		void Xmlize(XmlIO &xml);
		
		T& Add(const T &data) { return Array<T>::Add(data); }
		T& Add(T *data) { return Array<T>::Add(data); }
		
		// progress callback
		Callback2<int, int>Progress;
};

template<class T> void PolyXMLArray<T>::Xmlize(XmlIO &xml)
{
	if(xml.IsStoring())
	{
		int count = PolyXMLArray::GetCount();
		for(int i = 0; i < count; i++)
		{
			Progress(i, count);
			T &data = PolyXMLArray::operator[](i);
			if(!data.IsErased())
			{
				String tag = data.IsA();
				XmlIO io = xml.Add(tag);
				data.Xmlize(io);
			}
		}
	}
	else
	{
		PolyXMLArray::Clear();
		int count = xml->GetCount();
		for(int i = 0; i < count; i++)
		{
			Progress(i, count);
			if(xml->Node(i).IsTag())
			{
				String tag = xml->Node(i).GetTag();
				T *data = T::CreatePtr(tag);
				if(data)
				{
					XmlIO io = xml.At(i);
					data->Xmlize(io);
					Add(data);
				}
				else
				{
					// unknown class -- gather raw xml node
					String rawXml = AsXML(xml.At(i).Node());
					
					// strips xml header, we don't need it
//					rawXml = rawXml.Mid(rawXml.Find(xml->Node(i).GetTag()));
					
					// creates an unknown class and stores raw xml on it
					Add((T *)new PolyXMLUnknown<T>(tag, rawXml));
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// PolyXMLArrayMap class -- add support for streaming polymorphic maps of objects
template<class K, class T> class PolyXMLArrayMap : public ArrayMap<K, T>
{
	public:
		// Xmlizer
		void Xmlize(XmlIO &xml);
		
		T& Add(const K &key, const T &data) { return ArrayMap<K, T>::Add(key, data); }
		T& Add(const K &key, T *data) { return ArrayMap<K, T>::Add(key, data); }
		
		// progress callback
		Callback2<int, int>Progress;
};

template<class K, class T> void PolyXMLArrayMap<K, T>::Xmlize(XmlIO &xml)
{
	if(xml.IsStoring())
	{
		int count = PolyXMLArrayMap::GetCount();
		for(int i = 0; i < count; i++)
		{
			Progress(i, count);

			// skip unlinked elements
			if(ArrayMap<K, T>::IsUnlinked(i))
				continue;
			T &data = PolyXMLArrayMap::operator[](i);
			K const &key = PolyXMLArrayMap::GetKey(i);
			
			// skip data marked as erase too
			if(data.IsErased())
				continue;
			String tag = data.IsA();
			XmlIO ioKey = xml.Add("key");
			XmlizeStore(ioKey, key);
			XmlIO ioTag = xml.Add(tag);
			XmlizeStore(ioTag, data);
		}
	}
	else
	{
		PolyXMLArrayMap<K, T>::Clear();
		int count = xml->GetCount() - 1;
		for(int i = 0; i < count && xml->Node(i).IsTag("key");)
		{
			Progress(i, count);

			if(xml->Node(i).IsTag())
			{
				K key;

				XmlIO io = xml.At(i++);
				Upp::Xmlize(io, key);
				String tag = xml->Node(i).GetTag();
				T *data = T::CreatePtr(tag);
				if(data)
				{
					XmlIO io = xml.At(i++);
					data->Xmlize(io);
					Add(key, data);
				}
				else
				{
					// unknown class -- gather raw xml node
					String rawXml = AsXML(xml.At(i).Node());
					
					// creates an unknown class and stores raw xml on it
					Add(key, (T *)new PolyXMLUnknown<T>(tag, rawXml));
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// PolyXMLArrayMapOne class -- add support for streaming polymorphic maps of One<> objects
template<class K, class T> class PolyXMLArrayMapOne : public ArrayMap<K, One<T> >
{
	public:
		// Xmlizer
		void Xmlize(XmlIO &xml);
		
		One<T>& Add(const K &key, pick_ One<T> &data) { return ArrayMap<K, One<T> >::AddPick(key, pick(data)); }
		
		// progress callback
		Callback2<int, int>Progress;
};

template<class K, class T> void PolyXMLArrayMapOne<K, T>::Xmlize(XmlIO &xml)
{
	if(xml.IsStoring())
	{
		int count = PolyXMLArrayMapOne::GetCount();
		for(int i = 0; i < count; i++)
		{
			Progress(i, count);

			// skip unlinked elements
			if(ArrayMap<K, One<T> >::IsUnlinked(i))
				continue;
			One<T> &data = PolyXMLArrayMapOne::operator[](i);
			K const &key = PolyXMLArrayMapOne::GetKey(i);
			
			// skip data marked as erase too
			if(data->IsErased())
				continue;
			String tag = data->IsA();
			XmlIO ioKey = xml.Add("key");
			XmlizeStore(ioKey, key);
			XmlIO ioTag = xml.Add(tag);
			XmlizeStore(ioTag, *data);
		}
	}
	else
	{
		PolyXMLArrayMapOne<K, T>::Clear();
		int count = xml->GetCount() - 1;
		for(int i = 0; i < count && xml->Node(i).IsTag("key");)
		{
			Progress(i, count);

			if(xml->Node(i).IsTag())
			{
				K key;
				XmlIO io = xml.At(i++);
				Upp::Xmlize(io, key);
				String tag = xml->Node(i).GetTag();
				One<T> data = T::CreatePtr(tag);
				if(data)
				{
					XmlIO io = xml.At(i++);
					data->Xmlize(io);
					Add(key, data);
				}
				else
				{
					// unknown class -- gather raw xml node
					String rawXml = AsXML(xml.At(i).Node());
					
					// creates an unknown class and stores raw xml on it
					One<T> raw = (T *)new PolyXMLUnknown<T>(tag, rawXml);
					Add(key, raw);
				}
			}
		}
	}
}

END_UPP_NAMESPACE

#endif
