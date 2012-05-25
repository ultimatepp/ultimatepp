#include <PolyXML/PolyXML.h>

using namespace Upp;

class Base : public WithPolyXML<Base>, public Pte<Base>
{
	public:
		String BaseData;
		void Xmlize(XmlIO &xml) { xml("BaseData", BaseData); }
		Base() { BaseData = "Sample data in Base class"; }
};

class Derived : public Base
{
	public:
		String DerivedData;
		void Xmlize(XmlIO &xml) { Base::Xmlize(xml); xml("DerivedData", DerivedData); }
		Derived() { DerivedData = "Sample data in Derived class"; }
};

class Another : public Derived
{
	public:
		int AnotherData;
		void Xmlize(XmlIO &xml) { Derived::Xmlize(xml); xml("AnotherData", AnotherData); }
		Another() { AnotherData = 7; }
};

class OneMore : public Another
{
	public:
		double OneMoreData;
		void Xmlize(XmlIO &xml) { Derived::Xmlize(xml); xml("AnotherData", AnotherData); }
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
		else if(polyArray[i].IsA() == "*UNKNOWN*")
			Cerr() << "    Original class is '" << ((PolyXMLUnknown<Base> &)polyArray[i]).GetUnknownClassName() << "' -- STREAMED IN AS RAW XML\n";
		else
			Cerr() << "oops... known class, but I don't know how to handle it\n";
	}
}

void DumpMap(PolyXMLArrayMap<String, Base> &polyMap)
{
	for(int i = 0; i < polyMap.GetCount(); i++)
	{
		Cerr() << "  class with key '" << polyMap.GetKey(i) << "' is a '" << polyMap[i].IsA() << "'\n";
		if(polyMap[i].IsA() == "Base")
			Cerr() << "    BaseData    = '" << ((Base &)polyMap[i]).BaseData << "'\n";
		else if(polyMap[i].IsA() == "Derived")
		{
			Cerr() << "    BaseData    = '" << ((Derived &)polyMap[i]).BaseData << "'\n";
			Cerr() << "    DerivedData = '" << ((Derived &)polyMap[i]).DerivedData << "'\n";
		}
		else if(polyMap[i].IsA() == "Another")
		{
			Cerr() << "    BaseData    = '" << ((Another &)polyMap[i]).BaseData << "'\n";
			Cerr() << "    DerivedData = '" << ((Another &)polyMap[i]).DerivedData << "'\n";
			Cerr() << "    AnotherData = '" << ((Another &)polyMap[i]).AnotherData << "'\n";
		}
		else if(polyMap[i].IsA() == "OneMore")
		{
			Cerr() << "    BaseData    = '" << ((OneMore &)polyMap[i]).BaseData << "'\n";
			Cerr() << "    DerivedData = '" << ((OneMore &)polyMap[i]).DerivedData << "'\n";
			Cerr() << "    AnotherData = '" << ((OneMore &)polyMap[i]).AnotherData << "'\n";
			Cerr() << "    OneMoreData = '" << ((OneMore &)polyMap[i]).OneMoreData << "'\n";
		}
		else if(polyMap[i].IsA() == "*UNKNOWN*")
			Cerr() << "    Original class is '" << ((PolyXMLUnknown<Base> &)polyMap[i]).GetUnknownClassName() << "' -- STREAMED IN AS RAW XML\n";
		else
			Cerr() << "oops... known class, but I don't know how to handle it\n";
	}
}

