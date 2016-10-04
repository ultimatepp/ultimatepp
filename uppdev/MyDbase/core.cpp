#include "dbase.h"

dbDatabase::dbDatabase() {
	
	numOfTables = 0;
	fhandle = -1;
	opened = false;
	return;
}

dbDatabase::~dbDatabase() {
	Close();
	return;
}

int dbDatabase::Open(const String &dbname) {
	FindFile tb;
	int i;
	unsigned int head;
	unsigned int offset;
	
	if(opened == true) return MDTB_DB_OPEN;
	head = 0;
	fileName = dbname;

	fsize = GetFileLength(fileName);
#ifdef _WITH_DEBUG
RLOG("dbDatabase::Open():fsize=" + FormatInt(fsize));
#endif	
  if((fhandle = open(fileName, _O_RDWR|_O_BINARY)) >= 0) {
	  lseek(fhandle, 0, 0);
	  read(fhandle, &head, 2);
	  if(head != MTDB_DB_TYPE) {
	  	close(fhandle);
	    return MTDB_DB_TYPE_ERROR; // il db aperto non e' compatibile
	  }
	  lseek(fhandle, 16, 0);
	  read(fhandle, &numOfTables, 4);
		read(fhandle, &eodsp, 4);
		read(fhandle, &creationTime, sizeof(Time));
		read(fhandle, &lastModifyTime, sizeof(Time));
				
	  offset = MTDB_DB_HEAD;
	  for(i=0; i<numOfTables; i++) {
	    struct tableStruct ts;
	    ts.offset = offset;
	    tables.Add("", ts);
	    
			tbOpen(i);
			offset += tables[i].tableHead + (tables[i].numOfRecord * tables[i].recordLenght);
		}
		opened = true;
		return fhandle;
  }
	else return -2;
}

void dbDatabase::Close() {
	if(opened == true) {	
		tables.Clear();
		fields.Clear();
		numOfTables = 0;
		close(fhandle);
		opened = false;
	}
	return;
}

int dbDatabase::tbOpen(unsigned int handle) {
  int i, p;

  tables[handle].numOfRecord = 0;
  tables[handle].tableHead = 0;
  tables[handle].recordLenght = 0;
  tables[handle].numOfFields = 0;
  tables[handle].record = 0;
  tables[handle].autoincCntr = 0;
  
  lseek(fhandle, tables[handle].offset, 0);
  read(fhandle, &tables[handle].creationTime, 8);
  lseek(fhandle, tables[handle].offset+9, 0);
  read(fhandle, &tables[handle].numOfRecord, 4);
  read(fhandle, &tables[handle].tableHead, 2);
  read(fhandle, &tables[handle].recordLenght, 2);
  read(fhandle, &tables[handle].autoincCntr, 4);
  read(fhandle, &tables[handle].lastModifyTime, 8);
  lseek(fhandle, tables[handle].offset+MTDB_TB_NAME_PTR, 0);
  String::Buffer n(MTDB_TB_NAME_LEN);
  read(fhandle, n, MTDB_TB_NAME_LEN);
  tables[handle].name = n;
  if((p = tables[handle].name.Find('\0')) >= 0) tables[handle].name.Trim(p);
	String::Buffer l(MTDB_TB_COMMENT_LEN);
	lseek(fhandle, tables[handle].offset+MTDB_TB_COMMENT_PTR, 0);
  read(fhandle, l, MTDB_TB_COMMENT_LEN);
  tables[handle].title = l;
  if((p = tables[handle].title.Find('\0')) >= 0) tables[handle].title.Trim(p);
 	    
  tables[handle].numOfFields = ((tables[handle].tableHead-(MTDB_TB_INFO_LEN+1)) / MTDB_TB_FD_LEN);
  ArrayMap<String, struct fieldStruct> field;
  if(tables[handle].numOfFields > 0) {
	  unsigned int prevLength = 0;
	  for(i=0; i<tables[handle].numOfFields; i++) {
	    struct fieldStruct fs;
		  fs.length = 0;
		  fs.format = 0;
		  fs.flags = 0;
		  lseek(fhandle, tables[handle].offset+MTDB_TB_INFO_LEN+(MTDB_TB_FD_LEN*i), 0);
		  String::Buffer n(MTDB_TB_FD_LABEL_LEN);
		  read(fhandle, n, MTDB_TB_FD_LABEL_LEN);
		  fs.name = n;
		  if((p = fs.name.Find('\0')) >= 0) fs.name.Trim(p);
		  read(fhandle, &fs.format, 1);
		  fs.format = fs.format << 8;
		  read(fhandle, &fs.length, 1);
		  read(fhandle, &fs.format, 1);
		  read(fhandle, &fs.flags, 1);
		  read(fhandle, &fs.nDecimals, 1);
		  String::Buffer l(MTDB_TB_COMMENT_LEN);
			lseek(fhandle, tables[handle].offset+MTDB_TB_INFO_LEN+(MTDB_TB_FD_LEN*i)+64, 0);
			read(fhandle, l, MTDB_TB_FLD_CMT_LEN);
			fs.title = l;
			if((p = fs.title.Find('\0')) >= 0) fs.title.Trim(p);
			fs.prevLength = prevLength;
			prevLength += fs.length;
			field.Add(fs.name, fs);
	  }
  }
  tables.SetKey(handle, tables[handle].name);

	int j;
	if((j = fields.Find(tables[handle].name)) >= 0)
		fields[j] = field;
	else
		fields.Add(tables[handle].name, field);
	
  return MTDB_SUCCESS;
}

