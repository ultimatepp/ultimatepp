#include "Skylark.h"

#define LTIMING(x) // RTIMING(x)
#define LLOG(x)    // DLOG(x)

namespace Upp {

force_inline
bool Compiler::IsTrue(const Value& v)
{
	return !(IsNull(v) || IsNumber(v) && (int)v == 0 || IsValueArray(v) && v.GetCount() == 0);
}

String TypeMismatch(const char *s)
{
	return ErrorValue("<* type mismatch for '" + String(s) + "' *>");
}

Value Compiler::ExeVar::Eval(ExeContext& x) const
{
	LLOG("Retrieving var no " << var_index << " = " << x.stack[var_index]);
	return x.stack[var_index];
}

Value Compiler::ExeConst::Eval(ExeContext& x) const
{
	return value;
}

Value Compiler::ExeArray::Eval(ExeContext& x) const
{
	ValueArray va;
	for(int i = 0; i < item.GetCount(); i++)
		va.Add(item[i]->Eval(x));
	return va;
}

Value Compiler::ExeMap::Eval(ExeContext& x) const
{
	ValueMap m;
	for(int i = 0; i < key.GetCount(); i++)
		m.Add(key[i]->Eval(x), value[i]->Eval(x));
	return m;
}

Value Compiler::ExeNot::Eval(ExeContext& x) const
{
	return !IsTrue(a->Eval(x));
}

Value Compiler::ExeNeg::Eval(ExeContext& x) const
{
	Value v = a->Eval(x);
	if(IsNumber(v))
		return -(double)v;
	return TypeMismatch("unary -");
}

Value Compiler::ExeMul::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 * (double)v2;
	return TypeMismatch("*");
}

Value Compiler::ExeDiv::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 / (double)v2;
	return TypeMismatch("/");
}

Value Compiler::ExeMod::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	if(IsNumber(v1) && IsNumber(v2)) {
		int m = v2;
		if(m == 0)
			return Null;
		return (int)v1 % m;
	}
	return TypeMismatch("%");
}

Value Compiler::ExeAdd::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	if(IsString(v1) && IsString(v2))
		return (String)v1 + (String)v2;
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 + (double)v2;
	if(v1.Is<RawHtmlText>() || v2.Is<RawHtmlText>()) {
		RawHtmlText h;
		h.text = AsString(v1) + AsString(v2);
		return RawToValue(h);
	}
	return AsString(v1) + AsString(v2);
}

Value Compiler::ExeSub::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 - (double)v2;
	return TypeMismatch("-");
}

Value Compiler::ExeSll::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	if(IsNumber(v1) && IsNumber(v2))
		return (int)v1 << min(32, (int)v2);
	return TypeMismatch("<<");
}

Value Compiler::ExeSrl::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	if(IsNumber(v1) && IsNumber(v2))
		return int((unsigned)(int)v1 >> min(32, (int)v2));
	return TypeMismatch(">>>");
}

Value Compiler::ExeSra::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	if(IsNumber(v1) && IsNumber(v2))
		return (int)v1 >> min(32, (int)v2);
	return TypeMismatch(">>");
}

Value Compiler::ExeLt::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	if(IsString(v1) && IsString(v2))
		return (String)v1 < (String)v2;
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 < (double)v2;
	return AsString(v1) < AsString(v2);
}

Value Compiler::ExeLte::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	if(IsString(v1) && IsString(v2))
		return (String)v1 <= (String)v2;
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 <= (double)v2;
	return AsString(v1) <= AsString(v2);
}

Value Compiler::ExeEq::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	if(IsString(v1) && IsString(v2))
		return (String)v1 == (String)v2;
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 == (double)v2;
	return AsString(v1) == AsString(v2);
}

Value Compiler::ExeNeq::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	if(IsString(v1) && IsString(v2))
		return (String)v1 != (String)v2;
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 != (double)v2;
	return AsString(v1) != AsString(v2);
}

Value Compiler::ExeAnd::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	if(IsNumber(v1) && IsNumber(v2))
		return (int)v1 & (int)v2;
	return TypeMismatch("&");
}

Value Compiler::ExeXor::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	if(IsNumber(v1) && IsNumber(v2))
		return (int)v1 ^ (int)v2;
	return TypeMismatch("^");
}

Value Compiler::ExeOr::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	if(IsNumber(v1) && IsNumber(v2))
		return (int)v1 | (int)v2;
	return TypeMismatch("|");
}

Value Compiler::ExeAnl::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	return IsTrue(v1) && IsTrue(v2);
}

Value Compiler::ExeOrl::Eval(ExeContext& x) const
{
	Value v1 = a->Eval(x);
	Value v2 = b->Eval(x);
	return IsTrue(v1) || IsTrue(v2);
}

Value Compiler::ExeCond::Eval(ExeContext& x) const
{
	if(IsTrue(cond->Eval(x)))
		return ontrue->Eval(x);
	else
	if(onfalse)
		return onfalse->Eval(x);
	return Value();
}

Value Compiler::ExeField::Eval(ExeContext& x) const
{
	return value->Eval(x)[id];	
}

