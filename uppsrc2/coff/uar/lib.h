#ifndef _coff_uar_lib_h_
#define _coff_uar_lib_h_

class UARArchiveJob : public ArchiveJob
{
public:
	UARArchiveJob();

	static String  Usage();

	void           ReadCommand(const char *cmd);
	void           ReadCommand(const Vector<String>& cmdline);

	void           Build();

private:
	void           Print();
	void           Extract();
	void           Remove();

private:
	enum
	{
		DUMP_SYMBOLS           = 0x00000001,
		DUMP_SYMBOLS_DEMANGLED = 0x00000004,
	};
	int            dump_flags;
	enum COMMAND { CMD_ADD, CMD_REMOVE, CMD_EXTRACT, CMD_CONTENTS, CMD_PRINT };
	COMMAND        command;

	String         libraryfile;
	String         printfile;
	bool           delete_source_objects;
	bool           keep_archive_backups;
	bool           nologo;
};

#endif