void dbDatabase::dbUpdateHead() {

  lseek(fhandle, 16, 0);
  write(fhandle, &numOfTables, 4);
  write(fhandle, &eodsp, 4);
  lseek(fhandle, 32, 0);
  Time t = GetSysTime();
  write(fhandle, &t, 8);
  lastModifyTime = t;
	return;	
}

int dbDatabase::GetTableHandle(const String &tbname) {
	unsigned int i;
	
	for(i=0; i<numOfTables; i++) {
		if(tables[i].name == tbname) {
			return i;
		}
	}
	return MTDB_TB_NOT_FOUND; // table not found
}

bool dbDatabase::IsTable(const String &tbname)  {
	unsigned int i;
	
	for(i=0; i<numOfTables; i++) {
		if(tables[i].name == tbname) {
			return true;
		}
	}
	return false;
}

int dbDatabase::tbPack(unsigned int h) {
  unsigned int i;
  unsigned int pointer;
  
	for(i=1; i<=tables[h].numOfRecord; i++) {
		if(IsDeleted(h, i)) {
			pointer = tables[h].offset + tables[h].tableHead + ((i-1) * tables[h].recordLenght);
			Remove(pointer, tables[h].recordLenght);
			eodsp -= tables[h].recordLenght;
			tables[h].numOfRecord--;
			i--;
		}
	}
	dbUpdateHead();
	tbUpdateHead(h);
  ReOpen();
  return MTDB_SUCCESS;
}

