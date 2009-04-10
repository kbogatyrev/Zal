/*

*/

#include "stdafx.h"
#include "SqliteWrapper.h"
#include "ExtString.h"
#include "Lexeme.h"
#include "ZalSourceReader.h"

HRESULT CT_ZalSourceReader::FinalConstruct()
{
    CoInitialize (NULL);

    return S_OK;
}

void CT_ZalSourceReader::FinalRelease()
{
    // TODO: check for and report errors
    CoUninitialize();
}

HRESULT CT_ZalSourceReader::ProcessSourceFile (BSTR bstr_sourcePath,
                                               BSTR bstr_dbPath,
                                               int i_maxEntries)
{
    USES_CONVERSION;

    HRESULT h_r = S_OK;

    StatusUpdate (0);

    FILE * io_stream = NULL;
    errno_t i_error = _tfopen_s (&io_stream, OLE2W (bstr_sourcePath), L"r, ccs=UNICODE");
    if (0 != i_error)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to open source, error %d", i_error);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return E_FAIL;
    }

    CT_Sqlite co_sqlite;
    int i_r = co_sqlite.i_Open (OLE2W (bstr_dbPath));
    if (SQLITE_OK != i_r)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to open database, error %d", i_r);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, 0xF000 + i_r);
    }

 //   i_r = co_sqlite.i_Exec (L"CREATE TABLE Test (Entry TEXT)");
 //   if (SQLITE_OK != i_r)
 //   {
 //       CString cs_msg;
 //       cs_msg.Format (L"Unable to create DB table, error %d", i_r);
 //       ERROR_LOG ((LPCTSTR)cs_msg);
 //       return MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, 0xF000 + i_r);
 //   }

    long l_fileLength = _filelength (_fileno (io_stream))/sizeof (TCHAR);
    long l_charsRead = 0;
    int i_percentDone = 0;
    int i_entriesRead = 0;

    TCHAR sz_lineBuf[10000];
    BOOL ui_abort = false;
    for (; (!feof (io_stream) && !ui_abort); ++i_entriesRead)
    {
        if (i_entriesRead == i_maxEntries)
        {
            break;
        }

        TCHAR * sz_ret = _fgetts (sz_lineBuf, 10000, io_stream);
        if (NULL == sz_ret)
        {
            int i_error = ferror (io_stream);
            if (0 != i_error)
            {
                break;
            }
        }

        wstring str_line (sz_lineBuf);
        l_charsRead += str_line.length();

        if (i_maxEntries > 0)
        {
            i_percentDone = (int) (((double)i_entriesRead/(double)i_maxEntries) * 100);
        }
        else
        {
            int i_pd = (int) (((double)l_charsRead/(double)l_fileLength) * 100);
            if (i_pd > i_percentDone)
            {
                i_percentDone = i_pd;
            }
        }
        StatusUpdate (i_percentDone);

        CString cstr_query (L"INSERT INTO Test VALUES('");
        cstr_query += CString (sz_lineBuf);
        cstr_query += L"')";
        CComBSTR co_bstrQuery (cstr_query);
        co_sqlite.i_Exec (OLE2W (co_bstrQuery));

        StatusCheck (ui_abort);
    }

    if (!ui_abort)
    {
        StatusUpdate (100);
    }

    return S_OK;
}

HRESULT CT_ZalSourceReader::StatusUpdate (int i_progress)
{
    HRESULT h_r = S_OK;

    int i_nConnections = m_vec.GetSize();
    for (int i_c = 0; i_c < i_nConnections; ++i_c)
    {
        Lock();
        CComPtr<IUnknown> sp_unkConnection = m_vec.GetAt (i_c);
        CComQIPtr<IZalNotification> sp_qiConnection = sp_unkConnection;
        Unlock();
        if (sp_qiConnection)
        {
            h_r = sp_qiConnection->ProgressNotification (i_progress);
        }
    }

    return h_r;
}

HRESULT CT_ZalSourceReader::StatusCheck (BOOL& b_cancel)
{
    HRESULT h_r = S_OK;

    int i_nConnections = m_vec.GetSize();
    for (int i_c = 0; i_c < i_nConnections; ++i_c)
    {
        Lock();
        CComPtr<IUnknown> sp_unkConnection = m_vec.GetAt (i_c);
        CComQIPtr<IZalNotification> sp_qiConnection = sp_unkConnection;
        Unlock();
        if (sp_qiConnection)
        {
            h_r = sp_qiConnection->StatusCheck (&b_cancel);
        }
    }

    return h_r;
}

