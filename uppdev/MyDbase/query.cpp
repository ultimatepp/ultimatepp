#include "dbase.h"

//int dbDatabase::Query(const String &queryString, dbRecordSet &rs1) {
Value dbDatabase::Query(const String &queryString) {
	unsigned int i, j, f, h, p, b, n, a;
	String tbn, whr;
	Array<String> ord, groupBy;
	Array<unsigned int> ordStyle;
	bool b1, validRecord;
	//dbRecordSet rs1;
	dbRecordSet rs1;

	//rs1.Reset();
	Vector<String> tkn = StringToToken(queryString);
	if(String(tkn[0]) == "SELECT") {
		ExecuteSelect(tkn, rs1);
		//StringStream s;
		//s.SetStoring();
		//rs1.Serialize(s);
		//return String(s);
		return RawToValue(rs1);
	}
	else if(String(tkn[0]) == "DELETE") {
		return ExecuteDelete(tkn);
	}
	else if(String(tkn[0]) == "UPDATE") {
		return ExecuteUpdate(tkn);
	}	
	else if(String(tkn[0]) == "INSERT") {
		return ExecuteInsert(tkn);
	}
	return 0;
}

void dbDatabase::ExecuteSelect(Vector<String> &tkn, dbRecordSet &result) {
	unsigned int i, j, f, h, p, b, n, a;
	String tbn, whr;
	Array<String> ord, groupBy;
	Array<unsigned int> ordStyle;
	bool b1, validRecord;

	n = tkn.GetCount();
	// cerco il nome della tabella
	for(f=1; f<n; f++) {
		if(strcmp(tkn.At(f), "FROM") == 0) {
			tbn = tkn.At(f+1);
#ifdef	_WITH_DEBUG
RLOG("ExecuteSelect():From = " + tbn);
#endif
			break;
		}
	}
  if((h = GetTableHandle(tbn)) == MTDB_TB_NOT_FOUND) return;
	// nome tabella trovato;
	// cerco la condizione where
	for(f=1; f<n; f++) {
		if(strcmp(tkn.At(f), "WHERE") == 0) {
			whr = tkn.At(f+1);
#ifdef	_WITH_DEBUG
RLOG("ExecuteSelect():Where = " + whr);
#endif				
			break;
		}
	}
	// condizione where trovata;
	// cerco la condizione GROUP BY
	for(f=1; f<n; f++) {
		if(strcmp(tkn.At(f), "GROUP") == 0 && strcmp(tkn.At(f+1), "BY") == 0) {
			p = 2;
			while((f+p)<n && !IsSQLWord(tkn.At(f+p))) {
				groupBy.Add(tkn.At(f+p));
				p++;
#ifdef	_WITH_DEBUG
RLOG("ExecuteSelect():Group by = " + groupBy.Top());
#endif
			}
			break;
		}
	}
	
	// cerco la condizione Order By
	for(f=1; f<n; f++) {
		if(strcmp(tkn.At(f), "ORDER") == 0 && strcmp(tkn.At(f+1), "BY") == 0) {
			p = 2;
			for(i=0; i<3; i++) {
				if((f+p)<n && !IsSQLWord(tkn.At(f+p))) {
					ord.Add(tkn.At(f+p));
					if(strcmp(tkn.At(f+p+1), "DESC") == 0) {
						ordStyle.At(i) = MTDB_DESC;
						p += 2; 
					}	
					else if(strcmp(tkn.At(f+p+1), "ASC") == 0) {
						ordStyle.At(i) = MTDB_ASC;
						p += 2; 
					}
					else {
						ordStyle.At(i) = MTDB_ASC;
						p += 1; 
					}
#ifdef	_WITH_DEBUG
RLOG("ExecuteSelect():Order by = " + ord.Top() + " - " + AsString(ordStyle[i]));
#endif						
				}
			}
		}
	}
	// condizione Order By trovata;
	// creazione testa del RecorSet (metto in ordine i nomi di campo secondo l'ordine di query)
	Array<String> keys;
	for(f=1; f<n; f++) {
		if(!IsSQLWord(tkn.At(f))) {
			if(strcmp(tkn.At(f), "*") != 0) {
				for(i=0; i<GetNumOfField(h); i++) {
					if(strcmp(GetFieldName(h, i), tkn.At(f)) == 0) {
						keys.Add(String(GetFieldName(h, i)));
					}
				}
			}
			else {
				for(i=0; i<GetNumOfField(h); i++) {
					b1 = false;
					for(j=0; j<keys.GetCount(); j++) {
						if(strcmp(GetFieldName(h, i), keys[j]) == 0) {
							b1 = true;
						}	
					}
					if(!b1) keys.Add(String(GetFieldName(h, i)));
				}					
			}
		}
		else break;
	}
	// testa RecordSet creata
	tkn.SetCountR(n);
	dbRecord rec1;
		
	for(i=1; i<=GetNumOfRec(h); i++) {
		validRecord = false;
		if(GetRecord(h, i, rec1) == MTDB_SUCCESS) {
			if(EvalWhere(rec1, whr)) { // se il record è valido
#ifdef _WITH_DEBUG
RLOG("ExecuteSelect():type = " + String(tkn[0]));
#endif
				dbRecord rc2;
				for(unsigned int y=0; y<keys.GetCount(); y++) {
					Value v = rec1.GetValue(keys[y]);
					rc2.Add(keys[y], v);
				}
				result.Add((int)i, rc2, ord, ordStyle);
			}
#ifdef _WITH_DEBUG
RLOG("ExecuteSelect():recno = " + FormatInt(i));
#endif			
		}
	}
	return;	
}