int dbDatabase::CopyTableStructure(const String &tbname, const String &dbdest, const String &tbdest) {
	// if dbdest is empty, the copy is in the same database
  dbDatabase dbDest;
  unsigned int h, i, i1, i2, n;
  char buff[512];
  int handle;
  unsigned int eofPtr;

  if((h = GetTableHandle(tbname)) == MTDB_TB_NOT_FOUND)
  	return MTDB_TB_NOT_FOUND;

  // copy structure
  struct tableStruct newStru = tables[h];
	newStru.numOfRecord = 0;
	newStru.autoincCntr = 0;
	newStru.name = tbdest;

	if(dbdest.GetLength() > 0) {
		// bisognerebbe controllare che non sia aperto
 		handle = dbDest.Open(dbdest);
 		eofPtr = dbDest.GetEodsp();
 		dbDest.Insert(eofPtr, MTDB_TB_INFO_LEN+(MTDB_TB_FD_LEN*newStru.numOfFields)+1);
 		dbDest.SetEodsp(dbDest.GetEodsp() + (MTDB_TB_INFO_LEN+(MTDB_TB_FD_LEN*newStru.numOfFields)+1));
 		dbDest.dbUpdateHead();
 		n = dbDest.GetCount();
	}
	else {
		handle = fhandle;
		eofPtr = eodsp;
		Insert(eofPtr, MTDB_TB_INFO_LEN+(MTDB_TB_FD_LEN*newStru.numOfFields)+1);
		eodsp += (MTDB_TB_INFO_LEN+(MTDB_TB_FD_LEN*newStru.numOfFields)+1);
		dbUpdateHead();
		n = numOfTables;
	}

  newStru.offset = eofPtr;
	for(i=0; i<MTDB_TB_INFO_LEN; i++) buff[i] = 0;
	lseek(handle, newStru.offset+0, 0);
	write(handle, buff, MTDB_TB_INFO_LEN);

	Time t = GetSysTime();
	lseek(handle, newStru.offset, 0);
	write(handle, &t, 8);
	newStru.creationTime = t;
	lseek(handle, newStru.offset+9, 0);
	write(handle, &newStru.numOfRecord, 4);
	write(handle, &newStru.tableHead, 2);
	write(handle, &newStru.recordLenght, 2);
	write(handle, &newStru.autoincCntr, 4);
	write(handle, &t, 8);
	newStru.lastModifyTime = t;
	lseek(handle, newStru.offset+MTDB_TB_NAME_PTR, 0);
	write(handle, newStru.name, newStru.name.GetLength());
	lseek(handle, newStru.offset+MTDB_TB_COMMENT_PTR, 0);
	write(handle, newStru.title, newStru.title.GetLength());
	lseek(handle, newStru.offset+MTDB_TB_INFO_LEN, 0);
  
  ArrayMap<String, struct fieldStruct> &fm = fields[h];
  for(i1=0; i1<newStru.numOfFields; i1++) {
    strcpy(buff, fm[i1].name);
    for(i2=MTDB_TB_FD_LABEL_LEN+1; i2<MTDB_TB_FD_LEN; i2++) buff[i2] = 0;
    buff[MTDB_TB_FD_LABEL_LEN+0] = (char)(fm[i1].format >> 8);
    buff[MTDB_TB_FD_LABEL_LEN+1] = (char)(fm[i1].length);
    buff[MTDB_TB_FD_LABEL_LEN+2] = (char)(fm[i1].format);
    buff[MTDB_TB_FD_LABEL_LEN+3] = fm[i1].flags;
    buff[MTDB_TB_FD_LABEL_LEN+4] = (char)fm[i1].nDecimals;
    strcpy(&buff[64], fm[i1].title);
    write(handle, buff, MTDB_TB_FD_LEN);
  }
  i = n + 1;
  lseek(handle, 16, 0);
  write(handle, &i, 4);
  if(dbdest.GetLength() > 0) {
  	dbDest.Close();
  }
  else {
  	ReOpen();
  }
  return MTDB_SUCCESS;
}

int	dbDatabase::AddNewTable(const String &tbname, const String &comment, unsigned int numOfField) {
	int h, i;

	if(strlen((char*)~tbname) < 1) return MTDB_TB_NAME_ERROR;
	
  if((h = GetTableHandle(tbname)) == MTDB_TB_NOT_FOUND) {
    struct tableStruct ntb;
    ntb.name = tbname;
    ntb.title = comment;
    tables.Add(tbname, ntb);
    ArrayMap<String, struct fieldStruct> fs;
    for(i=0; i<numOfField; i++) {
    	struct fieldStruct fd;
    	fs.Add("", fd);
    }
    fields.Add(tbname, fs);
		h = numOfTables;
		numOfTables++;
  	tables[h].offset = 0;
  	tables[h].numOfFields = numOfField;
  	return MTDB_SUCCESS;
  }
  else return MTDB_TB_EXIST;
}

void dbDatabase::SetTitle(const String &tbname, const String &title) {
	int h;
	
  if((h = GetTableHandle(tbname)) >= 0) {
		tables[h].title = title;
  }
}