//
// Non-exposed methods
//
int i_DelStresses (wstring& str_lemma, int& i_s1, int& i_s2)
{
    // TODO: implement stress pos. ident
	i_s1 = i_s2 = 0;
	return 0;
}

int CT_ZalSourceReader::i_Parse (const wstring& str_entry)
{
    CT_ExtString xstr_entry (str_entry, _T(" ()[]"), _T(""), _T(""), _T(""));

	wstring str_token (L"");
    wstring str_lemma;
	int i_token = 0;
	int i_s1, i_s2;

	// Extract the main symbol
	/*
	********************  TO DO:  ********************
	Current code doesn't handle cases like мо <жо 1а>

	**************************************************
	*/
	str_lemma = xstr_entry.str_GetField (++i_token);

    CT_Lexeme * pco_entry = NULL;
	while (NULL == pco_entry)
	{
		str_token = xstr_entry.str_GetField (++i_token);
		if (str_token == L"мо" || 
            str_token == L"жо" || 
            str_token == L"м" || 
            str_token == L"ж" || 
            str_token == L"с" || 
            str_token == L"мо-жо")
		{
			pco_entry = new CT_Noun;
			pco_entry->e_PartOfSpeech = NOUN;
			pco_entry->str_MainSymbol = str_token;
			i_DelStresses (str_lemma, i_s1, i_s2);
			pco_entry->str_Lemma = str_lemma;
			pco_entry->i_PrimaryStress = i_s1;
			pco_entry->i_SecondaryStress = i_s2;
		}

		if (str_token == L"св" || str_token == L"нсв" || str_token == L"св-нсв")
		{
			pco_entry = new CT_Verb;
			pco_entry->e_PartOfSpeech = VERB;
			pco_entry->str_MainSymbol = str_token;
			i_DelStresses (str_lemma, i_s1, i_s2);
			pco_entry->str_Lemma = str_lemma;
			pco_entry->i_PrimaryStress = i_s1;
			pco_entry->i_SecondaryStress = i_s2;
		}
		if (L"п" == str_token)
		{
			pco_entry = new CT_Adj;
			pco_entry->e_PartOfSpeech = ADJ;
			pco_entry->str_MainSymbol = str_token;
			i_DelStresses (str_lemma, i_s1, i_s2);
			pco_entry->str_Lemma = str_lemma;
			pco_entry->i_PrimaryStress = i_s1;
			pco_entry->i_SecondaryStress = i_s2;
		}
		if (str_token == L"н" || 
            str_token == L"част." || 
            str_token == L"предл." || 
            str_token == L"союз" || 
            str_token == L"вводн." || 
            str_token == L"предик." || 
            str_token == L"межд." || 
            str_token == L"сравн.")
		{
			pco_entry = new CT_Uninflected;
			if (str_token == L"н")
            {
                pco_entry->e_PartOfSpeech = ADV;
            }
			if (str_token == L"част.")
            {
                pco_entry->e_PartOfSpeech = PART;
            }
			if (str_token == L"предл.") 
            {
                pco_entry->e_PartOfSpeech = PREP;
            }
			if (str_token == L"союз") 
            {
                pco_entry->e_PartOfSpeech = CONJ;
            }
			if (str_token == L"вводн.") 
            {
                pco_entry->e_PartOfSpeech = PARENTH;
            }
			if (str_token == L"предик.") 
            {
                pco_entry->e_PartOfSpeech = PREDIC;
            }
			if (str_token == L"межд.") 
            {
                pco_entry->e_PartOfSpeech = INTERJ;
            }
			if (str_token == L"сравн.") 
            {
                pco_entry->e_PartOfSpeech = COMPAR;
            }
			pco_entry->str_MainSymbol = str_token;
			i_DelStresses (str_lemma, i_s1, i_s2);
			pco_entry->str_Lemma = str_lemma;
			pco_entry->i_PrimaryStress = i_s1;
			pco_entry->i_SecondaryStress = i_s2;
		}
	}

	return 0;

}   //  i_Parse (...)
