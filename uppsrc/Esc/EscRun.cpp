#include "Esc.h"


NAMESPACE_UPP

#define LTIMING(x) // RTIMING(x)

void LambdaArgs(CParser& p, EscLambda& l)
{
	p.PassChar('(');
	if(!p.Char(')'))
		for(;;) {
			if(p.Char3('.', '.', '.')) {
				l.varargs = true;
				p.PassChar(')');
				break;
			}
			l.inout.Add(p.Char('&'));
			l.arg.Add(p.ReadId());
			if(p.Char('=')) {
				const char *s = p.GetPtr();
				int level = 0;
				for(;;) {
					if((p.IsChar(')') || p.IsChar(',')) && level == 0)
						break;
					if(p.Char(')'))
						level--;
					else
					if(p.Char('('))
						level++;
					else
						p.SkipTerm();
					if(p.IsEof())
						p.ThrowError("unexpected end of file while reading default value for argument "
						             + l.arg.Top());
				}
				l.def.Add(String(s, p.GetPtr()));
			}
			else
				if(l.def.GetCount())
					p.ThrowError("missing default value for argument " + l.arg.Top());
			if(p.Char(')'))
				break;
			p.PassChar(',');
		}
	l.inout.Shrink();
	l.arg.Shrink();
}

EscValue ReadLambda(CParser& p)
{
	EscValue lambda;
	EscLambda& l = lambda.CreateLambda();
	LambdaArgs(p, l);
	const char *t = p.GetPtr();
	l.filename = p.GetFileName();
	l.line = p.GetLine();
	if(!p.Char('{'))
		p.ThrowError("missing '{'");
	SkipBlock(p);
	l.code = String(t, p.GetPtr());
	return lambda;
}

EscValue ReadLambda(const char *s)
{
	CParser p(s);
	return ReadLambda(p);
}

String EscEscape::InCall()
{
	return IsNull(id) ? String() : " in call to '" + id + "'";
}

String EscEscape::DumpType(int i)
{
	if(i < arg.GetCount())
		return String().Cat() << " (" << arg[i].GetTypeName() << " present)";
	else
		return " (not enough arguments)";
}

void  EscEscape::CheckNumber(int i)
{
	if(i < arg.GetCount() && arg[i].IsNumber())
		return;
	ThrowError(String().Cat() << "number expected as parameter " << i + 1 << InCall()
	           << DumpType(i));
}

double EscEscape::Number(int i)
{
	if(i >= arg.GetCount())
		ThrowError("too little parameters" + InCall());
	return esc.Number(arg[i], "parameter" + InCall());
}

int EscEscape::Int(int i)
{
	if(i >= arg.GetCount())
		ThrowError("too little parameters" + InCall());
	return (int)esc.Int(arg[i], "parameter" + InCall());
}

void  EscEscape::CheckArray(int i)
{
	if(i < arg.GetCount() && arg[i].IsArray())
		return;
	ThrowError(String().Cat() << "array expected as parameter " << i + 1 << InCall()
	           << DumpType(i));
}

void  EscEscape::CheckMap(int i)
{
	if(i < arg.GetCount() && arg[i].IsMap())
		return;
	ThrowError(String().Cat() << "map expected as parameter " << i + 1 << InCall());
}

void Escape(ArrayMap<String, EscValue>& globals, const char *function, Callback1<EscEscape&> escape)
{
	CParser p(function);
	EscValue& v = globals.GetPut(p.ReadId());
	EscLambda& l = v.CreateLambda();
	l.escape = escape;
	LambdaArgs(p, l);
}

void Escape(ArrayMap<String, EscValue>& globals, const char *function, void (*escape)(EscEscape& e))
{
	Escape(globals, function, callback(escape));
}

void  EscValue::Escape(const char *method, Callback1<EscEscape&> escape)
{
	CParser p(method);
	String id = p.ReadId();
	EscValue v;
	EscLambda& l = v.CreateLambda();
	l.escape = escape;
	LambdaArgs(p, l);
	MapSet(id, v);
}

void  EscValue::Escape(const char *method, EscHandle *h, Callback1<EscEscape&> escape)
{
	CParser p(method);
	String id = p.ReadId();
	EscValue v;
	EscLambda& l = v.CreateLambda();
	l.escape = escape;
	l.handle = h;
	h->Retain();
	LambdaArgs(p, l);
	if(IsVoid())
		SetEmptyMap();
	MapSet(id, v);
}

