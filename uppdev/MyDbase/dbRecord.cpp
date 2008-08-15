#include "dbase.h"

Value& dbRecord::operator() (const String &field) { 
	int q = record.Find(field); 
	if(q < 0) {
		Value v;
		return record.GetAdd(field, v);
	}
	else return GetValue(q); 
}
