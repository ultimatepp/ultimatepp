struct ClangTidy {
	static String         path;
	static Index<String>  options;
	static Index<String>  groups;
	static Index<String>  active_checks;
	
	static bool   HasClangTidy();
	static String ClangTidyConfigPath();
	static void Load(const char *path);
	static void Save(const char *path);
	static void RunClangTidy(const Array<CompileCommand>& commands);
};

