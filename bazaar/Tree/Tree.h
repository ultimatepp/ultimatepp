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

	inline void DoLink(T& t)             { t.root = root; t.parent = (T *)this; }
	inline void Unlink(T& t)           { t.root = NULL; t.parent = NULL; }
public:
	void Relink() { for(int i = 0; i < B::GetCount(); i++) { T& t = B::operator[](i); DoLink(t); t.Relink();} }

public:
	T*       GetPtr()                   { return (T*) this; }
	const T* GetPtr() const             { return (const T *) this; }
	T*       GetParent()                { return parent; }
	const T* GetParent() const          { return parent; }
	T*       GetRoot()                  { return root; }
	const T* GetRoot() const            { return root; }
	void     SetAsRoot()                { ASSERT(parent == NULL); root = (T*)this; Relink(); }

// Array interface

	T&       Add()                      { T& t = B::Add(); DoLink(t); return t; }
	T&       Add(const T& x)            { T& t = B::Add(x); DoLink(t); t.Relink(); return t; }
	T&       AddPick(pick_ T& x)        { T& t = B::Add(x); DoLink(t); t.Relink(); return t; }
	T&       Add(T *newt)               { ASSERT(newt->parent == NULL); T& t = B::Add(newt); DoLink(t); t.Relink(); return t; }

	using B::operator[];
	using B::GetCount;
	using B::IsEmpty;

	using B::Trim;
	void     SetCount(int n)            { B::SetCount(n); for(int i = 0; i < B::GetCount(); i++) DoLink(B::operator[](i)); }
	void     SetCountR(int n)           { B::SetCountR(n); for(int i = 0; i < B::GetCount(); i++) DoLink(B::operator[](i)); }
	using B::Clear;

	T&       At(int i)                  { if(i >= GetCount()) SetCountR(i + 1); return B::Get(i); }
	T&       At(int i, const T& x)      { if(i >= GetCount()) SetCountR(i + 1, x); return B::Get(i); }

	using B::Remove;
	T&       Insert(int i)              { T& t = B::Insert(i); DoLink(t); return t; }
	void     InsertPick(int i, pick_ T& x) { DoLink(x); B::InsertPick(i, x); }

	using B::GetIndex;
	using B::Swap;
	using B::Move;

	T*       Detach(int i)              { T* t = B::Detach(i); Unlink(*t); return t;}
	T&       Set(int i, T *newt)        { ASSERT(newt->parent == NULL); T& t = B::Set(i, newt); DoLink(t); return t; }
	void     Insert(int i, T *newt)     { ASSERT(newt->parent == NULL); B::Insert(i, newt); DoLink(*newt); }

	using B::Drop;
	using B::Top;
//	using B::Pop;
	
	T*       PopDetach()                { T* t = B::PopDetach(); Unlink(*t); return t; }

	void     Swap(Tree& b)              { B::Swap(b); for(int i = 0; i < B::GetCount(); i++) DoLink(B::operator[](i)); for(int i = 0; i < b.GetCount(); i++) b.DoLink(b[i]); }

	Tree& operator<<(const T& x)        { Add(x); return *this; }
	Tree& operator<<(T *newt)           { Add(newt); return *this; }
	Tree& operator|(pick_ T& x)         { AddPick(x); return *this; }

	using B::IsPicked;

#ifdef UPP
	void     Serialize(Stream& s)       { StreamContainer(s, *this); }
#endif

	Tree()
		: parent(NULL)
		, root(NULL)
	{}

//pick
	Tree(pick_ Tree& v) 
		: parent(NULL)
		, root(NULL)
		, B(v)
	{ Relink(); }
	void operator=(pick_ Tree& v)       { (B&)(*this) = v; Relink(); }

// Deep copy
	Tree(const Tree& v, int) 
		: parent(NULL)
		, root(NULL)
		, B(v, 0)
	{ Relink(); }

	using B::ConstIterator;
	using B::Iterator;

	using B::ValueType;
	using B::Begin;
	using B::End;
	using B::GetIter;

