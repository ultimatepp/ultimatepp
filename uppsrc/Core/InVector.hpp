#define LLOG(x)   // DLOG(x)

int64 NewInVectorSerial();

template <class T>
void InVector<T>::Init()
{
	serial = NewInVectorSerial();
	slave = 0;
	Reset();
}

template <class T>
InVector<T>::InVector()
{
	Init();
}

template <class T>
void InVector<T>::Reset()
{
	hcount = count = 0;
	SetBlkPar();
}

template <class T>
void InVector<T>::Clear()
{
	if(slave)
		Slave()->Clear();
	data.Clear();
	index.Clear();
	Reset();
	ClearCache();
}

void SetInvectorCache__(int64 serial, int blki, int offset, int end);
void ClearInvectorCache__();
int  FindInvectorCache__(int64 serial, int& pos, int& off);

template <class T>
force_inline void InVector<T>::SetCache(int blki, int offset) const
{
#ifdef flagIVTEST
	Check(0, 0);
#endif
	SetInvectorCache__(serial, blki, offset, offset + data[blki].GetCount());
}

template <class T>
force_inline void InVector<T>::ClearCache() const
{
	ClearInvectorCache__();
}

template <class T>
force_inline int InVector<T>::FindBlock(int& pos, int& off) const
{
	int i = FindInvectorCache__(serial, pos, off);
	return i >= 0 ? i : FindBlock0(pos, off);
}

template <class T>
int InVector<T>::FindBlock0(int& pos, int& off) const
{
	LLOG("FindBlock " << pos);
	ASSERT(pos >= 0 && pos <= count);
	if(pos == count) {
		LLOG("Found last");
		pos = data.Top().GetCount();
		off = count - pos;
		return data.GetCount() - 1;
	}
	int blki = 0;
	int offset = 0;
	for(int i = index.GetCount(); --i >= 0;) {
		int n = index[i][blki];
		if(pos >= n) {
			blki++;
			pos -= n;
			offset += n;
		}
		blki += blki;
	}
	int n = data[blki].GetCount();
	if(pos >= n) {
		blki++;
		pos -= n;
		offset += n;
	}

	SetCache(blki, offset);

	off = offset;
	return blki;
}

template <class T>
force_inline int InVector<T>::FindBlock(int& pos) const
{
	int h;
	return FindBlock(pos, h);
}

template <class T>
const T& InVector<T>::operator[](int i) const
{
	LLOG("operator[] " << i);
	ASSERT(i >= 0 && i < count);
	int blki = FindBlock(i);
	return data[blki][i];
}

template <class T>
T& InVector<T>::operator[](int i)
{
	LLOG("operator[] " << i);
	ASSERT(i >= 0 && i < count);
	int blki = FindBlock(i);
	return data[blki][i];
}

template <class T>
void InVector<T>::Reindex()
{
	LLOG("--- Reindexing");
	ClearCache();
	SetBlkPar();
	index.Clear();
	if(slave)
		Slave()->Reindex();
	hcount = 0;
	Vector<T> *ds = data.Begin();
	Vector<T> *dend = data.End();
	int n = data.GetCount();
	if(n <= 2)
		return;
	Vector<int>& w = index.Add();
	hcount = 2;
	w.SetCount((n + 1) >> 1);
	int *t = w.Begin();
	while(ds != dend) {
		*t = (ds++)->GetCount();
		if(ds == dend)
			break;
		*t++ += (ds++)->GetCount();
	}
	int *s = w.Begin();
	int *end = w.End();
	n = w.GetCount();
	while(n > 2) {
		Vector<int>& w = index.Add();
		hcount += hcount;
		w.SetCount((n + 1) >> 1);
		t = w.Begin();
		while(s != end) {
			*t = *s++;
			if(s == end)
				break;
			*t++ += *s++;
		}
		s = w.Begin();
		end = w.End();
		n = w.GetCount();
	}
#ifdef flagIVTEST
	Check(0, 0);
#endif
}

template <class T>
void InVector<T>::SetBlkPar()
{
#if defined(_DEBUG) && defined(flagIVTEST)
	blk_high = 11;
	blk_low = 5;
#else
	int n = 2500 + data.GetCount() / 4;
	blk_high = minmax(n / (int)sizeof(T), 16, 65000);
	blk_low = minmax(n / 3 / (int)sizeof(T), 16, 65000);
#endif
}

