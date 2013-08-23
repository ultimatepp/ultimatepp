// vi: noexpandtab:tabstop=4

#ifndef _ADT_List_h
#define _ADT_List_h

template<class T>
struct SListNode {
	SListNode() : next(NULL) {}

	T* GetNext() const { return next; }
	void SetNext(T* other) { next = other; }

	// Return removed node.	
	T* RemoveNext() const {
		T* n = GetNext();
		SetNext(n->GetNext());
		return n;
	}
	
	void InsertAfterThis(T* first, T* last) {
		last->SetNext(next);
		SetNext(first);
	}
	void InsertAfterThis(T* other) {
		InsertAfterThis(other, other);
	}

private:
	T* next;
};

template<class T>
struct DListNode {
	DListNode() : prev(NULL), next(NULL) {}
	
	T* GetPrev() const { return prev; }
	T* GetNext() const { return next; }
	
	void SetPrev(T* other) { prev = other; }
	void SetNext(T* other) { next = other; }
	
	void RemoveThis() const {
		GetPrev()->SetNext(next);
		GetNext()->SetPrev(prev);
	}
	// Return removed node.	
	T* RemoveNext() {
		T* n = GetNext();
		n->RemoveThis();
		return n;
	}
	// Return removed node.	
	T* RemovePrev() {
		T* n = GetPrev();
		n->RemoveThis();
		return n;
	}
	
	void ReplaceThisWith(T* first, T* last) const {
		GetPrev()->SetNext(first);
		GetNext()->SetPrev(last);
		
		first->SetPrev(prev);
		last->SetNext(next);
	}
	void ReplaceThisWith(T* other) const {
		ReplaceThisWith(other, other);
	}
	
	void InsertAfterThis(T* first, T* last) {
		first->SetPrev(next->prev);
		GetNext()->SetPrev(last);
		
		// Code below is from SListNode::InsertAfterThis().
		last->SetNext(next);
		SetNext(first);
	}
	void InsertAfterThis(T* other) {
		InsertAfterThis(other, other);
	}
	
	void InsertBeforeThis(T* first, T* last) {
		last->SetNext(next->prev);
		first->SetPrev(prev);
		
		GetPrev()->SetNext(first);
		SetPrev(last);
	}
	void InsertBeforeThis(T* other) {
		InsertBeforeThis(other, other);
	}

private:
	T* prev;
	T* next;
};

// Intrusive single-linked list with sentinel.
// Names PushFront and PopFront are kept for compatibility with DList.
template<class T>
struct SList {
	typedef SList<T> SelfType;
	
	SList() {
		Clear();
	}
	
	void Clear() {
		sentinel.SetNext((T*)&sentinel);
	}

	T* GetFirst() const    	  { return (T*)sentinel.GetNext(); }
	T* GetEnd  () const       { return (T*)&sentinel; }

	bool IsEmpty()              const { return sentinel.GetNext() == (const T*)&sentinel; }
	bool IsFirst(const T* node) const { return node == sentinel.GetNext(); }
	bool IsNull (const T* node) const { return node == (const T*)&sentinel; }
	
	SelfType& PushFront(T* first, T* last) {
		sentinel.InsertAfterThis(first, last);
		return *this;
	}
	
	SelfType& PushFront(T* node) {
		return PushFront(node, node);
	}
	
	T* PopFront() const {
		ASSERT(!IsEmpty());
		return sentinel.RemoveNext();
	}
	///
	SelfType& operator <<(T& n) { return PushFront(&n); }

	///
	struct ForwardRange {
		typedef T ValueType;
		
		ForwardRange(const SList<T>& l) : list(&l), n(const_cast<T*>(l.GetFirst())) {}
		
		bool IsEmpty() const { return list->IsNull(n); }
		void PopFront() { ASSERT(!IsEmpty()); n = n->GetNext(); }
		T& Front() const { return *n; }
		
	protected:
		const SList<T>* list;
		T*              n;
	};
	
	ForwardRange ForwardR() const { return ForwardRange(*this); }
	
private:
	SListNode<T> sentinel;
};

// Intrusive double-linked list with sentinel.
template<class T>
struct DList {
	typedef DList<T> SelfType;
	
	DList() {
		Clear();
	}
	
	void Clear() {
		sentinel.SetNext((T*)&sentinel);
		sentinel.SetPrev((T*)&sentinel);
	}
	
	T* GetFirst() const    	  { return (T*)sentinel.GetNext(); }
	T* GetLast () const       { return (T*)sentinel.GetPrev(); }
	T* GetEnd  () const       { return (T*)&sentinel; }
	
	bool IsEmpty()              const { return sentinel.GetNext() == (const T*)&sentinel; }
	bool IsFirst(const T* node) const { return node == sentinel.GetNext(); }
	bool IsLast (const T* node) const { return node == sentinel.GetPrev(); }
	bool IsNull (const T* node) const { return node == (const T*)&sentinel; }
	
	SelfType& PushFront(T* first, T* last) {
		sentinel.InsertAfterThis(first, last);
		return *this;
	}
	
	SelfType& PushBack(T* first, T* last) {
		sentinel.InsertBeforeThis(first, last);
		return *this;
	}
	
	SelfType& PushFront(T* node) {
		return PushFront(node, node);
	}
	
	SelfType& PushBack(T* node) {
		return PushBack(node, node);
	}
	
	T* PopFront() const {
		return Remove(GetFirst());
	}
	
	T* PopBack() const {
		return Remove(GetLast());
	}
	
	static T* Remove(T* node) {
		node->Remove();
		return node;
	}
	
	SelfType& PushFront(DList<T>& other) {
		if (!other.IsEmpty())
			PushFront(other.GetFirst(), other.GetLast());
		return *this;
	}
	void PickPushFront(DList<T>& other) {
		PushFront(other);
		other.Clear();
	}
	
	SelfType& PushBack(DList<T>& other) {
		if (!other.IsEmpty())
			PushBack(other.GetFirst(), other.GetLast());
		return *this;
	}
	void PickPushBack(DList<T>& other) {
		PushBack(other);
		other.Clear();
	}

	///
	SelfType& operator <<(T& n) { return PushBack(&n); }

	///
	struct ForwardRange {
		typedef T ValueType;
		
		ForwardRange(const DList<T>& l) : list(&l), n(const_cast<T*>(l.GetFirst())) {}
		
		bool IsEmpty() const { return list->IsNull(n); }
		void PopFront() { ASSERT(!IsEmpty()); n = n->GetNext(); }
		T& Front() const { return *n; }
		
	protected:
		const DList<T>* list;
		T*              n;
	};
	
	ForwardRange ForwardR() const { return ForwardRange(*this); }

private:
	DListNode<T> sentinel;
};

#endif
