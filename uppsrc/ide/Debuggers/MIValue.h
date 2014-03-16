#ifndef _ide_Debuggers_MIValue_h_
#define _ide_Debuggers_MIValue_h_

#include <Core/Core.h>

using namespace Upp;

// this struct contains values returned by GDB MI interface
typedef enum { MIString, MIArray, MITuple } MIValueType;
class MIValue : public Moveable<MIValue>
{
	private:
		bool expect(String const &where, char exp, int i, String const &s);
	
		int ParsePair(String &name, MIValue &val, String const &s, int i = 0);
		int ParseTuple(String const &s, int i = 0);
		int ParseArray(String const &s, int i = 0);
		int ParseString(String const &s, int i = 0);
		int ParseAngle(String const &s, int i = 0);
		int ParseUnquotedString(String const &s, int i = 0);
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
		bool IsError(void) const;
		bool operator!(void) const { return IsError(); }
		operator bool() { return !IsError(); }
		
		// check for emptyness
		bool IsEmpty(void) const;
		
		MIValue &operator=(MIValue rval_ v);
		MIValue &operator=(String const &s);
		MIValue();
		MIValue(MIValue rval_ v);
		MIValue(String const &s);
		
		void Clear(void);
		
		// simple accessors
		int GetCount(void) const;
		int Find(const char *key) const;
		
		MIValue &Get(int i);
		MIValue const &Get(int i) const;
		
		MIValue &operator[](int i) { return Get(i); }
		MIValue const &operator[](int i) const { return Get(i); }
		
		MIValue &Get(const char *s);
		MIValue const &Get(const char *s) const;
		
		MIValue &operator[](const char *key) { return Get(key); }
		MIValue const &operator[](const char *key) const { return Get(key); }

		String &Get(void);
		String const &Get(void) const;

		// gets key by index for tuple values
		String GetKey(int idx) const;

		operator String&() { return Get(); }
		operator const String &() const { return Get(); }
		String &ToString(void) { return Get(); }
		String const &ToString(void) const { return Get(); }
		
		// tuple string member accessor with default value if not found
		String Get(const char *key, const char *def) const;
		String operator()(const char *key, const char *def) const  { return Get(key, def); }
		
		// setter for string (operator= starts parser...)
		void Set(String const &s);
		
		// some type checking
		bool IsArray(void) const { return type == MIArray; }
		void AssertArray(void) const { ASSERT(type == MIArray); }
		bool IsTuple(void) const { return type == MITuple; }
		void AssertTuple(void) const { ASSERT(type == MITuple); }
		bool IsString(void) const { return type == MIString; }
		void AssertString(void) const { ASSERT(type == MIString); }
		
		// dumps a string with special chars inside
		static String Dump(String const &s);

		// data dump
		String Dump(int level = 0) const;
		
		// finds breakpoint data given file and line
		MIValue &FindBreakpoint(String const &file, int line);
		
		// packs names inside tuples -- to make type recognition easy
		void PackNames(void);
		
		// fix arrays -- i.e. replace a tuple containing ALL unnamed elements
		// with the corresponding array
		void FixArrays(void);
		
		// add some data to a value

		// add an item to a tuple
		MIValue &Add(String const &key, MIValue rval_ v);
		MIValue &Add(String const &key, String const &data);
		MIValue &FindAdd(String const &key, String const &data);
		
		// add an item to an array
		MIValue &Add(MIValue rval_ v);
		MIValue &Add(String const &data);
		
		// remove a tuple key
		MIValue &Remove(String const &key);
};

#endif
