String BinDiff(const String& base, const String& data);
String BinUndiff(const String& base, const String& bin_diff);

class BinUndoRedo {
	struct Entry : Moveable<Entry> {
		String data;
		Value  info;
	};
	Entry          commit;
	Vector<Entry>  undo;
	Vector<Entry>  redo;
	int            undosize = 0;

public:
	void   Reset(const String& current, const Value& info = Value());
	bool   Commit(const String& current, const Value& info, int limit = 4096*1024);
	bool   Commit(const String& current, int limit = 4096*1024) { return Commit(current, String(), limit); }
	bool   IsUndo() const                                       { return undo.GetCount(); }
	bool   IsRedo() const                                       { return redo.GetCount(); }
	int    GetUndoCount() const                                 { return undo.GetCount(); }
	int    GetRedoCount() const                                 { return redo.GetCount(); }
	bool   DropUndo();
	bool   DropRedo();
	Value  GetUndoInfo(int i) const                             { return undo[i].info; }
	Value  GetRedoInfo(int i) const                             { return redo[i].info; }
	Value  GetCommitInfo() const                                { return commit.info; }
	String Undo(const String& current);
	String Redo(const String& current);
};
