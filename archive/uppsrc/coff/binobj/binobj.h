#ifndef _coff_binobj_binobj_h_
#define _coff_binobj_binobj_h_

#include <coff/coff.h>


NAMESPACE_UPP

class BinObjInfo {
public:
	BinObjInfo();

	void Parse(CParser& binscript, String base_dir);

	struct Block {
		Block() : index(-1), length(0), scriptline(-1), encoding(ENC_PLAIN), flags(0), offset(-1), len_meta_offset(-1) {}

		String ident;
		int    index;
		String file;
		int    length;
		int    scriptline;
		int    encoding;
		enum {
			ENC_PLAIN,
			ENC_ZIP,
			ENC_BZ2,
		};
		int    flags;
		enum {
			FLG_ARRAY = 0x01,
			FLG_MASK  = 0x02,
		};

		int    offset;
		int    off_meta_offset;
		int    len_meta_offset;
	};

	VectorMap< String, ArrayMap<int, Block> > blocks;
};

void BinaryToObject(String objectfile, CParser& binscript, String base_dir, Callback1<String> WhenConsole);

END_UPP_NAMESPACE

#endif
