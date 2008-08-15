#ifndef _src_dbase_dbDatabase_h_
#define _src_dbase_dbDatabase_h_

class dbRecord;
class dbRecordSet;

class dbDatabase {
protected:
						bool 				opened;
						String			name;
						String			fileName;
	unsigned	int					numOfTables;
						int					fhandle;
	unsigned	int					eodsp;
	unsigned 	int					fsize;
						Time				creationTime;
						Time				lastModifyTime;

	struct fieldStruct
	{
    					String		name;
    					String 		title;
		unsigned 	int   		length;
	  unsigned 	int	 			format;
	  unsigned 	int  			prevLength;
    		 			char  		flags;
    unsigned	char			nDecimals;
	};

	struct tableStruct
	{
    					String		name;		
							String    title;
    unsigned 	int 			numOfRecord;
    unsigned 	int  			recordLenght;
    unsigned 	int  			tableHead;
    unsigned 	int  			numOfFields;
    unsigned 	int 			record;
    unsigned 	int 			autoincCntr;
    unsigned	int				offset;
    					Time			creationTime;
    					Time			lastModifyTime;
	};
	
	ArrayMap<String, ArrayMap<String, struct fieldStruct> > fields;
	ArrayMap<String, struct tableStruct> tables;

protected:
	int							tbOpen(unsigned int handle);
	void						dbUpdateHead();
	int 						tbUpdateHead(unsigned int h);
	int 						tbPack(unsigned int h);
	void						Insert(unsigned int start, unsigned int length);
	void						Remove(unsigned int start, unsigned int length);
	void						SetEodsp(unsigned int v)						{ eodsp = v; }

	int 						FieldGet(unsigned int tbHandle, int recno, unsigned int field, Value& result);
	int 						FieldPut(unsigned int tbHandle, int recno, unsigned int field, Value value, bool f=false, bool p=false);	
	
public:
	int 						Open(const String &dbname);
	void						Close();
	int							ReOpen();
	unsigned int		Size()															{ return fsize; }
	String					GetFileName()												{ return fileName; }
	unsigned int		GetEodsp()													{ return eodsp; }
	bool						IsOpen()														{ return opened; }
	
	Time						GetCreationTime() 									{ return creationTime; }
	Time						GetModifyTime() 										{ return lastModifyTime; }	
	
	int							GetTableHandle(const String &tbname);
	unsigned int		GetNumOfTables() 										{ return numOfTables; }
	unsigned int		GetCount()		 											{ return numOfTables; }
	String  				GetTableName(unsigned int tbHandle)	{ return tables[tbHandle].name; }
	int							MakeDB(const String &dbname);
	int							Pack();
	
	Time						GetTableCreationTime(unsigned int tbHandle) { return tables[tbHandle].creationTime; }
	Time						GetTableModifyTime(unsigned int tbHandle) 	{ return tables[tbHandle].lastModifyTime; }

	int 						CopyTableStructure(const String &tbname, const String &dbdest, const String &tbdest);
	int 						MakeTable(const String &tbname);
	int 						AlterTable(const String &tbname);	
	int 						TableZap(const String &tbname);
	int							AddNewTable(const String &tbname, const String &comment, unsigned int numOfField);
	int							MakeField(const String &tbname, const String &field, const String &comment, unsigned int format, unsigned int length=0, char flags=0, unsigned	char nDecimals=0);
	int							AddField(const String &tbname, const String &field, const String &comment, unsigned int format, unsigned int length=0, char flags=0, unsigned	char nDecimals=0);							
	int							SetField(const String &tbname, unsigned int column, const String &field, const String &comment, unsigned int format, unsigned int length=0, char flags=0, unsigned	char nDecimals=0);
	int							RemoveField(const String &tbname, const String &field);
	bool 						SwapFieldUp(const String &tb, unsigned int field);
	bool 						SwapFieldDown(const String &tb, unsigned int field);
	
	int 						GetNumOfRec(const String &tbname);
	int							GetNumOfRec(unsigned int tbHandle)								{ return tables[tbHandle].numOfRecord; }
	String					GetTitle(unsigned int tbHandle)										{ return tables[tbHandle].title; }
	void						SetTitle(const String &tbname, const String &title);

	int							DeleteTable(int tbHandle);
	int							DeleteTable(const String &tbname);
	bool						IsTable(const String &tbname);
	
	String					GetFieldName(unsigned int h, unsigned int f);
	String					GetFieldTitle(unsigned int h, unsigned int f);
	unsigned int		GetFieldLength(unsigned int h, unsigned int f);
	unsigned int		GetFieldFormat(unsigned int h, unsigned int f);
	unsigned int		GetFieldFormat(const String &tbname, const String &field);
	char						GetFieldFlags(unsigned int h, unsigned int f);
	char						GetFieldFlags(const String &tbname, const String &field);
	unsigned char		GetFieldDecimals(unsigned int h, unsigned int f);
	unsigned char		GetFieldDecimals(const String &tbname, const String &field);
	bool						IsField(const String &tbname, const String &field);
	bool						IsField(unsigned int h, const String &field)						{ return IsField( tables[h].title, field); };
	
	unsigned int		GetNumOfField(unsigned int tbHandle)			{ return tables[tbHandle].numOfFields; }
	unsigned int		GetAutoincValue(unsigned int tbHandle)		{ return tables[tbHandle].autoincCntr; }

	bool						IsDeleted(unsigned int tbHandle, int recno);
	int 						GetRecord(const String &tbname, int recno, dbRecord& res);
	int 						GetRecord(unsigned int tbHandle, int recno, dbRecord& res);
	int 						PutRecord(const String &tbname, int recno, dbRecord& val, bool f=false);
	int 						PutRecord(unsigned int tbHandle, int recno, dbRecord& val, bool f=false);
	int 						GoToRec(const String &tbname, int value);
	int 						DelRec(const String &tbname, int recno);
	int 						DelRec(unsigned int tbHandle, int recno);
	
	//int							Query(const String &queryString, dbRecordSet &rs1);
	Value						Query(const String &queryString);
	void						ExecuteSelect(Vector<String> &tkn, dbRecordSet &result);
	int							ExecuteDelete(Vector<String> &tkn);
	int							ExecuteUpdate(Vector<String> &tkn);
	int							ExecuteInsert(Vector<String> &tkn);

	dbDatabase();
	~dbDatabase();
};

#endif