int dbDatabase::MakeField(const String &tbname, const String &field, const String &comment, unsigned int format, unsigned int length, char flags, unsigned	char nDecimals) {
	int h, i;
	
  if((h = GetTableHandle(tbname)) == MTDB_TB_NOT_FOUND)
  	return MTDB_TB_NOT_FOUND;

	ArrayMap<String, struct fieldStruct>& fs = fields[h];
	struct fieldStruct *fd;
  for(i=0; i<tables[h].numOfFields; i++) {
    fd = &fs[i];
    if(fd->name.GetLength() > 0) {
    	if(fd->name == field) {
        break;
    	}
    }
    else {
      break;
    }
  }
	if(i < tables[h].numOfFields) {
		fd->name = field;
		fd->title = comment;
		fd->format = format;
		fd->length = length;
		fd->flags = flags;
		fd->nDecimals = nDecimals;
		return MTDB_SUCCESS;
	}
	return MTDB_TB_ERROR;
}

int dbDatabase::AddField(const String &tbname, const String &field, const String &comment, unsigned int format, unsigned int length, char flags, unsigned	char nDecimals) {
	int h;
	
  if((h = GetTableHandle(tbname)) == MTDB_TB_NOT_FOUND)
  	return MTDB_TB_NOT_FOUND;

	struct fieldStruct fd;
	fd.name = field;
	fd.title = comment;
	fd.format = format;
	fd.length = length;
	fd.flags = flags;
	fd.nDecimals = nDecimals;
	ArrayMap<String, struct fieldStruct> &fs = fields[h];
	fs.Add(field, fd);
	tables[h].numOfFields++;
	return MTDB_SUCCESS;
}

int	dbDatabase::SetField(const String &tbname, unsigned int column, const String &field, const String &comment, unsigned int format, unsigned int length, char flags, unsigned	char nDecimals) {
	int h, i;
	
  if((h = GetTableHandle(tbname)) == MTDB_TB_NOT_FOUND)
  	return MTDB_TB_NOT_FOUND;

	i = column;
	if(i < tables[h].numOfFields) {
		ArrayMap<String, struct fieldStruct> &fs = fields[h];
		fs[i].name = field;
		fs[i].title = comment;
		fs[i].format = format;
		fs[i].length = length;
		fs[i].flags = flags;
		fs[i].nDecimals = nDecimals;
		//fs.SetKey(i, field);
		return MTDB_SUCCESS;
	}
	return MTDB_TB_ERROR;								
}

int dbDatabase::RemoveField(const String &tbname, const String &field) {
	int h, i;
	
  if((h = GetTableHandle(tbname)) == MTDB_TB_NOT_FOUND)
  	return MTDB_TB_NOT_FOUND;

	ArrayMap<String, struct fieldStruct>& fs = fields[h];
	for(i=0; i<tables[h].numOfFields; i++) {
    if(fs.GetKey(i) == field) {
      fs.Remove(i);
			tables[h].numOfFields--;
			return MTDB_SUCCESS;
    }
	}
	return MTDB_TB_ERROR;
}

bool dbDatabase::IsField(const String &tbname, const String &field) {
	int h, i;
	
  if((h = GetTableHandle(tbname)) == MTDB_TB_NOT_FOUND)
  	return false;
  
  ArrayMap<String, struct fieldStruct>& fs = fields[h];
	for(i=0; i<tables[h].numOfFields; i++) {
    if(fs.GetKey(i) == field) {
			return true;
    }
	}
	return false;
}

