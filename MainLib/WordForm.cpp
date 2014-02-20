#include "StdAfx.h"
#include "WordForm.h"
#include "Lexeme.h"

using namespace Hlib;

ILexeme * CWordForm::pLexeme()
{
    return m_pLexeme;
}

/////////////////////////////////////////////////////////////////

int CWordForm::iGramHash()
{
    CHasher h_ (const_cast<CWordForm&>(*this));
    return h_.iGramHash();
}

ET_ReturnCode CWordForm::eInitFromHash (int iHash)
{
    try
    {
        CHasher hasher (*this);
        ET_ReturnCode rc = hasher.eDecodeHash(iHash);
        this->m_eAnimacy = hasher.m_eAnimacy;
        this->m_eAspect = hasher.m_eAspect;
        this->m_eCase = hasher.m_eCase;
        this->m_eGender = hasher.m_eGender;
        this->m_eNumber = hasher.m_eNumber;
        this->m_ePerson = hasher.m_ePerson;
        this->m_ePos = hasher.m_ePos;
        this->m_eReflexive = hasher.m_eReflexive;
        this->m_eSubparadigm = hasher.m_eSubparadigm;
        
        return rc;
    }
    catch (CException ex)
    {
        return H_EXCEPTION;  // logging should be always done by callee
    }

}   //  eInitFromHash (...)

bool CWordForm::bSaveToDb()
{
    CSqlite * pDbHandle = NULL;
    try
    {
        pDbHandle = m_pLexeme->pGetDb();
        if (NULL == pDbHandle)
        {
            ERROR_LOG(L"No database access.");
            return false;
        }
        
        if (m_sLemma.bIsEmpty())
        {
            ERROR_LOG(L"No lemma.");
            return false;
        }

        CEString sQueryStem(L"SELECT id FROM stems WHERE stem_string=\"");
        sQueryStem += m_sLemma;
        sQueryStem += L"\";";

        pDbHandle->PrepareForSelect(sQueryStem);
        __int64 llStemId = -1;
        bool bIdExists = pDbHandle->bGetRow();
        if (bIdExists)
        {
            pDbHandle->GetData(0, llStemId);
        }
        else
        {
            pDbHandle->PrepareForInsert(L"stems", 1);
            pDbHandle->Bind(1, m_sLemma);
            pDbHandle->InsertRow();
            pDbHandle->Finalize();
            llStemId = pDbHandle->llGetLastKey();
        }

        const StLexemeProperties& stLexemeProperties = m_pLexeme->stGetProperties();

        CEString sQueryStemData(L"SELECT id FROM stem_data WHERE lexeme_id=\"");
        sQueryStemData += CEString::sToString(stLexemeProperties.iDbKey);
        sQueryStemData += L"\" AND stem_id=\"";
        sQueryStemData += CEString::sToString(llStemId);
        sQueryStemData += L"\" AND gram_hash=\"";
        sQueryStemData += CEString::sToString(iGramHash());
        sQueryStemData += L"\";";

        pDbHandle->PrepareForSelect(sQueryStemData);
        __int64 llStemDataId = -1;
        bIdExists = pDbHandle->bGetRow();
        if (bIdExists)
        {
            pDbHandle->GetData(0, llStemDataId);
        }
        else
        {
            pDbHandle->PrepareForInsert(L"stem_data", 3);
            pDbHandle->Bind(1, llStemId);
            pDbHandle->Bind(2, iGramHash());
            pDbHandle->Bind(3, stLexemeProperties.iDbKey);
            pDbHandle->InsertRow();
            pDbHandle->Finalize();
            llStemDataId = pDbHandle->llGetLastKey();
        }

        CEString sQueryEnding(L"SELECT id FROM endings WHERE ending_string=\"");
        sQueryEnding += m_sEnding;
        sQueryEnding += L"\";";

        pDbHandle->PrepareForSelect(sQueryEnding);
        __int64 llEndingId = -1;
        bIdExists = pDbHandle->bGetRow();
        if (bIdExists)
        {
            pDbHandle->GetData(0, llEndingId);
        }
        else
        {
            ERROR_LOG(L"No ending.");   // All endings must be known in advance (created by source conversion process)
            return false;
        }

        CEString sQueryEndingData(L"SELECT id FROM ending_data WHERE ending_id=\"");
        sQueryEndingData += CEString::sToString(llEndingId);
        sQueryEndingData += L"\";";

        pDbHandle->PrepareForSelect(sQueryEndingData);
        __int64 llEndingDataId = -1;
        bIdExists = pDbHandle->bGetRow();
        if (bIdExists)
        {
            pDbHandle->GetData(0, llEndingDataId);
        }
        else
        {
            pDbHandle->PrepareForInsert(L"ending_data", 1);
            pDbHandle->Bind(1, llEndingId);
            pDbHandle->InsertRow();
            pDbHandle->Finalize();
            llEndingDataId = pDbHandle->llGetLastKey();
        }

        pDbHandle->PrepareForInsert(L"wordforms", 2);
        pDbHandle->Bind(1, llStemDataId);
        pDbHandle->Bind(2, llEndingDataId);
        pDbHandle->InsertRow();
        pDbHandle->Finalize();
        __int64 llWordFormDbKey = pDbHandle->llGetLastKey();
    }
    catch (CException& exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            pDbHandle->GetLastError(sError);
            sMsg += CEString(L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg + CEString::sToString(pDbHandle->iGetLastError());
        ERROR_LOG(sMsg);

        return false;
    }

    return true;

}   //  bool bSaveToDb (...)

ET_ReturnCode CWordForm::SaveTestData()
{
    ET_ReturnCode rc = H_NO_ERROR;

    CSqlite& db = *m_pLexeme->pGetDb();

    int iHash = iGramHash();
    try
    {
        db.PrepareForInsert(L"test_data", 3);
        //        db.v_Bind (1, pco_Lexeme->i_DbKey);  // descriptor id
        //        db.v_Bind (1, (__int64)pco_Lexeme->ui_Hash());    // lexeme hash
        db.Bind(1, m_pLexeme->sHash());    // lexeme hash
        db.Bind(2, iHash);
        db.Bind(3, m_sWordForm);
        db.InsertRow();
        db.Finalize();

        __int64 llWordFormId = db.llGetLastKey();

        map<int, ET_StressType>::iterator itStress = m_mapStress.begin();
        for (; itStress != m_mapStress.end(); ++itStress)
        {
            db.PrepareForInsert(L"test_data_stress", 3);
            db.Bind(1, llWordFormId);
            db.Bind(2, (*itStress).first);
            ET_StressType eType = (*itStress).second ? STRESS_PRIMARY : STRESS_SECONDARY;
            db.Bind(3, eType);
            db.InsertRow();
            db.Finalize();
        }
    }
    catch (CException& exc)
    {
        db.RollbackTransaction();
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            db.GetLastError(sError);
            sMsg += L", error: ";
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(db.iGetLastError());
        ERROR_LOG(sMsg);

        return ET_ReturnCode(H_ERROR_DB);
    }

    return H_NO_ERROR;

}   //  SaveTestData()
