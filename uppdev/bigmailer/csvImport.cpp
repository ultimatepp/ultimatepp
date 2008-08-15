#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
#include <sTools/sTools.h>
#include "csvImport.h"

#define LAYOUTFILE <bigmailer/csvImport.lay>
#include <CtrlCore/lay.h>


CSVParser::CSVParser()
{
  sData = "";
  nPos = 0;
  separator = ',' ;

}

Vector<String> CSVParser::ParseLine(const String& sIn, int fieldCount)
{
  sData = sIn;
  nPos = 0;

  Vector<String> result ;
  for (int i=0; i < fieldCount; i++)
    result.Add( GetNext() ) ;
	return result ;
}
Vector<String> CSVParser::ParseLine(const String& sIn)
{
  sData = sIn;
  nPos = 0;

  String w ;
  Vector<String> result ;
  while(true)
  {
    w = GetNext() ;
    if (w.IsEmpty())
      break ;
    result.Add(w) ;
  }
	return result ;
}
String CSVParser::GetNext()
{
  bool bQuotes = false;
  String sOut = "";
	// Skip spaces
  while (nPos < sData.GetLength() && sData[nPos] == ' ')
    nPos++;

  // Jump past first " if necessary
  if (nPos < sData.GetLength() && sData[nPos] == '"')
  {
    bQuotes = true;
    nPos++;
  }

  while (nPos < sData.GetLength())
  {
    if (!bQuotes && sData[nPos] == separator)
      break;
    if (bQuotes && sData[nPos] == '"')
    {
      if (nPos + 1 >= sData.GetLength() - 1)
        break;
      if (sData[nPos+1] == separator)
        break;
    }
    sOut += sData[nPos++];
  }

  // Jump past last " if necessary
  if (bQuotes && nPos < sData.GetLength() && sData[nPos] == '"')
    nPos++;

  // Jump past , if necessary
  if (nPos < sData.GetLength() && sData[nPos] == separator)
    nPos++;

  return sOut ;
}
// end csvparser
// ----------------------------------------------------------------------------------------



class CSVDlg : public WithCSVLayout<TopWindow> {
public:

	typedef CSVDlg CLASSNAME;

	String filePath ;

	CSVDlg(const String& fPath, csvResult& r );

	void ShowGrid() ;

	csvResult&result ;

};

bool csvResult::ImportFrom( const String& filePath )
{
	csvResult r ;
	CSVDlg d(filePath, *this) ;
	int resp = d.Execute() ;
	return (resp == IDOK) ;
}





CSVDlg::CSVDlg( const String& fPath, csvResult& r ) : result(r)
{
	filePath = fPath ;
	CtrlLayout(*this, t_("Import csv file"));
	Sizeable() ;

	int ch ;
	while( (ch = enumCharSet()) != 0)
				dropCharSet.Add( ch, CharsetName( ch ) ) ;
	dropCharSet.SetData(GetDefaultCharset()) ;

	dropSeparator.Add('\t', t_("tab")) ;
	dropSeparator.Add(',', t_("coma")) ;
	dropSeparator.Add(';', t_("punto y coma")) ;
	dropSeparator.SetData(';') ;

	dropCharSet.WhenAction = dropSeparator.WhenAction = THISBACK(ShowGrid) ;
	btCancel.WhenAction = Breaker(IDEXIT) ;
	btOk.WhenAction = Breaker(IDOK) ;

	gridData.Hide() ;
	ShowGrid() ;
}
void CSVDlg::ShowGrid()
{

	int ch = dropCharSet.GetData() ;
	int sep	= dropSeparator.GetData() ;

	String x = ToCharset(CHARSET_DEFAULT, LoadFile(filePath), ch, ch) ;
	Vector<String> lines = String2VectorString(x) ;

	gridData.Reset() ;
	gridData.Absolute() ;

	CSVParser csv ;
	csv.SetSeparator(sep) ;
	result.fields.Clear() ; result.values.Clear() ;
	//Vector<String> fields, values ;
	for (int i = 0; i < lines.GetCount(); i++)
	{
		if (i == 0) // primera línea
		{
			result.fields = csv.ParseLine( lines[0] ) ;
			for (int j = 0; j < result.fields.GetCount(); j++)
				gridData.AddColumn(result.fields[j], 60) ;
		}
		else
		{
			Vector<String>&v = result.values.Add() ;

			v = csv.ParseLine(lines[i], result.fields.GetCount()) ;
			for( int j = 0; j < result.fields.GetCount(); j++)
			{
				gridData.Set(i-1, j, v[j]) ;
			}
		}
	}
	gridData.Show() ;
}
