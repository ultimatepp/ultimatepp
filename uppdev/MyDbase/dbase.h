#ifndef ___DBASE_H__
#define ___DBASE_H__

/*
	Definizioni:
	Filed: un singolo campo, individuato da una etichetta, appartenente ad un record
	Record: l'insieme dei campi "Field" univoci che costituiscono la struttura della tabella "Table"
	Table: il file strutturato a Record che contiene tutte le registrazioni

*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 

#include <MySupport/support.h>
#include <CtrlLib/CtrlLib.h>
#include <Core/Core.h>
#include <MyCsv/csv.h>

using namespace Upp;

// predefined values
#define   MTDB_version_		   		0x05		// versione del tb
#define		MTDB_DB_HEAD					1024		// lunghezza intestazione del db
#define		MTDB_TB_NAME_PTR			32			// posizione del nome tabella rispetto all'offset
#define		MTDB_TB_NAME_LEN			32			// lunghezza massima nome tabella
#define		MTDB_TB_INFO_LEN			320			// lunghezza area intestazione tb
#define		MTDB_TB_FD_LEN				320			// lunghezza area singolo campo
#define		MTDB_TB_FLD_CMT_LEN		256			// lunghezza in byte del commento campo
#define		MTDB_TB_EOF						0x27		// ultimo byte di fine tb
#define		MTDB_TB_RECORD_DELETED		0x2A		// marcatore record cancellato
#define		MTDB_TB_RECORD_ENABLED		0x20		// marcatore record disponibile
#define		MTDB_TB_FD_LABEL_LEN	0x20		// lunghezza etichetta campo
#define		MTDB_DB_TYPE					0xAA55	// tipo di tb
#define		MTDB_TB_MAX_FDS_N			256			// numero massimo di campi per tb
#define		MTDB_TB_COMMENT_PTR		64			// puntatore commento tabella
#define		MTDB_TB_COMMENT_LEN		256			// lunghezza in byte del commento tabella

// record selector
#define   MTDB_TB_APP 	 				(int)-1    // append record to new record
#define   MTDB_TB_ACT  					(int)-2    // use actual selected record
#define		MTDB_LAST_AFFECTED_REC				MTDB_TB_ACT

// returned error
#define		MTDB_SUCCESS					0				// no error returned - ok
#define   MTDB_TB_NOPOINTER    	-1  		// no record selected
#define   MTDB_TB_ERROR        	-2  		// generic error
#define   MTDB_FD_ERROR     		-3  		// field error generic
#define   MTDB_REC_DELETED     	-4  		// the selected record are deleted
#define   MTDB_NO_FD        		-5  		// selected field do not exist
#define   MTDB_INV_RECNO       	-6  		// invalid value of recno
#define		MTDB_DB_TYPE_ERROR		-7			// il file aperto non e' compatibile
#define		MTDB_UNKNOW_DATA			-8			// dato memorizzato sconosciuto
#define		MTDB_FD_WR_PROT				-9			// scrittura del campo bloccata
#define		MTDB_TB_NOT_FOUND			-10			// selected table not found in the database
#define		MTDB_TB_EXIST					-11			// this table arleady exist
#define		MTDB_TB_NAME_ERROR		-12			// wrong table name
#define		MTDB_FD_EMPTY					-13			// the filed is empty
#define		MDTB_DB_OPEN					-14			// the database is opened
#define		MTDB_INCOMP_DATA			-15			// il tipo di dato non compatibile con quello del database

// field flags
#define		MTDB_FD_DUPLIC				(short)1
#define		MTDB_FD_PRIM_KEY			(short)7

// field type
#define		MTDB_STR					0x4300

#define		MTDB_INT					0x4E00
#define		MTDB_DOUBLE				0x4E01

#define		MTDB_BOOL					0x4C00

#define		MTDB_DATETIME			0x4400
#define		MTDB_DATE					0x4401
#define		MTDB_TIME					0x4402

#define		MTDB_MEMOBIN			0x0100
#define		MTDB_MEMOASC			0x0101

#define		MTDB_STREAMV1			0x0200
#define		MTDB_STREAMV2			0x0201
#define		MTDB_STREAMV3			0x0202

// Sort criteria
#define		MTDB_ASCENDING		0
#define		MTDB_DESCENDING		1
#define		MTDB_ASC					MTDB_ASCENDING
#define		MTDB_DESC					MTDB_DESCENDING

#include <MyDbase/dbDatabase.h>
#include <MyDbase/dbRecord.h>
#include <MyDbase/dbRecordSet.h>
#include <MyDbase/compare.h>
#include <MyDbase/utility.h>

#endif