int dbDatabase::MakeTable(const String &tbname) {
  char buff[512];
  unsigned int i1, i2, i3, h;

  if((h = GetTableHandle(tbname)) == MTDB_TB_NOT_FOUND)
  	return MTDB_TB_NOT_FOUND;

  tables[h].numOfRecord = 0;
  tables[h].autoincCntr = 0;
  Time t = GetSysTime();
  tables[h].creationTime = t;
  tables[h].lastModifyTime = t;
  i3 = 0;
  ArrayMap<String, struct fieldStruct> &fs = fields[h];
	for(i1=0; i1<tables[h].numOfFields; i1++) {
    if(fs[i1].format == MTDB_STR) {
      i3 += fs[i1].length;
    }
    else if(fs[i1].format == MTDB_INT) {
    	i3 += 4;
     	fs[i1].length = 4;
    }
    else if(fs[i1].format == MTDB_DOUBLE) {
      i3 += 8;
     	fs[i1].length = 8;
    }
    else if(fs[i1].format == MTDB_BOOL) {
      i3 += 1;
      fs[i1].length = 1;
    }
    else if(fs[i1].format == MTDB_DATETIME || fs[i1].format == MTDB_DATE || fs[i1].format == MTDB_TIME) {
      //i3 += sizeof(int);
      //fs[i1].length = sizeof(time_t);
      i3 += sizeof(Time);
      fs[i1].length = sizeof(Time);      
    }
    else if(fs[i1].format >= 0x0100 && fs[i1].format <= 0x02ff) {
      i3 += 4;
      fs[i1].length = 4;
    }
  }
  i3++; // aggiungo 1 byte per l'informazione su record cancellato o no.
  tables[h].tableHead = MTDB_TB_INFO_LEN+1+(tables[h].numOfFields*MTDB_TB_FD_LEN);
  tables[h].recordLenght = i3;
  if(tables[h].offset == 0) {
	  tables[h].offset = eodsp;
	  Insert(eodsp, MTDB_TB_INFO_LEN+(MTDB_TB_FD_LEN*tables[h].numOfFields)+1);
	  eodsp += (MTDB_TB_INFO_LEN+(MTDB_TB_FD_LEN*tables[h].numOfFields)+1);
  }
  else {
    eodsp = tables[h].offset + (MTDB_TB_INFO_LEN+(MTDB_TB_FD_LEN*tables[h].numOfFields)+1);
  }
  tbUpdateHead(h);
  for(i1=0; i1<tables[h].numOfFields; i1++) {
    strcpy(buff, fs[i1].name);
    for(i2=MTDB_TB_FD_LABEL_LEN+1; i2<MTDB_TB_FD_LEN; i2++) buff[i2] = 0;
    buff[MTDB_TB_FD_LABEL_LEN+0] = (char)(fs[i1].format >> 8);
    buff[MTDB_TB_FD_LABEL_LEN+1] = (char)(fs[i1].length);
    buff[MTDB_TB_FD_LABEL_LEN+2] = (char)(fs[i1].format);
    buff[MTDB_TB_FD_LABEL_LEN+3] = fs[i1].flags;
    buff[MTDB_TB_FD_LABEL_LEN+4] = (char)fs[i1].nDecimals;
    strcpy(&buff[64], fs[i1].title);
    write(fhandle, buff, MTDB_TB_FD_LEN);
  }
  dbUpdateHead();
	ReOpen();
  return MTDB_SUCCESS;
}

int dbDatabase::TableZap(const String &tbname) {
	int i;
	
	for(i=1; i<=GetNumOfRec(tbname); i++) {
		DelRec(tbname, i);
	}
	tbPack(GetTableHandle(tbname));
  return MTDB_SUCCESS;
}

int dbDatabase::tbUpdateHead(unsigned int h) {
	unsigned int head;
	char buff[512];
	int i;
	
	for(i=0; i<MTDB_TB_INFO_LEN; i++) buff[i] = 0;
	lseek(fhandle, tables[h].offset+0, 0);
	write(fhandle, buff, MTDB_TB_INFO_LEN);
	
	lseek(fhandle, tables[h].offset, 0);
	write(fhandle, &tables[h].creationTime, 8);
	lseek(fhandle, tables[h].offset+9, 0);
	write(fhandle, &tables[h].numOfRecord, 4);
	write(fhandle, &tables[h].tableHead, 2);
	write(fhandle, &tables[h].recordLenght, 2);
	write(fhandle, &tables[h].autoincCntr, 4);
	Time t = GetSysTime();
	write(fhandle, &t, 8);
	tables[h].lastModifyTime = t;
	lseek(fhandle, tables[h].offset+MTDB_TB_NAME_PTR, 0);
	write(fhandle, tables[h].name, MTDB_TB_NAME_LEN);
	lseek(fhandle, tables[h].offset+MTDB_TB_COMMENT_PTR, 0);
	write(fhandle, tables[h].title, MTDB_TB_COMMENT_LEN);
	lseek(fhandle, tables[h].offset+MTDB_TB_INFO_LEN, 0);
	return MTDB_SUCCESS;
}

