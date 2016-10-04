#include "dbase.h"

int dbDatabase::FieldGet(unsigned int tbHandle, int recno, unsigned int field, Value& result) {

	ArrayMap<String, struct fieldStruct> &fs = fields[tbHandle];
	lseek(fhandle, (int)tables[tbHandle].offset+tables[tbHandle].tableHead+(tables[tbHandle].recordLenght*(recno-1))+fs[field].prevLength+1, 0);
  if(fs[field].format == MTDB_STR) {
    String::Buffer buff(fs[field].length+1);
  	read(fhandle, buff, fs[field].length);
  	buff[fs[field].length] = 0;
  	String s = buff;
  	s.Trim(s.Find('\0'));
		result = s;
		return MTDB_SUCCESS;
  }
  else if(fs[field].format >= 0x0100 && fs[field].format <= 0x02ff) {
    unsigned int jumpPointer, dataSize;
    read(fhandle, &jumpPointer, 4);
    if(jumpPointer == 0) {
      String::Buffer buff(0);
      result = (String)buff;
      return MTDB_SUCCESS;
    }
    else {
      Buffer<byte> b(1);
      lseek(fhandle, jumpPointer+eodsp-5, 0);
      read(fhandle, b, 1);
      if(b[0] == MTDB_TB_RECORD_ENABLED) {
        read(fhandle, &dataSize, 4);
        String::Buffer buff(dataSize);
		  	read(fhandle, buff, dataSize);
		  	if(fs[field].format == MTDB_MEMOASC) {
      		result = (String)buff;
		  	}
		  	else result = (String)buff;
				return MTDB_SUCCESS;
      }
      return MTDB_REC_DELETED;
    }
  }
 	else if(fs[field].format == MTDB_INT) {
 		Buffer<int> res1(1);
		read(fhandle, res1, sizeof(int));
		result = (int)*res1;
		return MTDB_SUCCESS;
	}
	else if(fs[field].format == MTDB_DOUBLE){
		Buffer<double> res2(1);
		read(fhandle, res2, sizeof(double));
		result = (double)*res2;
		return MTDB_SUCCESS;
	}
  else if(fs[field].format == MTDB_BOOL) {
    Buffer<bool> res4(1);
  	read(fhandle, res4, sizeof(bool));
		result = (bool)*res4;
		return MTDB_SUCCESS;
  }
  else if(fs[field].format == MTDB_DATETIME) {
    Buffer<Time> res5(1);
   	read(fhandle, res5, sizeof(Time));
    result = (Time)*res5;
    return MTDB_SUCCESS;
  }
  else if(fs[field].format == MTDB_DATE) {
    Time data2;
  	read(fhandle, &data2, sizeof(Time));
  	data2.hour = data2.minute = data2.second = 0;
		result = data2;
		return MTDB_SUCCESS;
  }
  else if(fs[field].format == MTDB_TIME) {
    Time data2;
  	read(fhandle, &data2, sizeof(Time));
  	data2.year = data2.month = data2.day = 0;
		result = data2;
		return MTDB_SUCCESS;
  }
  else return MTDB_UNKNOW_DATA;
}

int dbDatabase::FieldPut(unsigned int tbHandle, int recno, unsigned int field, Value value, bool f, bool p) {
  char buff[2];

	ArrayMap<String, struct fieldStruct> &fs = fields[tbHandle];
	if(_isBitSet(fs[field].flags, MTDB_FD_PRIM_KEY)) {
		if(!f) return MTDB_FD_WR_PROT;
		else {
			int l;
			l = (int)value;
			if(l >= tables[tbHandle].autoincCntr) tables[tbHandle].autoincCntr = l+1;
		}
	}
	if(fs[field].format >= 0x0100 && fs[field].format <= 0x02ff) {
		unsigned int jumpPointer, dataSize;
		
		if(String(value).GetLength() > 0) {
			if(!p) {
				char buff[1];
				lseek(fhandle, (int)tables[tbHandle].offset+tables[tbHandle].tableHead+(tables[tbHandle].recordLenght*(recno-1))+fs[field].prevLength+1, 0);
				read(fhandle, &jumpPointer, 4);
				if((jumpPointer+eodsp) > (fsize+5))	jumpPointer = fsize+5; // incorrect pointer, limited
				else if(jumpPointer == 0) jumpPointer = fsize+5; // empty field, jump to the EOF
				else {
					lseek(fhandle, eodsp+jumpPointer-4, 0);
					read(fhandle, &dataSize, 4);
					if(dataSize >= String(value).GetLength()) {
						jumpPointer += eodsp; // jump to the old area
					}
					else {
						jumpPointer = fsize+5; // jump to the EOF	
					}
				}
			}
			else { 
				jumpPointer = fsize+5; // forced at EOF for packing file.
			}
			lseek(fhandle, jumpPointer-5, 0);
			dataSize = String(value).GetLength();
			buff[0] = MTDB_TB_RECORD_ENABLED;
			write(fhandle, buff, 1);
			write(fhandle, &dataSize, 4);
			String s = value;
			write(fhandle, s, dataSize);
			if(jumpPointer > fsize) fsize+=(dataSize+5);
			jumpPointer -= eodsp;
		}
		else {
			jumpPointer = 0;
		}
		lseek(fhandle, (int)tables[tbHandle].offset+tables[tbHandle].tableHead+(tables[tbHandle].recordLenght*(recno-1))+fs[field].prevLength+1, 0);
		write(fhandle, &jumpPointer, 4);
	}
	else {
		lseek(fhandle, (int)tables[tbHandle].offset+tables[tbHandle].tableHead+(tables[tbHandle].recordLenght*(recno-1))+fs[field].prevLength+1, 0);
		if(fs[field].format == MTDB_STR) {
			String s = value;
			write(fhandle, s, fs[field].length);
		}
		else if(fs[field].format == MTDB_INT) {
			int v = value;
			write(fhandle, &v, fs[field].length);
		}
		else if(fs[field].format == MTDB_DATETIME || fs[field].format == MTDB_DATE || fs[field].format == MTDB_TIME) {
			Time v = value;
			write(fhandle, &v, fs[field].length);		  
		}
		else if(fs[field].format == MTDB_BOOL) {
			bool v = value;
			write(fhandle, &v, fs[field].length);
		}
		else if(fs[field].format == MTDB_DOUBLE) {
			double v = value;
			write(fhandle, &v, fs[field].length);
		}		
	}
  return MTDB_SUCCESS;
}
