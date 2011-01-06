#ifndef _PolyXML_h_
#define _PolyXML_h_

#include "ClassFactory.h"

NAMESPACE_UPP;

template<class T> class WithPolyXML : public WithFactory<T>
{
	template<class X> friend class PolyXMLArray;
	
	private:
		unsigned loadingVersion;
		
	public:
		// constructor
		WithPolyXML() { loadingVersion = 0; }
		
		// loading version checking
		unsigned GetLoadingVersion(void) { return loadingVersion; }
		
		// Xmlizer
		virtual void Xmlize(XmlIO xml) {};
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

template<class T> class PolyXMLArray : public Array<T>
{
	private:
		unsigned loadingVersion;
		unsigned currentVersion;

	protected:
		
	public:
		PolyXMLArray() { loadingVersion = 0; currentVersion = 0; }
		
		// Xmlizer
		void Xmlize(XmlIO xml);
		
		void Add(const T &data) { Array<T>::Add(data); }
		void Add(T *data) { Array<T>::Add(data); }
		
		// version handling
		unsigned GetCurrentVersion(void) { return currentVersion; }
		unsigned SetCurrentVersion(int v) { currentVersion = v; }
		unsigned GetLoadingVersion(void) { return loadingVersion; }
		unsigned SetLoadingVersion(int v) { loadingVersion = v; }
};

template<class T> void PolyXMLArray<T>::Xmlize(XmlIO xml)
{
	if(xml.IsStoring())
	{
		for(int i = 0; i < PolyXMLArray::GetCount(); i++)
		{
			T &data = PolyXMLArray::operator[](i);
			String tag = data.IsA();
			data.Xmlize(xml.Add(tag));
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
					// sets loading version in class
					// so xmlizers know how to handle older versions
					data->loadingVersion = loadingVersion;
					
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

END_UPP_NAMESPACE;

#endif
