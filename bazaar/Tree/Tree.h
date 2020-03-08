#ifndef _Tree_Tree_h
#define _Tree_Tree_h

#include <Core/Core.h>

NAMESPACE_UPP

template <class T>
class Tree
	: protected Array<T>, MoveableAndDeepCopyOption<Tree<T> >
{
protected:
	typedef Array<T> B;
	T* parent;
	T* root;

	inline void LinkChild(T& t)         { t.root = root; t.parent = (T *)this; }
	inline void UnlinkChild(T& t)       { t.root = NULL; t.parent = NULL; }
public:
	void Relink() { for(int i = 0; i < B::GetCount(); i++) { T& t = B::operator[](i); LinkChild(t); t.Relink();} }

public:
	T*       GetPtr()                   { return (T*) this; }
	const T* GetPtr() const             { return (const T *) this; }
	T*       GetParent()                { return parent; }
	const T* GetParent() const          { return parent; }
	T*       GetRoot()                  { return root; }
	const T* GetRoot() const            { return root; }
	void     SetAsRoot()                { ASSERT(parent == NULL); root = (T*)this; Relink(); }

// Array interface

	T&       Add()                      { T& t = B::Add(); LinkChild(t); return t; }
	T&       Add(const T& x)            { T& t = B::Add(x); LinkChild(t); t.Relink(); return t; }
	T&       Add(T&& x)                 { T& t = B::Add(x); LinkChild(t); t.Relink(); return t; }
	T&       Add(T *newt)               { ASSERT(newt->parent == NULL); T& t = B::Add(newt); LinkChild(t); t.Relink(); return t; }

	using B::operator[];
	using B::Get;
	using B::GetCount;
	using B::IsEmpty;

	using B::Trim;
	void     SetCount(int n)            { B::SetCount(n); for(int i = 0; i < B::GetCount(); i++) LinkChild(B::operator[](i)); }
	void     SetCount(int n, const T& init) { B::SetCount(n, init); Relink(); }
	void     SetCountR(int n)           { B::SetCountR(n); for(int i = 0; i < B::GetCount(); i++) LinkChild(B::operator[](i)); }
	void     SetCountR(int n, const T& init) { B::SetCountR(n, init); Relink(); }
	using B::Clear;

	T&       At(int i)                  { if(i >= GetCount()) SetCountR(i + 1); return B::Get(i); }
	T&       At(int i, const T& x)      { if(i >= GetCount()) SetCountR(i + 1, x); return B::Get(i); }

	using B::Shrink;
	using B::Reserve;
	using B::GetAlloc;

	void     Set(int i, const T& x, int count) { B::Set(i, x, count); for(int ii = i; ii < i+count; ii++) { LinkChild(B::operator[](ii)); B::operator[](ii).Relink(); } }
	T&       Set(int i, const T& x)     { Set(i, x, 1); return Get(i); }
	T&       Set(int i, T&& x)          { return At(i) = pick(x); }

	using B::Remove;
	void     InsertN(int i, int count = 1) { B::Insert(i, count); for(int ii = i; ii < i+count; ii++) LinkChild(B::operator[](ii)); }
	T&       Insert(int i)              { T& t = B::Insert(i); LinkChild(t); return t; }
	void     Insert(int i, const T& x, int count) { B::Insert(i, x, count); for(int ii = i; ii < i+count; ii++) { LinkChild(B::operator[](ii)); B::operator[](ii).Relink(); } }
	T&       Insert(int i, const T& x)  { Insert(i, x, 1); return Get(i); }
	T&       Insert(int i, T&& x)       { LinkChild(x); x.Relink(); return B::Insert(i, pick(x)); }

	using B::GetIndex; //????
	using B::Swap;
	using B::Move;

	T&       Set(int i, T *newt)        { ASSERT(newt->parent == NULL); T& t = B::Set(i, newt); LinkChild(t); t.Relink(); return t; }
	T&       Insert(int i, T *newt)     { ASSERT(newt->parent == NULL); T& t = B::Insert(i, newt); LinkChild(*newt); t.Relink(); return t; }

	using B::Drop;
	using B::Top;
//	using B::Pop;
	
	T*       Detach(int i)              { T* t = B::Detach(i); Unlink(*t); return t; }
	T*       Swap(int i, T *newt)       { T* t = B::Swap(i, newt); Unlink(*t); newt->Relink(); return t; }
	T*       PopDetach()                { T* t = B::PopDetach(); Unlink(*t); return t; }

	void     Swap(Tree& b)              { B::Swap(b); b.Relink(); Relink(); }

	Tree& operator<<(const T& x)        { Add(x); return *this; }
	Tree& operator<<(T *newt)           { Add(newt); return *this; }

#ifdef UPP
	void     Serialize(Stream& s)       { StreamContainer(s, *this); }
#endif

	Tree()
		: parent(NULL)
		, root(NULL)
	{}

//pick
	Tree(Tree&& v)
		: parent(NULL)
		, root(NULL)
		, B(pick(v))
	{ Relink(); }
	void operator=(Tree&& v)       { (B&)(*this) = pick(v); Relink(); }

// Deep copy
	Tree(const Tree& v, int)
		: parent(NULL)
		, root(NULL)
		, B(v, 0)
	{ Relink(); }

	using B::ConstIterator;
	using B::Iterator;

//	using B::ValueType;
//	using B::Begin;
//	using B::End;
//	using B::GetIter;

	using B::begin;
	using B::end;

// Array Interface end

	friend void Swap(Tree& a, Tree& b)                   { B::Swap(a, b); b.Relink(); a.Relink(); }

public:
#ifdef _DEBUG
	void Dump() {
		for(int i = 0; i < GetCount(); i++)
			LOG((*this)[i]);
		LOG("-------------------------------------");
	}
#endif
};

