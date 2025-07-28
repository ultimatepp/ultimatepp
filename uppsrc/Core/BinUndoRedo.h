String BinDiff(const String& base, const String& data);
String BinUndiff(const String& base, const String& bin_diff);

class BinUndoRedo {
	struct Entry : Moveable<Entry> {
		String data;
		String ids;
	};
	Entry          commit;
	Vector<Entry>  undo;
	Vector<Entry>  redo;
	int            undosize = 0;

public:
	void   Reset(const String& current, const String& ids = String());
	bool   Commit(const String& current, const String& ids, int limit = 4096*1024);
	bool   Commit(const String& current, int limit = 4096*1024) { return Commit(current, String(), limit); }
	bool   IsUndo() const                                       { return undo.GetCount(); }
	bool   IsRedo() const                                       { return redo.GetCount(); }
	String Undo(const String& current);
	String Redo(const String& current);
	void   Ids(Event<const String&> fn);
};