int	dbDatabase::DeleteTable(const String &tbname) {
	int h;

  if((h = GetTableHandle(tbname)) == MTDB_TB_NOT_FOUND)
  	return MTDB_TB_NOT_FOUND;
	
	return DeleteTable(h);
}

int	dbDatabase::DeleteTable(int tbHandle) {
	unsigned int length, r;

	if(tbHandle+1 < numOfTables) {
		length = tables[tbHandle+1].offset - tables[tbHandle].offset;
	}
	else {
		length = eodsp - tables[tbHandle].offset;
	}
	Remove(tables[tbHandle].offset, length);
	eodsp -= length;
	numOfTables--;
	dbUpdateHead();
	ReOpen();
	return MTDB_SUCCESS;
}

int	dbDatabase::AlterTable(const String &tbname) {
	unsigned int length, r, h, i;

  if((h = GetTableHandle(tbname)) == MTDB_TB_NOT_FOUND)
  	return MTDB_TB_NOT_FOUND;

	struct tableStruct new_table_stru = tables[h];
	ArrayMap<String, struct fieldStruct> new_fields_stru = fields[h];
	tbOpen(h); // reload old structure for correct query
	dbRecordSet rs1;
	//carico tutto il contenuto della vecchia tabella
	rs1.Fetch(Query("SELECT * FROM " + tbname));
	//restore new table stru
	Vector<String> oldkey;
	for(int z=0; z<new_fields_stru.GetCount(); z++) {
		oldkey.Add(new_fields_stru.GetKey(z));
	}
	tables[h] = new_table_stru;
	fields[h] = new_fields_stru;
	if(h+1 < numOfTables) {
		length = tables[h+1].offset - tables[h].offset;
	}
	else {
		length = eodsp - tables[h].offset;
	}
	Remove(tables[h].offset, length);
	eodsp -= length;
	tables[h].offset = 0;
	MakeTable(tbname);
	h = numOfTables-1;
	for(int z=0; z<oldkey.GetCount(); z++) {
		fields[h].SetKey(z, oldkey[z]);
	}
	//riscrivo i datinella tabella con la nuova formattazione
	dbRecord rc1;
	for(i=0; i<rs1.GetCount(); i++) {
		rc1 = rs1.Get();	
		String query;
		query = "INSERT INTO " + tbname + " (";
		for(int nc=0; nc<rc1.GetCount(); nc++) {
			query.Cat(rc1.GetName(nc));
			query.Cat(",");
		}
		query.Cat(") VALUES (");
		for(int nc=0; nc<rc1.GetCount(); nc++) {
			query.Cat(CSVQuote(AsString(rc1.GetValue(nc))));
			query.Cat(",");
		}
		query.Cat(")");
#ifdef _WITH_DEBUG		
RLOGHEXDUMP(query, query.GetCount());
#endif
		Query(query);
		rs1.Next();
	}
	ReOpen();
	return MTDB_SUCCESS;
}

int dbDatabase::DelRec(const String &tbname, int recno) {
  int h;

  if((h = GetTableHandle(tbname)) == MTDB_TB_NOT_FOUND)
  	return MTDB_TB_NOT_FOUND;

	return DelRec(h, recno);
}

int dbDatabase::DelRec(unsigned int tbHandle, int recno) {
	char buff[1];

  if(recno == MTDB_TB_APP)
    return MTDB_INV_RECNO;
  else if(recno > tables[tbHandle].numOfRecord)
    return MTDB_INV_RECNO;
  if(recno > 0)
    tables[tbHandle].record = recno;
  else if(recno == MTDB_TB_ACT)
    recno = tables[tbHandle].record;
  else
    return MTDB_TB_NOPOINTER;

  lseek(fhandle, (int)tables[tbHandle].offset+tables[tbHandle].tableHead+(tables[tbHandle].recordLenght*(recno-1)), 0);
  buff[0] = MTDB_TB_RECORD_DELETED;
  write(fhandle, buff, 1);
  return MTDB_SUCCESS;
}