int dbDatabase::ExecuteDelete(Vector<String> &tkn) {
	unsigned int i, j, f, h, p, b, n, a;
	String tbn, whr;
	bool b1, validRecord;

	n = tkn.GetCount();
	// cerco il nome della tabella
	for(f=1; f<n; f++) {
		if(strcmp(tkn.At(f), "FROM") == 0) {
			tbn = tkn.At(f+1);
			break;
		}
	}
  if((h = GetTableHandle(tbn)) == MTDB_TB_NOT_FOUND) return MTDB_TB_NOT_FOUND;
	// nome tabella trovato;
	// cerco la condizione where
	for(f=1; f<n; f++) {
		if(strcmp(tkn.At(f), "WHERE") == 0) {
			whr = tkn.At(f+1);
			break;
		}
	}
	// condizione where trovata;
	
	dbRecord rec1;
	n = 0;	
	for(i=1; i<=GetNumOfRec(h); i++) {
		validRecord = false;
		if(GetRecord(h, i, rec1) == MTDB_SUCCESS) {
			if(EvalWhere(rec1, whr)) { // se il record è valido
				DelRec(h, i);
				n++;
			}
		}
	}
	return n;	
}

int dbDatabase::ExecuteUpdate(Vector<String> &tkn) {
	unsigned int i, j, f, h, p, b, n, a;
	String table, whr;
	bool b1, validRecord;

	n = tkn.GetCount();
	// cerco il nome della tabella
	for(f=0; f<n; f++) {
		if(tkn.At(f) == "UPDATE") {
			table = tkn.At(f+1);
			break;
		}
	}
  if((h = GetTableHandle(table)) == MTDB_TB_NOT_FOUND) return MTDB_TB_NOT_FOUND;
	
	// cerco la condizione where
	for(f; f<n; f++) {
		if(tkn.At(f) == "WHERE") {
			whr = tkn.At(f+1);
			break;
		}
	}

	// creo l'array dei valori da aggiornare
	VectorMap<String, String> values;
	bool s = false;
	for(f=1; f<n; f++) {
		if(!s && tkn.At(f) == "SET") s = true;
		else if(s) {
			if(!IsSQLWord(tkn.At(f))) {
				int u;
				if((u = StringSearch(tkn.At(f), "=")) > 0) {
					values.Add(tkn.At(f).Left(u), tkn.At(f).Mid(u+1));
				}
			}
			else {
				s = false;
				break;	
			}
		}
	}
	
	dbRecord rec1;
	n = 0;	
	for(i=1; i<=GetNumOfRec(h); i++) {
		validRecord = false;
		if(GetRecord(h, i, rec1) == MTDB_SUCCESS) {
			if(EvalWhere(rec1, whr)) { // se il record è valido
				// sostituisco i nuovi valori
				for(int k=0; k<values.GetCount(); k++) {	
				 	if(rec1(values.GetKey(k)).GetType() == BOOL_V) {
				 		rec1(values.GetKey(k)) = (bool)atoi(~CSVDequote(values[k]));
					}
				 	if(rec1(values.GetKey(k)).GetType() == INT_V) {
				 		rec1(values.GetKey(k)) = (int)atoi(~CSVDequote(values[k]));
					}
					else if(rec1(values.GetKey(k)).GetType() == DOUBLE_V) {
						rec1(values.GetKey(k)) = (double)atof(~CSVDequote(values[k]));
					}
					else if(rec1(values.GetKey(k)).GetType() == STRING_V) {
						rec1(values.GetKey(k)) = CSVDequote(values[k]);
					}
					else if(rec1(values.GetKey(k)).GetType() == MTDB_DATETIME || rec1(values.GetKey(k)).GetType() == MTDB_DATE || rec1(values.GetKey(k)).GetType() == MTDB_TIME ) {
		  			ConvertTime cv;
						rec1(values.GetKey(k)) = (Time)cv.Scan(CSVDequote(values[k]));
					}
				}
				PutRecord(h, i, rec1, true);
				n++;
			}
		}
	}
	return n;
}

