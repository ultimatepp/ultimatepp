#ifndef _DropGrid_DropGrid_h_
#define _DropGrid_DropGrid_h_

#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>

namespace Upp {

class DropGrid : public Convert, public GridDisplay, public Ctrl
{
	public:
		enum
		{
			BTN_SELECT,
			BTN_LEFT,
			BTN_RIGHT,
			BTN_UP,
			BTN_DOWN,
			BTN_PLUS,
			BTN_CLEAN
		};

		class PopUpGrid : public GridCtrl
		{
			private:

				void CloseNoData();
				void CloseData();

			public:
				Event<> WhenPopDown;
				Event<> WhenClose;
				Event<> WhenCloseData;
				Event<> WhenCloseNoData;

				typedef PopUpGrid CLASSNAME;

				virtual void Deactivate();
				void PopUp(Ctrl *owner, const Rect &r);

				PopUpGrid();
		};

	protected:

		int key_col;
		int find_col;
		int value_col;
		Vector<int> value_cols;
		PopUpGrid list;
		MultiButtonFrame drop;
		GridButton clear;

	private:
	
		int rowid;
		int trowid;
		Value value;
		bool change;

		int list_width;
		int list_height;
		int drop_lines;

		bool display_all:1;
		bool header:1;
		bool valuekey:1;
		bool notnull:1;
		bool display_columns:1;
		bool drop_enter:1;
		bool data_action:1;
		bool searching:1;
		bool must_change:1;
		bool null_action:1;
		bool clear_button:1;
		bool nodrop:1;

		GridDisplay *display;
		
		String must_change_str;

		void Change(int dir);
		void SearchCursor();
		void DoAction(int row, bool action = true, bool chg = true);
		Vector<String> MakeVector(int r) const;
		Value MakeValue(int r = -1, bool columns = true) const;
		void UpdateValue();
		Value Format0(const Value& q, int rowid) const;

	public:

		typedef DropGrid CLASSNAME;
		DropGrid();

		void Close();
		void CloseData();
		void CloseNoData();
		void Drop();

		DropGrid& Width(int w);
		DropGrid& Height(int h);
		DropGrid& SetKeyColumn(int n);
		DropGrid& SetFindColumn(int n);
		DropGrid& SetValueColumn(int n);
		DropGrid& AddValueColumn(int n);
		DropGrid& AddValueColumns(int first = -1, int last = -1);
		DropGrid& DisplayAll(bool b = true);
		DropGrid& SetDropLines(int d);
		DropGrid& Header(bool b = true);
		DropGrid& NoHeader();
		DropGrid& Resizeable(bool b = true);
		DropGrid& ColorRows(bool b = true);
		DropGrid& NotNull(bool b = true);
		DropGrid& ValueAsKey(bool b = true);
		DropGrid& SetDisplay(GridDisplay &d);
		DropGrid& DisplayColumns(bool b = true);
		DropGrid& DropEnter(bool b = true);
		DropGrid& DataAction(bool b = true);
		DropGrid& Searching(bool b = true);
		DropGrid& MustChange(bool b = true, const char* s = "");
		DropGrid& NullAction(bool b = true);
		DropGrid& ClearButton(bool b = true);
		DropGrid& NoDrop(bool b = true);
		DropGrid& SetData();
		DropGrid& SearchHideRows(bool b = true);

		GridCtrl::ItemRect& AddColumn(const char *name, int width = -1, bool idx = false);
		GridCtrl::ItemRect& AddColumn(Id id, const char *name, int width = -1, bool idx = false);
		GridCtrl::ItemRect& AddIndex(const char *name = "");
		GridCtrl::ItemRect& AddIndex(Id id);

		MultiButton::SubButton& AddButton(int type, const Event<> &cb);
		MultiButton::SubButton& AddSelect(const Event<> &cb);
		MultiButton::SubButton& AddPlus(const Event<> &cb);
		MultiButton::SubButton& AddEdit(const Event<> &cb);
		MultiButton::SubButton& AddClear();
		MultiButton::SubButton& AddText(const char* label, const Event<>& cb);
		MultiButton::SubButton& GetButton(int n);
		
		int AddColumns(int cnt);

		void GoTop();

		int SetIndex(int n);
		int GetIndex() const;

		int GetCount() const;

		void Reset();
		void Clear();
		void Ready(bool b = true);
		void ClearValue();
		void DoClearValue();

		virtual Value GetData() const;
		virtual void SetData(const Value& v);

		Value GetValue() const;
		Value GetValue(int r) const;
		Value FindValue(const Value& v) const;
		Vector<String> FindVector(const Value& v) const;
		bool FindMove(const Value& v);
		Value GetKey() const;

		virtual bool Key(dword k, int);
		virtual void Paint(Draw& draw);
		virtual void LeftDown(Point p, dword keyflags);
		virtual void GotFocus();
		virtual void LostFocus();
		virtual void Serialize(Stream& s);
		virtual bool Accept();
		virtual Size GetMinSize() const;
		virtual void State(int reason);

		void Paint0(Draw &w, int lm, int rm, int x, int y, int cx, int cy, const Value &val, dword style, Color &fg, Color &bg, Font &fnt, bool found = false, int fs = 0, int fe = 0);
		virtual void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style, Color &fg, Color &bg, Font &fnt, bool found = false, int fs = 0, int fe = 0);

		Value Get(int c) const;
		Value Get(Id id) const;
		Value Get(int r, int c) const;
		Value Get(int r, Id id) const;
		Value GetPrev(int c) const;
		Value GetPrev(Id id) const;
		void  Set(int c, const Value& v);
		void  Set(Id id, const Value& v);
		void  Set(int r, int c, const Value& v);
		void  Set(int r, Id id, const Value& v);
		void  Set(int r, const Vector<Value> &v, int data_offset = 0, int column_offset = 0);
		void  Add(const Vector<Value> &v, int offset = 0, int count = -1, bool hidden = false);
		String GetString(Id id);

		Value& operator() (int r, int c);
		Value& operator() (int c);
		Value& operator() (Id id);
		Value& operator() (int r, Id id);

		GridCtrl::ItemRect& GetRow(int r);

		bool IsSelected();
		bool IsEmpty();
		bool IsChange();
		bool IsInit();
		
		void ClearChange();

		int Find(const Value& v, int col = 0, int opt = 0);
		int Find(const Value& v, Id id, int opt = 0);
		int GetCurrentRow() const;

		void CancelUpdate();

		GridCtrl& GetList() { return list; }

		virtual Value Format(const Value& q) const;
		
		Event<> WhenLeftDown;
		Event<> WhenDrop;

		GridCtrl::ItemRect& AddRow(int n = 1, int size = -1);
		DropGrid& Add() { AddRow(); return *this; }

		//$-DropCtrl& Add(const Value& [, const Value& ]...);
		#define  E__Add(I)      DropGrid& Add(__List##I(E__Value));
			__Expand(E__Add)
		#undef   E__Add
		//$+

		//$-GridCtrl::ItemRect& Add(const Value& [, const Value& ]...);
		#define  E__Add(I)      GridCtrl::ItemRect& AddRow(__List##I(E__Value));
			__Expand(E__Add)
		#undef   E__Add
		//$+

		DropGrid& AddSeparator(Color c);

};

}

#endif
