#include "Debuggers.h"

#ifdef PLATFORM_WIN32

#define LLOG(x) // LOG(x)

void Pdb::PrettyTreeNode(int parent, Pdb::Val val, int64 from)
{
	try {
		Pretty pp;
		if(val.ref || !PrettyVal(val, 0, 0, pp))
			return;
		if(pp.kind == SINGLE_VALUE) {
			Pretty p;
			PrettyVal(val, 0, 1, p);
			if(p.text.part.GetCount()) {
				NamedVal nv;
				Visual result;
				nv.name = "$$";
				for(const VisualPart& vp : p.text.part)
					result.Cat(vp.text, vp.ink);
				tree.Add(parent, Null, RawToValue(nv), RawPickToValue(pick(result)), false);
			}
			else
			if(p.data_type.GetCount() && p.data_ptr.GetCount())
				PrettyTreeNode(parent, MakeVal(p.data_type[0], p.data_ptr[0]), from);
		}
		if(pp.kind == CONTAINER) {
			int chunk = pp.chunk;
			int n = int(min(pp.data_count, from + chunk) - from);
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
					Visual result;
					nv.name << '[' << i + from << ']';
					nv.val = val;
					nv.exp = true;
					result.Cat(nv.name + ' ', SGray);
					try {
						for(int j = 0; j < p.data_type.GetCount(); j++) {
							if(j)
								result.Cat(": ");
							item[j].address = p.data_ptr[p.separated_types ? j * n + i : ii++];
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
					tree.Add(parent, Null, RawToValue(nv), RawPickToValue(pick(result)),
					         nv.key.type != UNKNOWN || nv.val.type > 0);
				}
				if(pp.data_count > n && from == 0 && pp.data_count != INT64_MAX) {
					NamedVal nv;
					nv.name << "..";
					nv.val = val;
					int64 ii = n;
					while(ii < pp.data_count) {
						nv.from = ii;
						nv.chunk = chunk;
						Visual v;
						v.Cat(String() << "[" << ii << ".." << min(pp.data_count - 1, ii + chunk - 1) << "]", SGray);
						tree.Add(parent, Null, RawToValue(nv), RawPickToValue(pick(v)), true);
						ii += chunk;
					}
				}
			}
		}
	}
	catch(LengthLimit) {}
	catch(CParser::Error e) {}
}

bool Pdb::TreeNode(int parent, const String& name, Pdb::Val val, int64 from, Color ink, bool exp)
{
	PrettyTreeNode(parent, val);
	NamedVal nv;
	nv.name = name;
	nv.val = val;
	nv.from = from;
	nv.exp = exp;
	Visual v;
	bool r = true;
	try {
		v.Cat(name, ink);
		if(!from) {
			v.Cat("=", SGray);
			Visualise(v, val, 0);
		}
	}
	catch(LengthLimit) {}
	catch(CParser::Error e) {
		v.Cat(e, SColorDisabled);
		r = false;
	}
	tree.Add(parent, Null, RawToValue(nv), RawPickToValue(pick(v)), val.type >= 0 || val.ref > 0);
	return r;
}

bool Pdb::TreeNodeExp(int parent, const String& name, Val val, int64 from, Color ink)
{
	return TreeNode(parent, name, val, from, ink, true);
}

void Pdb::TreeExpand(int node)
{
	if(tree.GetChildCount(node))
		return;
	try {
		Value v = tree.Get(node);
		if(!v.Is<NamedVal>())
			return;
		const NamedVal& nv = ValueTo<NamedVal>(tree.Get(node));
		Val val = nv.val;
		if(nv.key.type != UNKNOWN) {
			TreeNode(node, "key", nv.key);
			TreeNode(node, "value", val);
			SaveTree();
			return;
		}
		if(nv.val.ref > 0) {
			Val val0 = val;
			val = DeRef(val);
			if(val.type < 0 || val.ref > 0) {
				int sz = SizeOfType(val.type);
				val.address += sz * nv.from;
				for(int i = 0; i < (nv.from ? nv.chunk : 40); i++) {
					if(!TreeNodeExp(node, String() << "[" << i + nv.from << "]" , val, 0, SLtMagenta())) {
						SaveTree();
						return;
					}
					val.address += sz;
				}
				TreeNode(node, "[more]", val0, nv.from ? nv.from + nv.chunk : 40);
				SaveTree();
				return;
			}
		}
		else
		if(nv.from > 0) {
			PrettyTreeNode(node, val, nv.from);
			SaveTree();
			return;
		}
		if(val.type < 0) {
			SaveTree();
			return;
		}
		PrettyTreeNode(node, val, 0);
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
				TreeNode(node, Format("[%d]", i), ventry);
			}
			SaveTree();
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
			TreeNodeExp(node, t.member.GetKey(i), r);
		}
		for(int i = 0; i < t.static_member.GetCount(); i++) {
			Val r = t.static_member[i];
			TreeNodeExp(node, t.static_member.GetKey(i), r);
		}
		SaveTree();
	}
	catch(LengthLimit) {}
	catch(CParser::Error e) {}
}

