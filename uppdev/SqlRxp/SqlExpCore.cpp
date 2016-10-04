#include "SqlExp.h"

enum {
	SQLC_IF = 1,
	SQLC_ELSEIF,
	SQLC_ELSE,
	SQLC_ENDIF,
	SQLC_DATE,
	SQLC_TIME,
	SQLC_STRING,
};

template <class T>
String MakeSqlValue(int code, T& value)
{
	StringBuffer b(sizeof(T) + 1);
	b[0] = code;
	memcpy(~b + 1, &value, sizeof(T));
	return b;
}

template <class T>
T ReadSqlValue(T& x, const char *&s) {
	memcpy(&x, s, sizeof(T));
	s += sizeof(T);
	return x;
}

static String SqlFormat__(const char *s, int res, bool mysql) {
	if(!s || !*s) return "NULL";
	String result;
	result.Reserve(res);
	result.Cat("\'");
	for(; *s; s++) {
		if(*s == '\'')
			result.Cat(mysql ? "\\\'" : "\'\'");
		else {
			if((*s == '\"' || *s == '\\') && mysql)
				result.Cat('\\');
			result.Cat(*s);
		}
	}
	result.Cat('\'');
	return result;
}


void SqlCompile(const char *&s, StringBuffer *r, byte dialect)
{
	for(;;) {
		int c = *s++;
		switch(c) {
		case SQLC_IF: {
			StringBuffer *er = r;
			for(;;) {
				c = *s++;
				if(c & dialect) {
					SqlCompile(s, er, dialect);
					er = NULL;
				}
				else
					SqlCompile(s, NULL, dialect);
				if(*s == '\0')
					return;
				c = *s++;
				if(c == SQLC_ELSE) {
					SqlCompile(s, er, dialect);
					ASSERT(*s++ == SQLC_ENDIF);
					break;
				}
				if(c == SQLC_ENDIF)
					break;
				ASSERT(c == SQLC_ELSEIF);
			}
		}
		break;
		case SQLC_DATE: {
			Date x;
			ReadSqlValue(x, s);
			if(!r) break;
			if(IsNull(x)) {
				*r << "NULL";
				break;
			}
			switch(dialect) {
			case MSSQL:
				*r << Format("convert(datetime, '%d/%d/%d', 120)", x.year, x.month, x.day);
				break;
			case ORACLE:
				*r << Format("to_date('%d/%d/%d', 'SYYYY/MM/DD')", x.year, x.month, x.day);
				break;
			default:
				*r << Format("\'%04d-%02d-%02d\'", x.year, x.month, x.day);
			}
			break;
		}
		case SQLC_TIME: {
			Time x;
			ReadSqlValue(x, s);
			if(!r) break;
			if(IsNull(x)) {
				*r << "NULL";
				break;
			}
			switch(dialect) {
			case MSSQL:
				*r << Format(x.hour || x.minute || x.second
				             ? "convert(datetime, '%d/%d/%d %d:%d:%d', 120)"
				             : "convert(datetime, '%d/%d/%d', 120)",
				             x.year, x.month, x.day, x.hour, x.minute, x.second);
				break;
			case ORACLE:
				*r << Format("to_date('%d/%d/%d/%d', 'SYYYY/MM/DD/SSSSS')",
				             x.year, x.month, x.day, x.second + 60 * (x.minute + 60 * x.hour));
				break;
			default:
				*r << NFormat("\'%04d-%02d-%02d %02d:%02d:%02d\'",
				              x.year, x.month, x.day, x.hour, x.minute, x.second);
			}
			break;
		}
		case SQLC_STRING: {
			int l;
			memcpy(&l, s, sizeof(int));
			s += sizeof(int);
			String x = String(s, l);
			s += l;
			if(!r) break;
			if(IsNull(x)) {
				*r << "NULL";
				break;
			}
			for(; *s; s++) {
				if(*s == '\'')
				r->Cat(dialect == MYSQL ? "\\\'" : "\'\'");
				else {
					if((*s == '\"' || *s == '\\') && dialect == MYSQL)
						r->Cat('\\');
					r->Cat(*s);
				}
			}
			r->Cat('\'');
			break;
		}
		default:
			if(c >= 0 && c < 32) {
				s--;
				return;
			}
			else
				if(r) r->Cat(c);
		}
	}
}

String SqlCompile(byte dialect, const String& s)
{
	StringBuffer b;
	b.Reserve(s.GetLength() + 100);
	const char *q = s;
	SqlCompile(q, &b, dialect);
	return b;
}

String SqlFormat(int x)
{
	if(IsNull(x)) return "NULL";
	return Format("%d", x);
}

String SqlFormat(double x)
{
	if(IsNull(x)) return "NULL";
	return Format("%.10g", x);
}

String SqlFormat(int64 x)
{
	if(IsNull(x)) return "NULL";
	return FormatInt64(x);
}

String SqlFormat(const char *s, int l)
{
	StringBuffer b(1 + sizeof(int) + l);
	b[0] = SQLC_STRING;
	memcpy(~b + 1, &l, sizeof(int));
	memcpy(~b + 1 + sizeof(int), s, l);
	return b;
}

String SqlFormat(const char *s)
{
	return SqlFormat(s, strlen(s));
}

String SqlFormat(const String& x)
{
	return SqlFormat(x, x.GetLength());
}

String SqlFormat(Date x)
{
	return MakeSqlValue(SQLC_DATE, x);
}

String SqlFormat(Time x)
{
	return MakeSqlValue(SQLC_TIME, x);
}

String SqlFormat(const Value& x)
{
	if(x.IsNull()) return "NULL";
	switch(x.GetType()) {
	case INT_V:
		return SqlFormat((int) x);
	case INT64_V:
		return SqlFormat((int64) x);
	case DOUBLE_V:
		return SqlFormat((double) x);
	case STRING_V:
	case WSTRING_V:
		return SqlFormat(String(x));
	case DATE_V:
		return SqlFormat(Date(x));
	case TIME_V:
		return SqlFormat(Time(x));
	}
	NEVER();
	return "NULL";
}

String SqlCase::operator()(const String& text) {
	return s << (char)SQLC_ELSE << text << (char)SQLC_ENDIF;
}

String SqlCase::operator()() {
	return s << (char)SQLC_ENDIF;
}

SqlCase SqlCase::operator()(byte cond, const String& text) {
	s << (char)SQLC_ELSEIF << (char)cond << text;
	return *this;
}

SqlCase::SqlCase(byte cond, const String& text) {
	s << (char)SQLC_IF << (char)cond << text;
}
