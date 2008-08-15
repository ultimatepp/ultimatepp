#include "Uvs2.h"
#pragma hdrstop

#include "textdiff.h"

template <class I>
static int CompareGetCount(I a, I b, int max_count)
{
	if(max_count <= 0 || *a != *b)
		return 0;
	int left;
	for(left = max_count; --left > 0;)
		if(*++a != *++b)
			return max_count - left;
	return max_count;
}

Vector<String> GetLineMap(Stream& stream)
{
	Vector<String> out;
	int emp = 0;
	if(stream.IsOpen())
		while(!stream.IsEof()) {
			String s = stream.GetLine();
			const char *p = s, *e = s.End(), *f = e;
			while(e > p && (byte)e[-1] <= ' ')
				e--;
			if(e == p)
				emp++;
			else
			{
				while(emp-- > 0)
					out.Add(Null);
				if(e != f)
					s.Trim(e - p);
				out.Add(s);
				emp = 0;
			}
		}
	return out;
}

Vector<String> GetFileLineMap(const String& path)
{
	FileIn fi(path);
	return GetLineMap(fi);
}

Vector<String> GetStringLineMap(const String& s)
{
	StringStream ss(s);
	return GetLineMap(ss);
}

class TextComparator
{
public:
	TextComparator(const Vector<String>& f1, const Vector<String>& f2);

	Array<TextSection>    GetSections() const;

private:
	bool                  Find(int start1, int end1, int start2, int end2, int& best_match, int& best_count) const;
	void                  Split(Array<TextSection>& dest, int start1, int end1, int start2, int end2) const;

private:
	Vector<HashBase>      hash1;
	Vector<HashBase>      hash2;
	const Vector<String>& file1;
	const Vector<String>& file2;
};

Array<TextSection> CompareLineMaps(const Vector<String>& s1, const Vector<String>& s2)
{
	return TextComparator(s1, s2).GetSections();
}

static void CalcHash(Vector<HashBase>& hash, const Vector<String>& file, int limit)
{
	{ // 1st row
		HashBase& first = hash.Add();
		for(int i = 0; i < file.GetCount(); i++)
			first.Add(GetHashValue(file[i]));
	}
	static const int prime[] =
	{
		3,  5,  7,   11,  13,  17,  19,  21,
		23, 29, 31,  37,  41,  43,  47,  51,
		53, 61, 67,  71,  73,  79,  83,  87,
		89, 97, 101, 103, 107, 109, 113, 117,
	};
	const int *pp = prime;
	for(int l = 1; l < limit; l <<= 1) {
		HashBase& nhash = hash.Add();
		const HashBase& ohash = hash[hash.GetCount() - 2];
		int pri = *pp++;
		int t;
		for(t = l; t < ohash.GetCount(); t++)
			nhash.Add(ohash[t - l] + pri * ohash[t]);
		for(t -= l; t < ohash.GetCount(); t++)
			nhash.Add(ohash[t]);
	}
}

TextComparator::TextComparator(const Vector<String>& f1, const Vector<String>& f2)
: file1(f1), file2(f2)
{
	int limit = min(f1.GetCount(), f2.GetCount());
	CalcHash(hash1, f1, limit);
	CalcHash(hash2, f2, limit);
}

static bool CompareSection(const TextSection& ta, const TextSection& tb)
{
	return ta.start1 < tb.start1 || ta.start1 == tb.start1 && ta.start2 < tb.start2;
}

Array<TextSection> TextComparator::GetSections() const
{
	Array<TextSection> output;
	Split(output, 0, file1.GetCount(), 0, file2.GetCount());
	Sort(output, &CompareSection);
	return output;
}

static int GetHashLevel(int min_count, int hash_count)
{
	int l = 0;
	hash_count--;
	while(min_count > 1 && l < hash_count)
	{
		min_count >>= 1;
		l++;
	}
	return l;
}

bool TextComparator::Find(int start1, int end1, int start2, int end2, int& best_match, int& best_count) const
{
	ASSERT(end1 > start1 && end2 > start2);
	bool done = false;
	const String *f1 = file1.Begin() + start1;
	int len1 = end1 - start1;
	int lvl = GetHashLevel(best_count + 1, hash1.GetCount());
	int chunk = 1 << lvl;
	int last = max(best_count - chunk + 1, 0);
	const HashBase *hp1 = &hash1[lvl];
	const HashBase *hp2 = &hash2[lvl];
	const unsigned *h1 = hp1->Begin() + start1;

	int i = hp2->Find(*h1);
	while(i >= 0)
		if(i + best_count >= end2)
			return done;
		else {
			if(i >= start2 && h1[last] == (*hp2)[i + last]) {
				int top = min(len1, end2 - i);
				int hc = CompareGetCount(h1, hp2->Begin() + i, top) + chunk - 1;
				int cnt = CompareGetCount(f1, file2.Begin() + i, min(hc, top));
				if(cnt > best_count) {
					best_count = cnt;
					best_match = i;
					done = true;
					last = best_count - chunk + 1;
					if(best_count + 1 >= 2 * chunk)
					{
						lvl = GetHashLevel(best_count + 1, hash1.GetCount());
						chunk = 1 << lvl;
						last = best_count - chunk + 1;
						hp1 = &hash1[lvl];
						hp2 = &hash2[lvl];
						h1 = hp1->Begin() + start1;
						int oi = i;
						for(i = hp2->Find(*h1); i >= 0 && i <= oi; i = hp2->FindNext(i))
							;
						continue;
					}
				}
			}
			i = hp2->FindNext(i);
		}
	return done;
}

void TextComparator::Split(Array<TextSection>& dest, int start1, int end1, int start2, int end2) const
{
	ASSERT(start1 <= end1 && start2 <= end2);
	while(start1 < end1 && start2 < end2) {
		int new1 = -1, new2 = -1, count = 0;
		for(int i = start1; i + count < end1; i++)
			if(Find(i, end1, start2, end2, new2, count))
				new1 = i;
		if(count == 0)
			break; // no match at all
		ASSERT(new1 >= start1 && new1 + count <= end1);
		ASSERT(new2 >= start2 && new2 + count <= end2);
		dest.Add(TextSection(new1, count, new2, count, true));
		if(new1 - start1 >= end1 - new1 - count) { // head is longer - recurse for tail
			Split(dest, new1 + count, end1, new2 + count, end2);
			end1 = new1;
			end2 = new2;
		}
		else { // tail is longer - recurse for head
			Split(dest, start1, new1, start2, new2);
			start1 = new1 + count;
			start2 = new2 + count;
		}
		ASSERT(start1 <= end1 && start2 <= end2);
	}
	if(start1 < end1 || start2 < end2)
		dest.Add(TextSection(start1, end1 - start1, start2, end2 - start2, false));
}
