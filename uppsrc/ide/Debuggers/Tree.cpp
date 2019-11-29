#include "Debuggers.h"

#ifdef PLATFORM_WIN32

#define LLOG(x) // LOG(x)

void Pdb::PrettyTreeNode(int parent, Pdb::Val val, int64 from)
{
	try {
		Pretty pp;
		if(PrettyVal(val, 0, 0, pp) && pp.kind == CONTAINER) {
			int n = int(min(pp.data_count, from + 10000) - from);
			Pretty p;
			PrettyVal(val, from, n, p);

			if(p.data_type.GetCount()) {
				Buffer<Val> item(p.data_type.GetCount());
				for(int i = 0; i < p.data_type.GetCount(); i++) {
					(TypeInfo &)item[i] = GetTypeInfo(p.data_type[i]);
					item[i].context = val.context;
				}
				int ii = 0;
				int n = p.data_ptr.GetCount() / p.data_type.GetCount();
				for(int i = 0; i < n; i++) {
					NamedVal nv;
					nv.name << '[' << i + from << ']';
					nv.val = val;
					Visual result;
					try {
						for(int j = 0; j < p.data_type.GetCount(); j++) {
							if(j)
								result.Cat(": ");
							item[j].address = p.data_ptr[ii++];
							nv.val = item[j];
							if(p.data_type.GetCount() > 1 && j == 0)
								nv.key = item[0];
							Visualise(result, item[j], MEMBER);
						}
					}
					catch(LengthLimit) {}
					catch(CParser::Error e) {
						result.Cat("??");
					}
					tree.Add(parent, Null, RawToValue(nv), nv.name + ' ' + result.GetString(),
					         nv.key.type != UNKNOWN || nv.val.type > 0);
				}
				if(pp.data_count > n && from == 0) {
					NamedVal nv;
					nv.name << "..";
					nv.val = val;
					int64 ii = n;
					while(ii < pp.data_count) {
						nv.from = ii;
						tree.Add(parent, Null, RawToValue(nv), String() << "[" << ii << ".." << min(pp.data_count - 1, ii + 9999) << "]", true);
						ii += 10000;
					}
				}
			}
		}
	}
	catch(LengthLimit) {}
	catch(CParser::Error e) {}
}

void Pdb::TreeNode(int parent, const String& name, Pdb::Val val)
{
	PrettyTreeNode(parent, val);
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
	if(nv.key.type != UNKNOWN) {
		TreeNode(node, "key", nv.key);
		TreeNode(node, "value", val);
		return;
	}
	if(nv.from > 0) {
		PrettyTreeNode(node, val, nv.from);
		return;
	}
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
	tree_exp = exp;
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
	PrettyTreeNode(0, nv.val);
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
	SyncTreeDisas();
}

void Pdb::SetTreeA(ArrayCtrl *array)
{
	SetTree(array->Get(0));
}

#endif