// Array Interface end

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

template <class K, class T, class HashFn = StdHash<K> >
class TreeMap
	: protected ArrayMap<K,T,HashFn>, MoveableAndDeepCopyOption<TreeMap<T,K,HashFn> >
{
protected:
	typedef ArrayMap<K,T,HashFn> B;
	T* parent;
	T* root;

	inline void DoLink(T& t)             { t.root = root; t.parent = (T *)this; }
	inline void Unlink(T& t)           { t.root = NULL; t.parent = NULL; }
public:
	void Relink() { for(int i = 0; i < B::GetCount(); i++) { T& t = B::operator[](i); DoLink(t); t.Relink();} }

public:
	T*       GetPtr()                   { return (T*) this; }
	const T* GetPtr() const             { return (const T *) this; }
	T*       GetParent()                { return parent; }
	const T* GetParent() const          { return parent; }
	T*       GetRoot()                  { return root; }
	const T* GetRoot() const            { return root; }
	void SetAsRoot()                    { ASSERT(parent == NULL); root = (T*)this; Relink(); }

// AMap interface

	T&       Add(const K& k, const T& x)            { T& t = B::Add(k, x); DoLink(t); t.Relink(); return t; }
	T&       Add(const K& k)                        { T& t = B::Add(k); DoLink(t); return t; }
	T&       AddPick(const K& k, pick_ T& x)        { T& t = B::AddPick(); DoLink(t); t.Relink(); return t; }

	using B::Find;
	using B::FindNext;
	using B::FindLast;
	using B::FindPrev;

	int      FindAdd(const K& k)                    { int i = B::FindAdd(k); DoLink(operator[](i)); return i; }
	int      FindAdd(const K& k, const T& init)     { int i = B::FindAdd(k, init); DoLink(operator[](i)); return i; }
	int      FindAddPick(const K& k, pick_ T& init) { int i = B::FindAddPick(k, init); DoLink(operator[](i)); return i; }

	int      Put(const K& k, const T& x)            { int i = B::Put(k, x); DoLink(operator[](i)); return i; }
	int      PutPick(const K& k, pick_ T& x)        { int i = B::PutPick(k, x); DoLink(operator[](i)); return i; }
	T&       Put(const K& k)                        { T& t = B::Put(k); DoLink(t); return t; }

	int      FindPut(const K& k)                    { int i = B::FindPut(k); DoLink(operator[](i)); return i; }
	int      FindPut(const K& k, const T& init)     { int i = B::FindPut(k, init); DoLink(operator[](i)); return i; }
	int      FindPutPick(const K& k, pick_ T& init) { int i = B::FindPutPick(k, init); DoLink(operator[](i)); return i; }

	using B::Get;

	T&       GetAdd(const K& k)                     {T& t = B::GetAdd(k); DoLink(t); return t; }

	T&       GetAdd(const K& k, const T& x)         {T& t = B::GetAdd(k, x); DoLink(t); return t; }
	T&       GetAddPick(const K& k, pick_ T& x)     {T& t = B::GetAddPick(k, x); DoLink(t); return t; }

	T&       GetPut(const K& k)                     {T& t = B::GetPut(k); DoLink(t); return t; }

	T&       GetPut(const K& k, const T& x)         {T& t = B::GetPut(k, x); DoLink(t); return t; }
	T&       GetPutPick(const K& k, pick_ T& x)     {T& t = B::GetPutPick(k, x); DoLink(t); return t; }

	using B::SetKey;
	using B::FindPtr;
	using B::Unlink;
	using B::UnlinkKey;
	using B::IsUnlinked;
	using B::Sweep;

	T&       Insert(int i, const K& k)             { T& t = B::Insert(i, k); DoLink(t); return t; }
	T&       Insert(int i, const K& k, const T& x) { T& t = B::Insert(i, k, x); DoLink(t); return t; }

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

	void     Swap(TreeMap& b)           { B::Swap(b); for(int i = 0; i < B::GetCount(); i++) DoLink(B::operator[](i)); for(int i = 0; i < b.GetCount(); i++) b.DoLink(b[i]); }

	using B::GetIndex;
	using B::PickIndex;

	using B::GetKeys;
	using B::PickKeys;

	using B::GetValues;
	Vector<T> PickValues() pick_        { Vector<T> v = B::PickValues(); for(int i = 0; i < v.GetCount(); i++) v[i].SetAsRoot(); return v; }

	using B::IsPicked;

	using B::KeyType;
	using B::KeyConstIterator;

	using B::KeyBegin;
	using B::KeyEnd;
	using B::KeyGetIter;
	
	using B::ValueType;
	using B::ConstIterator;
	using B::Iterator;

	using B::Begin;
	using B::End;
	using B::GetIter;

// ArrayMap interface

	T&        Add(const K& k, T *newt)             { T& t = B::Add(k, newt); DoLink(t); t.Relink(); return t; }
	template <class TT> TT& CreateB(const K& k)    { TT *q = B::Create(k); DoLink(q); return *q; }

	T&        Set(int i, T *ptr)                   { ASSERT(ptr->parent == NULL); T& t = B::Set(i, ptr); DoLink(t); return t; }
	T*        PopDetach()                          { T* t = B::PopDetach(); Unlink(*t); return t; }
	T*        Detach(int i)                        { T* t = B::Detach(i); Unlink(*t); return t;}

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
	friend void     Swap(TreeMap& a, TreeMap& b)   { a.B::Swap(b); for(int i = 0; i < a.GetCount(); i++) DoLink(a[i]); for(int i = 0; i < b.GetCount(); i++) b.DoLink(b[i]); }
};

