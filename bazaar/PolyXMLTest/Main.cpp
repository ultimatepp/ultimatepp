#include <PolyXML/PolyXML.h>

using namespace Upp;

class Base : public WithPolyXML<Base>
{
	public:
		String BaseData;
		void Xmlize(XmlIO xml) { xml("BaseData", BaseData); }
};

class Derived : public Base
{
	public:
		String DerivedData;
		void Xmlize(XmlIO xml) { Base::Xmlize(xml); xml("DerivedData", DerivedData); }
};

class Another : public Derived
{
	public:
		String AnotherData;
		void Xmlize(XmlIO xml) { Derived::Xmlize(xml); xml("AnotherData", AnotherData); }
};

REGISTERCLASS(Base);
REGISTERCLASS(Derived, "you can add a description");
REGISTERCLASS(Another, "you can add a description and also an index", 10);

CONSOLE_APP_MAIN
{
	Cerr() << "You have registered " << Base::Classes().GetCount() << ":\n";
	for(int i = 0; i < Base::Classes().GetCount(); i++)
		Cerr() << "  Class#" << i << " is a '" << Base::Classes()[i] << "'  Description '" << Base::GetClassDescription(Base::Classes()[i]) << "'\n";

	PolyXMLArray<Base> polyArray;
	
	Base *b = new Base;
	b->BaseData = "Sample data in base class";
	polyArray.Add(b);
	
	Derived *d = new Derived;
	d->BaseData = "Sample data in derived class";
	d->DerivedData = "Another sample data in derived class";
	polyArray.Add(d);
	
	Cerr() << "\nArray content before streaming out: " << polyArray.GetCount() << " classes:\n";
	for(int i = 0; i < polyArray.GetCount(); i++)
	{
		Cerr() << "  class #" << i << " is a '" << polyArray[i].IsA() << "'\n";
		if(polyArray[i].IsA() == "Base")
			Cerr() << "    BaseData    = '" << ((Base &)polyArray[i]).BaseData << "'\n";
		else
		{
			Cerr() << "    BaseData    = '" << ((Derived &)polyArray[i]).BaseData << "'\n";
			Cerr() << "    DerivedData = '" << ((Derived &)polyArray[i]).DerivedData << "'\n";
		}
	}
	
	String s = StoreAsXML(polyArray, "PolyXMLTest");
	
	Cerr() << "\nStreamed XML : \n\n" << s ;
	
	polyArray.Clear();
	LoadFromXML(polyArray, s);
	
	Cerr() << "\nArray content after streaming in : " << polyArray.GetCount() << " classes:\n";
	for(int i = 0; i < polyArray.GetCount(); i++)
	{
		Cerr() << "  class #" << i << " is a '" << polyArray[i].IsA() << "'\n";
		if(polyArray[i].IsA() == "Base")
			Cerr() << "    BaseData    = '" << ((Base &)polyArray[i]).BaseData << "'\n";
		else
		{
			Cerr() << "    BaseData    = '" << ((Derived &)polyArray[i]).BaseData << "'\n";
			Cerr() << "    DerivedData = '" << ((Derived &)polyArray[i]).DerivedData << "'\n";
		}
	}
}
