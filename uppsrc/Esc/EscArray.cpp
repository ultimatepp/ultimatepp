#include "Esc.h"

namespace Upp {

#define LTIMING(x) // RTIMING(x)

Vector<EscValue>& EscValue::CloneArray()
{
	LTIMING("CloneArray");
	ASSERT(IsArray());
	if(array->refcount != 1) {
		EscArray *c = new EscArray;
		c->array <<= array->array;
		array->Release();
		array = c;
	}
	array->cached_hash = 0;
	return array->array;
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
	array = new EscArray;
}

void EscValue::ArraySet(int i, EscValue val)
{
	LTIMING("ArraySet");
	CloneArray().At(i) = val;
}

void EscValue::ArrayAdd(EscValue val)
{
	LTIMING("ArrayAdd");
	if(IsVoid())
		SetEmptyArray();
	CloneArray().Add(val);
}

void EscValue::Replace(int i, int n, EscValue a)
{
	LTIMING("Array Replace");
	ASSERT(i >= 0);
	Vector<EscValue>& ta = CloneArray();
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
}

void EscValue::Append(EscValue a)
{
	if(IsVoid())
		SetEmptyArray();
	Replace(GetCount(), 0, a);
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
	return WString(s);
}

void EscValue::InitString(const WString& s)
{
	type = ESC_ARRAY;
	array = new EscArray;
	Vector<EscValue>& a = array->array;
	a.SetCount(s.GetCount());
	for(int i = 0; i < s.GetCount(); i++)
		a[i] = (int64)s[i];
}

}
