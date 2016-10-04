#include <Core/Core.h>
#include <sql.h>
#include <sqlext.h>

#define MAXDSN      25
#define MAXUID      25
#define MAXAUTHSTR   25
#define MAXBUFLEN   255
#define BUFFERSIZE   450

using namespace Upp;

void   ProcessLogMessages(HDBC hdbc, HSTMT hstmt, char *logstring)
{
	SQLCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER    NativeError;
	SQLSMALLINT   MsgLen;
	if(SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, 1, SqlState, &NativeError,
	                 Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA) {
		DDUMP((char *)SqlState);
		DDUMP((char *)Msg);
	}
	LOG(logstring);
}
	             

CONSOLE_APP_MAIN
{
	HENV   henv = SQL_NULL_HENV;
	HDBC   hdbc1 = SQL_NULL_HDBC;
	HSTMT   hstmt1 = SQL_NULL_HSTMT;
	char   logstring[MAXBUFLEN] = "";

   RETCODE retcode;
   SWORD   cntr;
   //SQLGetData variables.
   UCHAR   Data[BUFFERSIZE];
   SDWORD   cbBatch = (SDWORD)sizeof(Data)-1;
   SDWORD   cbBinSize;
   // Clear data array.
   for(cntr = 0; cntr < BUFFERSIZE; cntr++)
      Data[cntr] = 0x00;
    // Allocate the ODBC environment and save handle.
   retcode = SQLAllocEnv (&henv);
	
	SQLAllocConnect(henv, &hdbc1);
	
	char tmpStr[1024];
	char maxStr[1024];
//	strcpy(tmpStr, "Provider=SQL Server Native Client 10.0;Data Source=QUAD\\SQLEXPRESS;Trusted_Connection=Yes;");
	strcpy(tmpStr, "DRIVER=SQL Server Native Client 10.0;SERVER=QUAD\\SQLEXPRESS;DATABASE=Holly;UID=test;PWD=koblih;");
//	strcpy(tmpStr, "DRIVER=SQL Server Native Client 10.0;SERVER=QUAD\\SQLEXPRESS;Trusted_Connection=Yes;");
	memset(maxStr, 0x00, sizeof(maxStr));
	SQLSMALLINT returnSize=0;
	retcode = ::SQLDriverConnect(hdbc1, NULL, (SQLCHAR *)tmpStr, strlen(tmpStr), (SQLCHAR *)maxStr, sizeof(maxStr), &returnSize,
	                             SQL_DRIVER_NOPROMPT );
	DDUMP(retcode);
/*
   // Allocate ODBC connection and connect.
   retcode = SQLAllocConnect(henv, &hdbc1);
   // Make the connection, then print the information messages.
            retcode = SQLConnect(hdbc1, szDSN, (SWORD)strlen((char *)szDSN),
               szUID, (SWORD)strlen((char *)szUID),
               szAuthStr,
               (SWORD)strlen((char *)szAuthStr));*/
   if ( (retcode != SQL_SUCCESS) &&
        (retcode != SQL_SUCCESS_WITH_INFO) ) {
         ProcessLogMessages(hdbc1, hstmt1,
                "SQLConnect() Failed\n\n");
         return;
   }
   else {
        ProcessLogMessages(hdbc1, hstmt1,
                "\nConnect Successful\n\n");
   }
   // Allocate the statement handle.
            retcode = SQLAllocStmt(hdbc1,&hstmt1);
   // Execute the SELECT statement.
            retcode = SQLExecDirect(hstmt1, (SQLCHAR *)"SELECT TEXT FROM SUBJECT_ATTR", SQL_NTS);
   if ( (retcode != SQL_SUCCESS) &&
        (retcode != SQL_SUCCESS_WITH_INFO) ) {
         ProcessLogMessages(hdbc1, hstmt1,
                "SQLExecDirect hstmt1 Failed\n\n");
         return;
   }
   // Get first row.
   for(;;) {
	   retcode = SQLFetch(hstmt1);
	   if(retcode == SQL_NO_DATA)
	       break;
	   if ( (retcode != SQL_SUCCESS) &&
	        (retcode != SQL_SUCCESS_WITH_INFO) ) {
	         ProcessLogMessages(hdbc1, hstmt1,
	                "SQLFetch hstmt1 Failed\n\n");
	         return;
	   }
	   
	   SQLLEN len;
	   byte buffer[1024];
	   SQLGetData(hstmt1, 1, SQL_C_CHAR, buffer, 1024, &len);
	   if(len > 0)
		DDUMP(String(buffer, len));
	   else
	    LOG("NULL");   
	   
	   // Get the SQL_LONG column. CbBatch has size of data chunk
	   // the buffer can handle. Call SQLGetData until
	   // SQL_NO_DATA_FOUND.  cbBinSize on each call has the
	   // amount of data left to transfer.
/*	   cntr = 1;
	   do {
	      retcode = SQLGetData(hstmt1,   // hstmt
	         1,         // ipar
	         SQL_C_BINARY,      // fCType
	         Data,         // rgbValue
	         cbBatch,      // cbValueMax
	         &cbBinSize);      // pcbValue
	      printf("GetData iteration %d, pcbValue = %d\n", cntr++, cbBinSize);
	      if ( (retcode != SQL_SUCCESS) &&
	                    (retcode != SQL_SUCCESS_WITH_INFO) &&
	           retcode != SQL_NO_DATA_FOUND ){
	            ProcessLogMessages(hdbc1, hstmt1, "SQLGetData hstmt1 Failed\n\n");
	            return;
	      }
	   } while (retcode != SQL_NO_DATA_FOUND);
*/	   /* Clean up. */
   }
   LOG("OK");
   SQLFreeStmt(hstmt1, SQL_DROP);
   SQLDisconnect(hdbc1);
   SQLFreeConnect (hdbc1);
   SQLFreeEnv(henv);
   return;
}
