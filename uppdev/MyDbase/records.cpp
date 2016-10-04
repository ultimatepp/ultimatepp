#include "dbase.h"

int dbDatabase::GetRecord(const String &tbname, int recno, dbRecord& Record) {
  int h;

  if((h = GetTableHandle(tbname)) == MTDB_TB_NOT_FOUND)
  	return MTDB_TB_NOT_FOUND;

	return GetRecord(h, recno, Record);
}

int dbDatabase::GetRecord(unsigned int tbHandle, int recno, dbRecord& Record) {
  int i, r;
  char buff[1];
  Value v;
  
  if(recno > 0)
    tables[tbHandle].record = recno;
  else if(recno == MTDB_TB_ACT)
    recno = tables[tbHandle].record;
  else
    return MTDB_INV_RECNO;
  
  if(IsDeleted(tbHandle, recno)) return MTDB_REC_DELETED;
  Record.Reset();
	for(i=0; i<tables[tbHandle].numOfFields; i++) {
		if((r = FieldGet(tbHandle, recno, i, v)) == MTDB_SUCCESS) {
			ArrayMap<String, struct fieldStruct> &fs = fields[tbHandle];
			String s = fs[i].name;
			Record.Add(s, v);
		}
		else return r;
	}
	return MTDB_SUCCESS;
}

int dbDatabase::PutRecord(const String &tbname, int recno, dbRecord& Record, bool f) {
  int h;

#ifdef _WITH_DEBUG
RLOG("Put record: " + FormatInt(recno));
RLOG("Table: " + tbname);
#endif
  if((h = GetTableHandle(tbname)) == MTDB_TB_NOT_FOUND)
  	return MTDB_TB_NOT_FOUND;

	return PutRecord(h, recno, Record, f);
}

int dbDatabase::PutRecord(unsigned int tbHandle, int recno, dbRecord& Record, bool f) {
  unsigned int j, i, r;
  int z;
  char buff[2];

	ArrayMap<String, struct fieldStruct> &fs = fields[tbHandle];
  if( (recno > 0) && (recno <= tables[tbHandle].numOfRecord)) {
    if(IsDeleted(tbHandle, recno)) return MTDB_REC_DELETED;
    tables[tbHandle].record = recno;
  }
  else if((recno == MTDB_TB_APP) || (recno == tables[tbHandle].numOfRecord+1)) {
    tables[tbHandle].numOfRecord++;
    tables[tbHandle].record = tables[tbHandle].numOfRecord;
    recno = tables[tbHandle].record;
    Insert(tables[tbHandle].offset + tables[tbHandle].tableHead + ((recno-1) * tables[tbHandle].recordLenght), tables[tbHandle].recordLenght);
    eodsp += tables[tbHandle].recordLenght;
    for(r=tbHandle+1; r<numOfTables; r++) {
      tables[r].offset += tables[tbHandle].recordLenght;
    }
    dbUpdateHead();
    buff[0] = MTDB_TB_RECORD_ENABLED;
    lseek(fhandle, (int)tables[tbHandle].offset+tables[tbHandle].tableHead+(tables[tbHandle].recordLenght*(recno-1)), 0);
    write(fhandle, buff, 1);
    // verifico ed aggiorno la chiave primaria
    for(i=0; i<tables[tbHandle].numOfFields; i++) {
    	if (_isBitSet(fs[i].flags, MTDB_FD_PRIM_KEY)) {
				lseek(fhandle, (int)tables[tbHandle].offset+tables[tbHandle].tableHead+(tables[tbHandle].recordLenght*(recno-1))+fs[i].prevLength+1, 0);
				write(fhandle, &tables[tbHandle].autoincCntr, fs[i].length);
        tables[tbHandle].autoincCntr++;
  		}
    }
		// fine agg. chiave primaria
		tbUpdateHead(tbHandle);
  }
  else if(recno == MTDB_TB_ACT)
    recno = tables[tbHandle].record;
  else
    return MTDB_INV_RECNO;
  
#ifdef _WITH_DEBUG
RLOG("Put record: " + FormatInt(recno));
#endif

	for(j=0; j<Record.GetCount(); j++) {
		ArrayMap<String, struct fieldStruct> &fs = fields[tbHandle];
		z = fs.Find(Record.GetName(j));
		if(z >= 0) FieldPut(tbHandle, recno, z, Record[j], f);
	}

	Time t = GetSysTime();	
	lseek(fhandle, (int)tables[tbHandle].offset+21, 0);
	write(fhandle, &t, 8);
	tables[tbHandle].lastModifyTime = t;
	
	return MTDB_SUCCESS;
}