int dbDatabase::ExecuteInsert(Vector<String> &tkn) {
	unsigned int i, j, f, h, p, b, n, a;
	String table;
	bool b1, validRecord;

	n = tkn.GetCount();
	// cerco il nome della tabella
	for(f=1; f<n; f++) {
		if(tkn.At(f) == "INTO") {
			table = tkn.At(f+1);
			f += 2;
			break;
		}
	}
  if((h = GetTableHandle(table)) == MTDB_TB_NOT_FOUND) return MTDB_TB_NOT_FOUND;
	
	// creo l'array delle chiavi da inserire
	VectorMap<String, String> values;
	Vector<String> buff = StringToToken(tkn.At(f).Mid(1, tkn.At(f).GetLength()-2));
	for(int u=0; u<buff.GetCount(); u++) {
		values.Add(buff[u]);	
	}

	for(; f<n; f++) {
		if(tkn.At(f) == "VALUES") {
			buff = StringToToken(tkn.At(f+1).Mid(1, tkn.At(f+1).GetLength()-2));
			for(int u=0; u<buff.GetCount(); u++) {
				values[u] = buff[u];
			}
			break;
		}
	}

	n = 0;
	dbRecord rec1;
	for(int k=0; k<values.GetCount(); k++) {	
		int t = GetFieldFormat(table, values.GetKey(k));
	 	if(t == MTDB_INT || t == MTDB_BOOL) {
	 		rec1(values.GetKey(k)) = (int)atoi(~CSVDequote(values[k]));
		}
		else if(t == MTDB_DOUBLE) {
			rec1(values.GetKey(k)) = (double)atof(~CSVDequote(values[k]));
		}
		else if(t == MTDB_DATETIME || t == MTDB_DATE || t == MTDB_TIME ) {
		  ConvertTime cv;
			rec1(values.GetKey(k)) = (Time)cv.Scan(CSVDequote(values[k]));
		}		
		else if((t >= 0x0100 && t <= 0x02ff) || t == MTDB_STR) {
			rec1(values.GetKey(k)) = CSVDequote(values[k]);
		}
	}
	PutRecord(h, MTDB_TB_APP, rec1, true);
	n++;
	
	return n;
}
