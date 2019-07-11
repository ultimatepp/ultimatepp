#include "TextDiffCtrl.h"

namespace Upp {

int Patch::MatchLen(const String& a, const String& b)
{
	int n = min(a.GetLength(), b.GetLength());
	for(int i = 0; i < n; i++)
		if(a[i] != b[i])
			return i;
	return n;
}

int Patch::MatchCount(const char *dir)
{
	int count = 0;
	for(const String& p : file.GetKeys())
		if(FileExists(AppendFileName(dir, p)))
			count++;
	return count;
}

bool Patch::Load(Stream& in, Progress& pi)
{
	pi.SetText("Loading patch file");
	target_dir.Clear();
	file.Clear();
	String ln0;
	String ln = in.GetLine();
	for(;;) {
		while(!ln.TrimStart("--- ")) {
			if(pi.StepCanceled())
				return false;
			if(in.IsEof())
				return true;
			ln = in.GetLine();
		}
		ln = TrimLeft(ln);
		int q = ln.Find('\t');
		if(q >= 0)
			ln.Trim(q);
		String fn = UnixPath(ln);
		Array<Chunk>& fp = file.GetAdd(fn);
		while(!ln.StartsWith("@@")) {
			if(pi.StepCanceled())
				return false;
			if(in.IsEof())
				return true;
			ln0 = ln = in.GetLine();
		}
		while(ln.TrimStart("@@")) {
			if(pi.StepCanceled())
				return false;
			int line = atoi(ln);
			if(line <= 0) {
				Chunk& ch = fp.Add();
				ch.src << ln0 << "\n";
				ch.line = max(-line - 1, 0);
				for(;;) {
					if(pi.StepCanceled())
						return false;
					if(in.IsEof())
						return true;
					ln0 = ln = in.GetLine();
					if(*ln == ' ') {
						ch.orig.Add(ln.Mid(1));
						ch.patch.Add(ln.Mid(1));
					}
					else
					if(*ln == '+')
						ch.patch.Add(ln.Mid(1));
					else
					if(*ln == '-')
						ch.orig.Add(ln.Mid(1));
					else
						break;
					ch.src << ln << "\n";
				}
			}
		}
	}
	return true;
}

bool Patch::MatchFiles(const Vector<String>& dir, Progress& pi)
{
	const Vector<String>& h = file.GetKeys();
	if(h.GetCount() == 0)
		return false;
	common_path = h[0];
	for(int i = 1; i < h.GetCount(); i++)
		common_path.Trim(MatchLen(common_path, h[i]));
	common_path.TrimEnd("/");
	common_path.TrimStart("/");
	for(int i = 0; i < h.GetCount(); i++)
		file.SetKey(i, h[i].Mid(common_path.GetCount() + 1));

	pi.SetText("Matching directories");
	int best = 0;
	String com_path = common_path;
	while(com_path.GetCount()) {
		for(String d : dir) {
			while(d.GetCount() > 3) {
				if(pi.StepCanceled())
					return false;
				String dir = AppendFileName(d, com_path);
				int n = MatchCount(dir);
				if(n > best) {
					best = n;
					target_dir = dir;
				}
				d = GetFileFolder(d);
			}
		}
		int q = com_path.Find('/');
		if(q >= 0)
			com_path = com_path.Mid(q + 1);
		else
			break;
	}
	return best;
}

bool Patch::Load(const char *fn, Progress& pi)
{
	FileIn in(fn);
	return Load(in, pi);
}

String Patch::GetPatch(int i) const
{
	String r;
	for(const Chunk& ch : file[i])
		r << ch.src;
	return r;
}

String Patch::GetPatchedFile(int i) const
{
	if(GetFileLength(GetPath(i)) > 10000000)
		return String::GetVoid();
	Vector<String> lines;
	bool crlf = true;
	String path = GetPath(i);
	if(FileExists(path)) {
		String s = LoadFile(GetPath(i));
		crlf = s.Find('\r') >= 0;
		lines = Split(Filter(s, [](int c) { return c == '\r' ? 0 : c; }), '\n', false);
	}

	const Array<Chunk>& chs = file[i];
	Vector<int> ch_pos;

	for(int i = 0; i < chs.GetCount(); i++) {
		const Chunk& ch = chs[i];
		int pos = -1;
		for(int d = 0; d < 200; d++) { // 200 lines of tolerance in both directions when matching the patch
			auto Match = [&](int li) {
				if(li < 0)
					return false;
				for(int i = 0; i < ch.orig.GetCount(); i++) {
					if(li + i >= lines.GetCount() || ch.orig[i] != lines[li + i])
						return false;
				}
				return true;
			};
			if(Match(ch.line + d)) {
				pos = ch.line + d;
				break;
			}
			if(d && Match(ch.line - d)) {
				pos = ch.line - d;
				break;
			}
		}
		
		if(pos < 0 || ch_pos.GetCount() && ch_pos.Top() > pos)
			return String::GetVoid();
		ch_pos.Add(pos);
	}
	
	for(int i = 0; i < ch_pos.GetCount(); i++) {
		const Chunk& ch = chs[i];
		lines.Remove(ch_pos[i], ch.orig.GetCount());
		lines.Insert(ch_pos[i], ch.patch);
		int added = ch.patch.GetCount() - ch.orig.GetCount(); // negative if removed
		for(int j = i + 1; j < ch_pos.GetCount(); j++)
			ch_pos[j] += added;
	}
	
	return Join(lines, crlf ? "\r\n" : "\n");
}

};
