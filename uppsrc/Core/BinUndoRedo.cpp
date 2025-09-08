#include "Core/Core.h"

#define LLOG(x)

namespace Upp {

namespace BinDiffDetail {

struct DataFinder {
	const char   *ptr;
	const char   *end;
	const char   *end8;
	Index<uint64> pos;
	
	int FindLongest(const char *s, const char *e, int& len) const;
	
	DataFinder(const char *s, const char *e, size_t max_pos = 1024*1024);
};

DataFinder::DataFinder(const char *s, const char *e, size_t max_pos)
{
	ptr = s;
	end = e;
	size_t len = e - s;
	if(len < 8)
		return;
	const char *end8 = s + min(len - 8, max_pos);
	for(const char *s = ptr; s < end8; s++)
		pos.Add(Peek64(s));
}

int DataFinder::FindLongest(const char *s, const char *e, int& len) const
{
	int found = 0;
	len = 0;

	if(e - s <= 8)
		return 0;
	
	int p = pos.Find(Peek64(s));
	while(p >= 0 && len < e - s) {
		const char *q = ptr + p;
		if(end - q > len && q[len] == s[len]) {
			q += 8;
			const char *t = s + 8;
			int l = (int)min(memeq8len(q, t, min(end - q, e - t)) + 8, (size_t)INT_MAX);
			if(l > len) {
				len = l;
				found = p;
			}
		}
		p = pos.FindNext(p);
	}
	
	return found;
}

}

String BinUndiff(const String& base, const String& bin)
{
	String data;
	int base_len = base.GetCount();
	const byte *s = bin;
	const byte *e = (const byte *)bin.End();
	auto Get = [&] {
		if(s < e) return *s++;
		throw Exc();
	};
	auto GetInt = [&]() -> int {
		byte h = Get();
		byte l = Get();
		if(h & 128) { // 32 bit
			byte h0 = Get();
			byte l0 = Get();
			return MAKELONG(MAKEWORD(l0, h0), MAKEWORD(l, h & 127));
		}
		return MAKEWORD(l, h);
	};
	try {
		while(s < e) {
			byte b = *s++;
			if(b == 0) {
				int pos = GetInt();
				int len = GetInt();
				LLOG("Pos " << pos << ", len " << len);
				if(pos >= 0 && pos < base_len && base_len - pos >= len)
					data.Cat(base.Mid(pos, len));
				else
					return String::GetVoid();
			}
			else {
				if(b <= e - s) {
					data.Cat(s, b);
					s += b;
				}
				else
					return String::GetVoid();
			}
		}
	}
	catch(Exc) {
		return String::GetVoid();
	}
	return data;
}

String BinDiff(const String& base, const String& data)
{
	using namespace BinDiffDetail;

	String diff;
	const char *s = data;
	const char *unmatched = s;
	int  unmatched_len = 0;
	auto PutInt = [&](int n) {
		LLOG("PutInt " << n);
		if(n < 32768) {
			diff.Cat(HIBYTE(n));
			diff.Cat(LOBYTE(n));
		}
		else {
			diff.Cat(HIBYTE(HIWORD(n)) | 128);
			diff.Cat(LOBYTE(HIWORD(n)));
			diff.Cat(HIBYTE(LOWORD(n)));
			diff.Cat(LOBYTE(LOWORD(n)));
		}
	};
	auto FlushUnmatched = [&] {
		if(unmatched_len) {
			LLOG("Unmatched " << String(unmatched, unmatched_len));
			diff.Cat(unmatched_len);
			diff.Cat(unmatched, unmatched_len);
			unmatched_len = 0;
		}
	};
	DataFinder f(~base, base.end());
	while(s < data.end()) {
		int matchlen = 0;
		int foundpos = 0;
		foundpos = f.FindLongest(s, data.end(), matchlen);
		if(matchlen < 8) {
			int l = max(matchlen, 1);
			if(l + unmatched_len > 255) {
				FlushUnmatched();
				unmatched_len = l;
				unmatched = s;
				s += l;
			}
			else {
				unmatched_len += l;
				s += l;
			}
		}
		else {
			FlushUnmatched();
			diff.Cat(0); // use data from base
			PutInt(foundpos);
			PutInt(matchlen);
			s += matchlen;
			unmatched = s;
		}
	}
	FlushUnmatched();
	return diff;
}

void BinUndoRedo::Reset(const String& current, const String& ids)
{
	undo.Clear();
	redo.Clear();
	undosize = 0;
	commit.data = current;
	commit.ids = ids;
}

bool BinUndoRedo::Commit(const String& current, const String& ids, int limit)
{
	bool ret = false;
	if(redo.GetCount()) {
		redo.Clear();
		ret = true;
	}
	if(current != commit.data || ids != commit.ids) {
		String u = BinDiff(current, commit.data);
		while(undo.GetCount() && undosize + u.GetCount() > limit) {
			undosize -= undo[0].data.GetCount();
			undosize -= undo[0].ids.GetCount();
			undo.Remove(0);
		}
		Entry& e = undo.Add();
		e.data = u;
		e.ids = commit.ids;
		undosize += u.GetCount();
		undosize += ids.GetCount();
		commit.data = current;
		commit.ids = ids;
		ret = true;
	}
	return ret;
}

String BinUndoRedo::Undo(const String& current)
{
	if(undo.GetCount()) {
		Entry prev = undo.Pop();
		commit.data = BinUndiff(commit.data, prev.data);
		
		Entry& e = redo.Add();
		e.data = BinDiff(commit.data, current);
		e.ids = prev.ids;
		return commit.data;
	}
	return String::GetVoid();
}

String BinUndoRedo::Redo(const String& current)
{
	if(redo.GetCount()) {
		Entry next = redo.Pop();
		commit.data = BinUndiff(commit.data, next.data);
		Entry& e = undo.Add();
		e.data = BinDiff(commit.data, current);
		e.ids = next.ids;
		return commit.data;
	}
	return String::GetVoid();
}

void BinUndoRedo::Ids(Event<const String&> fn)
{
	fn(commit.ids);
	for(const Entry& e : undo)
		fn(e.ids);
	for(const Entry& e : redo)
		fn(e.ids);
}

}
