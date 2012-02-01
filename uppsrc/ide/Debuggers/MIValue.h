#ifndef _ide_Debuggers_MIValue_h_
#define _ide_Debuggers_MIValue_h_

#include <Core/Core.h>

using namespace Upp;

// this struct contains values returned by GDB MI interface
typedef enum { MIString, MIArray, MITuple } MIValueType;
class MIValue : public Moveable<MIValue>
{
	private:
		int ParsePair(String &name, MIValue &val, String const &s, int i = 0);
		int ParseTuple(String const &s, int i = 0);
		int ParseArray(String const &s, int i = 0);
		int ParseString(String const &s, int i = 0);
		int ParseValue(String const &s, int i = 0);
		int Parse(String const &s, int i = 0);

		MIValueType type;
		String string;
		Vector<MIValue> array;
		VectorMap<String, MIValue> tuple;
		
	public:
		// sets value to an error condition
		MIValue &SetError(String const &msg);

		// check if value contains an error
		bool IsError(void);
		
		// check for emptyness
		bool IsEmpty(void);
		
		MIValue &operator=(pick_ MIValue &v);
		MIValue &operator=(String const &s);
		MIValue();
		MIValue(MIValue pick_ &v);
		MIValue(String const &s);
		
		void Clear(void);
		
		// simple accessors
		int GetCount(void) const;
		int Find(const char *key) const;
		MIValue &Get(int i);
		MIValue &operator[](int i) { return Get(i); }
		MIValue &Get(const char *s);
		MIValue &operator[](const char *key) { return Get(key); }
		String &Get(void);
		String const &Get(void) const;
		operator String&() { return Get(); }
		operator const String &() const { return Get(); }
		String &ToString(void) { return Get(); }
		String const &ToString(void) const { return Get(); }
		
		// tuple string member accessor with default value if not found
		String Get(const char *key, const char *def) const;
		String operator()(const char *key, const char *def) const  { return Get(key, def); }
		
		// some type checking
		bool IsArray(void) { return type == MIArray; }
		void AssertArray(void) { ASSERT(type == MIArray); }
		bool IsTuple(void) { return type == MITuple; }
		void AssertTuple(void) { ASSERT(type == MITuple); }
		bool IsString(void) { return type == MIString; }
		void AssertString(void) { ASSERT(type == MIString); }
		
		// data dump
		String Dump(int level = 0);
		
		// finds breakpoint data given file and line
		MIValue &FindBreakpoint(String const &file, int line);
};

#endif
