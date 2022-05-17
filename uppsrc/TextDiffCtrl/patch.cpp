#include "TextDiffCtrl.h"

namespace Upp {

bool Patch::Load0(Stream& in, Progress& pi)
{
	pi.SetText("Loading patch file");
	target_dir.Clear();
	file.Clear();
	String ln0;
	String ln = in.GetLine();
	bool ok = false; // at least one hunk was loaded
	for(;;) {
		while(!ln.TrimStart("--- ")) {
			if(pi.StepCanceled())
				return false;
			if(in.IsEof())
				return ok;
			ln = in.GetLine();
			if(ln.StartsWith("-- ")) // git format-patch "EOF"
				return ok;
		}
		ln = TrimLeft(ln);
		int q = ln.Find('\t');
		if(q >= 0)
			ln.Trim(q);
		String fn = UnixPath(TrimLeft("\"", TrimRight("\"", ln)));
		Array<Chunk>& fp = file.GetAdd(fn);
		while(!ln.StartsWith("@@")) {
			if(pi.StepCanceled())
				return false;
			if(in.IsEof())
				return ok;
			ln0 = ln = in.GetLine();
			if(ln.StartsWith("-- ")) // git format-patch "EOF"
				return ok;
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
						return ok;
					ln0 = ln = in.GetLine();
					if(ln.StartsWith("-- ")) // git format-patch "EOF"
						return ok;
					if(ln.StartsWith("--- "))
						break;
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
					ok = true;
					ch.src << ln << "\n";
				}
			}
		}
	}

	return ok;
}

bool Patch::Load(Stream& in, Progress& pi)
{
	common_path.Clear();
	if(!Load0(in, pi))
		return false;
	const Vector<String>& h = file.GetKeys();
	if(h.GetCount() == 0)
		return false;
	common_path = GetFileFolder(h[0]);
	for(int i = 1; i < h.GetCount(); i++)
		common_path.Trim(MatchLen(common_path, h[i]));
	common_path.TrimEnd("/");
	common_path.TrimStart("/");
	if(common_path.GetCount())
		for(int i = 0; i < h.GetCount(); i++)
			file.SetKey(i, h[i].Mid(common_path.GetCount() + 1));
	return true;
}

int Patch::MatchLen(const String& a, const String& b)
{
	int n = min(a.GetLength(), b.GetLength());
	int q = 0;
	for(int i = 0; i < n; i++) {
		if(a[i] != b[i])
			return q;
		if(a[i] == '/')
			q = i;
	}
	return a[n] || b[n] ? b[n] == '/' || a[n] == '/' ? n : q : n;
}

int Patch::MatchCount(const char *dir)
{
	int count = 0;
	for(const String& p : file.GetKeys()) {
		if(FileExists(AppendFileName(dir, p)))
			count++;
	}
	return count;
}

bool Patch::MatchFiles(const Vector<String>& dir, Progress& pi)
{
	pi.SetText("Matching directories");
	int best = 0;
	String com_path = common_path;
	com_path.Replace("\\", "/");
	if(dir.GetCount())
		for(int pass = 0; pass < 2; pass++) {
			for(;;) {
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
			com_path = GetFileFolder(dir[0]);
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

String Patch::GetPatchedFile(int i, const String& text) const
{
	if(GetFileLength(GetPath(i)) > 4 * 1024 * 1024)
		return String::GetVoid();
	Vector<String> lines;
	bool crlf = true;
	String path = GetPath(i);
	if(FileExists(path) || IsNull(text)) {
		String s = Nvl(text, LoadFile(GetPath(i)));
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
				for(int i = 0; i < ch.orig.GetCount(); i++)
					if(li + i >= lines.GetCount() || ch.orig[i] != lines[li + i])
						return false;
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
