#include <CtrlLib/CtrlLib.h>
#include <sTools/sTools.h>
#include <bigmailer/cfg.h>
#include "csvImport.h"

// la configuración
bigMailerCfg theCfg ; // la configuración global

static int myFilter( int c )
{
	if (c == '\r' || c == '\n')
		return c ;
	return 0 ;
}

void bigMailerCfg::Save()
{
	const String enter = "\r\n" ;
	String s = "lastList=" + lastList + enter +
						 "lastMsg=" + lastMsg + enter +
						 "smtpServer=" + smtpServer + enter +
						 "user=" +  user + enter +
						 "pass=" + pass + enter ;

	for (int i = 0; i < blackList.GetCount(); i++)
		s << "blacklist_" << i << "=" << blackList[i] << enter ;

	SaveFile(ConfigFile("bigmailer.ini"), s ) ;

}
void bigMailerCfg::Load()
{
	VectorMap<String, String> ini = LoadIniFile(ConfigFile("bigmailer.ini")) ;
	lastList = ini.Get("lastList", "") ;
	lastMsg = ini.Get("lastMsg", "" ) ;
	smtpServer = ini.Get("smtpServer", "" ) ;
	user = ini.Get("user", "" ) ;
	pass = ini.Get("pass", "" ) ;



	String s ;
	blackList.Clear() ;
	for (int i = 0; true; i++)
	{
		s = ini.Get("blacklist_" + AsString(i), Null) ;
		if (s.IsEmpty())
				break ;
		blackList.Add(s) ;
	}

}
// definición de la lista ----------------------------------------------------------------------
// La lista global -----
defList theDefList ;

defList::defList()
{
	Clear() ;
}
void defList::Clear()
{
	path = "" ;
	name = "" ;
	fields.Clear() ;
	data.Clear() ;
}
void defList::Load(String filePath)
{
	Clear() ;

	path = filePath ;
	name = GetFileTitle(path) ;
	VectorMap<String,String> ini = LoadIniFile(path) ;
	fields = Split(ini.Get("fields", ""), ',' ) ;

	CSVParser csv ;
	for( int i=0; true; i++)
	{
		String s = ini.Get("data_" + AsString(i), "") ;
		if (s.IsEmpty())
			break ;

		data.Add(csv.ParseLine(s, fields.GetCount()) ) ;
	}

	for (int i=0; true; i++)
	{
		String s = ini.Get("error_" + AsString(i), "") ;
		if (s.IsEmpty())
			break ;
		errors.Add(s) ;
	}


	// guardamos este como último cargado
	theCfg.lastList = path ;
	theCfg.Save() ;

}
void defList::Save()
{
	if (path.IsEmpty())
	{
		path = ConfigFile(name +  ".list") ;
	}
	else if ( name != GetFileTitle(path))
	{
		path = GetFileDirectory(path) + name + ".list" ;
	}
	String s ;
	s << "name=" << name << "\r\n"
		<< "fields=" ;
	for (int i = 0; i < fields.GetCount(); i++)
		s << fields[i] << "," ;
	s << "\r\n" ;
	for (int i = 0; i < data.GetCount(); i++)
	{
		s << "data_" << i << "=" ;
		Vector<String>&line = data[i] ;
		for (int j = 0; j < line.GetCount(); j++)
		{
			if (j > 0)
				s << "," ;
			s << "\"" << line[j] << "\"" ;
		}
		s << "\r\n" ;
	}
	for (int i = 0; i < errors.GetCount(); i++)
		s << "error_" << i << "=" << errors[i] << "\r\n" ;


	SaveFile(path, s) ;
}
// ------------------------------------------------------------------------------
defMsg theMsg ;

void defMsg::Load( String filePath )
{

	Clear() ;

	path = filePath ;
	name = GetFileTitle(path) ;
	VectorMap<String,String> ini = LoadIniFile(path) ;
	from = ini.Get("from")  ;
	subject = ini.Get("subject")  ;

	String s ;
	for (int i = 0; true; i++)
	{
		s = ini.Get("body" + AsString(i), "") ;
		if (s.IsEmpty())
			break ;
		qtfBody += s + "\r\n" ;
	}

	// guardamos este como último cargado
	theCfg.lastMsg = path ;
	theCfg.Save() ;

}
void defMsg::Save()
{
	if (path.IsEmpty())
	{
		path = ConfigFile(name +  ".msg") ;
	}
	else if ( name != GetFileTitle(path))
	{
		path = GetFileDirectory(path) + name + ".msg" ;
	}
	String f ;
	f << "from=" << from << "\r\n"
		<< "subject="<< subject << "\r\n" ;

	Vector<String> lines = String2VectorString(qtfBody) ;
	for (int i = 0; i < lines.GetCount(); i++)
		f << "body" << i << "=" << lines[i] << "\r\n" ;

	SaveFile(path, f) ;
}
void defMsg::Clear()
{
	name = from = subject = qtfBody= "" ;
}