bool dbDatabase::IsDeleted(unsigned int tbHandle, int recno) {
	char buff[1];

	if(tables[tbHandle].numOfRecord < recno) return false;
	
  lseek(fhandle, (int)tables[tbHandle].offset+tables[tbHandle].tableHead+(tables[tbHandle].recordLenght*(recno-1)), 0);
  read(fhandle, buff, 1);
  if(buff[0] == MTDB_TB_RECORD_DELETED) return true;
	return false;
}

int dbDatabase::MakeDB(const String &dbname) {
	int handle;
	char buff[1024];
	int i;
	
	handle = open((char*)~dbname, _O_CREAT|_O_RDWR|_O_BINARY, S_IRUSR | S_IWUSR);
  for(i=0; i<1024; i++) buff[i] = 0;
  unsigned int head;
  unsigned char ver;
  head = MTDB_DB_TYPE;
  ver = MTDB_version_;
  numOfTables = 0;
  eodsp = 1024;
  lseek(handle, 0, 0);
  write(handle, &buff, 1024);
  lseek(handle, 0, 0);
  write(handle, &head, 2);
  write(handle, &ver, 1);
  lseek(handle, 16, 0);
  write(handle, &numOfTables, 4);
  write(handle, &eodsp, 4);
  Time t = GetSysTime();
  write(handle, &t, 8);
  write(handle, &t, 8);
  close(handle);

	return MTDB_SUCCESS;
}

int dbDatabase::Pack() {
	int i, r;
	
	// pack the fixed space tables
	for(i=0; i<numOfTables; i++) {
		r = tbPack(i);
		if(r != MTDB_SUCCESS) return r;
	}
	
	// pack the undefined size area
	int	newHandle;
	String tempFName;
	char mem1[65536];
	unsigned int l, n;
	unsigned int rest;
	dbDatabase dbx;
	Value v;
	
	tempFName = GetFileDirectory(fileName) + "temp.tmp";
	newHandle = open((char*)~tempFName, _O_CREAT|_O_RDWR|_O_BINARY, S_IRUSR | S_IWUSR);
	lseek(fhandle, 0, 0);
	lseek(newHandle, 0, 0);
	l = (eodsp/65536);
	rest = eodsp-(l*65536);
	while(l > 0) {
		read(fhandle, &mem1, 65536);
		write(newHandle, &mem1, 65536);
		l--;	
	}
	read(fhandle, &mem1, rest);
	write(newHandle, &mem1, rest);
	close(newHandle);
	dbx.Open(tempFName);
	for(l=0; l<numOfTables; l++) {
		ArrayMap<String, struct fieldStruct> &fs = fields[l];
		for(i=0; i<tables[l].numOfFields; i++) {
			if(fs[i].format >= 0x0100 && fs[i].format <= 0x02ff) {
				for(n=1; n<=tables[l].numOfRecord; n++) {
					if(FieldGet(l, n, i, v) == 0) {
						dbx.FieldPut(l, n, i, v, false, true);
					}
				}
			}
		}
	}
	dbx.Close();
	close(fhandle);
  FileDelete(fileName);
  FileMove((char*)~tempFName, fileName);
  fsize = GetFileLength(fileName);	
	fhandle = open(fileName, _O_RDWR|_O_BINARY);
	return MTDB_SUCCESS;
}

int dbDatabase::ReOpen() {
	String buff;
	
	buff.Cat(fileName);
	Close();
	return Open(buff);
}

