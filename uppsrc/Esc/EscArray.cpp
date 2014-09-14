#include "Esc.h"

NAMESPACE_UPP

#define LTIMING(x) // RTIMING(x)

Vector<EscValue>& EscValue::CloneArray()
{
	LTIMING("CloneArray");
	ASSERT(IsArray());
	if(AtomicRead(array->refcount) != 1) {
		EscArray *c = new EscArray;
		c->array <<= array->array;
		array->Release();
		array = c;
	}
	hash = 0;
	return array->array;
}

const Vector<EscValue>& EscValue::GetArray() const
{
	ASSERT(IsArray());
	return array->array;
}

EscValue EscValue::ArrayGet(int i) const
{
	return GetArray()[i];
}

EscValue EscValue::ArrayGet(int i, int n) const
{
	LTIMING("ArrayGet");
	const Vector<EscValue>& sa = GetArray();
	ASSERT(i >= 0 && i + n <= sa.GetCount());
	EscValue t;
	t.SetEmptyArray();
	t.array->array.Append(sa, i, n);
	return t;
}

void EscValue::SetEmptyArray()
{
	Free();
	type = ESC_ARRAY;
	hash = 0;
	array = new EscArray;
}

bool EscValue::ArraySet(int i, EscValue val)
{
	LTIMING("ArraySet");
	Vector<EscValue>& ta = CloneArray();
	if(i > max_total || i - ta.GetCount() + total > max_total)
		return false;
	CloneArray().At(i) = val;
	return true;
}

void EscValue::ArrayAdd(EscValue val)
{
	LTIMING("ArrayAdd");
	if(IsVoid())
		SetEmptyArray();
	CloneArray().Add(val);
}

bool EscValue::Replace(int i, int n, EscValue a)
{
	LTIMING("Array Replace");
	ASSERT(i >= 0);
	Vector<EscValue>& ta = CloneArray();
	if(i > max_total || i + n > max_total || i + n - ta.GetCount() + total > max_total)
		return false;
	if(i > 0)
		ta.At(i - 1);
	const Vector<EscValue>& sa = a.GetArray();
	int q = sa.GetCount() - n;
	if(q > 0)
		ta.InsertN(i, q);
	else
		ta.Remove(i, -q);
	for(q = 0; q < sa.GetCount(); q++)
		ta[q + i] = sa[q];
	return true;
}

bool EscValue::Append(EscValue a)
{
	if(IsVoid())
		SetEmptyArray();
	return Replace(GetCount(), 0, a);
}

EscValue::operator WString() const
{
	LTIMING("operator String");
	WStringBuffer s;
	if(IsArray()) {
		const Vector<EscValue>& a = GetArray();
		for(int i = 0; i < a.GetCount(); i++)
			if(a[i].IsInt()) {
				int c = a[i].GetInt();
				if(c >= 0 && c < 65536)
					s.Cat(c);
			}
	}
	return s;
}

void EscValue::InitString(const WString& s)
{
	type = ESC_ARRAY;
	array = new EscArray();
	hash = 0;
	Vector<EscValue>& a = array->array;
	a.SetCount(s.GetCount());
	for(int i = 0; i < s.GetCount(); i++)
		a[i] = (int64)s[i];
	total++;
}

int   EscValue::GetTotalCount()
{
	return total;
}

void  EscValue::SetMaxTotalCount(int n)
{
	max_total = n;
}

int   EscValue::GetMaxTotalCount()
{
	return max_total;
}

EscValue::EscValue()
{
	type = ESC_VOID; hash = 0; total++;
}

EscValue::EscValue(double n)
{
	number = n;
	type = ESC_DOUBLE;
	hash = 0;
	total++;
}

EscValue::EscValue(int64 n)
{
	i64 = n;
	type = ESC_INT64;
	hash = 0;
	total++;
}

EscValue::EscValue(int n)
{
	i64 = n;
	type = ESC_INT64;
	hash = 0;
	total++;
}

END_UPP_NAMESPACE
