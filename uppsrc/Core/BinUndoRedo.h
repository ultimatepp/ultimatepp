String BinDiff(const String& base, const String& data);
String BinUndiff(const String& base, const String& bin);

class BinUndoRedo {
	String         commit;
	Vector<String> undo;
	Vector<String> redo;
	int            undosize = 0;

public:
	void   Reset(const String& current);
	bool   Commit(const String& current, int limit = 4096*1024);
	bool   IsUndo() const               { return undo.GetCount(); }
	bool   IsRedo() const               { return redo.GetCount(); }
	String Undo(const String& current);
	String Redo(const String& current);
};
