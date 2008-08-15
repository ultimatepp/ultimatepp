#ifndef _cfg_h_
#define _cfg_h_
// configuración--------------------------------
struct bigMailerCfg
{
	String lastList, lastMsg ;

	String smtpServer, user, pass ; // smtp config

	Vector<String> blackList ;
	//String dnsServers ;
	//String blackList ;

	void Save() ;
	void Load() ;
} ;
extern bigMailerCfg theCfg ;

// definición de una lista -------------------------
struct defList
{
	String path ;
	String name ;
	Vector<String> fields ;

	//Vector<String> data ; // cada row es un string separado por comas
	Vector< Vector< String > > data ;
	Vector< String > errors ; // one line to each data, contains the errors from the last transmision

	defList() ;
	void Load(String filePath) ;
	void Save() ;
	void Clear() ;
	bool IsActive() const { return ! name.IsEmpty() ; }
} ;
extern defList theDefList ;

struct defMsg
{
	String name ; // name of msg
	String path ; // file path

	String from ;
	String subject ;
	String qtfBody ;

	defMsg() { Clear() ; }
	void Load( String filePath ) ;
	void Save() ;
	void Clear() ;
	bool IsActive() const { return ! name.IsEmpty() ; }
} ;
extern defMsg theMsg ;
#endif
