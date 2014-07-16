#ifndef _OfficeAutomation_OfficeAutomationMethods_h_
#define _OfficeAutomation_OfficeAutomationMethods_h_


#define Sheet_METHOD_LIST					\
	virtual bool AddSheet(bool visible);					\
	virtual bool OpenSheet(String fileName, bool visible);	\
															\
	virtual bool SetValue(int col, int row, Value value);	\
	virtual bool SetValue(String cell, Value value);		\
	virtual bool SetValue(Value value);						\
	virtual Value GetValue(int col, int row);				\
	virtual Value GetValue(String cell);					\
	virtual Value GetText(int col, int row);				\
	virtual Value GetText(String cell);						\
	virtual bool SetHyperlink(int col, int row, String address, String text);	\
	virtual bool SetHyperlink(String cell, String address, String text);	\
	virtual bool SetHyperlink(String address, String text);					\
	virtual bool Replace(Value search, Value replace);		\
															\
	virtual bool SetBold(int col, int row, bool bold);		\
	virtual bool SetBold(String cell, bool bold);			\
	virtual bool SetBold(bool bold);						\
	virtual bool SetItalic(int col, int row, bool italic);	\
	virtual bool SetItalic(String cell, bool italic);		\
	virtual bool SetItalic(bool italic);					\
	virtual bool SetUnderline(bool underline);				\
	virtual bool SetUnderline(String cell, bool underline);	\
	virtual bool SetUnderline(int col, int row, bool underline);	\
	virtual bool SetFont(int col, int row, String name, int size);	\
	virtual bool SetFont(String cell, String name, int size);		\
	virtual bool SetFont(String name, int size);			\
	virtual bool SetColor(int col, int row, Color color);	\
	virtual bool SetColor(String cell, Color color);		\
	virtual bool SetColor(Color color);						\
	virtual bool SetBackColor(int col, int row, Color color);	\
	virtual bool SetBackColor(String cell, Color color);	\
	virtual bool SetBackColor(Color color);					\
															\
	virtual bool SetRowHeight(int row, double height);		\
	virtual bool SetColWidth(int col, double width);		\
															\
	enum {LEFT = 0, CENTER, RIGHT, JUSTIFY, TOP, BOTTOM, MAX_JUSTIFY};	\
																		\
	virtual bool SetHorizAlignment(String cell, int alignment);			\
	virtual bool SetHorizAlignment(int col, int row, int alignment);	\
	virtual bool SetVertAlignment(String cell, int alignment);			\
	virtual bool SetVertAlignment(int col, int row, int alignment); 	\
																		\
	enum {BORDER_DIAG_DOWN = 0, BORDER_DIAG_UP, BORDER_LEFT, BORDER_TOP, BORDER_BOTTOM, BORDER_RIGHT};	\
	enum {HAIRLINE = 0, MEDIUM, THIN, THICK};					\
	enum {NONE = 0, CONTINUOUS, DASH, DASHDOT, DOT};			\
																\
	virtual bool SetBorder(int borderIndx, int lineStyle, int weight, Color color);						\
	virtual bool SetBorder(int col, int row, int borderIndx, int lineStyle, int weight, Color color);	\
																	\
	virtual bool Select(String range);								\
	virtual bool Select(int fromX, int fromY, int toX, int toY);	\
	virtual bool Select();											\
	virtual bool EnableCommandVars(bool enable); 				\
																\
	virtual bool MatrixAllocate(int width, int height);			\
	virtual bool MatrixDelete();								\
	virtual bool MatrixFillSelection();							\
	virtual bool MatrixFill(int fromX, int fromY, Vector<Vector < ::Value> > &data);		\
	virtual bool MatrixSetValue(int i, int j, ::Value value);							\
														\
	virtual bool Print();								\
														\
	virtual bool SaveAs(String fileName, String type);	\
	virtual bool SetSaved(bool);						\
	virtual bool Quit();								\
														\
	virtual bool InsertTab(String name);				\
	virtual bool ChooseTab(String name);				\
	virtual bool ChooseTab(int index);					\
	virtual bool RemoveTab(String name);				\
	virtual bool RemoveTab(int index);					\
	virtual int GetNumTabs();
	
	
#define Doc_METHOD_LIST										\
	virtual bool AddDoc(bool visible);						\
	virtual bool OpenDoc(String fileName, bool visible);	\
															\
	virtual bool SetFont(String font, int size);			\
	virtual bool SetBold(bool bold);						\
	virtual bool SetItalic(bool italic);					\
	virtual bool WriteText(String value);					\
															\
	virtual bool Select();									\
	virtual bool EnableCommandVars(bool enable); 			\
															\
	virtual bool Replace(String search, String replace);	\
															\
	virtual bool Print();									\
															\
	virtual bool SetSaved(bool saved);						\
	virtual bool SaveAs(String fileName, String type = "doc");	\
	virtual bool Quit();
	
	

#endif