template <class T>
void InVector<T>::Index(int q, int n)
{
	for(int i = 0; i < index.GetCount(); i++)
		index[i].At(q >>= 1, 0) += n;
}

template <class T>
T *InVector<T>::Insert0(int ii, int blki, int pos, int off, const T *val)
{
	if(data[blki].GetCount() > blk_high) {
		if(slave)
			Slave()->Split(blki, data[blki].GetCount() / 2);
		Vector<T>& x = data.Insert(blki + 1);
		x.InsertSplit(0, data[blki], data[blki].GetCount() / 2);
		data[blki].Shrink();
		Reindex();
		pos = ii;
		blki = FindBlock(pos, off);
	}
	LLOG("blki: " << blki << ", pos: " << pos);
	count++;
	if(slave) {
		Slave()->Count(1);
		Slave()->Index(blki, 1);
		Slave()->Insert(blki, pos);
	}
	Index(blki, 1);
	if(val)
		data[blki].Insert(pos, *val);
	else
		data[blki].Insert(pos);
	SetCache(blki, off);
	return &data[blki][pos];
}

template <class T>
T *InVector<T>::Insert0(int ii, const T *val)
{
	ASSERT(ii >= 0 && ii <= GetCount());
	if(data.GetCount() == 0) {
		count++;
		ClearCache();
		if(slave) {
			Slave()->Count(1);
			Slave()->AddFirst();
		}
		if(val) {
			data.Add().Add(*val);
			return &data[0][0];
		}
		return &data.Add().Add();
	}
	int pos = ii;
	int off;
	int blki = FindBlock(pos, off);
	return Insert0(ii, blki, pos, off, val);
}

template <class T>
template <class Range>
void InVector<T>::Insert_(int ii, const Range& r, bool def)
{
	int n = r.GetCount();

	ASSERT(ii >= 0 && ii <= GetCount() && n >= 0 && !slave);

	if(n <= 0)
		return;
	
	auto s = r.begin();

	if(data.GetCount() == 0) {
		int m = (blk_high + blk_low) / 2;
		count = n;
		while(n > 0) {
			int n1 = min(m, n);
			if(def)
				data.Add().SetCount(n1);
			else
				data.Add().AppendRange(SubRange(s, n1));
			s += n1;
			n -= n1;
		}
		Reindex();
		return;
	}

	int pos = ii;
	int off;
	int blki = FindBlock(pos, off);
	int bc = data[blki].GetCount();
	
	count += n;

	if(bc + n < blk_high) { // block will not be bigger than threshold after insert
		if(def)
			data[blki].InsertN(pos, n);
		else
			data[blki].InsertRange(pos, SubRange(s, n));
		Index(blki, n);
		SetCache(blki, off);
	}
	else
	if(bc - pos + n < blk_high) { // splitting into 2 blocks is enough
		Vector<T>& t = data.Insert(blki + 1);
		if(def)
			t.InsertN(0, n);
		else
			t.InsertRange(0, SubRange(s, n));
		t.InsertSplit(n, data[blki], pos);
		data[blki].Shrink();
		Reindex();
	}
	else { // need to insert several blocks
		int m = (blk_high + blk_low) / 2;
		int bn = (n + m - 1) / m;
		int ti;
		if(pos) { // need to split first block
			ti = blki + 1; // TODO should add some of data to splitted blocks
			data.InsertN(ti, bn + 1);
			data[ti + bn].InsertSplit(0, data[blki], pos);
			data[blki].Shrink();
		}
		else {
			ti = blki;
			data.InsertN(ti, bn);
		}
		for(int i = 0; i < bn; i++) {
			int q = min(m, n);
			if(def)
				data[ti + i].SetCount(q);
			else
				data[ti + i].AppendRange(SubRange(s, q));
			s += q;
			n -= q;
		}
		ASSERT(n == 0);
		Reindex();
	}
#ifdef flagIVTEST
	Check(0, 0);
#endif
}

template <class T>
void InVector<T>::Join(int blki)
{
	data[blki].AppendPick(pick(data[blki + 1]));
	data.Remove(blki + 1);
}