void DumpMapOne(PolyXMLArrayMapOne<String, Base> &polyMap)
{
	for(int i = 0; i < polyMap.GetCount(); i++)
	{
		Cerr() << "  class with key '" << polyMap.GetKey(i) << "' is a '" << polyMap[i]->IsA() << "'\n";
		if(polyMap[i]->IsA() == "Base")
			Cerr() << "    BaseData    = '" << ((One<Base> &)polyMap[i])->BaseData << "'\n";
		else if(polyMap[i]->IsA() == "Derived")
		{
			Cerr() << "    BaseData    = '" << ((One<Derived> &)polyMap[i])->BaseData << "'\n";
			Cerr() << "    DerivedData = '" << ((One<Derived> &)polyMap[i])->DerivedData << "'\n";
		}
		else if(polyMap[i]->IsA() == "Another")
		{
			Cerr() << "    BaseData    = '" << ((One<Another> &)polyMap[i])->BaseData << "'\n";
			Cerr() << "    DerivedData = '" << ((One<Another> &)polyMap[i])->DerivedData << "'\n";
			Cerr() << "    AnotherData = '" << ((One<Another> &)polyMap[i])->AnotherData << "'\n";
		}
		else if(polyMap[i]->IsA() == "OneMore")
		{
			Cerr() << "    BaseData    = '" << ((One<OneMore> &)polyMap[i])->BaseData << "'\n";
			Cerr() << "    DerivedData = '" << ((One<OneMore> &)polyMap[i])->DerivedData << "'\n";
			Cerr() << "    AnotherData = '" << ((One<OneMore> &)polyMap[i])->AnotherData << "'\n";
			Cerr() << "    OneMoreData = '" << ((One<OneMore> &)polyMap[i])->OneMoreData << "'\n";
		}
		else if(polyMap[i]->IsA() == "*UNKNOWN*")
			Cerr() << "    Original class is '" << ((One<PolyXMLUnknown<Base> > &)polyMap[i])->GetUnknownClassName() << "' -- STREAMED IN AS RAW XML\n";
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

	{
		Cerr() << "\n/////////////////////////////////////////////////////////////////////////////////////////";
		Cerr() << "\nPolyXMLArray tests";
		Cerr() << "\n/////////////////////////////////////////////////////////////////////////////////////////";
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

	{	
		Cerr() << "\n/////////////////////////////////////////////////////////////////////////////////////////";
		Cerr() << "\nPolyXMLArrayMap tests";
		Cerr() << "\n/////////////////////////////////////////////////////////////////////////////////////////";
	
		PolyXMLArrayMap<String, Base> polyMap;
		
		Base *b = new Base;
		b->BaseData = "Sample data in base class";
		polyMap.Add("BaseKey", b);
		
		Derived *d = new Derived;
		d->BaseData = "Sample data in derived class";
		d->DerivedData = "Another sample data in derived class";
		polyMap.Add("DerivedKey", d);
		
		Another *a = dynamic_cast<Another *>(Base::CreatePtr("Another"));
		a->BaseData = "Sample data in Derived class";
		a->DerivedData = "Another sample data in Derived class";
		a->AnotherData = 12345;
		polyMap.Add("AnotherKey", a);
		
		OneMore *o = new OneMore;
		o->BaseData = "Sample data in OneMore class";
		o->DerivedData = "Another sample data in OneMore class";
		o->AnotherData = 12;
		o->OneMoreData = 3.1418;
		polyMap.Add("OneMoreKey", o);
		
		Cerr() << "\nMap content before streaming out: " << polyMap.GetCount() << " classes:\n";
		DumpMap(polyMap);
	
		String s = StoreAsXML(polyMap, "PolyXMLTest");
		Cerr() << "\nStreamed XML : \n\n" << s ;
		
		polyMap.Clear();
		LoadFromXML(polyMap, s);
		
		Cerr() << "\nMap content after streaming in : " << polyMap.GetCount() << " classes:\n";
		DumpMap(polyMap);
		
		// Now we de-register all classes and re-register all of them BESIDES one
		// to test in-streaming with an unknown class
		Base::UnregisterAll();
		Base::Register<Base>("Base");
		Derived::Register<Derived>("Derived", "you can add a description");
		OneMore::Register<OneMore>("OneMore", "last class with description and index", 12);
	
		Cerr() << "\nStreaming in XML with an unknown class : \n\n";
		
		polyMap.Clear();
		LoadFromXML(polyMap, s);
		
		Cerr() << "\nMap content after streaming in : " << polyMap.GetCount() << " classes:\n";
		DumpMap(polyMap);
		
		Cerr() << "\nStreaming out XML with an unknown class : \n\n";
		s = StoreAsXML(polyMap, "PolyXMLTest");
		
		Cerr() << "\nRedefining missing class and streaming it the whole stuff again : \n\n";
		Another::Register<Another>("Another", "you can add a description and also an index", 10);
		polyMap.Clear();
		LoadFromXML(polyMap, s);
		DumpMap(polyMap);
	}

	{	
		Cerr() << "\n/////////////////////////////////////////////////////////////////////////////////////////";
		Cerr() << "\nPolyXMLArrayMapOne tests";
		Cerr() << "\n/////////////////////////////////////////////////////////////////////////////////////////";
	
		PolyXMLArrayMapOne<String, Base> polyMapOne;
		
		One<Base> b = new Base;
		b->BaseData = "Sample data in base class";
		polyMapOne.Add("BaseKey", b);
		
		One<Derived> d = new Derived;
		d->BaseData = "Sample data in derived class";
		d->DerivedData = "Another sample data in derived class";
		polyMapOne.Add("DerivedKey", (One<Base>&)d);
		
		One<Another> a = dynamic_cast<Another *>(Base::CreatePtr("Another"));
		a->BaseData = "Sample data in Derived class";
		a->DerivedData = "Another sample data in Derived class";
		a->AnotherData = 12345;
		polyMapOne.Add("AnotherKey", (One<Base>&)a);
		
		One<OneMore> o = new OneMore;
		o->BaseData = "Sample data in OneMore class";
		o->DerivedData = "Another sample data in OneMore class";
		o->AnotherData = 12;
		o->OneMoreData = 3.1418;
		polyMapOne.Add("OneMoreKey", (One<Base>&)o);
		
		Cerr() << "\nMap content before streaming out: " << polyMapOne.GetCount() << " classes:\n";
		DumpMapOne(polyMapOne);
	
		String s = StoreAsXML(polyMapOne, "PolyXMLTest");
		Cerr() << "\nStreamed XML : \n\n" << s ;
		
		polyMapOne.Clear();
		LoadFromXML(polyMapOne, s);
		
		Cerr() << "\nMap content after streaming in : " << polyMapOne.GetCount() << " classes:\n";
		DumpMapOne(polyMapOne);
		
		// Now we de-register all classes and re-register all of them BESIDES one
		// to test in-streaming with an unknown class
		Base::UnregisterAll();
		Base::Register<Base>("Base");
		Derived::Register<Derived>("Derived", "you can add a description");
		OneMore::Register<OneMore>("OneMore", "last class with description and index", 12);
	
		Cerr() << "\nStreaming in XML with an unknown class : \n\n";
		
		polyMapOne.Clear();
		LoadFromXML(polyMapOne, s);
		
		Cerr() << "\nMap content after streaming in : " << polyMapOne.GetCount() << " classes:\n";
		DumpMapOne(polyMapOne);
		
		Cerr() << "\nStreaming out XML with an unknown class : \n\n";
		s = StoreAsXML(polyMapOne, "PolyXMLTest");
		
		Cerr() << "\nRedefining missing class and streaming it the whole stuff again : \n\n";
		Another::Register<Another>("Another", "you can add a description and also an index", 10);
		polyMapOne.Clear();
		LoadFromXML(polyMapOne, s);
		DumpMapOne(polyMapOne);
	}
}

