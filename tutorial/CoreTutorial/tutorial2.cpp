#include "Tutorial.h"

GUI_APP_MAIN
{
	SECTION("Basics");
	DO(Logging);
	DO(StringTutorial);
	DO(StringBufferTutorial);
	DO(WStringTutorial);
	DO(DateTime);
	DO(AsStringTutorial);
	DO(CombineHashTutorial);
	DO(ComparableTutorial);

	SECTION("Streams");
	DO(Stream);
	DO(SpecialStream);
	DO(Serialize);

	SECTION("Array containers");
	DO(Vector1);
	DO(Vector2);
	DO(Transfer);
	DO(ContainerClientTypes);
	DO(ArrayTutorial);
	DO(PolyArray);
	DO(Bidirectional);
	DO(IndexTutorial);
	DO(IndexClient);
	DO(Map);
	DO(OneTutorial);
	DO(AnyTutorial);
	DO(InVectorTutorial);
	DO(SortedMap);
	DO(TupleTutorial);

	SECTION("Ranges and algorithms");
	DO(Range);
	DO(Algo);
	DO(Sorting);
	
	SECTION("Value");
	DO(ValueTutorial);
	DO(NullTutorial);
	DO(Value2Tutorial);
	DO(ValueArrayMap);
	
	SECTION("Function and lambdas");
	DO(FunctionTutorial);
	DO(CapturingContainers);

	SECTION("Multithreading");
	DO(ThreadTutorial);
	DO(MutexTutorial);
	DO(ConditionVariableTutorial);
	DO(CoWorkTutorial);
	DO(AsyncTutorial);
	DO(CoPartitionTutorial);
	DO(CoLoopTutorial);
	DO(CoAlgoTutorial);

	MakeTutorial();
}
