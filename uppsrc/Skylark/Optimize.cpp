#include "Skylark.h"

#define LLOG(x)   // DLOG(x)

namespace Upp {

void Compiler::Iterate(Vector< One<Exe> >& a, Callback1< One<Exe>& > op)
{
	for(int i = 0; i < a.GetCount(); i++)
		op(a[i]);
}

void Compiler::OptimizeConst(One<Exe>& exe)
{
	One<Exe> oxe;
	Vector<Value> stack;
	ExeContext x(stack);
	oxe.Create<ExeConst>().value = exe->Eval(x);
	LLOG("OPTIMIZED constant: " << exe->Eval(stack, out));
	exe = pick(oxe);
	optimized = true;
}

void Compiler::Iterate(One<Exe>& exe, Callback1< One<Exe>& > op)
{
	if(ExeBlock *e = dynamic_cast<ExeBlock *>(~exe)) {
		Iterate(e->item, op);
		return;
	}
	if(ExeFor *e = dynamic_cast<ExeFor *>(~exe)) {
		op(e->value);
		op(e->body);
		op(e->onempty);
		return;
	}
	if(ExeLink *e = dynamic_cast<ExeLink *>(~exe)) {
		Iterate(e->arg, op);
		return;
	}
	if(ExeFn *e = dynamic_cast<ExeFn *>(~exe)) {
		Iterate(e->arg, op);
		return;
	}
	if(ExeField *e = dynamic_cast<ExeField *>(~exe)) {
		op(e->value);
		return;
	}
	if(ExeCond *e = dynamic_cast<ExeCond *>(~exe)) {
		op(e->cond);
		op(e->ontrue);
		op(e->onfalse);
		return;
	}
	if(ExeMap *e = dynamic_cast<ExeMap *>(~exe)) {
		Iterate(e->key, op);
		Iterate(e->value, op);
		return;
	}
	if(ExeArray *e = dynamic_cast<ExeArray *>(~exe)) {
		Iterate(e->item, op);
		return;
	}
	if(Exe2 *e = dynamic_cast<Exe2 *>(~exe)) {
		op(e->a);
		op(e->b);
		return;
	}
	if(Exe1 *e = dynamic_cast<Exe1 *>(~exe)) {
		op(e->a);
		return;
	}
}

void Compiler::CountNodes(One<Exe>& exe)
{
	count_node++;
	Iterate(exe, THISBACK(CountNodes));
}

int  Compiler::GetNodeCount(One<Exe>& exe)
{
	count_node = 0;
	CountNodes(exe);
	return count_node;
}

void Compiler::Optimize(One<Exe>& exe)
{
	if(!exe)
		return;
	bool optimized2 = false;
#ifdef _DEBUG0
	String name = "Optimizing " + String(typeid(*~exe).name());
	LLOGBLOCK(name);
#endif
	do {
		optimized = false;
		Iterate(exe, THISBACK(Optimize));
		if(ExeField *e = dynamic_cast<ExeField *>(~exe)) {
			if(ExeVar *e1 = dynamic_cast<ExeVar *>(~e->value)) {
				One<Exe> oxe;
				ExeVarField& vf = oxe.Create<ExeVarField>();
				vf.id = e->id;
				vf.var_index = e1->var_index;
				exe = pick(oxe);
				optimized = true;
				LLOG("OPTIMIZED ExeVarField");
			}
		}
		if(ExeLink *e = dynamic_cast<ExeLink *>(~exe)) {
			ExeVarField *e1;
			if(e->arg.GetCount() == 1 && (e1 = dynamic_cast<ExeVarField *>(~e->arg[0]))) {
				One<Exe> oxe;
				ExeLinkVarField1& o = oxe.Create<ExeLinkVarField1>();
				o.id = e1->id;
				o.var_index = e1->var_index;
				o.part = e->part;
				exe = pick(oxe);
				optimized = true;
				LLOG("OPTIMIZED ExeLinkVarField1");
			}
		}
		if(ExeBlock *e = dynamic_cast<ExeBlock *>(~exe)) {
			Vector< One<Exe> >& m = e->item;
			int i = 0;
			while(i < m.GetCount() - 1) {
				ExeConst *e1 = dynamic_cast<ExeConst *>(~m[i]);
				ExeConst *e2 = dynamic_cast<ExeConst *>(~m[i + 1]);
				if(e1 && e2 && e1->value.Is<RawHtmlText>() && e2->value.Is<RawHtmlText>()) {
					RawHtmlText t;
					t.text = ValueTo<RawHtmlText>(e1->value).text + ValueTo<RawHtmlText>(e2->value).text;
					One<Exe> oxe;
					oxe.Create<ExeConst>().value = RawToValue(t);
					m[i] = pick(oxe);
					m.Remove(i + 1);
					optimized = true;
					LLOG("OPTIMIZED ExeBlock constant folding");
				}
				else
					i++;
			}
		}
		if(Exe2 *e = dynamic_cast<Exe2 *>(~exe)) {
			if(dynamic_cast<ExeConst *>(~e->a) && dynamic_cast<ExeConst *>(~e->b))
				OptimizeConst(exe);
		}
		else
		if(Exe1 *e = dynamic_cast<Exe1 *>(~exe)) {
			if(dynamic_cast<ExeConst *>(~e->a))
				OptimizeConst(exe);
		}
		optimized2 = optimized2 || optimized;
		LLOG("---------");
	}
	while(optimized);
	
	optimized = optimized2;
}

};