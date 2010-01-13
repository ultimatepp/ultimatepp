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

REGISTERCLASS(Base);
REGISTERCLASS(Derived);

CONSOLE_APP_MAIN
{
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
