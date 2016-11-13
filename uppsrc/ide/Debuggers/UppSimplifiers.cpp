#include "TypeSimplify.h"

#define EVALDEEP
#define EVALDEEP_VECTOR		5
#define EVALDEEP_ARRAY		5
#define EVALDEEP_VECTORMAP	5
#define EVALDEEP_ARRAYMAP	5
#define EVALDEEP_INDEX		5

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SIMPLIFIERS MUST BE CODED AS STATE MACHINES -- THEY'LL BE CALLED MANY TIMES, WITH A 'step' PARAMETER
// STEP = 0 MEANS BASE SIMPLIFY AND CHECK IF MORE STEPS ARE NEEDED
//          RETURN NEXT STEP, OR 0 IF NONE
// STEP = N MEANS A SIMPLIFY STEP
//          RETURN NEXT STEP, OR 0 IF NONE
// THEY MUST CHANGE 'value' MEMBER OF PASSED VarItem object ON EACH STEP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int UppStringSimplify(VarItem &varItem, int step)
{
	enum { SMALL = 0, MEDIUM = 31 }; // SMALL has to be 0 because of GetSpecial and because is it ending zero
	enum { KIND = 14, SLEN = 15, LLEN = 2, SPECIAL = 13 };
	union
	{
		char chr[16];
		char  *ptr;
		dword *wptr;
		qword *qptr;
		word   v[8];
		dword  w[4];
		qword  q[2];
	} u;
	
	// see Upp::String code for how it works....
	MIValue val = varItem.EvaluateExpression("(" + varItem.evaluableExpression + ")." + "chr");
	if(!val.IsString())
		return 0;
	String chrs = val.ToString();
	memcpy(u.chr, ~chrs, 16);

	bool isSmall = (u.chr[14] == 0);
	String s;
	if(isSmall)
	{
		byte len = u.chr[SLEN];
		s = chrs.Left(len);
	}
	else
	{
		dword len = u.w[LLEN];
		MIValue val = varItem.EvaluateExpression("(" + varItem.evaluableExpression + ").ptr[0]@" + FormatInt(len));
		if(!val.IsString())
			return 0;
		s = val.ToString();
	}
	varItem.value = "\"" + s + "\"";
	varItem.kind = VarItem::SIMPLE;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int UppVectorSimplify(VarItem &varItem, int step)
{
	// setup item type
	varItem.kind = VarItem::ARRAY;
	
	// if we're just doing first scan phase, signal that we need further evaluation later
#ifdef EVALDEEP
	if(!step)
		// next step is 1
		return 1;
#else
	varItem.value = placeHolder;
	return 0;
#endif
	
	// just getting items count...
	if(step == 1)
	{
		// initialize default value
		varItem.value = "<can't evaluate>";
		
		// get items count
		MIValue val = varItem.EvaluateExpression(varItem.evaluableExpression + ".items");
		if(val.IsError() || !val.IsString())
			return 0;
		varItem.items = atoi(val.ToString());
		
		// update value
		varItem.value = Format("Upp::Vector with %d elements", varItem.items, "");
		
		// if no elements, just quit
		if(!varItem.items)
			return 0;
		return 2;
	}
	
	int count = min(EVALDEEP_VECTOR, varItem.items);
	
	// start from item 0
	step -= 2;
	
	// fetch elements, check on first if they're SIMPLE, so displayable
	VarItem vItem(&varItem.Debugger(), varItem.evaluableExpression + Format(".vector[%d]", step));
	if(!vItem)
	{
		varItem.value << " <can't evaluate contents>";
		return 0;
	}
	if(vItem.kind != VarItem::SIMPLE)
	{
		varItem.value << " = [...]";
		return 0;
	}
	vItem.Simplify();

	if(!step)
		varItem.value << " = [ ]";
	
	const char *sep = step ? " , " : "";
	varItem.value = varItem.value.Left(varItem.value.GetCount() - 2) + sep + vItem.value + " ]";
	if(++step >= count)
		return 0;
	else
		return step + 2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int UppVectorMapSimplify(VarItem &varItem, int step)
{
	// setup item type
	varItem.kind = VarItem::MAP;
	
	// if we're just doing first scan phase, signal that we need further evaluation later
	// if we're just doing first scan phase, signal that we need further evaluation later
#ifdef EVALDEEP
	if(!step)
		// next step is 1
		return 1;
#else
	varItem.value = placeHolder;
	return 0;
#endif
	
	// just getting items count...
	if(step == 1)
	{
		// initialize default value
		varItem.value = "<can't evaluate>";
		
		// get items count
		MIValue val = varItem.EvaluateExpression(varItem.evaluableExpression + ".key.key.items");
		if(val.IsError() || !val.IsString())
			return 0;
		varItem.items = atoi(val.ToString());
		
		// update value
		varItem.value = Format("Upp::VectorMap with %d elements", varItem.items, "");
		
		// if no elements, just quit
		if(!varItem.items)
			return 0;
		return 2;
	}
	
	int count = min(EVALDEEP_VECTORMAP, varItem.items);
	
	// start from item 0
	step -= 2;
	
	// fetch elements, check on first if they're SIMPLE, so displayable
	VarItem kItem(&varItem.Debugger(), varItem.evaluableExpression + Format(".key.key.vector[%d]", step));
	if(!kItem)
	{
		varItem.value << " <can't evaluate contents>";
		return 0;
	}
	// for complex types, just return placeholder
	if(kItem.kind != VarItem::SIMPLE)
	{
		varItem.value << " = {...}";
		return 0;
	}
	kItem.Simplify();

	VarItem vItem(&varItem.Debugger(), varItem.evaluableExpression + Format(".value.vector[%d]", step));
	if(!vItem)
	{
		varItem.value << " <can't evaluate contents>";
		return 0;
	}
	// for complex types, just return placeholder
	if(vItem.kind != VarItem::SIMPLE)
	{
		varItem.value << " = {...}";
		return 0;
	}
	vItem.Simplify();
	
	if(!step)
		varItem.value << " = { }";
	
	const char *sep = step ? " , " : "";
	varItem.value = varItem.value.Left(varItem.value.GetCount() - 2) + sep + "(" + kItem.value + " , " + vItem.value + ") }";
	if(++step >= count)
		return 0;
	else
		return step + 2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int UppArraySimplify(VarItem &varItem, int step)
{
	// setup item type
	varItem.kind = VarItem::ARRAY;
	
	// if we're just doing first scan phase, signal that we need further evaluation later
#ifdef EVALDEEP
	if(!step)
		// next step is 1
		return 1;
#else
	varItem.value = placeHolder;
	return 0;
#endif
	
	// just getting items count...
	if(step == 1)
	{
		// initialize default value
		varItem.value = "<can't evaluate>";
		
		// get items count
		MIValue val = varItem.EvaluateExpression(varItem.evaluableExpression + ".vector.items");

		if(val.IsError() || !val.IsString())
			return 0;
		varItem.items = atoi(val.ToString());
		
		// update value
		varItem.value = Format("Upp::Array with %d elements", varItem.items, "");
		
		// if no elements, just quit
		if(!varItem.items)
			return 0;
		return 2;
	}
	
	int count = min(EVALDEEP_ARRAY, varItem.items);
	
	// start from item 0
	step -= 2;
	
	// fetch elements, check on first if they're SIMPLE, so displayable
	VarItem vItem(&varItem.Debugger(), varItem.evaluableExpression + Format(".vector.vector[%d][0]", step));
	if(!vItem)
	{
		varItem.value << " <can't evaluate contents>";
		return 0;
	}
	if(vItem.kind != VarItem::SIMPLE)
	{
		varItem.value << " = [...]";
		return 0;
	}

	if(!step)
		varItem.value << " = [ ]";

	vItem.Simplify();
	const char *sep = step ? " , " : "";
	varItem.value = varItem.value.Left(varItem.value.GetCount() - 2) + sep + vItem.value + " ]";

	if(++step >= count)
		return 0;
	else
		return step + 2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int UppArrayMapSimplify(VarItem &varItem, int step)
{
	// setup item type
	varItem.kind = VarItem::MAP;
	
	// if we're just doing first scan phase, signal that we need further evaluation later
	// if we're just doing first scan phase, signal that we need further evaluation later
#ifdef EVALDEEP
	if(!step)
		// next step is 1
		return 1;
#else
	varItem.value = placeHolder;
	return 0;
#endif
	
	// just getting items count...
	if(step == 1)
	{
		// initialize default value
		varItem.value = "<can't evaluate>";
		
		// get items count
		MIValue val = varItem.EvaluateExpression(varItem.evaluableExpression + ".key.key.items");
		if(val.IsError() || !val.IsString())
			return 0;
		varItem.items = atoi(val.ToString());
		
		// update value
		varItem.value = Format("Upp::ArrayMap with %d elements", varItem.items, "");
		
		// if no elements, just quit
		if(!varItem.items)
			return 0;
		return 2;
	}
	
	int count = min(EVALDEEP_VECTORMAP, varItem.items);
	
	// start from item 0
	step -= 2;
	
	// fetch elements, check on first if they're SIMPLE, so displayable
	VarItem kItem(&varItem.Debugger(), varItem.evaluableExpression + Format(".key.key.vector[%d]", step));
	if(!kItem)
	{
		varItem.value << " <can't evaluate contents>";
		return 0;
	}
	// for complex types, just return placeholder
	if(kItem.kind != VarItem::SIMPLE)
	{
		varItem.value << " = {...}";
		return 0;
	}
	kItem.Simplify();

	VarItem vItem(&varItem.Debugger(), varItem.evaluableExpression + Format(".value.vector.vector[%d][0]", step));
	if(!vItem)
	{
		varItem.value << " <can't evaluate contents>";
		return 0;
	}
	// for complex types, just return placeholder
	if(vItem.kind != VarItem::SIMPLE)
	{
		varItem.value << " = {...}";
		return 0;
	}
	vItem.Simplify();
	
	if(!step)
		varItem.value << " = { }";
	
	const char *sep = step ? " , " : "";
	varItem.value = varItem.value.Left(varItem.value.GetCount() - 2) + sep + "(" + kItem.value + " , " + vItem.value + ") }";
	if(++step >= count)
		return 0;
	else
		return step + 2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int UppIndexSimplify(VarItem &varItem, int step)
{
	// setup item type
	varItem.kind = VarItem::ARRAY;
	
	// if we're just doing first scan phase, signal that we need further evaluation later
#ifdef EVALDEEP
	if(!step)
		// next step is 1
		return 1;
#else
	varItem.value = placeHolder;
	return 0;
#endif
	
	// just getting items count...
	if(step == 1)
	{
		// initialize default value
		varItem.value = "<can't evaluate>";
		
		// get items count
		MIValue val = varItem.EvaluateExpression(varItem.evaluableExpression + ".key.items");
		if(val.IsError() || !val.IsString())
			return 0;
		varItem.items = atoi(val.ToString());
		
		// update value
		varItem.value = Format("Upp::Index with %d elements", varItem.items, "");
		
		// if no elements, just quit
		if(!varItem.items)
			return 0;
		return 2;
	}
	
	int count = min(EVALDEEP_VECTOR, varItem.items);
	
	// start from item 0
	step -= 2;
	
	// fetch elements, check on first if they're SIMPLE, so displayable
	VarItem vItem(&varItem.Debugger(), varItem.evaluableExpression + Format(".key.vector[%d]", step));
	if(!vItem)
	{
		varItem.value << " <can't evaluate contents>";
		return 0;
	}
	if(vItem.kind != VarItem::SIMPLE)
	{
		varItem.value << " = [...]";
		return 0;
	}
	vItem.Simplify();

	if(!step)
		varItem.value << " = [ ]";
	
	const char *sep = step ? " , " : "";
	varItem.value = varItem.value.Left(varItem.value.GetCount() - 2) + sep + vItem.value + " ]";
	if(++step >= count)
		return 0;
	else
		return step + 2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int UppOneSimplify(VarItem &varItem, int step)
{
	// setup item type
	varItem.kind = VarItem::COMPLEX;
	
	// if we're just doing first scan phase, signal that we need further evaluation later
#ifdef EVALDEEP
	if(!step)
		// next step is 1
		return 1;
#else
	varItem.value = placeHolder;
	return 0;
#endif

	// de-reference and forward simplify
	MIValue val = varItem.EvaluateExpression(varItem.evaluableExpression + "." + "ptr");
	if(val.IsError() || !val.IsString())
	{
		varItem.value = "Upp::One<> = <can't evaluate contents>";
		return 0;
	}
	String ptr = val.ToString();
	if(ptr == "0x0")
	{
		varItem.value = "Upp::One<> = <EMPTY>";
		return 0;
	}
	
	// replace variable with de-referenced one
	VarItem vItem(&varItem.Debugger(), "*" + varItem.evaluableExpression + "." + "ptr");
	varItem = vItem;
	return varItem.GetSimplifyStep();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int UppTimeSimplify(VarItem &varItem, int step)
{
	MIValue val = varItem.EvaluateExpression(varItem.evaluableExpression);
	val.PackNames();
	int day = atoi(val[0]["day"].ToString());
	int month = atoi(val[0]["month"].ToString());
	int year = atoi(val[0]["year"].ToString());
	int hour = atoi(val["hour"].ToString());
	int minute = atoi(val["minute"].ToString());
	int second = atoi(val["second"].ToString());
	varItem.value = Format("Upp::Time = %02d/%02d/%04d - %02d:%02d:%02d", day, month, year, hour, minute, second);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int UppDateSimplify(VarItem &varItem, int step)
{
	MIValue val = varItem.EvaluateExpression(varItem.evaluableExpression);
	val.PackNames();
	int day = atoi(val["day"].ToString());
	int month = atoi(val["month"].ToString());
	int year = atoi(val["year"].ToString());
	varItem.value = Format("Upp::Date = %02d/%02d/%04d", day, month, year);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int UppValueSimplify(VarItem &varItem, int step)
{
	enum { SMALL = 0, MEDIUM = 31 }; // SMALL has to be 0 because of GetSpecial and because is it ending zero
	enum { KIND = 14, SLEN = 15, LLEN = 2, SPECIAL = 13 };
	enum { STRING = 0, REF = 255, VOIDV = 3 };

	// get the embedded 'data' string 'chr' member
	// it contains info about value type
	union
	{
		char chr[16];
		char  *ptr;
		dword *wptr;
		qword *qptr;
		word   v[8];
		dword  w[4];
		qword  q[2];
		
		int iData;
		int64 i64Data;
		double dData;
		bool bData;
		struct
		{
			byte   day;
			byte   month;
			int16  year;
			byte   hour;
			byte   minute;
			byte   second;
		};
	} u;
	
	// see Upp::String code for how it works....
	MIValue val = varItem.EvaluateExpression("(" + varItem.evaluableExpression + ").data.chr");
	if(!val.IsString())
		return 0;
	String chrs = val.ToString();
	memcpy(u.chr, ~chrs, 16);

	// get value type, among the fixed ones
	// we could try later to decode registered types....
	dword type;
	bool isSpecial = !u.v[7] && u.v[6];
	if(!isSpecial)
		type = STRING_V;
	else
	{
		byte st = u.chr[SPECIAL];
		if(st == REF)
		{
			// ptr()->GetType()
			// by now, just mark as ref...
			type = REF;
		}
		else if(st == VOIDV)
			type = VOID_V;
		else
			type = st;
	}
	
	// by now, treat all types beyond VALUEMAP_V as unknown
	if(type > VALUEMAP_V)
		type = UNKNOWN_V;
	
	// now, based on type, we can decode it
	varItem.kind = VarItem::SIMPLE;
	switch(type)
	{
		case VOID_V:
		{
			varItem.value = "<VOID>";
			return 0;
		}
			
		case INT_V:
		{
			varItem.value = FormatInt(u.iData);
			return 0;
		}
			
		case DOUBLE_V:
		{
			varItem.value = FormatDouble(u.dData);
			return 0;
		}
			
		case STRING_V:
		{
			// we simply replace the VarItem with the string
			VarItem vItem(&varItem.Debugger(), "(" + varItem.evaluableExpression + ").data");
			vItem.evaluableExpression = varItem.evaluableExpression;
			vItem.shortExpression = varItem.shortExpression;
			varItem = vItem;
			return 0;
		}
			
		case DATE_V:
		{
			varItem.value = Format("Upp::Date = %02d/%02d/%04d", u.day, u.month, u.year);
			return 0;
		}
			
		case TIME_V:
		{
			varItem.value = Format("Upp::Time = %02d/%02d/%04d - %02d:%02d:%02d", u.day, u.month, u.year, u.hour, u.minute, u.second);
			return 0;
		}
			break;
			
		case ERROR_V:
		{
			varItem.value = "<ERROR_V>";
			return 0;
		}
			
		case VALUE_V:
		{
			varItem.value = "<VALUE_V>";
			return 0;
		}
			
		case WSTRING_V:
		{
			varItem.value = "<WSTRING_V>";
			return 0;
		}
			
		case VALUEARRAY_V:
		{
			varItem.value = "<VALUEARRAY_V>";
			return 0;
		}
			
		case INT64_V:
		{
			varItem.value = FormatInt64(u.i64Data);
			return 0;
		}
			
		case BOOL_V:
		{
			varItem.value = (u.bData ? "TRUE" : "FALSE");
			return 0;
		}
			
		case VALUEMAP_V:
		{
			varItem.value = "<VALUEMAP_V>";
			return 0;
		}
			
		case UNKNOWN_V:
		default:
		{
			varItem.value = "<UNKNOWN_V>";
			return 0;
		}
	}

}

INITIALIZER(UppSimplifiers) {}

// Register the simplifiers
REGISTERSIMPLIFIER("Upp::String"				, UppStringSimplify);
REGISTERSIMPLIFIER("Upp::Vector<"				, UppVectorSimplify);
REGISTERSIMPLIFIER("Upp::VectorMap<"			, UppVectorMapSimplify);
REGISTERSIMPLIFIER("Upp::Array<"				, UppArraySimplify);
REGISTERSIMPLIFIER("Upp::ArrayMap<"				, UppArrayMapSimplify);
REGISTERSIMPLIFIER("Upp::Index<"				, UppIndexSimplify);
REGISTERSIMPLIFIER("Upp::One<"					, UppOneSimplify);
REGISTERSIMPLIFIER("Upp::Time"					, UppTimeSimplify);
REGISTERSIMPLIFIER("Upp::Date"					, UppDateSimplify);
REGISTERSIMPLIFIER("Upp::Value"					, UppValueSimplify);