void dbDatabase::Insert(unsigned int start, unsigned int length) {
	int	newHandle;
	String tempFName;
	char mem1[65536];
	unsigned int i;
	unsigned int rest;

	tempFName = GetFileDirectory(fileName) + "~temp.tmp";
	newHandle = open((char*)~tempFName, _O_CREAT|_O_RDWR|_O_BINARY, S_IRUSR | S_IWUSR);
	lseek(fhandle, 0, 0);
	lseek(newHandle, 0, 0);

	if(start > 0) {
		i = (start/65536);
		rest = start-(i*65536);
		while(i > 0) {
			read(fhandle, &mem1, 65536);
			write(newHandle, &mem1, 65536);
			i--;	
		}
		read(fhandle, &mem1, rest);
		write(newHandle, &mem1, rest);
	}
	for(i=0; i<65536; i++) mem1[i] = 0;
	i = (length/65536);
	rest = length-(i*65536);
	while(i > 0) {
		write(newHandle, &mem1, 65536);
		i--;	
	}
	write(newHandle, &mem1, rest);
	if(start<=fsize) {
		i=((fsize-start)/65536);
		rest=(fsize-start)-(i*65536);
		while(i > 0) {
			read(fhandle, &mem1, 65536);
			write(newHandle, &mem1, 65536);
			i--;	
		}
		read(fhandle, &mem1, rest);
		write(newHandle, &mem1, rest);
	}
	close(fhandle);
	close(newHandle);
  FileDelete(fileName);
  FileMove((char*)~tempFName, fileName);
  fsize = GetFileLength(fileName);	
	fhandle = open(fileName, _O_RDWR|_O_BINARY);
	return;	
}

void dbDatabase::Remove(unsigned int start, unsigned int length) {
	int	newHandle;
	String tempFName;
	char mem1[65536];
	unsigned int i;
	unsigned int rest;

	tempFName = GetFileDirectory(fileName) + "~temp.tmp";
	newHandle = open((char*)~tempFName, _O_CREAT|_O_RDWR|_O_BINARY, S_IRUSR | S_IWUSR);
	lseek(fhandle, 0, 0);
	lseek(newHandle, 0, 0);

	if(start > 0) {	
		i = (start/65536);
		rest = start - (i * 65536);
		while(i > 0) {
			read(fhandle, &mem1, 65536);
			write(newHandle, &mem1, 65536);
			i--;	
		}
		read(fhandle, &mem1, rest);
		write(newHandle, &mem1, rest);
	}
	
	lseek(fhandle, start+length, 0);	
	
	if((start+length) < fsize) {
		i = ((fsize-(start+length))/65536);
		rest = ((fsize-(start+length)) - (i * 65536));
		while(i > 0) {
			read(fhandle, &mem1, 65536);
			write(newHandle, &mem1, 65536);
			i--;	
		}
		read(fhandle, &mem1, rest);
		write(newHandle, &mem1, rest);
	}
	close(fhandle);
	close(newHandle);
  FileDelete(fileName);
  FileMove((char*)~tempFName, fileName);
  fsize = GetFileLength(fileName);	
#ifdef _WITH_DEBUG
RLOG("dbDatabase::Remove():fsize=" + FormatInt(fsize));
#endif
	fhandle = open(fileName, _O_RDWR|_O_BINARY);
	return;	
}

bool dbDatabase::SwapFieldUp(const String &tb, unsigned int field) {
	int h;

  if((h = GetTableHandle(tb)) == MTDB_TB_NOT_FOUND) return false;

	if(field > 0 && field <= fields[h].GetCount()) {
		struct fieldStruct field_buff = fields[h][field];
		String key_buff = fields[h].GetKey(field);
		fields[h][field] = fields[h][field-1];
		fields[h][field-1] = field_buff;
		fields[h].SetKey(field, fields[h].GetKey(field-1));
		fields[h].SetKey(field-1, key_buff);
		return true;
	}
	return false;
}

bool dbDatabase::SwapFieldDown(const String &tb, unsigned int field) {
	int h;

  if((h = GetTableHandle(tb)) == MTDB_TB_NOT_FOUND) return false;

	if(field < (fields[h].GetCount()-1) && field >= 0) {
		struct fieldStruct field_buff = fields[h][field];
		String key_buff = fields[h].GetKey(field);
		fields[h][field] = fields[h][field+1];
		fields[h][field+1] = field_buff;
		fields[h].SetKey(field, fields[h].GetKey(field+1));
		fields[h].SetKey(field+1, key_buff);
		return true;
	}
	return false;
}
