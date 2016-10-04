#include "dbase.h"

unsigned int dbDatabase::GetFieldFormat(const String &tbname, const String &field) {
	unsigned int h, t;

  if((h = GetTableHandle(tbname)) != MTDB_TB_NOT_FOUND) {
    ArrayMap<String, struct fieldStruct> &fs = fields[h];
    for(t=0; t<tables[h].numOfFields; t++) {
    	if(fs.GetKey(t) == field) {
        return GetFieldFormat(h, t);
    	}
  	}
  }
  else return 0;
}

unsigned int dbDatabase::GetFieldFormat(unsigned int h, unsigned int f) {
	if(h < numOfTables && f < tables[h].numOfFields) {
		ArrayMap<String, struct fieldStruct> &fs = fields[h];
		return fs[f].format;
	}
	else return 0xffff;
}

unsigned char dbDatabase::GetFieldDecimals(const String &tbname, const String &field) {
	unsigned int h, t;

  if((h = GetTableHandle(tbname)) != MTDB_TB_NOT_FOUND) {
    ArrayMap<String, struct fieldStruct> &fs = fields[h];
    for(t=0; t<tables[h].numOfFields; t++) {
    	if(fs.GetKey(t) == field) {
        return GetFieldDecimals(h, t);
    	}
  	}
  }
  else return 0xff;
}

unsigned char dbDatabase::GetFieldDecimals(unsigned int h, unsigned int f) {
	if(h < numOfTables && f < tables[h].numOfFields) {
		ArrayMap<String, struct fieldStruct> &fs = fields[h];
		return fs[f].nDecimals;
	}
	else return 0xff;
}

char dbDatabase::GetFieldFlags(const String &tbname, const String &field) {
	unsigned int h, t;

  if((h = GetTableHandle(tbname)) != MTDB_TB_NOT_FOUND) {
    ArrayMap<String, struct fieldStruct> &fs = fields[h];
    for(t=0; t<tables[h].numOfFields; t++) {
    	if(fs.GetKey(t) == field) {
        return GetFieldFlags(h, t);
    	}
  	}
  }
  else return 0;
}

char dbDatabase::GetFieldFlags(unsigned int h, unsigned int f)	{ 
	if(h < numOfTables && f < tables[h].numOfFields) {
		ArrayMap<String, struct fieldStruct> &fs = fields[h];
		return fs[f].flags;
	}
	else return 0xff;
}

String dbDatabase::GetFieldName(unsigned int h, unsigned int f) { 
	if(h < numOfTables && f < tables[h].numOfFields) {
		ArrayMap<String, struct fieldStruct> &fs = fields[h];
		return fs[f].name;
	}
	else return "";
}

String dbDatabase::GetFieldTitle(unsigned int h, unsigned int f) { 
	if(h < numOfTables && f < tables[h].numOfFields) {
		ArrayMap<String, struct fieldStruct> &fs = fields[h];
		return fs[f].title;
	}
	else return "";
}

unsigned int dbDatabase::GetFieldLength(unsigned int h, unsigned int f) { 
	if(h < numOfTables && f < tables[h].numOfFields) {
		ArrayMap<String, struct fieldStruct> &fs = fields[h];
		return fs[f].length;
	}
	else return 0xffff;
}

int dbDatabase::GoToRec(const String &tbname, int value) {
	int h;
	
  if((h = GetTableHandle(tbname)) == MTDB_TB_NOT_FOUND)
   	return MTDB_TB_NOT_FOUND;

  return tables[h].record = value;
}

int dbDatabase::GetNumOfRec(const String &tbname) {
	int h;

  if((h = GetTableHandle(tbname)) == MTDB_TB_NOT_FOUND)
  	return MTDB_TB_NOT_FOUND;

  return tables[h].numOfRecord;
}
