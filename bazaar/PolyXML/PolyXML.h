#ifndef _PolyXML_h_
#define _PolyXML_h_

#include "ClassFactory.h"

NAMESPACE_UPP;

template<class T> class WithPolyXML : public WithFactory<T>
{
	public:
		// Xmlizer
		virtual void Xmlize(XmlIO xml) {};
};

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
					data->Xmlize(xml.At(i));
					Add(data);
				}
			}
		}
	}
}

END_UPP_NAMESPACE;

#endif
