#include <Core/Core.h>
#include <sTools/sTools.h>

void replaceInto( const char* marca, const char* newText, String& buffer )
{
  const int lMarca = strlen( marca ) ;
  const int lNew = strlen( newText ) ;
  int pos = 1 ;
  while (pos < buffer.GetLength() )
  {
    if (buffer.Mid(pos, lMarca) == marca)
    {
			buffer.Remove(pos, lMarca ) ;
      buffer.Insert(pos, newText ) ;
      pos += lNew ;
    }
    else
      pos ++ ;
  }
}
String SetQ( const String s )
{
	return "'" + s + "'" ;
}
String Set2Q( const String s )
{
	return "\"" + s +  "\"" ;
}

String VectorString2String( Vector<String>& v, const char * separador)
{
	String result ;
	for (int i = 0; i < v.GetCount(); i++)
	{
		if (i > 0)
			result += separador ;
		result += v[i] ;
	}
	return result ;
}

static int myFilter( int c )
{
	if (c == '\r' || c == '\n')
		return c ;
	return 0 ;
}
Vector<String> String2VectorString( const String& str)
{
	return Split(str, myFilter) ;
}

int enumCharSet()
{
	static int chs[] = {
	CHARSET_ISO8859_1, CHARSET_ISO8859_2, CHARSET_ISO8859_3, CHARSET_ISO8859_4,CHARSET_ISO8859_5,
	CHARSET_ISO8859_6, CHARSET_ISO8859_7, CHARSET_ISO8859_8, CHARSET_ISO8859_9, CHARSET_ISO8859_10,
	CHARSET_ISO8859_13, CHARSET_ISO8859_14, CHARSET_ISO8859_15, CHARSET_ISO8859_16, CHARSET_WIN1250,
	CHARSET_WIN1251, CHARSET_WIN1252, CHARSET_WIN1253, CHARSET_WIN1254, CHARSET_WIN1255,CHARSET_WIN1256,
	CHARSET_WIN1257, CHARSET_WIN1258, CHARSET_KOI8_R, CHARSET_CP852, CHARSET_MJK,
	CHARSET_UTF8 } ;
	static int i = -1, size = sizeof(chs) / sizeof(chs[0]) ;
	if (i >= size )
	{
		i = -1 ;
		return 0 ;
	}
	else if (i == -1)
		i = 0 ;
	else
		i++ ;
	return chs[i] ;
}
