#include <RichEdit/RichEdit.h>

struct FindBrokenRefIterator : RichTxt::Iterator {
	int cursor;
	Uuid itemstyle;

	virtual bool operator()(int pos, const RichPara& para)
	{
		if(pos >= cursor) {
			if(para.format.label != "noref") {
				if(IsNull(para.format.label))
					return true;
				String nest;
				String key;
				if(!SplitNestKey(para.format.label, nest, key))
					return true;
				int q = BrowserBase().Find(nest);
				if(q < 0 || BrowserBase()[q].key.Find(key) < 0)
					return true;
			}
			cursor = pos;
		}
		return false;
	}

	FindBrokenRefIterator() {
	}
};

