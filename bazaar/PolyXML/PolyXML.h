#ifndef _PolyXML_h_
#define _PolyXML_h_

#include "ClassFactory.h"

NAMESPACE_UPP;

template<class T> class WithPolyXML : public WithFactory<T>
{
	public:
		// Xmlizer
		virtual void Xmlize(XmlIO xml) {};
		
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
		
		virtual void Xmlize(XmlIO xml)
		{
			if(xml.IsStoring())
			{
				XmlNode node = ParseXML(rawXML);
				xml.Add();
				xml.Node() = node;
			}
		}
};

////////////////////////////////////////////////////////////////////////////////////////////////
// PolyXMLArray class -- add support for streaming polymorphic arrays of objects
template<class T> class PolyXMLArray : public Array<T>
{
	public:
		// Xmlizer
		void Xmlize(XmlIO xml);
		
		void Add(const T &data) { Array<T>::Add(data); }
		void Add(T *data) { Array<T>::Add(data); }
};

template<class T> void PolyXMLArray<T>::Xmlize(XmlIO xml)
{
	if(xml.IsStoring())
	{
		for(int i = 0; i < PolyXMLArray::GetCount(); i++)
		{
			T &data = PolyXMLArray::operator[](i);
			if(!data.IsErased())
			{
				String tag = data.IsA();
				data.Xmlize(xml.Add(tag));
			}
		}
	}
	else
	{
		PolyXMLArray::Clear();
		for(int i = 0; i < xml->GetCount(); i++)
		{
			if(xml->Node(i).IsTag())
			{
				String tag = xml->Node(i).GetTag();
				T *data = T::CreatePtr(tag);
				if(data)
				{
					data->Xmlize(xml.At(i));
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
		void Xmlize(XmlIO xml);
		
		void Add(const K &key, const T &data) { ArrayMap<K, T>::Add(key, data); }
		void Add(const K &key, T *data) { ArrayMap<K, T>::Add(key, data); }
};

template<class K, class T> void PolyXMLArrayMap<K, T>::Xmlize(XmlIO xml)
{
	if(xml.IsStoring())
	{
		for(int i = 0; i < PolyXMLArrayMap::GetCount(); i++)
		{
			// skip unlinked elements
			if(ArrayMap<K, T>::IsUnlinked(i))
				continue;
			T &data = PolyXMLArrayMap::operator[](i);
			K const &key = PolyXMLArrayMap::GetKey(i);
			
			// skip data marked as erase too
			if(data.IsErased())
				continue;
			String tag = data.IsA();
			XmlizeStore(xml.Add("key"), key);
			XmlizeStore(xml.Add(tag), data);
		}
	}
	else
	{
		PolyXMLArrayMap<K, T>::Clear();
		for(int i = 0; i < xml->GetCount() - 1 && xml->Node(i).IsTag("key");)
		{
			if(xml->Node(i).IsTag())
			{
				K key;
				Upp::Xmlize(xml.At(i++), key);
				String tag = xml->Node(i).GetTag();
				T *data = T::CreatePtr(tag);
				if(data)
				{
					data->Xmlize(xml.At(i++));
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
		void Xmlize(XmlIO xml);
		
		void Add(const K &key, const One<T> &data) { ArrayMap<K, One<T> >::Add(key, data); }
};

template<class K, class T> void PolyXMLArrayMapOne<K, T>::Xmlize(XmlIO xml)
{
	if(xml.IsStoring())
	{
		for(int i = 0; i < PolyXMLArrayMapOne::GetCount(); i++)
		{
			// skip unlinked elements
			if(ArrayMap<K, One<T> >::IsUnlinked(i))
				continue;
			One<T> &data = PolyXMLArrayMapOne::operator[](i);
			K const &key = PolyXMLArrayMapOne::GetKey(i);
			
			// skip data marked as erase too
			if(data->IsErased())
				continue;
			String tag = data->IsA();
			XmlizeStore(xml.Add("key"), key);
			XmlizeStore(xml.Add(tag), *data);
		}
	}
	else
	{
		PolyXMLArrayMapOne<K, T>::Clear();
		for(int i = 0; i < xml->GetCount() - 1 && xml->Node(i).IsTag("key");)
		{
			if(xml->Node(i).IsTag())
			{
				K key;
				Upp::Xmlize(xml.At(i++), key);
				String tag = xml->Node(i).GetTag();
				One<T> data = T::CreatePtr(tag);
				if(data)
				{
					data->Xmlize(xml.At(i++));
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

END_UPP_NAMESPACE;

#endif