template <class T>
force_inline bool InVector<T>::JoinSmall(int blki)
{
	if(blki < data.GetCount()) {
		int n = data[blki].GetCount();
		if(n == 0) {
			if(slave)
				Slave()->RemoveBlk(blki, 1);
			data.Remove(blki);
			return true;
		}
		if(n < blk_low) {
			if(blki > 0 && data[blki - 1].GetCount() + n <= blk_high) {
				if(slave)
					Slave()->Join(blki - 1);
				Join(blki - 1);
				return true;
			}
			if(blki + 1 < data.GetCount() && n + data[blki + 1].GetCount() <= blk_high) {
				if(slave)
					Slave()->Join(blki);
				Join(blki);
				return true;
			}
		}
	}
	return false;
}

template <class T>
void InVector<T>::Remove(int pos, int n)
{
	ASSERT(pos >= 0 && pos + n <= GetCount());
	if(n == 0)
		return;
	int off;
	int blki = FindBlock(pos, off);
	count -= n;
	if(slave)
		Slave()->Count(-n);
	if(pos + n < data[blki].GetCount()) {
		if(slave)
			Slave()->Remove(blki, pos, n);
		data[blki].Remove(pos, n);
		if(JoinSmall(blki))
			Reindex();
		else {
			if(slave)
				Slave()->Index(blki, -n);
			Index(blki, -n);
			SetCache(blki, off);
		}
	}
	else {
		int b1 = blki;
		int nn = min(n, data[b1].GetCount() - pos);
		if(slave)
			Slave()->Remove(b1, pos, nn);
		data[b1++].Remove(pos, nn);
		n -= nn;
		int b2 = b1;
		while(b2 < data.GetCount() && n >= data[b2].GetCount()) {
			n -= min(n, data[b2].GetCount());
			b2++;
		}
		if(slave)
			Slave()->RemoveBlk(b1, b2 - b1);
		data.Remove(b1, b2 - b1);
		if(b1 < data.GetCount()) {
			if(slave)
				Slave()->Remove(b1, 0, n);
			data[b1].Remove(0, n);
		}
		JoinSmall(blki + 1);
		JoinSmall(blki);
		Reindex();
	}
#ifdef flagIVTEST
	Check(0, 0);
#endif
}

template <class T>
void InVector<T>::SetCount(int n)
{
	if(n < GetCount())
		Trim(n);
	else
		InsertN(GetCount(), n - GetCount());
}

template <class T>
void InVector<T>::Shrink()
{
	for(int i = 0; i < data.GetCount(); i++)
		data[i].Shrink();
	data.Shrink();
	for(int i = 0; i < index.GetCount(); i++)
		index[i].Shrink();
	index.Shrink();
}

template <class T>
void InVector<T>::Set(int i, const T& x, int count)
{
	Iterator it = GetIter(i);
	while(count-- > 0)
		*it++ = x;
}

template <class T>
InVector<T>::InVector(const InVector<T>& v, int)
{
	data <<= v.data;
	index <<= v.index;
	count = v.count;
	hcount = v.hcount;
	blk_high = v.blk_high;
	blk_low = v.blk_low;
	serial = NewInVectorSerial();
	slave = v.slave;
}

template <class T>
void InVector<T>::Pick(InVector&& v)
{
	data = pick(v.data);
	index = pick(v.index);
	count = v.count;
	hcount = v.hcount;
	blk_high = v.blk_high;
	blk_low = v.blk_low;
	serial = v.serial;
	slave = v.slave;

	v.Init();
}


template <class T>
template <class L>
int InVector<T>::FindUpperBound(const T& val, const L& less, int& off, int& pos) const
{
	if(data.GetCount() == 0) {
		pos = off = 0;
		return 0;
	}
	int blki = 0;
	int ii = 0;
	int offset = 0;
	int half = hcount;
	for(int i = index.GetCount(); --i >= 0;) {
		int m = blki + half;
		if(m - 1 < data.GetCount() && !less(val, data[m - 1].Top())) {
			blki = m;
			offset += index[i][ii];
			ii++;
		}
		ii += ii;
		half >>= 1;
	}
#ifdef flagIVTEST
	Check(blki, offset);
#endif
	if(blki < data.GetCount()) {
		if(!less(val, data[blki].Top()))
			offset += data[blki++].GetCount();
		if(blki < data.GetCount()) {
			pos = Upp::FindUpperBound(data[blki], val, less);
			off = offset;
			SetCache(blki, offset);
			return blki;
		}
	}
	pos = data.Top().GetCount();
	off = count - pos;
	blki--;
	SetCache(blki, off);
	return blki;
}

