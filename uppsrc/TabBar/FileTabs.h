class FileTabs : public TabBar
{
private:
	bool stackedicons:1;
	bool greyedicons:1;
	Color filecolor;
	Color extcolor;
protected:
	// Overload this to change grouping behaviour
	virtual String GetFileGroup(const String &file);
	// Overload this to change stacking behaviour
	virtual String GetStackId(const Tab &a);
	virtual hash_t GetStackSortOrder(const Tab &a);	
	
	virtual void ComposeTab(Tab& tab, const Font &font, Color ink, int style);
	virtual void ComposeStackedTab(Tab& tab, const Tab& stacked_tab, const Font &font, Color ink, int style);
	virtual Size GetStackedSize(const Tab &t);
	
public:
	FileTabs();

	virtual void Serialize(Stream& s);

	void  	AddFile(const WString &file, bool make_active = true);
	void  	AddFile(const WString &file, Image img, bool make_active = true);
	void  	InsertFile(int ix, const WString &file, bool make_active = true);
	void  	InsertFile(int ix, const WString &file, Image img, bool make_active = true);

	void 	AddFiles(const Vector<String> &files, bool make_active = true);
	void  	AddFiles(const Vector<String> &files, const Vector<Image> &img, bool make_active = true);
	void  	InsertFiles(int ix, const Vector<String> &files, bool make_active = true);
	void  	InsertFiles(int ix, const Vector<String> &files, const Vector<Image> &img, bool make_active = true);
	
	void 	RenameFile(const WString &from, const WString &to, Image icon = Null);
	
	FileTabs& FileColor(Color c) 	{ filecolor = c; Refresh(); return *this; }
	FileTabs& ExtColor(Color c) 	{ extcolor = c; Refresh(); return *this; }
	
	FileTabs& FileIcons(bool normal = true, bool stacked = true, bool stacked_greyedout = true);
	
	Vector<String>	GetFiles() const;
	FileTabs& operator<<(const FileTabs &src);
};