template<class T>
inline void Xmlize(XmlIO& xml, Tree<T>& data)
{
	XmlizeContainer(xml, "tree", data);
}

template <class T>
inline void DumpContainer(Stream& s, const Tree<T>& c) {
	DumpContainer(s, c.Begin(), c.End());
}

//Tree Node helper class

template<class T>
class Node
	: public Tree<Node<T> >
{
public:
	typedef Node<T> CLASSNAME;
	typedef Tree<Node<T> > R;

	Node() {}
	Node(const T& leaf) : leaf(leaf) {}

	T leaf;
};

template <class T>
inline Stream& operator%(Stream& s, Node<T>& x)
{
	s % x.leaf % (Tree<Node<T> >&)x;
	return s;
}

template<class T>
inline void Xmlize(XmlIO& xml, Node<T>& a)
{
	xml("leaf", a.leaf); Xmlize(xml, (Tree<Node<T> >&)a);
}

template<class BB> //B conflicts with Tree::B
class NodeB
	: public Tree<NodeB<BB> >
	, public BB //leaf
{
public:
	typedef NodeB<BB> CLASSNAME;
	typedef Tree<NodeB<BB> > R;
};

template <class BB>
inline Stream& operator%(Stream& s, NodeB<BB>& x)
{
	s % (BB&)x % (Tree<NodeB<BB> >&)x;
	return s;
}

template<class BB>
inline void Xmlize(XmlIO& xml, NodeB<BB>& a)
{
	xml("leaf", (BB&)a); Xmlize(xml, (Tree<NodeB<BB> >&)a);
}

////

