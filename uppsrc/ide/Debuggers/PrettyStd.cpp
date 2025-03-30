#include "Debuggers.h"

#ifdef PLATFORM_WIN32

void Pdb::PrettyStdString(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	adr_t a;
	int size;
	bool w = tparam[0] == "wchar_t";
	if(HasAttr(val, "__r_")) { // CLANG variant
		Val r = GetAttr(val, "__r_");
		Val v = GetAttr(r, "__value_");
		Val s = GetAttr(v, "__s");
		if(GetInt(GetAttr(s, "__is_long_"))) {
			s = GetAttr(v, "__l");
			a = DeRef(GetAttr(s, "__data_")).address;
		}
		else
			a = GetAttr(s, "__data_").address;
		size = GetIntAttr(s, "__size_");
	}
	else {
		Val q = GetAttr(GetAttr(val, "_Mypair"), "_Myval2");
		size = GetIntAttr(q, "_Mysize");
		int res = GetIntAttr(q, "_Myres");
		a = GetAttr(GetAttr(q, "_Bx"), "_Buf").address;
		if(res >= (w ? 8 : 16))
			a = PeekPtr(a);
	}
	p.data_count = size;
	p.data_type << (w ? "short int" : "char");
	int sz = w + 1;
	for(int i = 0; i < count; i++)
		p.data_ptr.Add(a + sz * (from + i));
	p.kind = TEXT;
}

void Pdb::PrettyStdVector(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	Val begin, end;
	if(HasAttr(val, "__begin_")) { // CLANG's std::vector
		begin = DeRef(GetAttr(val, "__begin_"));
		end = DeRef(GetAttr(val, "__end_"));
	}
	else {
		Val q = GetAttr(GetAttr(val, "_Mypair"), "_Myval2");
		begin = DeRef(GetAttr(q, "_Myfirst"));
		end = DeRef(GetAttr(q, "_Mylast"));
	}
	int sz = SizeOfType(tparam[0]);
	p.data_count = sz ? (end.address - begin.address) / sz : 0;
	for(int i = 0; i < count; i++)
		p.data_ptr.Add(begin.address + (i + from) * sz);
}

void Pdb::TraverseTree(bool set, Pdb::Val head, Val node, int64& from, int& count, Pdb::Pretty& p, int depth)
{
	if(depth > 40) // avoid problems if tree is damaged
		return;
	if(depth && node.address == head.address || count <= 0) // we are at the end or have read enough items
		return;
	TraverseTree(set, head, DeRef(GetAttr(node, "_Left")), from, count, p, depth + 1);
	if(node.address != head.address) {
		if(from == 0) {
			Val v = GetAttr(node, "_Myval");
			if(set)
				p.data_ptr.Add(v.address);
			else {
				p.data_ptr.Add(GetAttr(v, "first").address);
				p.data_ptr.Add(GetAttr(v, "second").address);
			}
			count--;
		}
		else
			from--;
	}
	TraverseTree(set, head, DeRef(GetAttr(node, "_Right")), from, count, p, depth + 1);
}

void Pdb::TraverseTreeClang(bool set, int nodet, Val node, int64& from, int& count, Pdb::Pretty& p, int depth, int key_size, int value_size)
{
	if(depth > 40 || count <= 0) // avoid problems if tree is damaged
		return;

	Val left = DeRef(GetAttr(node, "__left_"));
	if(left.address)
		TraverseTreeClang(set, nodet, left, from, count, p, depth + 1, key_size, value_size);

	node.type = nodet;
	Val data = GetAttr(node, "__value_");
	if(from == 0) {
		p.data_ptr.Add(data.address);
		if(!set)
			p.data_ptr.Add(Align(data.address + key_size, value_size));
		count--;
	}
	else
		from--;

	Val right = DeRef(GetAttr(node, "__right_"));
	if(right.address)
		TraverseTreeClang(set, nodet, right, from, count, p, depth + 1, key_size, value_size);
}

void Pdb::PrettyStdTree(Pdb::Val val, bool set, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	if(HasAttr(val, "__tree_")) {
		String nodet;
		if(set)
			nodet << "std::__1::__tree_node<" << tparam[0] << ",void *>";
		else {
			nodet = "std::__1::__tree_node<std::__1::__value_type<" << tparam[0] << "," << tparam[1];
			if(*nodet.Last() == '>')
				nodet << ' ';
			nodet << ">,void *>";
		}
		Val tree = GetAttr(val, "__tree_");
		Val value = GetAttr(GetAttr(tree, "__pair1_"), "__value_");
		p.data_count = GetIntAttr(GetAttr(tree, "__pair3_"), "__value_");
		Val node = DeRef(GetAttr(value, "__left_"));
		TraverseTreeClang(set, GetTypeInfo(nodet).type, node, from, count, p, 0, SizeOfType(tparam[0]),  SizeOfType(tparam[1]));
	}
	else {
		val = GetAttr(GetAttr(GetAttr(val, "_Mypair"), "_Myval2"), "_Myval2");
		p.data_count = GetIntAttr(val, "_Mysize");
		Val head = DeRef(GetAttr(val, "_Myhead")); // points to leftmost element (!)
		Val top = DeRef(GetAttr(head, "_Left"));
		for(int i = 0; i < 40; i++) { // find topmost node, i is depth limit
			Val v = DeRef(GetAttr(top, "_Parent"));
			if(v.address == head.address)
				break;
			top = v;
		}
		TraverseTree(set, head, top, from, count, p, 0);
	}
}