template <class T>
template <class L>
int InVector<T>::FindLowerBound(const T& val, const L& less, int& off, int& pos) const
{
	if(data.GetCount() == 0) {
		pos = off = 0;
		return 0;
	}
	int blki = 0;
	int ii = 0;
	int offset = 0;
	int half = hcount;
	for(int i = index.GetCount(); --i >= 0;) {
		int m = blki + half;
		if(m < data.GetCount() && less(data[m][0], val)) {
			blki = m;
			offset += index[i][ii];
			ii++;
		}
		ii += ii;
		half >>= 1;
	}
#ifdef flagIVTEST
	Check(blki, offset);
#endif
	if(blki < data.GetCount()) {
		if(blki + 1 < data.GetCount() && less(data[blki + 1][0], val))
			offset += data[blki++].GetCount();
		if(blki < data.GetCount()) {
			pos = Upp::FindLowerBound(data[blki], val, less);
			off = offset;
			SetCache(blki, offset);
			return blki;
		}
	}
	pos = data.Top().GetCount();
	off = count - pos;
	blki--;
	SetCache(blki, off);
	return blki;
}

template <class T>
template <class L>
int InVector<T>::InsertUpperBound(const T& val, const L& less)
{
	if(data.GetCount() == 0) {
		count++;
		ClearCache();
		if(slave) {
			Slave()->Count(1);
			Slave()->AddFirst();
		}
		data.Add().Insert(0) = val;
		return 0;
	}
	int off;
	int pos;
	int blki = FindUpperBound(val, less, off, pos);
	Insert0(off + pos, blki, pos, off, &val);
	return off + pos;
}

template <class T>
template <class L>
int InVector<T>::Find(const T& val, const L& less) const
{
	int i = FindLowerBound(val, less);
	return i < GetCount() && !less(val, (*this)[i]) ? i : -1;
}


template <class T>
void InVector<T>::SetIter(ConstIterator& it, int ii) const
{
	if(count) {
		it.v = this;
		it.blki = FindBlock(ii, it.offset);
		it.begin = data[it.blki].Begin();
		it.end = data[it.blki].End();
		it.ptr = it.begin + ii;
	}
	else
		SetEnd(it);
}

template <class T>
void InVector<T>::SetBegin(ConstIterator& it) const
{
	if(count) {
		it.v = this;
		it.blki = 0;
		it.ptr = it.begin = data[0].Begin();
		it.end = data[0].End();
		it.offset = 0;
	}
	else
		SetEnd(it);
}

template <class T>
void InVector<T>::SetEnd(ConstIterator& it) const
{
	if(count) {
		it.v = this;
		it.blki = data.GetCount() - 1;
		it.begin = data.Top().Begin();
		it.ptr = it.end = data.Top().End();
		it.offset = count - data.Top().GetCount();
	}
	else {
		it.v = this;
		it.blki = 0;
		it.ptr = it.begin = it.end = NULL;
		it.offset = 0;		
	}
}

template <typename T>
force_inline typename InVector<T>::ConstIterator& InVector<T>::ConstIterator::operator+=(int d)
{
	if(d >= 0 ? d < end - ptr : -d < ptr - begin)
		ptr += d;
	else
		v->SetIter(*this, GetIndex() + d);
	ASSERT(end - begin == v->data[blki].GetCount());
	return *this;
}

template <typename T>
void InVector<T>::ConstIterator::NextBlk()
{
	ASSERT(end - begin == v->data[blki].GetCount());
	if(blki + 1 < v->data.GetCount()) {
		offset += v->data[blki].GetCount();
		++blki;
		ptr = begin = v->data[blki].Begin();
		end = v->data[blki].End();
	}
}

template <typename T>
void InVector<T>::ConstIterator::PrevBlk()
{
	--blki;
	begin = v->data[blki].Begin();
	ptr = end = v->data[blki].End();
	offset -= v->data[blki].GetCount();
}

template <typename T>
void InVector<T>::Swap(InVector& b)
{
	Upp::Swap(data, b.data);
	Upp::Swap(index, b.index);
	Upp::Swap(count, b.count);
	Upp::Swap(hcount, b.hcount);
	Upp::Swap(serial, b.serial);
	Upp::Swap(blk_high, b.blk_high);
	Upp::Swap(blk_low, b.blk_low);
	Upp::Swap(slave, b.slave);
}