template <class K, class T>
class TreeMap
	: protected ArrayMap<K,T>, MoveableAndDeepCopyOption<TreeMap<T,K> >
{
protected:
	typedef ArrayMap<K,T> B;
	T* parent;
	T* root;

	inline void LinkChild(T& t)         { t.root = root; t.parent = (T *)this; }
	inline void UnlinkChild(T& t)       { t.root = NULL; t.parent = NULL; }
public:
	void Relink() { for(int i = 0; i < B::GetCount(); i++) { T& t = B::operator[](i); LinkChild(t); t.Relink();} }

public:
	T*       GetPtr()                   { return (T*) this; }
	const T* GetPtr() const             { return (const T *) this; }
	T*       GetParent()                { return parent; }
	const T* GetParent() const          { return parent; }
	T*       GetRoot()                  { return root; }
	const T* GetRoot() const            { return root; }
	void SetAsRoot()                    { ASSERT(parent == NULL); root = (T*)this; Relink(); }

// ArrayMap interface

	T&       Add(const K& k, const T& x)           { T& t = B::Add(k, x); LinkChild(t); t.Relink(); return t; }
	T&       Add(const K& k)                       { T& t = B::Add(k); LinkChild(t); return t; }
	T&       Add(const K& k, T&& x)                { T& t = B::Add(k, pick(x)); LinkChild(t); t.Relink(); return t; }
	T&       Add(const K& k, T *newt)              { ASSERT(newt->parent == NULL); T& t = B::Add(k, newt); LinkChild(t); t.Relink(); return t; }

	T&       Add(K&& k, const T& x)                { T& t = B::Add(pick(k), x); LinkChild(t); t.Relink(); return t; }
	T&       Add(K&& k, T&& x)                     { T& t = B::Add(pick(k), pick(x)); LinkChild(t); t.Relink(); return t; }
	T&       Add(K&& k)                            { T& t = B::Add(pick(k)); LinkChild(t); return t; }
	T&       Add(K&& k, T *newt)                   { ASSERT(newt->parent == NULL); T& t = B::Add(pick(k), newt); LinkChild(t); t.Relink(); return t; }

	T&       Insert(int i, const K& k, T *newt)    { T& t = B::Insert(i, k, newt); LinkChild(t); t.Relink(); return t; }
	T&       Insert(int i, K&& k, T *newt)         { T& t = B::Insert(i, pick(k), newt); LinkChild(t); t.Relink(); return t; }

	T&        Set(int i, T *ptr)                   { ASSERT(ptr->parent == NULL); T& t = B::Set(i, ptr); LinkChild(t); t.Relink(); }
	T*        PopDetach()                          { T* t = B::PopDetach(); Unlink(*t); return t; }
	T*        Detach(int i)                        { T* t = B::Detach(i); Unlink(*t); return t; }
	T*        Swap(int i, T *newt)                 { T* t = B::Swap(i, newt); Unlink(*t); newt->Relink(); return t; }

// AMap interface

	using B::Find;
	using B::FindNext;
	using B::FindLast;
	using B::FindPrev;

	int      FindAdd(const K& k)                    { int i = B::FindAdd(k); LinkChild(operator[](i)); return i; }
	int      FindAdd(const K& k, const T& init)     { int i = B::FindAdd(k, init); LinkChild(operator[](i)); operator[](i).Relink(); return i; }
	int      FindAdd(const K& k, T&& init)          { int i = B::FindAdd(k, pick(init)); LinkChild(operator[](i)); operator[](i).Relink(); return i; }
	int      FindAdd(const K&& k)                   { int i = B::FindAdd(pick(k)); LinkChild(operator[](i)); return i; }
	int      FindAdd(const K&& k, const T& init)    { int i = B::FindAdd(pick(k), init); LinkChild(operator[](i)); operator[](i).Relink(); return i; }
	int      FindAdd(const K&& k, T&& init)         { int i = B::FindAdd(pick(k), pick(init)); LinkChild(operator[](i)); operator[](i).Relink(); return i; }

	T&       Put(const K& k)                        { T& t = B::Put(k); LinkChild(t); return t; }
	T&       Put(K&& k)                             { T& t = B::Put(pick(k)); LinkChild(t); return t; }

	int      Put(const K& k, const T& x)            { int i = B::Put(k, x); LinkChild(operator[](i)); operator[](i).Relink(); return i; }
	int      Put(const K& k, T&& x)                 { int i = B::Put(k, pick(x)); LinkChild(operator[](i)); operator[](i).Relink(); return i; }
	int      Put(K&& k, const T& x)                 { int i = B::Put(pick(k), x); LinkChild(operator[](i)); operator[](i).Relink(); return i; }
	int      Put(K&& k, T&& x)                      { int i = B::Put(pick(k), pick(x)); LinkChild(operator[](i)); operator[](i).Relink(); return i; }

	int      PutDefault(const K& k)                 { int i = B::PutDefault(k); LinkChild(operator[](i)); operator[](i).Relink(); return i; }
	int      PutDefault(K&& k)                      { int i = B::PutDefault(pick(k)); LinkChild(operator[](i)); operator[](i).Relink(); return i; }

	int      FindPut(const K& k)                    { int i = B::FindPut(k); LinkChild(operator[](i)); return i; }
	int      FindPut(const K& k, const T& init)     { int i = B::FindPut(k, init); LinkChild(operator[](i)); operator[](i).Relink(); return i; }
	int      FindPut(const K& k, T&& init)          { int i = B::FindPut(k, pick(init)); LinkChild(operator[](i)); operator[](i).Relink(); return i; }
	int      FindPut(K&& k)                         { int i = B::FindPut(pick(k)); LinkChild(operator[](i)); return i; }
	int      FindPut(K&& k, const T& init)          { int i = B::FindPut(pick(k), init); LinkChild(operator[](i)); operator[](i).Relink(); return i; }
	int      FindPut(K&& k, T&& init)               { int i = B::FindPut(pick(k), pick(init)); LinkChild(operator[](i)); operator[](i).Relink(); return i; }

	using B::Get;

	T&       GetAdd(const K& k)                     { T& t = B::GetAdd(k); LinkChild(t); return t; }
	T&       GetAdd(const K& k, const T& x)         { T& t = B::GetAdd(k, x); LinkChild(t); t.Relink(); return t; }
	T&       GetAdd(const K& k, T&& x)              { T& t = B::GetAdd(k, pick(x)); LinkChild(t); t.Relink(); return t; }
	T&       GetAdd(K&& k)                          { T& t = B::GetAdd(pick(k)); LinkChild(t); return t; }
	T&       GetAdd(K&& k, const T& x)              { T& t = B::GetAdd(pick(k), x); LinkChild(t); t.Relink(); return t; }
	T&       GetAdd(K&& k, T&& x)                   { T& t = B::GetAdd(pick(k), pick(x)); LinkChild(t); t.Relink(); return t; }

	T&       GetPut(const K& k)                     { T& t = B::GetPut(k); LinkChild(t); return t; }
	T&       GetPut(const K& k, const T& x)         { T& t = B::GetPut(k, x); LinkChild(t); t.Relink(); return t; }
	T&       GetPut(const K& k, T&& x)              { T& t = B::GetPut(k, pick(x)); LinkChild(t); t.Relink(); return t; }
	T&       GetPut(K&& k)                          { T& t = B::GetPut(pick(k)); LinkChild(t); return t; }
	T&       GetPut(K&& k, const T& x)              { T& t = B::GetPut(pick(k), x); LinkChild(t); t.Relink(); return t; }
	T&       GetPut(K&& k, T&& x)                   { T& t = B::GetPut(pick(k), pick(x)); LinkChild(t); t.Relink(); return t; }

	using B::SetKey;
	using B::FindPtr;
	using B::FindLastPtr;
	using B::Unlink;
	using B::UnlinkKey;
	using B::IsUnlinked;
	using B::Sweep;
	using B::HasUnlinked;

	T&       Insert(int i, const K& k)             { T& t = B::Insert(i, k); LinkChild(t); return t; }
	T&       Insert(int i, const K& k, const T& x) { T& t = B::Insert(i, k, x); LinkChild(t); t.Relink(); return t; }
	T&       Insert(int i, const K& k, T&& x)      { T& t = B::Insert(i, k, pick(x)); LinkChild(t); t.Relink(); return t; }
	T&       Insert(int i, K&& k)                  { T& t = B::Insert(i, pick(k)); LinkChild(t); return t; }
	T&       Insert(int i, K&& k, const T& x)      { T& t = B::Insert(i, pick(k), x); LinkChild(t); t.Relink(); return t; }
	T&       Insert(int i, K&& k, T&& x)           { T& t = B::Insert(i, pick(k), pick(x)); LinkChild(t); t.Relink(); return t; }

	using B::Remove;
	using B::RemoveKey;

	using B::operator[];
	using B::GetCount;
	using B::IsEmpty;
	using B::Clear;
	using B::Shrink;
	using B::Reserve;
	using B::GetAlloc;

	using B::GetHash;

	using B::Drop;
	using B::Top;
	using B::TopKey;
//	using B::Pop;
	using B::PopKey;
	using B::Trim;
	using B::GetKey;
#ifdef UPP
	void Serialize(Stream& s) {
		int version = 0;
		s / version % B::key % B::value;
		for(int i = 0; i < B::GetCount(); i++)
		{
			T& t = B::operator[](i); DoLink(t);
			//t.Relink(); //serialize will recurse
		}
	}

#endif

	void     Swap(TreeMap& b)           { B::Swap(b); b.Relink(); Relink(); }

	using B::GetIndex;
	using B::PickIndex;

	using B::GetKeys;
	using B::PickKeys;

	using B::GetValues;
	Vector<T> PickValues()              { Vector<T> v = B::PickValues(); for(int i = 0; i < v.GetCount(); i++) { v[i].SetAsRoot(); v[i].Relink(); } return v; }

	using B::begin;
	using B::end;
	
//	using B::KeyType;
//	using B::KeyConstIterator;

//	using B::KeyBegin;
//	using B::KeyEnd;
//	using B::KeyGetIter;
	
//	using B::ValueType;
//	using B::ConstIterator;
//	using B::Iterator;

//	using B::Begin;
//	using B::End;
//	using B::GetIter;

// ArrayMap interface



	template <class TT> TT& CreateB(const K& k)    { TT *q = B::Create(k); LinkChild(q); return *q; }


	TreeMap(const TreeMap& s, int)
		: parent(NULL)
		, root(NULL)
		, B(s, 1)
	{ Relink(); }
#if 0
	ArrayMap(pick_ Index<K, HashFn>& ndx, pick_ Array<T>& val) : AMap<K, T, Array<T>, HashFn>(ndx, val) {}
	ArrayMap(pick_ Vector<K>& ndx, pick_ Array<T>& val) : AMap<K, T, Array<T>, HashFn>(ndx, val) {}
#endif
	TreeMap()
		: parent(NULL)
		, root(NULL)
	{}
	
	friend void     Swap(TreeMap& a, TreeMap& b)   { a.B::Swap(b); b.Relink(); a.Relink(); }
};

