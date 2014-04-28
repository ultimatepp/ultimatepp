#include "Core.h"

BinObjInfo::BinObjInfo()
{
}

void BinObjInfo::Parse(CParser& binscript, String base_dir)
{
	while(!binscript.IsEof()) {
		String binid = binscript.ReadId();
		bool ba = (binid == "BINARY_ARRAY");
		bool bm = (binid == "BINARY_MASK");
		if(binid == "BINARY" || ba || bm) {
			binscript.PassChar('(');
			Block blk;
			blk.scriptline = binscript.GetLine();
			blk.ident = binscript.ReadId();
			ArrayMap<int, Block>& brow = blocks.GetAdd(blk.ident);
			binscript.PassChar(',');
			blk.index = -1;
			if(ba) {
				blk.flags |= Block::FLG_ARRAY;
				blk.index = binscript.ReadInt();
				if(blk.index < 0 || blk.index > 1000000)
					binscript.ThrowError(NFormat("invalid array index: %d", blk.index));
				binscript.PassChar(',');
			}
			else if(bm) {
				blk.flags |= Block::FLG_MASK;
				blk.index = brow.GetCount();
			}
			String file = binscript.ReadString();
			if(binscript.Id("ZIP"))
				blk.encoding = Block::ENC_ZIP;
			else if(binscript.Id("BZ2"))
				blk.encoding = Block::ENC_BZ2;
			binscript.PassChar(')');
			FindFile ff;
			String searchpath = NormalizePath(file, base_dir);
			String searchdir = GetFileDirectory(searchpath);
			Vector<String> files;
			Vector<int64> lengths;
			if(ff.Search(searchpath))
				do
					if(ff.IsFile()) {
						files.Add(ff.GetName());
						lengths.Add(ff.GetLength());
					}
				while(ff.Next());
			if(files.IsEmpty())
				binscript.ThrowError(NFormat("'%s' not found or not a file", file));
			if(!(blk.flags & Block::FLG_MASK) && files.GetCount() > 1)
				binscript.ThrowError(NFormat("Multiple files found (e.g. %s, %s) in single file mode", files[0], files[1]));
			IndexSort(files, lengths);
			for(int i = 0; i < files.GetCount(); i++) {
				blk.file = AppendFileName(searchdir, files[i]);
				blk.length = (int)lengths[i];
				int f = brow.Find(blk.index);
				if(f >= 0)
					binscript.ThrowError(NFormat("%s[%d] already seen at line %d", blk.ident, blk.index, brow[f].scriptline));
				if(blk.index < 0 && !brow.IsEmpty() || blk.index >= 0 && !brow.IsEmpty() && brow[0].index < 0)
					binscript.ThrowError(NFormat("%s: mixing non-array and array elements", blk.ident));
				brow.Add(blk.index, blk);
				if(!(blk.flags & Block::FLG_MASK))
					break;
				blk.index++;
			}
		}
		else
			binscript.ThrowError("binary script item identifier expected");
	}
}
