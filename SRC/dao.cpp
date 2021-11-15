/* DAO.CPP

020107	pulizia; implementato PR_DAODBOPEN e PR_DAODBCLOSE
991214	aggiunto PR_DAOEXEC e DaoExecute; aggiunto argomento a Database()
990414	aggiunto chiusura database in DaoQuery
*/

#include "stdafx.h"
#include <afxdao.h>
#include <afxdisp.h>

#include "crack.h"

#include "Iperlogo.h"
#include "ilcpp.h"

void argToText (int i, char* name)
{
	if (types[i] == LIST) MemScrivi (name, args[i]);
	else getnam (name, args[i]);
}
CString daoDatabaseName = "";
CDaoDatabase* pDaoDatabase = NULL;
CDaoRecordset* pDaoRecordset = NULL;

CDaoDatabase* Database (const char* dbName, BOOL bReadOnly)
{
	if (pDaoDatabase != NULL)
		if (dbName == daoDatabaseName)
			return pDaoDatabase;
		else
			pDaoDatabase->Close();

	BOOL bExclusive = FALSE;
	bReadOnly = FALSE;
	LPCTSTR lpszConnect = _T("");

	CDaoDatabase* pNewDatabase = new(CDaoDatabase) (NULL);
	try
	{
		pNewDatabase->Open(dbName, bExclusive, bReadOnly, lpszConnect);
	}
	catch(CDaoException daoE) {
		AfxMessageBox ("Iperlogo: Dao Exception (Open)");
		return NULL;
	}
	catch(CMemoryException memoryE) {
		AfxMessageBox ("Iperlogo: Memory Exception (Open)");
		return NULL;
	}

	daoDatabaseName = dbName;
	pDaoDatabase = pNewDatabase;

	return pNewDatabase;
}

CDaoRecordset* DaoRecordset (const char* dbName, const char* sql)
{
	CDaoRecordset* pDaoRs;

	if (   (pDaoDatabase != NULL)
		&& (dbName == daoDatabaseName)
		&& (pDaoRecordset != NULL)
		)
		pDaoRs = pDaoRecordset;
	else {
		BOOL bReadOnly = TRUE;
		CDaoDatabase* pDaoDb = Database (dbName, bReadOnly);
		CDaoRecordset* pNewRecordset = new (CDaoRecordset) (pDaoDb);

		pDaoRs = pNewRecordset;
		pDaoRecordset = pNewRecordset;
	}

	int nOptions = 0;
	try
	{
		pDaoRs->Open(AFX_DAO_USE_DEFAULT_TYPE, sql, nOptions);
	}
	catch(CDaoException daoE) {
		AfxMessageBox ("Iperlogo: Dao Exception");
		return NULL;
	}
	catch(CMemoryException memoryE) {
		AfxMessageBox ("Iperlogo: Memory Exception");
		return NULL;
	}

	return pDaoRs;
}

void DaoExecute (const char* dbName, const char* sql)
{
	BOOL bReadOnly = FALSE;
	CDaoDatabase* pDaoDb = Database (dbName, bReadOnly);

	int nOptions = 0;
	try
	{
		pDaoDb->Execute(sql, nOptions);
	}
	catch(CDaoException daoE) {
		AfxMessageBox ("Iperlogo: Dao Exception");
	}
	catch(CMemoryException memoryE) {
		AfxMessageBox ("Iperlogo: Memory Exception");
	}
//	pDaoDb->Close();
//	delete(pDaoDb);
}

node rassoc (node nome, node val, node lista);