template<class K, class T, class H>
inline void Xmlize(XmlIO& xml, TreeMap<K, T>& data)
{
	XmlizeMap<K, T>(xml, "tkey", "tvalue", data);
}

template <class K, class T>
inline void DumpContainer(Stream& s, const TreeMap<K, T>& c) {
	DumpContainer(s, c.Begin(), c.End());
}

template<class K, class T>
class MapNode
	: public TreeMap<K, MapNode<K,T> >
{
public:
	typedef MapNode<K, T> CLASSNAME;
	typedef TreeMap<K, MapNode<K,T> > R;

	MapNode() {}
	MapNode(const T& leaf) : leaf(leaf) {}

	T leaf;
};

template <class K, class T>
inline Stream& operator%(Stream& s, MapNode<K, T>& x)
{
	s % x.leaf % (TreeMap<K, MapNode<K,T> >&)x;
	return s;
}

template<class K, class T>
inline void Xmlize(XmlIO& xml, MapNode<K, T>& a)
{
	xml("leaf", a.leaf); Xmlize(xml, (TreeMap<K, MapNode<K,T> >&)a);
}

template<class K, class BB> //B conflicts with Tree::B
class MapNodeB
	: public TreeMap<K, MapNodeB<K, BB> >
	, public BB //leaf
{
public:
	typedef MapNodeB<K, BB> CLASSNAME;
	typedef TreeMap<K, MapNodeB<K,BB> > R;
};

template <class K, class BB>
inline Stream& operator%(Stream& s, MapNodeB<K, BB>& x)
{
	s % (BB&)x % (TreeMap<K, MapNodeB<K,BB> >&)x;
	return s;
}

template<class K, class BB>
inline void Xmlize(XmlIO& xml, MapNodeB<K, BB>& a)
{
	xml("leaf", (BB&)a); Xmlize(xml, (TreeMap<K, MapNodeB<K,BB> >&)a);
}

END_UPP_NAMESPACE

#endif
