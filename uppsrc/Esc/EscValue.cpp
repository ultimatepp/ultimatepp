#include <Esc/Esc.h>


NAMESPACE_UPP

#define LTIMING(x)  // RTIMING(x)

String EscTypeName(int sv_type)
{
	switch(sv_type)
	{
	case ESC_VOID:   return "void";
	case ESC_DOUBLE: return "double";
	case ESC_INT64:  return "int64";
	case ESC_ARRAY:  return "array";
	case ESC_MAP:    return "map";
	case ESC_LAMBDA: return "lambda";
	default:         return NFormat("unknown(%d)", sv_type);
	}
}

int EscValue::total;
int EscValue::max_total = 1000000;

void EscValue::Free()
{
	LTIMING("Free");
	if(type == ESC_ARRAY)
		array->Release();
	if(type == ESC_MAP)
		map->Release();
	if(type == ESC_LAMBDA)
		lambda->Release();
	type = ESC_VOID;
	hash = 0;
}

EscValue::~EscValue()
{
	LTIMING("~EscValue");
	total--;
	Free();
}

void EscValue::Assign(const EscValue& s)
{
	LTIMING("Assign");
	type = s.type;
	hash = s.hash;
	switch(type) {
	case ESC_ARRAY:
		array = s.array;
		array->Retain();
		break;
	case ESC_MAP:
		map = s.map;
		map->Retain();
		break;
	case ESC_LAMBDA:
		lambda = s.lambda;
		lambda->Retain();
		break;
	case ESC_DOUBLE:
		number = s.number;
		break;
	case ESC_INT64:
		i64 = s.i64;
		break;
	}
}

EscValue& EscValue::operator=(const EscValue& s)
{
	LTIMING("Sval=Sval");
	Free();
	Assign(s);
	return *this;
}

EscValue::EscValue(const EscValue& s)
{
	LTIMING("Sval(Sval)");
	Assign(s);
	total++;
}

EscLambda& EscValue::CreateLambda()
{
	Free();
	lambda = new EscLambda;
	type = ESC_LAMBDA;
	hash = 0;
	return *lambda;
}

const EscLambda& EscValue::GetLambda() const
{
	ASSERT(IsLambda());
	return *lambda;
}

int EscValue::GetCount() const
{
	switch(type) {
	case ESC_VOID:
		return 0;
	case ESC_ARRAY:
		return array->array.GetCount();
	case ESC_MAP:
		return map->count;
	}
	return 1;
}

unsigned EscValue::GetHashValue() const
{
	LTIMING("GetHashValue");
	if(hash == 0) {
		switch(type) {
		case ESC_VOID:
			hash = 1;
			break;
		case ESC_DOUBLE:
			hash = UPP::GetHashValue(number) | 0x80000;
			break;
		case ESC_INT64:
			hash = UPP::GetHashValue(i64) | 0x80000;
			break;
		case ESC_ARRAY:
			for(int i = 0; i < array->array.GetCount(); i++)
				hash = hash ^ array->array[i].GetHashValue();
			hash |= 0x40000;
			break;
		case ESC_MAP:
			for(int i = 0; i < map->map.GetCount(); i++)
				if(!map->map[i].IsVoid())
					hash ^= map->map.GetKey(i).GetHashValue() ^ map->map[i].GetHashValue();
			hash |= 0x8000000;
			break;
		case ESC_LAMBDA:
			hash = UPP::GetHashValue(lambda->code) | 0x4000000;
			break;
		}
	}
	return hash;
}

template <class T>
bool EqVector(const T& a, const T& b)
{
	if(a.GetCount() != b.GetCount())
		return false;
	for(int i = 0; i < a.GetCount(); i++)
		if(a[i] != b[i])
			return false;
	return true;
}

bool EscValue::operator==(const EscValue& a) const
{
	LTIMING("operator==");
	int i;
	switch(type) {
	case ESC_VOID:
		return a.type == ESC_VOID;
	case ESC_INT64:
		if(a.type == ESC_INT64)
			return i64 == a.i64;
	case ESC_DOUBLE:
		return a.IsNumber() && GetNumber() == a.GetNumber();
	case ESC_ARRAY:
		if(a.type != ESC_ARRAY) return false;
		if(array->array.GetCount() != a.array->array.GetCount())
			return false;
		return EqVector(array->array, a.array->array);
	case ESC_MAP: {
		if(a.type != ESC_MAP) return false;
		const VectorMap<EscValue, EscValue>& x = GetMap();
		const VectorMap<EscValue, EscValue>& y = a.GetMap();
		for(i = 0; i < x.GetCount(); i++)
			if(!x.IsUnlinked(i)) {
				int q = y.Find(x.GetKey(i));
				if(q < 0)
					return false;
				if(x[i] != y[q])
					return false;
			}
		for(i = 0; i < y.GetCount(); i++)
			if(!y.IsUnlinked(i))
				if(x.Find(y.GetKey(i)) < 0)
					return false;
		return true;
	}
	case ESC_LAMBDA:
		return lambda->code == a.lambda->code &&
		       EqVector(lambda->arg, a.lambda->arg) && EqVector(lambda->inout, a.lambda->inout);
	}
	return false;
}

