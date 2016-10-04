#include	"dbase.h"

dbRecordSet::dbRecordSet() {
	ptr = 0;
}

void dbRecordSet::Reset() {
	recordSet.Clear();
	ptr = 0;
}

void dbRecordSet::Add(int record, dbRecord &rec, Array<String> &orderField, Array<unsigned int> &ordStyle) {
	unsigned int j, n, f, l, p, k;

	k = orderField.GetCount();
#ifdef _WITH_DEBUG
RLOG("# keys: " + FormatInt(k));
#endif	
	// sorting the new record in the correct position with three key (field) level
	bool s;
	f=0; l=recordSet.GetCount(), p=l;
	for(n=0; n<k; n++) {
		s=true;
		for(j=f; j<l; j++) {
			if(ordStyle[n] == MTDB_ASC && s) {
				if(IsBiggerEqual(recordSet[j].Get(orderField[n]), AsString(rec.GetValue(orderField[n])))) {
	    		p = j;
	    		f = j;
	    		s = false;
				}
			}
			if(ordStyle[n] == MTDB_DESC && s) {
				if(IsSmallerEqual(recordSet[j].Get(orderField[n]), AsString(rec.GetValue(orderField[n])))) {
	    		p = j;
	    		f = j;
	    		s = false;
				}
			}
			if(!s) {
				if(!IsEqual(recordSet[j].Get(orderField[n]), AsString(rec.GetValue(orderField[n])))) {
					l = j;
				}
			}
		}
		if(s) {
			p=l;
			f=l;
		}
	}
	recordSet.Insert((int)p, record, rec.Get());
	return;
}

bool dbRecordSet::Next() { 
	if(recordSet.GetCount()>0 && ptr<(recordSet.GetCount()-1)) {
		ptr++; 
		return true; 
	} 
	return false; 
}

bool dbRecordSet::Previous() { 
	if(recordSet.GetCount()>0 && ptr>0) {
		ptr--; 
		return true; 
	} return false; 
}

bool dbRecordSet::GoTo(int recno) {
	int q;
	if((q = recordSet.Find(recno)) >= 0) {
		ptr = q;
		return true;
	}
	return false;
}

Value& dbRecordSet::GetValue(int field) { 
	VectorMap<String, Value> &rec = recordSet[ptr]; 
	Value &res = rec[field]; 
	return res; 
}

Value	dbRecordSet::GetValue(const String &field)	{ 
	VectorMap<String, Value> &rec = recordSet[ptr]; 
	int q = rec.Find(field); 
	if(q >= 0) {
		Value res = rec[q]; 
		return res;
	}
	else return Nuller();
}

const Value	dbRecordSet::GetValue(const String &field) const	{ 
	return GetValue(field);
}

Value	dbRecordSet::GetValue(int record, const String &field) {
	VectorMap<String, Value> &rec = recordSet.Get(record); 
	int q = rec.Find(field); 
	Value res = rec[q]; 
	return res;	
}

const Value	dbRecordSet::GetValue(int record, const String &field) const {
	return GetValue(record, field);	
}

Value& dbRecordSet::GetValue(int record, int field) {
	//VectorMap<String, Value> &rec = recordSet.Get(record);
	//return rec[field];
	
	int q = recordSet.Find(record); 
	if(q < 0) {
		VectorMap<String, Value> r;
		recordSet.GetAdd(record, r);
		q = recordSet.GetCount()-1;
	}
	VectorMap<String, Value> &rec = recordSet[q];
	return rec[field];
}
