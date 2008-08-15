#include <bigmailer/bigmailer.h>
#include "smtpEx.h"

/*
class sCrono
{
	Time a, b ;
public:
	void start() { a = b = GetSysTime() ; }
	void end() { b = GetSysTime() ; }
	double result() { return b-a ; }
} ;
*/


static String GetHostFromAddress( const String& address )
{
	int posAt = address.Find('@') ;
	if (posAt == -1)
		return address ;

	return address.Mid(posAt+1) ;
}

void bigmailer::onSend()
{
	const String enter = "\r\n" ;
	if (messagePage.editor.IsModified())
		messagePage.Save() ;
	// mapa de los campos
	VectorMap<String, int> fldMap ;
	for (int i = 0; i < theDefList.fields.GetCount(); i++)
		fldMap.Add(theDefList.fields[i], i) ;

	String outHTML ;

	Index<String> css ;
	VectorMap<String, String> links ;
	String path = "./" ;
 	String html = EncodeHtml(messagePage.editor.Get(), css, links, path ) ;

	outHTML <<
		"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\r\n"
		"<html>\r\n"
		"<head>\r\n"
		"<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\r\n"
		"<style>\r\n"
		<< AsCss( css ) << "\r\n"
		<< "</style>\r\n"
	     "</head>\r\n"
		   "<body>\r\n"
		<< html << "\r\n"
		<< "</body>\r\n"
		   "</html>\r\n" ;

	Progress prog ;
	prog.Set(0, theDefList.data.GetCount()) ;
	prog.Show() ;

	theDefList.errors.Clear() ;

	String theHtml ;
	for (int row = 0; row < theDefList.data.GetCount(); row++)
	{
		String addrTo = theDefList.data[row][1] ;
		if (addrTo.IsEmpty())
		{
			theDefList.errors.Add(t_("dirección incorrecta")) ;
			continue ;
		}

		if (prog.Canceled())
			break ;


		theHtml = outHTML ;

		String s = AsString(row+1) + " / " + AsString( theDefList.data.GetCount() ) ;
		prog.SetText(s) ;

		for (int i = 0; i < theDefList.fields.GetCount(); i++)
		{
			String toFind = "[%" + theDefList.fields[i] + "%]" ;
			int from = 0 ;
			while( (from = theHtml.Find(toFind, from )) != -1)
			{
				theHtml.Remove( from, toFind.GetCount() ) ;
				theHtml.Insert( from, theDefList.data[row][ fldMap.Get(theDefList.fields[i])] ) ;
			}
		}

		prog.SetPos(row+1) ;

		SmtpMailEx mail ;

		mail.Host( theCfg.smtpServer )
				.User( theCfg.user )
				.Password( theCfg.pass )
				.From( theMsg.from )
				.ReplyTo( theMsg.from )
				.To( addrTo )
				.Subject( theMsg.subject )
				.Text( "" )
		    .Attach("MENSAJE", theHtml, "text/html; charset=utf-8") ;

		if ( ! mail.Send() )
			theDefList.errors.Add(mail.GetError()) ;
		else
			theDefList.errors.Add("OK") ;

	}

	theDefList.Save() ;
}
