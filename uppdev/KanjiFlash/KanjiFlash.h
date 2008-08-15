#ifndef _KanjiFlash_KanjiFlash_h
#define _KanjiFlash_KanjiFlash_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <KanjiFlash/KanjiFlash.lay>
#include <CtrlCore/lay.h>

#include "Kanji.h"

class KanjiFlash : public WithKanjiFlashLayout<TopWindow> {
public:
	typedef KanjiFlash CLASSNAME;

	String filename;

	FileSel fs;

	Vector<Kanji> list;

	KanjiFlash();

	void Open();
	void Select();
	void Clear() { lstKanji.Clear(); }
	void PopulateTree();
	void TreeClick();
};

#endif
