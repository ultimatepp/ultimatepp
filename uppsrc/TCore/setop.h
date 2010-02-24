NAMESPACE_UPP

enum SetOpCode
{
	SET_OP2_NOP,   // empty set
	SET_OP2_LEFT,  // left set
	SET_OP2_RIGHT, // right set
	SET_OP2_LADD,  // left addition - duplicates are kept in left set
	SET_OP2_RADD,  // right additions - duplicates are kept in right set
	SET_OP2_AND,   // intersection (left set order)
	SET_OP2_LSUB,  // left subtraction (left - right)
	SET_OP2_RSUB,  // right subtraction (right - left)
	SET_OP2_XOR,   // symmetrical difference
};

template <> inline unsigned GetHashValue(const SetOpCode& c) { return c; }

NTL_MOVEABLE(SetOpCode);

template<class V, class I>
Index<V> SetGetIndex(I begin, I end)
{
	Index<V> result;
	while(begin != end)
		result.FindAdd(*begin++);
	return result;
}

template <class C, class I>
C SetLAdd(I begin1, I end1, I begin2, I end2)
{
	typedef typename C::ValueType V;
	C result;
	if(begin1 == end1) // left set is empty
		Append(result, begin2, end2);
	else
	{
		Append(result, begin1, end1);
		if(begin2 != end2)
		{
			Index<V> hash = SetGetIndex<V>(begin1, end1);
			for(; begin2 != end2; begin2++)
				if(hash.Find(*begin2) < 0)
					result.Add(*begin2);
		}
	}
	return result;
}

template <class C, class I>
C SetRAdd(I begin1, I end1, I begin2, I end2)
{
	typedef typename C::ValueType V;
	C result;
	if(begin2 == end2)
		Append(result, begin1, end1);
	else
	{
		if(begin1 != end1)
		{
			Index<V> hash = SetGetIndex<V>(begin2, end2);
			while(begin1 != end1)
			{
				if(hash.Find(*begin1) < 0)
					result.Add(*begin1);
				begin1++;
			}
		}
		Append(result, begin2, end2);
	}
	return result;
}

template <class C, class I>
C SetAnd(I begin1, I end1, I begin2, I end2)
{
	typedef typename C::ValueType V;
	C result;
	int count1 = end1 - begin1;
	int count2 = end2 - begin2;
	if(count1 == 0 || count2 == 0)
		return result; // empty intersection
	if(count1 > count2)
	{ // 1st is longer, generate hash on 2nd
		Index<V> hash = SetGetIndex<V>(begin2, end2);
		while(begin1 != end1)
		{
			if(hash.Find(*begin1) >= 0)
				result.Add(*begin1);
			begin1++;
		}
	}
	else
	{ // 1st is shorter, generate hash on 1st
		Index<V> hash = SetGetIndex<V>(begin1, end1);
		while(begin2 != end2)
		{
			if(hash.Find(*begin2) >= 0)
				result.Add(*begin2);
			begin2++;
		}
	}
	return result;
}

template <class C, class I>
C SetSub(I begin1, I end1, I begin2, I end2)
{
	typedef typename C::ValueType V;
	C result;
	if(begin1 == end1)
		; // no-op - empty set
	else if(begin2 == end2)
		Append(result, begin1, end1);
	else
	{
		Index<V> hash = SetGetIndex<V>(begin2, end2);
		while(begin1 != end1)
		{
			if(hash.Find(*begin1) < 0)
				result.Add(*begin1);
			begin1++;
		}
	}
	return result;
}

template <class C, class I>
C SetXor(I begin1, I end1, I begin2, I end2)
{
	typedef typename C::ValueType V;
	C result;
	if(begin1 == end1)
		Append(result, begin2, end2);
	else if(begin2 == end2)
		Append(result, begin1, end1);
	else
	{
		Index<V> hash = SetGetIndex<V>(begin2, end2);
		for(I temp = begin1; temp != end1; temp++)
			if(hash.Find(*temp) < 0)
				result.Add(*temp);
		hash = SetGetIndex<V>(begin1, end1);
		while(begin2 != end2)
		{
			if(hash.Find(*begin2) < 0)
				result.Add(*begin2);
			begin2++;
		}
	}
	return result;
}

template <class C, class I>
C SetOperation(I begin1, I end1, I begin2, I end2, SetOpCode op)
{
	C result;
	switch(op)
	{
	default: NEVER();
	case SET_OP2_NOP:    break;
	case SET_OP2_LEFT:   Append(result, begin1, end1); break;
	case SET_OP2_RIGHT:  Append(result, begin2, end2); break;
	case SET_OP2_LADD:   return SetLAdd<C>(begin1, end1, begin2, end2);
	case SET_OP2_RADD:   return SetRAdd<C>(begin1, end1, begin2, end2);
	case SET_OP2_AND:    return SetAnd<C>(begin1, end1, begin2, end2);
	case SET_OP2_LSUB:   return SetSub<C>(begin1, end1, begin2, end2);
	case SET_OP2_RSUB:   return SetSub<C>(begin2, end2, begin1, end1);
	case SET_OP2_XOR:    return SetXor<C>(begin1, end1, begin2, end2);
	}
	return result;
}

template <class T>
T SetOperation(const T& set1, const T& set2, SetOpCode op)
{
	return SetOperation<T>(set1.Begin(), set1.End(), set2.Begin(), set2.End(), op);
}

template <class T> T SetLAdd(const T& set1, const T& set2) { return SetLAdd<T>(set1.Begin(), set1.End(), set2.Begin(), set2.End()); }
template <class T> T SetRAdd(const T& set1, const T& set2) { return SetRAdd<T>(set1.Begin(), set1.End(), set2.Begin(), set2.End()); }
template <class T> T SetAnd (const T& set1, const T& set2) { return SetAnd<T>(set1.Begin(), set1.End(), set2.Begin(), set2.End());  }
template <class T> T SetSub (const T& set1, const T& set2) { return SetSub<T>(set1.Begin(), set1.End(), set2.Begin(), set2.End()); }
template <class T> T SetXor (const T& set1, const T& set2) { return SetXor<T>(set1.Begin(), set1.End(), set2.Begin(), set2.End());  }

END_UPP_NAMESPACE