template<class K, class T, class H>
inline void Xmlize(XmlIO& xml, TreeMap<K, T, H>& data)
{
	XmlizeMap<K, T>(xml, "tkey", "tvalue", data);
}

template <class K, class T, class H>
inline void DumpContainer(Stream& s, const TreeMap<K, T, H>& c) {
	DumpContainer(s, c.Begin(), c.End());
}

template<class K, class T, class H = StdHash<K> >
class MapNode
	: public TreeMap<K, MapNode<K,T,H>, H>
{
public:
	typedef MapNode<K, T, H> CLASSNAME;
	typedef TreeMap<K, MapNode<K,T,H>, H> R;

	MapNode() {}
	MapNode(const T& leaf) : leaf(leaf) {}

	T leaf;
};

template <class K, class T, class H>
inline Stream& operator%(Stream& s, MapNode<K, T, H>& x)
{
	s % x.leaf % (TreeMap<K, MapNode<K,T,H>, H>&)x;
	return s;
}

template<class K, class T, class H>
inline void Xmlize(XmlIO& xml, MapNode<K, T, H>& a)
{
	xml("leaf", a.leaf); Xmlize(xml, (TreeMap<K, MapNode<K,T,H>, H>&)a);
}

template<class K, class BB, class H = StdHash<K> > //B conflicts with Tree::B
class MapNodeB
	: public TreeMap<K, MapNodeB<K, BB, H>, H>
	, public BB //leaf
{
public:
	typedef MapNodeB<K, BB, H> CLASSNAME;
	typedef TreeMap<MapNodeB<K,BB,H>, H> R;
};

template <class K, class BB, class H>
inline Stream& operator%(Stream& s, MapNodeB<K, BB, H>& x)
{
	s % (BB&)x % (TreeMap<K, MapNodeB<K,BB,H>, H>&)x;
	return s;
}

template<class K, class BB, class H>
inline void Xmlize(XmlIO& xml, MapNodeB<K, BB, H>& a)
{
	xml("leaf", (BB&)a); Xmlize(xml, (TreeMap<K, MapNodeB<K,BB,H>, H>&)a);
}

END_UPP_NAMESPACE

#endif
