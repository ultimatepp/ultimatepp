#include	<Core/Core.h>
#include	"dbase.h"
#include	"compare.h"

bool IsEqual(const Value &val1, const String &s) {

 	if(val1.GetType() == INT_V) {
 		Value val2 = (int)atoi(~s);
		return(bool)((int)val1==(int)val2);
	}
	else if(val1.GetType() == DOUBLE_V) {
		Value val2 = (double)atof(~s);
		return(bool)((double)val1==(double)val2);
	}
	else if(val1.GetType() == STRING_V) {
		return(bool)(strcmp((String)val1, s)==0);
	}
	else return false;	
}

bool 	IsBigger(const Value &val1, const String &s) {
	
 	if(val1.GetType() == INT_V) {
 		Value val2 = (int)atoi(~s);
		return(bool)((int)val1>(int)val2);
	}
	else if(val1.GetType() == DOUBLE_V) {
		Value val2 = (double)atof(~s);
		return(bool)((double)val1>(double)val2);
	}
	else if(val1.GetType() == STRING_V) {
		return(bool)(strcmp((String)val1, s)>0);
	}
	else return false;	
}

bool 	IsSmaller(const Value &val1, const String &s) {
	
 	if(val1.GetType() == INT_V) {
 		Value val2 = (int)atoi(~s);
		return(bool)((int)val1<(int)val2);
	}
	else if(val1.GetType() == DOUBLE_V) {
		Value val2 = (double)atof(~s);
		return(bool)((double)val1<(double)val2);
	}
	else if(val1.GetType() == STRING_V) {
		return(bool)(strcmp((String)val1, s)<0);
	}
	else return false;	
}

bool 	IsBiggerEqual(const Value &val1, const String &s) {
	
 	if(val1.GetType() == INT_V) {
 		Value val2 = (int)atoi(~s);
		return(bool)((int)val1>=(int)val2);
	}
	else if(val1.GetType() == DOUBLE_V) {
		Value val2 = (double)atof(~s);
		return(bool)((double)val1>=(double)val2);		
	}
	else if(val1.GetType() == STRING_V) {
		return(bool)(strcmp((String)val1, s)>=0);
	}
	else return false;	
}

bool 	IsSmallerEqual(const Value &val1, const String &s) {
	
 	if(val1.GetType() == INT_V) {
 		Value val2 = (int)atoi(~s);
		return(bool)((int)val1<=(int)val2);
	}
	else if(val1.GetType() == DOUBLE_V) {
		Value val2 = (double)atof(~s);
		return(bool)((double)val1<=(double)val2);
	}
	else if(val1.GetType() == STRING_V) {
		return(bool)(strcmp((String)val1, s)<=0);
	}
	else return false;	
}
