#ifndef _Tree_Tree_h
#define _Tree_Tree_h

#include <Core/Core.h>
using namespace Upp;

template <class T>
class Tree 
	: protected Array<T>
{
protected:
	typedef Array<T> B;
	T * parent;
	T * root;

	inline void Link(T & t)             { t.root = root; t.parent = (T *)this; }
	inline void Unlink(T & t)           { t.root = NULL; t.parent = NULL; }

	void Relink() { for(int i = 0; i < B::GetCount(); i++) { T & t = B::operator[](i); Link(t); t.Relink();} }

public:
	T       *GetPtr()                   { return (T *) this; }
	const T *GetPtr() const             { return (const T *) this; }
	T       *GetParent()                { return parent; }
	const T *GetParent() const          { return parent; }
	T       *GetRoot()                  { return root; }
	const T *GetRoot() const            { return root; }
	void SetAsRoot()                    { ASSERT(parent == NULL); root = (T *)this; Relink(); }

// Array interface

	T&       Add()                      { T & t = B::Add(); Link(t); return t; }
	void     Add(const T& x)            { T & t = B::Add(DeepCopyNew(x)); Link(t); }// return t;  }
	void     AddPick(pick_ T& x)        { T & t = B::Add(new T(x)); Link(t); }// return t; }
	T&       Add(T *newt)               { ASSERT(newt->parent == NULL); T & t = B::Add(newt); Link(t); return t; }

	using B::operator[];
	using B::GetCount;
	using B::IsEmpty;

	using B::Trim;
	void     SetCount(int n)            { B::SetCount(n); for(int i = 0; i < B::GetCount(); i++) Link(B::operator[](i)); }
	void     SetCountR(int n)           { B::SetCountR(n); for(int i = 0; i < B::GetCount(); i++) Link(B::operator[](i)); }
	using B::Clear;

	T&       At(int i)                  { if(i >= GetCount()) SetCountR(i + 1); return B::Get(i); }
	T&       At(int i, const T& x)      { if(i >= GetCount()) SetCountR(i + 1, x); return B::Get(i); }

	using B::Remove;
	T&       Insert(int i)              { T & t = B::Insert(i); Link(t); return t; }
	void     InsertPick(int i, pick_ T& x) { Link(x); B::InsertPick(i, x); }

	using B::GetIndex;
	using B::Swap;
	using B::Move;

	T       *Detach(int i)              { T *t = B::Detach(i); Unlink(t); }
	T&       Set(int i, T *newt)        { ASSERT(newt->parent == NULL); T & t = B::Set(i, newt); Link(t); return t; }
	void     Insert(int i, T *newt)     { ASSERT(newt->parent == NULL); B::Insert(i, newt); Link(*newt); }

	using B::Drop;
	using B::Top;
	
	T       *PopDetach()                { T * t = B::PopDetach(); Unlink(*t); return t; }

	void     Swap(Tree& b)              { B::Swap(b); for(int i = 0; i < B::GetCount(); i++) Link(B::operator[](i)); for(int i = 0; i < b.GetCount(); i++) b.Link(b[i]); }

	Tree& operator<<(const T& x)        { Add(x); return *this; }
	Tree& operator<<(T *newt)           { Add(newt); return *this; }
	Tree& operator|(pick_ T& x)         { AddPick(x); return *this; }

#ifdef UPP
	//void     Serialize(Stream& s)       { B::Serialize(); }
	using B::Serialize;
#endif

// Array Interface end

	Tree()
		: parent(NULL)
		, root(NULL)
	{}

private:
	Tree(const Tree&);
	void operator=(const Tree&);

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
void Xmlize(XmlIO xml, Tree<T>& data)
{
	XmlizeContainer(xml, "item", data);
}

//Tree Node helper class

template<class T>
class Node
	: public Tree<Node<T> >
{
public:
	typedef Node CLASSNAME;
	Node() {}
	Node(const T& leaf) : leaf(leaf) {}

	void Xmlize(XmlIO xml) { xml("leaf", leaf); XmlizeContainer(xml, "item", (Tree<Node<T> >&)*this); }
	void Serialize(Stream& s) { s % leaf % (Tree<Node<T> >&)*this; }

	T leaf;
};

template<class BB> //B conflicts with Tree::B
class Leaf
	: public Tree<Leaf<BB> >
	, public BB
{
public:
	typedef Leaf<BB> CLASSNAME;
	void Xmlize(XmlIO xml) { xml("leaf", (BB&)*this); XmlizeContainer(xml, "item", (Tree<Leaf<BB> >&)*this); }
	void Serialize(Stream& s) { s % (BB&)*this % (Tree<Leaf<BB> >&)*this; }
};

#endif