void Pdb::StoreTree(StringBuffer& result, int parent)
{
	int n = tree.GetChildCount(parent);
	for(int i = 0; i < n; i++) {
		int child = tree.GetChild(parent, i);
		bool first = true;
		if(tree.IsOpen(child)) {
			const NamedVal& nv = ValueTo<NamedVal>(tree.Get(child));
			if(!first)
				result << ";";
			first = false;
			result << " " << AsCString(nv.name) << " {";
			StoreTree(result, child);
			result << "}";
		}
	}
}

void Pdb::SaveTree()
{
	if(restoring_tree)
		return;
	Value v = tree.Get(0);
	if(!IsType<NamedVal>(v))
		return;
	const NamedVal& nv = ValueTo<NamedVal>(v);
	if(nv.val.type < 0)
		return;
	StringBuffer w;
	Point p = tree.GetScroll();
	w << p.x << ' ' << p.y << ' ';
	int id = tree.GetCursor();
	Vector<String> cursor;
	while(id >= 0) {
		const NamedVal& nv = ValueTo<NamedVal>(tree.Get(id));
		cursor.Add(AsCString(nv.name));
		id = tree.GetParent(id);
	}
	Reverse(cursor);
	w << Join(cursor, ";") << ": ";
	StoreTree(w, 0);
	String r = w;
	treetype.GetAdd(GetType(nv.val.type).name) = r;
	LOG("SaveTree " << GetType(nv.val.type).name << ' ' << r);
}

int Pdb::FindChild(int parent, String id)
{
	int n = tree.GetChildCount(parent);
	for(int i = 0; i < n; i++) {
		int child = tree.GetChild(parent, i);
		const NamedVal& nv = ValueTo<NamedVal>(tree.Get(child));
		if(nv.name == id)
			return child;
	}
	return -1;
}

void Pdb::ExpandTreeType(int parent, CParser& p)
{
	while(p.IsString()) {
		int child = FindChild(parent, p.ReadString());
		if(child >= 0) {
			tree.Open(child);
			p.Char('{');
			ExpandTreeType(child, p);
			p.Char('}');
		}
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
	if(nv.val.reference) {
		nv.val = DeRef(nv.val);
		nv.val.reference = false;
	}
	nv.name = exp;
	String n = exp;
	if(nv.val.type >= 0)
		n = GetType(nv.val.type).name;

	Visual v;
	try {
		v.Cat(n, SBlack);
		v.Cat("=", SGray);
		Visualise(v, nv.val, 0);
	}
	catch(LengthLimit) {}
	catch(CParser::Error e) {
		v.Cat(e, SColorDisabled);
	}
	tree.SetDisplay(visual_display);

	tree.SetRoot(Null, RawToValue(nv), RawPickToValue(pick(v)));
	if(nv.val.type >= 0) {
		String w = treetype.Get(n, Null);
		LOG("SetTree " << n << ' ' << w);
		tree.Open(0);
		CParser p(w);
		try {
			Point sc;
			sc.x = p.ReadInt();
			sc.y = p.ReadInt();
			Vector<String> cursor;
			while(!p.Char(':')) {
				cursor.Add(p.ReadString());
				p.Char(';');
			}
			restoring_tree++;
			ExpandTreeType(0, p);
			restoring_tree--;
			tree.ScrollTo(sc);
			if(cursor.GetCount()) {
				int ii = 0;
				for(int i = 1; i < cursor.GetCount(); i++) {
					ii = FindChild(ii, cursor[i]);
					if(ii < 0)
						break;
				}
				if(ii >= 0)
					tree.SetCursor(ii);
			}
		}
		catch(CParser::Error) {}
	}
	SyncTreeDisas();
}

void Pdb::SetTreeA(ArrayCtrl *array)
{
	SetTree(array->Get(0));
}

String Pdb::GetTreeText(int id)
{
	return tree.GetValue(id).To<Visual>().GetString();
}

void Pdb::GetTreeText(String& r, int id, int depth) {
	int n = tree.GetChildCount(id);
	r << String('\t', depth) << GetTreeText(id) << "\n";
	for(int i = 0; i < n; i++)
		GetTreeText(r, tree.GetChild(id, i), depth + 1);
}

void Pdb::TreeMenu(Bar& bar)
{
	bar.Add(tree.IsCursor(), "Watch", [=] { TreeWatch(); });
	bar.Add(tree.IsCursor(), "Copy", [=] {
		ClearClipboard();
		AppendClipboardText(GetTreeText(tree.GetCursor()));
	});
	bar.Add(tree.GetLineCount(), "Copy all", [=] {
		ClearClipboard();
		String r;
		GetTreeText(r, 0, 0);
		AppendClipboardText(r);
	});
}

void Pdb::TreeWatch()
{
	int id = tree.GetCursor();
	String exp;
	while(id >= 0) {
		Value v = tree.Get(id);
		if(!v.Is<NamedVal>())
			break;
		const NamedVal& nv = v.To<NamedVal>();
		if(nv.exp) {
			if(IsAlpha(*exp))
				exp = '.' + exp;
			exp = nv.name + exp;
		}
		id = tree.GetParent(id);
	}
	AddWatch(tree_exp + "." + exp);
}

#endif