template <class T>
void InVector<T>::Xmlize(XmlIO& xio, const char *itemtag)
{
	XmlizeContainer(xio, itemtag, *this);
}

template <class T>
void InVector<T>::Jsonize(JsonIO& jio)
{
	JsonizeArray<InVector<T>>(jio, *this);
}

template <class T>
String InVector<T>::ToString() const
{
	return AsStringArray(*this);
}

template <class T>
void InVector<T>::DumpIndex() const
{
	String h;
	RLOG("------- InVector dump, count: " << GetCount() << ", index depth: " << index.GetCount());
	int64 alloc = 0;
	for(int i = 0; i < data.GetCount(); i++) {
		if(i)
			h << ", ";
		h << data[i].GetCount() << " (" << data[i].GetAlloc() << ")";
		alloc += data[i].GetAlloc();
	}
	RLOG("Data blocks: " << data.GetCount() << ", sizeof: " << data.GetCount() * sizeof(Vector<T>));
	RLOG("Total alloc: " << alloc);
	RLOG(h);
	for(int j = 0; j < index.GetCount(); j++) {
		h.Clear();
		h << index[j].GetCount() << ": ";
		for(int k = 0; k < index[j].GetCount(); k++) {
			if(k)
				h << ", ";
			h << index[j][k];
		}
		RLOG(h);
	}
	RLOG(".");
}

#ifdef flagIVTEST
template <class T>
void InVector<T>::Check(int blki, int offset) const
{
	int off = 0;
	int all = 0;
	for(int i = 0; i < data.GetCount(); i++) {
		if(i < blki)
			off += data[i].GetCount();
		all += data[i].GetCount();
	}
	ASSERT(off == offset);
	ASSERT(all == count);
}
#endif

template <class T>
void InArray<T>::Delete(IVIter it, int count)
{
	ASSERT(count >= 0);
	while(count--)
		delete (T *)*it++;
}

template <class T>
void InArray<T>::Delete(int i, int count)
{
	Delete(iv.GetIter(i), count);
}

template <class T>
void InArray<T>::Init(int i, int count)
{
	ASSERT(count >= 0);
	IVIter it = iv.GetIter(i);
	while(count--)
		*it++ = new T;
}

template <class T>
void InArray<T>::InsertN(int i, int count)
{
	iv.InsertN(i, count);
	Init(i, count);
}

template <class T>
template <class Range>
void InArray<T>::InsertRange(int i, const Range& r)
{
	int count = r.GetCount();
	iv.InsertN(i, count);
	IVIter it = iv.begin() + i;
	auto s = r.begin();
	while(count--)
		*it++ = new T(*s++);
}

template <class T>
void InArray<T>::Remove(int i, int count)
{
	Delete(i, count);
	iv.Remove(i, count);
}

template <class T>
void InArray<T>::SetCount(int n)
{
	if(n < GetCount())
		Trim(n);
	else
		InsertN(GetCount(), n - GetCount());
}

template <class T>
void InArray<T>::Clear()
{
	Free();
	iv.Clear();
}

template <class T>
void InArray<T>::Set(int i, const T& x, int count)
{
	Iterator it = GetIter(i);
	while(count-- > 0)
		*it++ = x;
}

template <class T>
void InArray<T>::SetIter(ConstIterator& it, int ii) const
{
	it.it = iv.GetIter(ii);
}

template <class T>
void InArray<T>::SetBegin(ConstIterator& it) const
{
	it.it = iv.Begin();
}

template <class T>
void InArray<T>::SetEnd(ConstIterator& it) const
{
	it.it = iv.End();
}

template <class T>
InArray<T>::InArray(const InArray& v, int)
{
	int n = v.GetCount();
	iv.SetCount(v.GetCount());
	ConstIterator s = v.Begin();
	IVIter it = iv.Begin();
	while(n--)
		*it++ = new T(clone(*s++));
}

#ifdef UPP
template <class T>
void InArray<T>::Xmlize(XmlIO& xio, const char *itemtag)
{
	XmlizeContainer(xio, itemtag, *this);
}

template <class T>
void InArray<T>::Jsonize(JsonIO& jio)
{
	JsonizeArray<InArray<T>>(jio, *this);
}

template <class T>
String InArray<T>::ToString() const
{
	return AsStringArray(*this);
}

#endif

#ifdef LLOG
#undef  LLOG
#endif