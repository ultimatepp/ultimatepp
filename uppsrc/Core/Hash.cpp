#include <Core/Core.h>

NAMESPACE_UPP

#ifdef _MSC_VER
#pragma inline_depth(255)
#pragma optimize("t", on)
#endif

unsigned Pow2Bound(unsigned i)
{
	unsigned n = 1;
	while(n < i) {
		n <<= 1;
		if(n == 0)
			return 1 << 31;
	}
	return n;
}

unsigned PrimeBound(unsigned n)
{
	static unsigned prime_tab[] = {
		17, 29, 61, 127, 257, 509, 1021, 2053, 4093, 8191, 16381,
		32771, 65537, 131071, 262147, 524287, 1048573, 2097143,
		4194301, 8388617, 16777213, 33554467, 67108859, 134217757,
		268435459, 536870909, 1073741827, 2147483647u, 4294967291u
	};
	return *FindUpperBoundIter(prime_tab, prime_tab + __countof(prime_tab), n);
}

#ifdef FOLDHASH
inline unsigned HashBound(unsigned i) { return Pow2Bound(i); }
#else
inline unsigned HashBound(unsigned i) { return PrimeBound(i); }
#endif

void HashBase::Free()
{
	if(map)
		delete [](byte *)map;
	map = NULL;
	mcount = 0;
}

void HashBase::ClearIndex()
{
	link.Clear();
	unlinked = -1;
	Free();
}

HashBase::HashBase()
{
	unlinked = -1;
	map = NULL;
	mcount = 0;
}

HashBase::HashBase(HashBase rval_ b)
: hash(pick(b.hash)),
  link(pick(b.link))
{
	map = b.map;
	mcount = b.mcount;
	unlinked = b.unlinked;
	const_cast<HashBase &>(b).map = NULL;
}

void HashBase::operator=(HashBase rval_ b)
{
	hash = pick(b.hash);
	link = pick(b.link);
	Free();
	map = b.map;
	mcount = b.mcount;
	unlinked = b.unlinked;
	const_cast<HashBase &>(b).map = NULL;
}

HashBase::HashBase(const HashBase& b, int)
: hash(b.hash, 0)
{
	unlinked = -1;
	mcount = 0;
	map = NULL;
	Reindex();
}

void HashBase::operator<<=(const HashBase& b)
{
	ClearIndex();
	hash <<= b.hash;
	Reindex();
}

HashBase::~HashBase()
{
	Free();
}

force_inline void HashBase::LinkBefore(int i, Link& l, int bi)
{
	Link& bl = link[bi];
	l.next = bi;
	l.prev = bl.prev;
	bl.prev = i;
	link[l.prev].next = i;
}

void HashBase::Trim(int count)
{
	ASSERT(count <= hash.GetCount() && count >= 0);
	for(int i = count; i < link.GetCount(); i++)
		Unlink(i, link[i]);
	link.Trim(count);
	hash.SetCount(count);
}

void HashBase::Drop(int n)
{
	Trim(hash.GetCount() - n);
}

void HashBase::FinishIndex()
{
	int q = link.GetCount();
	link.Reserve(hash.GetAlloc());
	link.AddN(hash.GetCount() - q);
	for(int i = q; i < hash.GetCount(); i++)
		LinkTo(i, link[i], hash[i] & UNSIGNED_HIBIT ? unlinked : Mapi(i));
}

void HashBase::Reindex(int n)
{
	ClearIndex();
	Free();
	mcount = n = HashBound(n);
	map = (int *)new byte[n * sizeof(int)];
	Fill(map, map + n, -1);
	FinishIndex();
}

void HashBase::Reindex()
{
	Reindex(hash.GetCount());
}

void HashBase::Add(unsigned _hash)
{
	hash.Add(_hash & ~UNSIGNED_HIBIT);
	DoIndex();
}

void  HashBase::SetUn(int i, unsigned _hash)
{
	if(map) {
		Link& lnk = link[i];
		Unlink(i, lnk);
		LinkTo(i, lnk, Maph(_hash & ~UNSIGNED_HIBIT));
	}
	hash[i] = _hash & ~UNSIGNED_HIBIT;
}

Vector<int> HashBase::GetUnlinked() const
{
	Vector<int> r;
	int q = unlinked;
	if(q >= 0) {
		do {
			r.Add(q);
			q = link[q].next;
		}
		while(q != unlinked);
	}
	return r;
}