node DaoQuery (const char* dbName, const char* sql, BOOL withHeading, BOOL collapseRows)
{
	node tabella = NULLP;
	CDaoRecordset* pDaoRs = DaoRecordset (dbName, sql);
	BOOL isBOF = FALSE, isEOF;

	try
	{
		isEOF = pDaoRs->IsEOF();
	}
	catch(CDaoException daoE) {
		AfxMessageBox ("Iperlogo: Dao Exception (IsEOF)");
		return NULL;
	}
	catch(CMemoryException memoryE) {
		AfxMessageBox ("Iperlogo: Memory Exception (IsEOF)");
		return NULL;
	}

if (! isEOF) {

///	GetRecordCount riporta il n. di record gia' acceduti !!!
//	long recordCount = pDaoRs->GetRecordCount();
//	throw( CDaoException, CMemoryException );

	short fieldCount;
	try
	{
		fieldCount = pDaoRs->GetFieldCount();
	}
	catch(CDaoException daoE) {
		AfxMessageBox ("Iperlogo: Dao Exception (GetFieldCount)");
		return NULL;
	}
	catch(CMemoryException memoryE) {
		AfxMessageBox ("Iperlogo: Memory Exception (GetFieldCount)");
		return NULL;
	}

	node nome, val, riga;

	int iField;
	CDaoFieldInfo fieldInfo;
	CString fieldName;
	COleVariant varValue;
	CString fieldValue;

	try
	{
		pDaoRs->MoveLast();
	}
	catch(CDaoException daoE) {
		AfxMessageBox ("Iperlogo: Dao Exception (MoveLast)");
		return NULL;
	}
	catch(CMemoryException memoryE) {
		AfxMessageBox ("Iperlogo: Memory Exception (MoveLast)");
		return NULL;
	}

	DWORD dwInfoOptions = AFX_DAO_PRIMARY_INFO;
	while (! isBOF) {
		riga = NULLP;
		for (iField = fieldCount-1; iField >= 0; --iField) {

			try
			{
				pDaoRs->GetFieldInfo(iField, fieldInfo, dwInfoOptions);
			}
			catch(CDaoException daoE) {
				AfxMessageBox ("Iperlogo: Dao Exception (GetFieldInfo)");
				return NULL;
			}
			catch(CMemoryException memoryE) {
				AfxMessageBox ("Iperlogo: Memory Exception (GetFieldInfo)");
				return NULL;
			}
			nome = metti_sc (fieldInfo.m_strName);

			try
			{
			pDaoRs->GetFieldValue(iField, varValue);
			}
			catch(CDaoException daoE) {
				AfxMessageBox ("Iperlogo: Dao Exception (GetFieldValue)");
				return NULL;
			}
			catch(CMemoryException memoryE) {
				AfxMessageBox ("Iperlogo: Memory Exception (GetFieldValue)");
				return NULL;
			}

			fieldValue = CCrack::strVARIANT(varValue);
			val = NULLP;

			if (_stricmp (fieldValue, "null") == 0)
				val = NULLP;
			else {
				switch(fieldInfo.m_nType){
				case (dbBoolean):
					val = (_stricmp (fieldValue, "True") == 0) ?
							pc_VERO : pc_FALSO;
				case (dbByte):
					val = consreal ((double) atoi (fieldValue));
					break;
				case (dbInteger):
					val = consreal ((double) atoi (fieldValue));
					break;
				case (dbLong):
					val = consreal ((double) atol (fieldValue));
					break;
				case (dbCurrency):
					val = logstr (fieldValue);
					break;
				case (dbSingle):
					val = consreal (atof (fieldValue));
					break;
				case (dbDouble):
					val = consreal (atof (fieldValue));
					break;
				case (dbDate):
					val = logstr (fieldValue);
					break;
				case (dbText):
					val = logstr (fieldValue);
					break;
				case (dbLongBinary):
					val = logstr (fieldValue);
					break;
				case (dbMemo):
					val = logstr (fieldValue);
					break;
				case (dbGUID):
					val = logstr (fieldValue);
					break;
				}
				if (withHeading)
					riga = rassoc (nome, val, riga);
				else
					riga = cons (val, riga, LIST);
			}
		}
		if (collapseRows)
			tabella = cons (car (riga), tabella, LIST);
		else
			tabella = cons (riga, tabella, LIST);

		try
		{
			pDaoRs->MovePrev();
		}
		catch(CDaoException daoE) {
			AfxMessageBox ("Iperlogo: Dao Exception (MovePrev)");
			return NULL;
		}
		catch(CMemoryException memoryE) {
			AfxMessageBox ("Iperlogo: Memory Exception (MovePrev)");
			return NULL;
		}

		try
		{
			isBOF = pDaoRs->IsBOF();
		}
		catch(CDaoException daoE) {
			AfxMessageBox ("Iperlogo: Dao Exception (IsBOF)");
			return NULL;
		}
		catch(CMemoryException memoryE) {
			AfxMessageBox ("Iperlogo: Memory Exception (IsBOF)");
			return NULL;
		}
	}
}

	try
	{
		pDaoRs->Close();
	}
	catch(CDaoException daoE) {
		AfxMessageBox ("Iperlogo: Dao Exception (Close)");
		return NULL;
	}

///	delete(pDaoRs);

///	pDaoDatabase->Close();
///	delete(pDaoDatabase);

	return tabella;
}

#ifdef DOC

struct CDaoFieldInfo
{
   CString m_strName;           // Primary
   short m_nType;               // Primary
   long m_lSize;                // Primary
   long m_lAttributes;          // Primary
   short m_nOrdinalPosition;    // Secondary
   BOOL m_bRequired;            // Secondary
   BOOL m_bAllowZeroLength;     // Secondary
   long m_lCollatingOrder;      // Secondary
   CString m_strForeignName;    // Secondary
   CString m_strSourceField;    // Secondary
   CString m_strSourceTable;    // Secondary
   CString m_strValidationRule; // All
   CString m_strValidationText; // All
   CString m_strDefaultValue;   // All
};

#endif // DOC

// GetRecordCount riporta il n. di record gia' acceduti !!!
void PR_DAOQUERY ()
{
	char dbName [MAXCHARS];
	char sql [64*K];
	argToText (0, dbName);
	argToText (1, sql);
	risultato = DaoQuery (dbName, sql, TRUE, FALSE);
}

void PR_DAOEXECUTE ()
{
	char dbName [MAXCHARS];
	char sql [64*K];
	argToText (0, dbName);
	argToText (1, sql);
	DaoExecute (dbName, sql);
}

// CDaoDatabase* pDaoDb;	020107

void PR_DAODBOPEN ()
{
//>020107
//	pDaoDb = new(CDaoDatabase) (NULL);
	char dbName [MAXCHARS];
	BOOL bReadOnly = FALSE;
	argToText (0, dbName);
	if (n_argomenti > 1)
		bReadOnly = bool_ (arg_2);
	CDaoDatabase* pDaoDb = Database (dbName, bReadOnly);
	if (pDaoDb != NULL)
		risultato = arg_1;
//<020107
}

void PR_DAODBCLOSE ()
{
//>020107
//	pDaoDb->Close ();
//	delete (pDaoDb);
	if (pDaoDatabase != NULL) {
		pDaoDatabase->Close ();
		delete (pDaoDatabase);
		pDaoDatabase = NULL;
		risultato = logstr (daoDatabaseName);
	}
//<020107
}

