#include "Debuggers.h"

#ifdef PLATFORM_WIN32

#define LLOG(x) // LOG(x)

void Pdb::TreeNode(int parent, const String& name, Pdb::Val val)
{
	NamedVal nv;
	nv.name = name;
	nv.val = val;
	tree.Add(parent, Null, RawToValue(nv), name + "=" + Visualise(val).GetString(), val.type >= 0 || val.ref > 0);
}

void Pdb::TreeExpand(int node)
{
	if(tree.GetChildCount(node))
		return;
	Value v = tree.Get(node);
	if(!v.Is<NamedVal>())
		return;
	const NamedVal& nv = ValueTo<NamedVal>(tree.Get(node));
	Val val = nv.val;
	if(nv.val.ref > 0) {
		val = DeRef(val);
		if(val.type < 0 || val.ref > 0) {
			TreeNode(node, '*' + nv.name, val);
			SaveTree();
			return;
		}
	}
	if(val.type < 0) {
		SaveTree();
		return;
	}
	const Type& t = GetType(val.type);
	if(t.vtbl_typeindex == -2) {
		int count = GetSymInfo(t.modbase, type.GetKey(val.type), TI_GET_COUNT);
		Val prtti;
		prtti.ref = 1;
		prtti.type = UINT4;
		prtti.address = val.address - 4;
		Val rtti = GetRVal(prtti);
		FnInfo rtf = GetFnInfo(rtti.address);
		TreeNode(node, rtf.name, prtti);
		for(int i = 0; i < count; i++) {
			Val ventry;
			ventry.type = PFUNC;
			ventry.address = val.address + 4 * i;
			TreeNode(node, NFormat("[%d]", i), ventry);
		}
		return;
	}
	if(t.vtbl_typeindex >= 0) {
		Val vtbl;
		vtbl.ref = 1;
		vtbl.address = val.address + t.vtbl_offset;
		vtbl.type = t.vtbl_typeindex;
		TreeNode(node, "virtual", vtbl);
/*
		Val vloc = GetRVal(vtbl);
		FnInfo vtbl_type = GetFnInfo(vloc.address);
		const char *p = vtbl_type.name, *e = p;
		while(*e && !(*e == ':' && e[1] == ':'))
			e++;
		String fullsym(p, e);
		FnInfo ffs = GetFnInfo(fullsym);
		if(ffs.pdbtype) {
			Val typeval;
			typeval.address = val.address;
			TypeVal(typeval, ffs.pdbtype, t.modbase);
			TreeNode(node, String().Cat() << '[' << ffs.name << ']', typeval);
		}
*/
	}
	for(int i = 0; i < t.base.GetCount(); i++) {
		Val r = t.base[i];
		r.address += val.address;
		if(r.type >= 0) {
			const Type& bt = GetType(r.type);
			TreeNode(node, bt.name, r);
		}
	}
	for(int i = 0; i < t.member.GetCount(); i++) {
		Val r = t.member[i];
		r.address += val.address;
		TreeNode(node, t.member.GetKey(i), r);
	}
	for(int i = 0; i < t.static_member.GetCount(); i++) {
		Val r = t.static_member[i];
		TreeNode(node, t.static_member.GetKey(i), r);
	}
}

String Pdb::StoreTree(int parent)
{
	String result;
	int n = tree.GetChildCount(parent);
	for(int i = 0; i < n; i++) {
		int child = tree.GetChild(parent, i);
		if(tree.IsOpen(child)) {
			const NamedVal& nv = ValueTo<NamedVal>(tree.Get(child));
			if(!IsNull(result))
				result << ';';
			result << nv.name;
			String w = StoreTree(child);
			if(!IsNull(w))
				result << '{' << w << '}';
		}
	}
	return result;
}

void Pdb::SaveTree()
{
	Value v = tree.Get(0);
	if(!IsType<NamedVal>(v))
		return;
	const NamedVal& nv = ValueTo<NamedVal>(v);
	if(nv.val.type < 0)
		return;
	String w;
	Point p = tree.GetScroll();
	w << p.x << ' ' << p.y << ' ' << tree.GetCursor() << ' ' << StoreTree(0);
	treetype.GetAdd(GetType(nv.val.type).name) = w;
	LOG("SaveTree " << GetType(nv.val.type).name << ' ' << w);
}

void Pdb::ExpandTreeType(int parent, CParser& p)
{
	for(;;) {
		String id;
		if(p.Char('*'))
			id = "*";
		if(p.IsId())
			id << p.ReadId();
		else
			break;
		for(;;)
			if(p.Char('<'))
				id << '<';
			else
			if(p.Char('>'))
				id << '>';
			else
			if(p.Char(','))
				id << ',';
			else
			if(p.IsInt())
				id << AsString(p.ReadInt());
			else
			if(p.IsString())
				id << '\"' << AsCString(p.ReadString()) << '\"';
			else
			if(p.IsId())
				id << p.ReadId();
			else
				break;
		int n = tree.GetChildCount(parent);
		for(int i = 0; i < n; i++) {
			int child = tree.GetChild(parent, i);
			const NamedVal& nv = ValueTo<NamedVal>(tree.Get(child));
			if(nv.name == id) {
				tree.Open(child);
				if(p.Char('{')) {
					ExpandTreeType(child, p);
					p.PassChar('}');
				}
				break;
			}
		}
		p.Char(';');
	}
}

void Pdb::SetTree(const String& exp)
{
	SaveTree();
	tree.Clear();
	NamedVal nv;
	try {
		CParser p(exp);
		nv.val = Exp(p);
	}
	catch(CParser::Error) {
		return;
	}
	nv.name = exp;
	String n = exp;
	if(nv.val.type >= 0)
		n = GetType(nv.val.type).name;
	tree.SetRoot(Null, RawToValue(nv), n + '=' + Visualise(nv.val).GetString());
	if(nv.val.type >= 0) {
		String w = treetype.Get(n, Null);
		LOG("SetTree " << n << ' ' << w);
		tree.Open(0);
		CParser p(w);
		try {
			Point sc;
			sc.x = p.ReadInt();
			sc.y = p.ReadInt();
			int cursor = p.ReadInt();
			ExpandTreeType(0, p);
			tree.ScrollTo(sc);
			if(cursor >= 0)
				tree.SetCursor(cursor);
		}
		catch(CParser::Error) {}
	}
}

void Pdb::SetTreeA(ArrayCtrl *array)
{
	SetTree(array->Get(0));
}

#endif
