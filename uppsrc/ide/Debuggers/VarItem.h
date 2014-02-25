#ifndef _ide_Debuggers_VarItem_h_
#define _ide_Debuggers_VarItem_h_

#include <Core/Core.h>
using namespace Upp;

#include "MIValue.h"

// item for a GDB variable
class Gdb_MI2;

class VarItem : Moveable<VarItem>
{
		private:
		
		// connected debugger object
		Gdb_MI2 *debugger;
		
		// error/empty state
		bool empty;
		
		// next simplify step, 0 if completed
		int simplifyStep;
	
		// gdb internal variable name
		String varName;
		
		// fetch variable children
		Vector<VarItem>  GetChildren0(MIValue const &children, String const &prePath);

		// helper for simplifiers
		void ListChildren0(String const &varName, MIValue &res) const;

		// cleanup support -- variables can't be deleted in destructor
		// as usually destructor is called when thread is hard-stopped
		// by main thread with an exception.
		// so we store all var names in a static index and delete them
		// when back to main thread
		static StaticMutex varMutex;
		static Vector<String> deletedVars;
		void PutDeleted(String const &name);

	public:
		typedef enum { SIMPLE, COMPLEX, ARRAY, MAP } VarKind;
		
		// short name
		String shortExpression;
		
		// evaluable expression
		String evaluableExpression;
		
		// type
		String type;
		
		// kind
		int kind;
		
		// value of expression for non-sequence types
		String value;
		
		// children
		int numChildren;
		
		// number of items for array and maps
		int items;
		
		// check if value contains an error
		bool IsEmpty(void) const { return empty; }
		bool operator!(void) const { return IsEmpty(); }
		operator bool() { return !IsEmpty(); }
		
		// check if value is simplified
		bool IsSimplified(void) { return simplifyStep == 0; }

		// clears contents
		void Clear(void);
		
		// evaluate expression
		bool Evaluate(String const &expr);
	
		// deep simplify known types
		bool Simplify(void);

		// constructors
		VarItem(Gdb_MI2 *dbg);
		VarItem(Gdb_MI2 *dbg, String const &expr);
		
		// destructor
		~VarItem();
		
		// copy (pick)
		VarItem(pick_ VarItem &v);
		VarItem const &operator=(pick_ VarItem &v);
		
		// get children
		Vector<VarItem>GetChildren(void);

		// fetch array elements
		Vector<VarItem> GetArray(int start = 0, int count = -1);
		
		// fetch map elements
		VectorMap<VarItem, VarItem> GetMap(int start = 0, int count = -1);
		
		// helpers for simplifiers
		Gdb_MI2 &Debugger() { return *debugger; }
		MIValue ListChildren(void) const;
		MIValue EvaluateExpression(String const &exp) const;
		
		// cleanup support -- variables can't be deleted in destructor
		// as usually destructor is called when thread is hard-stopped
		// by main thread with an exception.
		// so we store all var names in a static index and delete them
		// when back to main thread
		static void CleanVariables(Gdb_MI2 *deb);
		
};

#endif