int HashBase::Put(unsigned _hash)
{
	if(unlinked < 0) return -1;
	Link& l = link[unlinked];
	int i = unlinked;
	unlinked = link[unlinked].next;
	if(i == unlinked)
		unlinked = -1;
	else {
		link[l.next].prev = l.prev;
		link[l.prev].next = l.next;
	}
	LinkTo(i, l, Maph(_hash & ~UNSIGNED_HIBIT));
	hash[i] = _hash & ~UNSIGNED_HIBIT;
	return i;
}

void HashBase::Set(int i, unsigned _hash) {
	if(map) {
		Link& lnk = link[i];
		Unlink(i, lnk);
		int& mi = Maph(_hash & ~UNSIGNED_HIBIT);
		int ii = mi;
		if(ii < 0)
			mi = lnk.prev = lnk.next = i;
		else
		if(i < ii) {
			LinkBefore(i, lnk, ii);
			mi = i;
		}
		else {
			int l = ii;
			int h = link[ii].prev;
			if(h - i < i - l) {
				while(i < h) {
					h = link[h].prev;
				}
				LinkBefore(i, lnk, link[h].next);
			}
			else {
				l = link[l].next;
				while(i > l && l != ii) {
					l = link[l].next;
				}
				LinkBefore(i, lnk, l);
			}
		}
	}
	hash[i] = _hash & ~UNSIGNED_HIBIT;
}

void HashBase::Shrink() {
	hash.Shrink();
	if((int)HashBound(hash.GetCount()) < mcount) {
		ClearIndex();
		DoIndex();
	}
	else
		link.Shrink();
}

void HashBase::Reserve(int n)
{
	hash.Reserve(n);
	link.Reserve(n);
	if((int)HashBound(n) > mcount)
		Reindex(n);
}

void HashBase::Remove(int i)
{
	hash.Remove(i);
	ClearIndex();
	Reindex();
}

void HashBase::Remove(int i, int count)
{
	hash.Remove(i, count);
	ClearIndex();
	Reindex();
}

void HashBase::Remove(const int *sorted_list, int count)
{
	hash.Remove(sorted_list, count);
	ClearIndex();
	Reindex();
}

void HashBase::Insert(int i, unsigned _hash) {
	hash.Insert(i, _hash & ~UNSIGNED_HIBIT);
	ClearIndex();
	Reindex();
}

#ifdef UPP
void HashBase::Serialize(Stream& s) {
	int version = 0;
	s / version;
	if(s.IsLoading())
		ClearIndex();
	hash.Serialize(s);
	Reindex();
}
#endif

void HashBase::Swap(HashBase& b) {
	UPP::Swap(hash, b.hash);
	UPP::Swap(link, b.link);
	UPP::Swap(map, b.map);
	UPP::Swap(mcount, b.mcount);
	UPP::Swap(unlinked, b.unlinked);
}


#ifdef CPU_X86

unsigned memhash(const void *ptr, size_t count)
{
	unsigned hash = 1234567890U;

	const unsigned *ds = (unsigned *)ptr;
	const unsigned *de = ds + (count >> 2);
	while(ds < de)
		hash = ((hash << 5) - hash) ^ *ds++;

	const byte *s = (byte *)ds;
	const byte *e = s + (count & 3);
	while(s < e)
		hash = ((hash << 5) - hash) ^ *s++;

	return hash;
}

#else

unsigned memhash(const void *ptr, size_t count)
{
	unsigned hash = 1234567890U;

	const byte *s = (byte *)ptr;
	const byte *e = s + count;
	while(s < e)
		hash = ((hash << 5) - hash) ^ *s++;

	return hash;
}

#endif

unsigned GetHashValue0(const double& d)
{
	return memhash(&d, sizeof(double));
}

void Bits::Clear()
{
	if(bp && alloc >= 0)
		delete[] bp;
	alloc = 0;
	bp = NULL;
}

void Bits::Set(int i, bool b)
{
	ASSERT(i >= 0 && alloc >= 0);
	int q = i >> 5;
	if(q >= alloc) {
		int nalloc = 4 * q / 3 + 1;
		dword *nbp = new dword[nalloc];
		if(bp) {
			Copy(nbp, bp, bp + alloc);
			delete[] bp;
		}
		Fill(nbp + alloc, nbp + nalloc, (dword)0);
		bp = nbp;
		alloc = nalloc;
	}
	i &= 31;
	ASSERT(q < alloc);
	bp[q] = (bp[q] & ~(1 << i)) | (b << i);
}

void Bits::Set(int i, bool b, int count)
{
	while(count--)
		Set(i++, b);
}

END_UPP_NAMESPACE