void Pdb::PrettyStdListM(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p, bool map)
{
	if(HasAttr(val, "__end_")) {
		p.data_count = GetIntAttr(GetAttr(val, "__size_alloc_"), "__value_");
		int node_type = GetTypeInfo("std::__1::__list_node<" + tparam[0] + ",void *>").type;
		Val next = DeRef(GetAttr(GetAttr(val, "__end_"), "__next_"));
		while(count > 0) {
			if(from == 0) {
				Val h = next;
				h.type = node_type;
				p.data_ptr.Add(GetAttr(h, "__value_").address);
				count--;
			}
			else
				from--;
			next = DeRef(GetAttr(next, "__next_"));
		}
	}
	else {
		val = GetAttr(GetAttr(val, "_Mypair"), "_Myval2");
		p.data_count = GetIntAttr(val, "_Mysize");
		Val next = DeRef(GetAttr(val, "_Myhead"));
		while(count > 0) {
			next = DeRef(GetAttr(next, "_Next"));
			Val v = GetAttr(next, "_Myval");
			if(from == 0) {
				if(map) { // support for MS std::unordered_map
					p.data_ptr.Add(GetAttr(v, "first").address);
					p.data_ptr.Add(GetAttr(v, "second").address);
				}
				else
					p.data_ptr.Add(GetAttr(next, "_Myval").address);
				count--;
			}
			else
				from--;
		}
	}
}

void Pdb::PrettyStdList(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	PrettyStdListM(val, tparam, from, count, p);
}

void Pdb::PrettyStdForwardList(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	String value, next;
	p.data_count = 0;
	if(HasAttr(val, "__before_begin_")) {
		val = DeRef(GetAttr(GetAttr(GetAttr(val, "__before_begin_"), "__value_"), "__next_"));
		value = "__value_";
		next = "__next_";
	}
	else {
		val = DeRef(GetAttr(GetAttr(GetAttr(val, "_Mypair"), "_Myval2"), "_Myhead"));
		value = "_Myval";
		next = "_Next";
	}
	while(val.address) {
		if(from == 0) {
			if(count) {
				p.data_ptr.Add(GetAttr(val, value).address);
				count--;
			}
		}
		else
			from--;
		val = DeRef(GetAttr(val, next));
		p.data_count++;
		if(count == 0 && p.data_count > 10000) {
			p.data_count = INT64_MAX;
			break;
		}
	}
}

void Pdb::PrettyStdDeque(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	int sz = SizeOfType(tparam[0]);
	int block_size, start;
	adr_t map;
	if(HasAttr(val, "__size_")) {
		p.data_count = GetIntAttr(GetAttr(val, "__size_"), "__value_");
		block_size = sz && sz < 256 ? 4096 / sz : 16;
		start = GetIntAttr(val, "__start_");
		map = DeRef(GetAttr(GetAttr(val, "__map_"), "__begin_")).address;
	}
	else {
		val = GetAttr(GetAttr(val, "_Mypair"), "_Myval2");
		p.data_count = GetIntAttr(val, "_Mysize");
		block_size = sz <= 1 ? 16 : sz <= 2 ? 8 : sz <= 4 ? 4 : sz <= 8 ? 2 : 1;
		start = GetIntAttr(val, "_Myoff");
		map = DeRef(GetAttr(val, "_Map")).address;
	}
	
	for(int i = 0; i < count; i++) {
		int64 q = i + from + start;
		p.data_ptr.Add(PeekPtr(map + (q / block_size) * (win64 ? 8 : 4)) + q % block_size * sz);
	}
}

void Pdb::PrettyStdUnordered(Pdb::Val val, bool set, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	if(HasAttr(val, "__table_")) {
		val = GetAttr(val, "__table_");
		p.data_count = GetIntAttr(GetAttr(val, "__p2_"), "__value_");
		String nodet = set ? "std::__1::__hash_node<" << tparam[0] << ",void *>"
		                   : "std::__1::__hash_node<std::__1::__hash_value_type<"
		                      << tparam[0] << "," << tparam[1] << " >,void *>";
		int ntype = GetTypeInfo(nodet).type;
		adr_t next = DeRef(GetAttr(GetAttr(GetAttr(val, "__p1_"), "__value_"), "__next_")).address;
		int key_size = SizeOfType(tparam[0]);
		int value_size = SizeOfType(tparam[1]);
		while(next && count > 0) {
			Val v = val;
			v.type = ntype;
			v.address = next;
			if(from == 0) {
				Val vl = GetAttr(v, "__value_");
				p.data_ptr.Add(vl.address);
				if(!set)
					p.data_ptr.Add(Align(vl.address + key_size, value_size));
			}
			else
				from--;
			next = DeRef(GetAttr(v, "__next_")).address;
		}
	}
	else
		PrettyStdListM(GetAttr(val, "_List"), tparam, from, count, p, !set);
}

void Pdb::PrettyStdAtomic(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	p.data_ptr << val.address;
	p.data_type << tparam[0];
	p.kind = SINGLE_VALUE;
}

#endif