Value Compiler::ExeBracket::Eval(ExeContext& x) const
{
	Value m = value->Eval(x);
	Value q = index->Eval(x);
	if(IsNumber(q) && m.Is<ValueArray>()) {
		ValueArray va = m;
		int i = q;
		if(i >= 0 && i < va.GetCount())
			return va[i];
	}
	if(IsValueMap(m)) {
		ValueMap map = m;
		return map[q];
	}
	return Value();	
}

Value Compiler::ExeVarField::Eval(ExeContext& x) const
{
	return x.stack[var_index][id];
}

Value Compiler::ExeFn::Eval(ExeContext& x) const
{
	Vector<Value> v;
	v.SetCount(arg.GetCount());
	for(int i = 0; i < arg.GetCount(); i++)
		v[i] = arg[i]->Eval(x);
	return (*fn)(v, x.renderer);
}

Value Compiler::ExeLink::Eval(ExeContext& x) const
{
	LTIMING("ExeLink");
	Vector<Value> v;
	v.SetCount(arg.GetCount());
	for(int i = 0; i < arg.GetCount(); i++) {
		LTIMING("arg eval");
		v[i] = arg[i]->Eval(x);
	}
	StringBuffer r;
	r << "\"";
	MakeLink(r, *part, v);
	r << "\"";
	return Raw(r);
}

Value Compiler::ExeLinkVarField1::Eval(ExeContext& x) const
{
	LTIMING("ExeLinkVarField");
	Vector<Value> v;
	v.Add(x.stack[var_index][id]);
	StringBuffer r;
	r << "\"";
	MakeLink(r, *part, v);
	r << "\"";
	return Raw(r);
}

Value Compiler::ExeFirst::Eval(ExeContext& x) const
{
	const LoopInfo& f = ValueTo<LoopInfo>(x.stack[var_index]);
	return f.first;
}

Value Compiler::ExeLast::Eval(ExeContext& x) const
{
	const LoopInfo& f = ValueTo<LoopInfo>(x.stack[var_index]);
	return f.last;
}

Value Compiler::ExeIndex::Eval(ExeContext& x) const
{
	const LoopInfo& f = ValueTo<LoopInfo>(x.stack[var_index]);
	return f.index;
}

Value Compiler::ExeKey::Eval(ExeContext& x) const
{
	const LoopInfo& f = ValueTo<LoopInfo>(x.stack[var_index]);
	return f.key;
}

void EscapeHtml(StringBuffer& out, const String& txt)
{
	const char *s = txt;
	const char *e = txt.End();
	while(s != e) {
		if(*s == 31)
			out.Cat("&nbsp;");
		else
		if(*s == '<')
			out.Cat("&lt;");
		else
		if(*s == '>')
			out.Cat("&gt;");
		else
		if(*s == '&')
			out.Cat("&amp;");
		else
		if(*s == '\"')
			out.Cat("&quot;");
		else
		if((byte)*s < ' ')
			out.Cat(NFormat("&#%d;", (byte)*s));
		else
			out.Cat(*s);
		s++;
	}
}

String EscapeHtml(const String& s)
{
	StringBuffer out;
	EscapeHtml(out, s);
	return out;
}

force_inline
static void sCatAsString(StringBuffer& out, const Value& v)
{
	LTIMING("sCatAsString");
	if(IsNull(v))
		return;
	if(v.Is<RawHtmlText>()) {
		LTIMING("Cat RawHtml");
		out.Cat(ValueTo<RawHtmlText>(v).text);
	}
	else {
		String h;
		if(v.Is<String>())
			h = ValueTo<String>(v);
		else {
			LTIMING("AsString");
			h = AsString(v);
		}
		LTIMING("Escape html");
		EscapeHtml(out, h);
	}
}

Value Compiler::ExeFor::Eval(ExeContext& x) const
{
	LTIMING("ExeFor");
	Value array = value->Eval(x);
	LLOG("ExeFor array: " << array);
	if(array.GetCount() == 0 && onempty)
		return onempty->Eval(x);
	ValueMap m;
	bool map = array.Is<ValueMap>();
	if(map)
		m = array;
	int q = x.stack.GetCount();
	x.stack.Add();
	x.stack.Add();
	for(int i = 0; i < array.GetCount(); i++) {
		x.stack[q] = array[i];
		LoopInfo f;
		f.first = i == 0;
		f.last = i == array.GetCount() - 1;
		f.index = i;
		f.key = map ? m.GetKeys()[i] : (Value)i;
		x.stack[q + 1] = RawToValue(f);
		sCatAsString(x.out, body->Eval(x));
	}
	x.stack.SetCount(q);
	return Value();
}

Value Compiler::ExeBlock::Eval(ExeContext& x) const
{
	int q = x.stack.GetCount();
	for(int i = 0; i < item.GetCount(); i++)
		sCatAsString(x.out, item[i]->Eval(x));
	x.stack.SetCount(q);
	return Value();
}

String Render(const One<Exe>& exe, Renderer *r, Vector<Value>& var)
{
	LTIMING("Render0");
	ExeContext x(var, r);
	Value v = exe->Eval(x);
	x.out.Cat(AsString(v));
	return x.out;
}

};
