#include <PolyXML/PolyXML.h>

using namespace Upp;

class Base : public WithPolyXML<Base>
{
	public:
		String BaseData;
		void Xmlize(XmlIO xml) { xml("BaseData", BaseData); }
		Base() { BaseData = "Sample data in Base class"; }
};

class Derived : public Base
{
	public:
		String DerivedData;
		void Xmlize(XmlIO xml) { Base::Xmlize(xml); xml("DerivedData", DerivedData); }
		Derived() { DerivedData = "Sample data in Derived class"; }
};

class Another : public Derived
{
	public:
		int AnotherData;
		void Xmlize(XmlIO xml) { Derived::Xmlize(xml); xml("AnotherData", AnotherData); }
		Another() { AnotherData = 7; }
};

class OneMore : public Another
{
	public:
		double OneMoreData;
		void Xmlize(XmlIO xml) { Derived::Xmlize(xml); xml("AnotherData", AnotherData); }
		OneMore() { OneMoreData = 3.14; }
};

// USUALLY WE REGISTER CLASSES AT STARTUP, LIKE FOLLOWING (COMMENTED) CODE
// NOW, TO MAKE MULTIPLE TESTS, WE MANUALLY REGISTER THEM INSIDE MAIN CODE
// AND DE-REGISTER ALL TO ALLOW NEW TESTS (IN PARTICULAR, TO SEE WHAT HAPPENS
// LOADING AN UNREGISTERED CLASS
/*
REGISTERCLASS(Base);
REGISTERCLASS(Derived, "you can add a description");
REGISTERCLASS(Another, "you can add a description and also an index", 10);
REGISTERCLASS(OneMore, "last class with description and index", 12);
*/

void DumpArray(PolyXMLArray<Base> &polyArray)
{
	
	Cerr() << "\nArray content after streaming in : " << polyArray.GetCount() << " classes:\n";
	for(int i = 0; i < polyArray.GetCount(); i++)
	{
		Cerr() << "  class #" << i << " is a '" << polyArray[i].IsA() << "'\n";
		if(polyArray[i].IsA() == "Base")
			Cerr() << "    BaseData    = '" << ((Base &)polyArray[i]).BaseData << "'\n";
		else if(polyArray[i].IsA() == "Derived")
		{
			Cerr() << "    BaseData    = '" << ((Derived &)polyArray[i]).BaseData << "'\n";
			Cerr() << "    DerivedData = '" << ((Derived &)polyArray[i]).DerivedData << "'\n";
		}
		else if(polyArray[i].IsA() == "Another")
		{
			Cerr() << "    BaseData    = '" << ((Another &)polyArray[i]).BaseData << "'\n";
			Cerr() << "    DerivedData = '" << ((Another &)polyArray[i]).DerivedData << "'\n";
			Cerr() << "    AnotherData = '" << ((Another &)polyArray[i]).AnotherData << "'\n";
		}
		else if(polyArray[i].IsA() == "OneMore")
		{
			Cerr() << "    BaseData    = '" << ((OneMore &)polyArray[i]).BaseData << "'\n";
			Cerr() << "    DerivedData = '" << ((OneMore &)polyArray[i]).DerivedData << "'\n";
			Cerr() << "    AnotherData = '" << ((OneMore &)polyArray[i]).AnotherData << "'\n";
			Cerr() << "    OneMoreData = '" << ((OneMore &)polyArray[i]).OneMoreData << "'\n";
		}
		else if(polyArray[i].IsA() == POLYXMLUNKNOWN)
			Cerr() << "    Original class is '" << ((PolyXMLUnknown<Base> &)polyArray[i]).GetUnknownClassName() << "' -- STREAMED IN AS RAW XML\n";
		else
			Cerr() << "oops... known class, but I don't know how to handle it\n";
	}
}

CONSOLE_APP_MAIN
{
	// MANUAL CLASS REGISTRATION, SEE ABOVE COMMENT !
	Base::Register<Base>("Base");
	Derived::Register<Derived>("Derived", "you can add a description");
	Another::Register<Another>("Another", "you can add a description and also an index", 10);
	OneMore::Register<OneMore>("OneMore", "last class with description and index", 12);

	Cerr() << "You have registered " << Base::Classes().GetCount() << " classes:\n";
	for(int i = 0; i < Base::Classes().GetCount(); i++)
		Cerr() << "  Class#" << i << " is a '" << Base::Classes()[i] 
			<< "'  Description '" << Base::GetClassDescription(Base::Classes()[i]) 
			<< "'  Index '" << Base::GetClassIndex(Base::Classes()[i]) << "'\n";

	PolyXMLArray<Base> polyArray;
	
	Base *b = new Base;
	b->BaseData = "Sample data in base class";
	polyArray.Add(b);
	
	Derived *d = new Derived;
	d->BaseData = "Sample data in derived class";
	d->DerivedData = "Another sample data in derived class";
	polyArray.Add(d);
	
	Another *a = dynamic_cast<Another *>(Base::CreatePtr("Another"));
	a->BaseData = "Sample data in Derived class";
	a->DerivedData = "Another sample data in Derived class";
	a->AnotherData = 12345;
	polyArray.Add(a);
	
	OneMore *o = new OneMore;
	o->BaseData = "Sample data in OneMore class";
	o->DerivedData = "Another sample data in OneMore class";
	o->AnotherData = 12;
	o->OneMoreData = 3.1418;
	polyArray.Add(o);
	
	Cerr() << "\nArray content before streaming out: " << polyArray.GetCount() << " classes:\n";
	DumpArray(polyArray);

	String s = StoreAsXML(polyArray, "PolyXMLTest");
	Cerr() << "\nStreamed XML : \n\n" << s ;
	
	polyArray.Clear();
	LoadFromXML(polyArray, s);
	
	Cerr() << "\nArray content after streaming in : " << polyArray.GetCount() << " classes:\n";
	DumpArray(polyArray);
	
	// Now we de-register all classes and re-register all of them BESIDES one
	// to test in-streaming with an unknown class
	Base::UnregisterAll();
	Base::Register<Base>("Base");
	Derived::Register<Derived>("Derived", "you can add a description");
	OneMore::Register<OneMore>("OneMore", "last class with description and index", 12);

	Cerr() << "\nStreaming in XML with an unknown class : \n\n";
	
	polyArray.Clear();
	LoadFromXML(polyArray, s);
	
	Cerr() << "\nArray content after streaming in : " << polyArray.GetCount() << " classes:\n";
	DumpArray(polyArray);
	
	Cerr() << "\nStreaming out XML with an unknown class : \n\n";
	s = StoreAsXML(polyArray, "PolyXMLTest");
	
	Cerr() << "\nRedefining missing class and streaming it the whole stuff again : \n\n";
	Another::Register<Another>("Another", "you can add a description and also an index", 10);
	polyArray.Clear();
	LoadFromXML(polyArray, s);
	DumpArray(polyArray);

}