String EscValue::ToString(int maxlen, int indent_step, bool hex, int indent) const
{
	String ind(' ', indent);
	StringBuffer r;
	String s;
	int i;
	switch(type) {
	case ESC_DOUBLE:
		{
			s = ind;
			if((int64)number == number)
				s << FormatInt64((int64)number);
			else
				s << Format("%.8g", number);
			if(hex && number >= INT64_MIN && number <= INT64_MAX && (int64)number == number)
				s << " 0x" << Format64Hex((int64)number);
			if(hex && number >= 0 && number < 65536 && (int)number == number)
				s << ' ' << AsCString(ToUtf8(WString((int)number, 1)));
			return s;
		}
	case ESC_INT64:
		{
			s = ind;
			s << FormatInt64(i64);
			if(hex)
				s << " 0x" << Format64Hex(i64);
			if(hex && i64 >= 0 && i64 < 65536)
				s << ' ' << AsCString(ToUtf8(WString((int)i64, 1)));
			return s;
		}
	case ESC_ARRAY:
		{
			const Vector<EscValue>& a = GetArray();
			int i;
			int c = min(100, a.GetCount());
			for(i = 0; i < a.GetCount(); i++) {
				if(!a[i].IsInt())
					break;
				int c = a[i].GetInt();
				if(c >= 32 && c < 256)
					s.Cat(c);
				else
					break;
			}
			if(i < a.GetCount()) {
				r << ind << "[ ";
				for(i = 0; i < c; i++) {
					if(i)
						r << ", ";
					r << array->array[i].ToString(maxlen, indent_step, hex, 0);
				}
				r << " ]";
				if(r.GetLength() >= maxlen) {
					r.Clear();
					r << ind << "[\n";
					for(i = 0; i < c; i++) {
						if(i)
							r << ",\n";
						r << array->array[i].ToString(maxlen, indent_step, hex, indent + indent_step);
					}
					r << '\n' << ind << "]";
				}
				s = r;
			}
			else {
				r << ind << '\"' << s << '\"';
				s = r;
			}
			if(a.GetCount() > 100)
				s << ind << "\n...more than 100 elements";
			return s;
		}
	case ESC_LAMBDA:
		r << ind << "@(";
		for(i = 0; i < lambda->arg.GetCount(); i++) {
			if(i)
				r << ", ";
			if(lambda->inout[i])
				r << "&";
			r << lambda->arg[i];
		}
		r << ")\n" << lambda->code;
		return r;
	case ESC_MAP:
		r << ind << "{ ";
		int c = min(map->map.GetCount(), 100);
		bool q = false;
		for(i = 0; i < c; i++) {
			if(q)
				r << ", ";
			if(!map->map.IsUnlinked(i)) {
				r << map->map.GetKey(i).ToString(maxlen, indent_step, hex, 0)
				  << ":" << map->map[i].ToString(maxlen, indent_step, hex, 0);
				q = true;
			}
		}
		r << " }";
		if(r.GetLength() >= maxlen) {
			r.Clear();
			r << ind << "{\n";
			q = false;
			for(i = 0; i < c; i++) {
				if(q)
					r << ",\n";
				if(!map->map.IsUnlinked(i)) {
					r << map->map.GetKey(i).ToString(maxlen, indent_step, hex, indent + indent_step)
					  << ":\n" << map->map[i].ToString(maxlen, indent_step, hex, indent + indent_step);
					q = true;
				}
			}
			r << '\n' << ind << "}";
		}
		s = r;
		if(map->map.GetCount() > 100)
			s << ind << "\n...more than 100 elements";
		return s;
	}
	return "void";
}

double EscValue::GetNumber() const
{
	if(type == ESC_INT64)
		return (double)i64;
	if(type == ESC_DOUBLE)
		return number;
	return 0;
}

int64 EscValue::GetInt64() const
{
	if(type == ESC_INT64)
		return i64;
	if(type == ESC_DOUBLE && number >= (double)INT64_MIN && number <= (double)INT64_MAX)
		return (int64)number;
	return 0;
}

bool EscValue::IsInt() const
{
	if(IsInt64()) {
		int64 n = GetInt64();
		return n >= INT_MIN && n <= INT_MAX;
	}
	if(IsNumber()) {
		double n = GetNumber();
		return n >= INT_MIN && n <= INT_MAX;
	}
	return false;
}

int EscValue::GetInt() const
{
	return IsInt() ? (int)GetInt64() : 0;
}

bool IsTrue(const EscValue& a)
{
	if(a.IsNumber())
		return a.GetNumber();
	return a.GetCount();
}

END_UPP_NAMESPACE