void Scan(ArrayMap<String, EscValue>& global, const char *file, const char *filename)
{
	LTIMING("Scan");
	CParser p(file, filename);
	while(!p.IsEof()) {
		EscValue& v = global.GetPut(p.ReadId());;
		v = ReadLambda(p);
	}
}

EscValue Execute(ArrayMap<String, EscValue>& global, EscValue *self,
                 const EscValue& lambda, Vector<EscValue>& arg, int op_limit)
{
	const EscLambda& l = lambda.GetLambda();
	if(arg.GetCount() != l.arg.GetCount()) {
		String argnames;
		for(int i = 0; i < l.arg.GetCount(); i++)
			argnames << (i ? ", " : "") << l.arg[i];
		throw CParser::Error(NFormat("invalid number of arguments (%d passed, expected: %s)", arg.GetCount(), argnames));
	}
	EscValue ret;
	{
		Esc sub(global, l.code, op_limit, l.filename, l.line);
		if(self)
			sub.self = *self;
		for(int i = 0; i < l.arg.GetCount(); i++)
			sub.var.GetPut(l.arg[i]) = arg[i];
		sub.Run();
		if(self)
			*self = sub.self;
		ret = sub.return_value;
	}
	return ret;
}

EscValue Execute(ArrayMap<String, EscValue>& global, EscValue *self,
                 const char *name, Vector<EscValue>& arg, int op_limit)
{
	if(!self->IsMap())
		return EscValue();
	const VectorMap<EscValue, EscValue>& m = self->GetMap();
	int ii = m.Find(String(name));
	if(ii >= 0 && m[ii].IsLambda())
		return Execute(global, self, m[ii], arg, op_limit);
	return EscValue();
}

EscValue Execute(ArrayMap<String, EscValue>& global, const char *name, int op_limit)
{
	int ii = global.Find(String(name));
	Vector<EscValue> arg;
	if(ii >= 0 && global[ii].IsLambda())
		return Execute(global, NULL, global[ii], arg, op_limit);
	return EscValue();
}

EscValue Evaluatex(const char *expression, ArrayMap<String, EscValue>& global, int oplimit)
{
	Esc sub(global, expression, oplimit, "", 0);
	for(int i = 0; i < global.GetCount(); i++)
		sub.var.Add(global.GetKey(i), global[i]);
	EscValue v;
	v = sub.GetExp();
	for(int i = 0; i < sub.var.GetCount(); i++)
		global.GetAdd(sub.var.GetKey(i)) = sub.var[i];
	return v;
}

EscValue Evaluate(const char *expression, ArrayMap<String, EscValue>& global, int oplimit)
{
	try {
		return Evaluatex(expression, global, oplimit);
	}
	catch(CParser::Error&) {}
	return EscValue();
}

String   Expand(const String& doc, ArrayMap<String, EscValue>& global,
                int oplimit, String (*format)(const Value& v))
{
	String out;
	const char *term = doc;
	bool cond = true;
	while(*term)
		if(term[0] == '<' && term[1] == ':') {
			term += 2;
			try {
				Esc sub(global, term, oplimit, "", 0);
				for(int i = 0; i < global.GetCount(); i++)
					sub.var.Add(global.GetKey(i), global[i]);
				EscValue v;
				if(*term == '{') {
					sub.Run();
					v = sub.return_value;
				}
				else
				if(sub.Char('!')) {
					EscValue& v = global.GetPut(sub.ReadId());
					v = ReadLambda(sub);
				}
				else
				if(sub.Char('?'))
					cond = IsTrue(sub.GetExp());
				else
				if(sub.Char('/'))
					cond = !cond;
				else
				if(sub.Char('.'))
					cond = true;
				else
					v = sub.GetExp();
				if(cond)
					out << format(StdValueFromEsc(v));
				sub.Spaces();
				term = sub.GetPtr();
				if(term[0] != ':' || term[1] != '>')
					throw CParser::Error("missing :>" + String(term));
				term += 2;
			}
			catch(CParser::Error& e) {
				out << "(#ERROR: " << e << "#)";
			}
		}
		else {
			if(cond)
				out.Cat(*term);
			term++;
		}
	return out;
}

END_UPP_NAMESPACE
