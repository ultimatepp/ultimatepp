#ifndef _csvImport_h_
#define _csvImport_h_

struct csvResult
{
	Vector<String> fields ;
	Vector< Vector<String> > values ;

	bool ImportFrom( const String& filePath ) ;
} ;

// CSVParser inspirated in code from other people ......
class CSVParser {
private:
  String sData;
  int    nPos;
  String GetNext();

  int separator ;
public:
  CSVParser();
  void SetSeparator(int sep ) { separator = sep ; }

  Vector<String> ParseLine(const String &sIn);
  Vector<String> ParseLine(const String &sIn, int fieldCount );
};

#endif
