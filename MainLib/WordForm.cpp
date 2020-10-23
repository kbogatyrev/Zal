#include "WordForm.h"
#include "Lexeme.h"

using namespace Hlib;

CWordForm::CWordForm() : m_pLexeme(0),
                         m_ullDbInsertHandle(0),
                         m_llDbKey(-1),
                         m_sWordForm(L""),
                         m_sStem(L""),
                         m_llStemId(0),
                         m_sEnding(L""),
                         m_llEndingDataId(-1),
                         m_llLexemeId(-1),
                         m_ePos(POS_UNDEFINED),
                         m_eCase(CASE_UNDEFINED),
                         m_eNumber(NUM_UNDEFINED),
                         m_eSubparadigm(SUBPARADIGM_UNDEFINED),
                         m_eGender(GENDER_UNDEFINED),
                         m_ePerson(PERSON_UNDEFINED),
                         m_eAnimacy(ANIM_UNDEFINED),
                         m_eReflexivity(REFL_UNDEFINED),
                         m_eAspect(ASPECT_UNDEFINED),
                         m_eStatus(STATUS_COMMON),
                         m_bIrregular(false),
                         m_bIsEdited(false),
                         m_bIsDifficult(false)
{
    m_sWordForm.SetVowels(g_szRusVowels);
    m_sStem.SetVowels(g_szRusVowels);
    m_sEnding.SetVowels(g_szRusVowels);
}

CWordForm::CWordForm(const CEString& sHash) : m_pLexeme(0),
                                              m_ullDbInsertHandle(0),
                                              m_llDbKey(-1),
                                              m_sWordForm(L""),
                                              m_sStem(L""),
                                              m_llStemId(0),
                                              m_sEnding(L""),
                                              m_llEndingDataId(-1),
                                              m_llLexemeId(-1),
                                              m_ePos(POS_UNDEFINED),
                                              m_eCase(CASE_UNDEFINED),
                                              m_eNumber(NUM_UNDEFINED),
                                              m_eSubparadigm(SUBPARADIGM_UNDEFINED),
                                              m_eGender(GENDER_UNDEFINED),
                                              m_ePerson(PERSON_UNDEFINED),
                                              m_eAnimacy(ANIM_UNDEFINED),
                                              m_eReflexivity(REFL_UNDEFINED),
                                              m_eAspect(ASPECT_UNDEFINED),
                                              m_eStatus(STATUS_COMMON),
                                              m_bIrregular(false),
                                              m_bIsEdited(false),
                                              m_bIsDifficult(false)
{
    ET_ReturnCode rc = eInitFromHash(sHash);
    if (rc != H_NO_ERROR)
    {
        ASSERT(0);
        CEString sMsg(L"eInitFromHash() failed, hash value =");
        sMsg += sHash + L".";
        ERROR_LOG(sMsg);
        throw CException(H_ERROR_INVALID_ARG, sMsg);
    }

    m_sWordForm.SetVowels(g_szRusVowels);
    m_sStem.SetVowels(g_szRusVowels);
    m_sEnding.SetVowels(g_szRusVowels);
}

CWordForm::CWordForm(const CWordForm * pSource)
{
    Copy(*pSource);
}

CWordForm::CWordForm(const CWordForm& source)
{
    Copy(source);
}

ILexeme * CWordForm::pLexeme()
{
    return m_pLexeme;
}

void CWordForm::SetLexeme(ILexeme * pLexeme)
{
    m_pLexeme = dynamic_cast<CLexeme *>(pLexeme);
}

CEString CWordForm::sGramHash()
{
    CHasher h_ (const_cast<CWordForm&>(*this));
    return h_.sGramHash();
}

ET_ReturnCode CWordForm::eInitFromHash (const CEString& sHash)
{
    try
    {
        CHasher hasher (*this);
        ET_ReturnCode rc = hasher.eDecodeHash(sHash);
        this->m_eAnimacy = hasher.m_eAnimacy;
        this->m_eAspect = hasher.m_eAspect;
        this->m_eCase = hasher.m_eCase;
        this->m_eGender = hasher.m_eGender;
        this->m_eNumber = hasher.m_eNumber;
        this->m_ePerson = hasher.m_ePerson;
        this->m_ePos = hasher.m_ePos;
        this->m_eReflexivity = hasher.m_eReflexivity;
        this->m_eSubparadigm = hasher.m_eSubparadigm;
        
        return rc;
    }
    catch (CException ex)
    {
        return H_EXCEPTION;  // logging should be always done by callee
    }

}   //  eInitFromHash (...)

ET_ReturnCode CWordForm::eClone(IWordForm *& pClonedObject)
{
    pClonedObject = new CWordForm(const_cast<const CWordForm&>(*this));

    return H_NO_ERROR;
}

ET_ReturnCode CWordForm::eGetFirstStressPos(int& iPos, ET_StressType& eType)
{
    m_sWordForm.SetVowels(g_szRusVowels);

    m_itStressPos = m_mapStress.begin();
    if (m_mapStress.end() == m_itStressPos)
    {
        iPos = -1;
        eType = STRESS_TYPE_UNDEFINED;
        return H_FALSE;
    }

    try
    {
        iPos = m_sWordForm.uiGetVowelPos(m_itStressPos->first);
        eType = m_itStressPos->second;
    }
    catch (CException ex)
    {
        return H_EXCEPTION;
    }

    return H_NO_ERROR;
}

ET_ReturnCode CWordForm::eGetNextStressPos(int& iPos, ET_StressType& eType)
{
    m_sWordForm.SetVowels(g_szRusVowels);

    if (m_itStressPos != m_mapStress.end())
    {
        ++m_itStressPos;
    }
    if (m_mapStress.end() == m_itStressPos)
    {
        return H_NO_MORE;
    }

    try
    {
        iPos = m_sWordForm.uiGetVowelPos(m_itStressPos->first);
        eType = m_itStressPos->second;
    }
    catch (CException ex)
    {
        return H_EXCEPTION;
    }

    return H_NO_ERROR;
}

ET_ReturnCode CWordForm::eSetStressPositions(map<int, ET_StressType> mapStress)
{
    m_mapStress = mapStress;

    return H_NO_ERROR;
}

//
// Used in manual editing
//
ET_ReturnCode CWordForm::eSaveIrregularForm()
{
    auto spDbHandle = m_pLexeme->spGetDb();
    if (nullptr == spDbHandle)
    {
        ERROR_LOG(L"No database access.");
        return H_ERROR_DB;
    }

    const StLexemeProperties& stProps = m_pLexeme->stGetProperties();

    if (m_llDbKey >= 0)
    {
        try
        {
            CEString sDelQuery(L"DELETE FROM irregular_forms WHERE id = ");
            sDelQuery += CEString::sToString(m_llDbKey);
            spDbHandle->Delete(sDelQuery);

            for (auto pairStress : m_mapStress)
            {
                sDelQuery = L"DELETE FROM irregular_stress WHERE form_id = ";
                sDelQuery += CEString::sToString(m_llDbKey);
                spDbHandle->Delete(sDelQuery);
            }
        }
        catch (CException & exc)
        {
            CEString sMsg(exc.szGetDescription());
            CEString sError;
            try
            {
                spDbHandle->GetLastError(sError);
                sMsg += CEString(L", error %d: ");
                sMsg += sError;
            }
            catch (...)
            {
                sMsg = L"Apparent DB error ";
            }

            sMsg += CEString::sToString(spDbHandle->iGetLastError());
            ERROR_LOG(sMsg);

            return H_ERROR_DB;
        }
    }

    if (!m_pLexeme->bHasIrregularForms())
    {
        m_pLexeme->SetHasIrregularForms(true);
        m_pLexeme->eSaveDescriptorInfo(m_pLexeme);
    }

    try
    {
        spDbHandle->PrepareForInsert(L"irregular_forms", 7);
        spDbHandle->Bind(1, stProps.llDescriptorId);
        spDbHandle->Bind(2, sGramHash());
        spDbHandle->Bind(3, m_sWordForm);
        spDbHandle->Bind(4, false);
        spDbHandle->Bind(5, sLeadComment());            // TODO
        spDbHandle->Bind(6, sTrailingComment());        // TODO
        spDbHandle->Bind(7, true);                      // is_edited

        spDbHandle->InsertRow();
        spDbHandle->Finalize();

        m_llDbKey = spDbHandle->llGetLastKey();
        m_bIsEdited = true;
    }
    catch (CException & exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            spDbHandle->GetLastError(sError);
            sMsg += CEString(L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(spDbHandle->iGetLastError());
        ERROR_LOG(sMsg);

        return H_ERROR_DB;
    }

    try
    {
        for (auto pair : m_mapStress)
        {
            spDbHandle->PrepareForInsert(L"irregular_stress", 4);
            spDbHandle->Bind(1, m_llDbKey);
            int iCharPos = m_sWordForm.uiGetVowelPos(pair.first);
            spDbHandle->Bind(2, iCharPos);
            bool bIsPrimary = (ET_StressType::STRESS_PRIMARY == pair.second) ? true : false;
            spDbHandle->Bind(3, bIsPrimary);
            spDbHandle->Bind(4, true);       // is_edited

            spDbHandle->InsertRow();
            spDbHandle->Finalize();
        }

        long long llFormKey = spDbHandle->llGetLastKey();
    }
    catch (CException & exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            spDbHandle->GetLastError(sError);
            sMsg += CEString(L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(spDbHandle->iGetLastError());
        ERROR_LOG(sMsg);

        return H_ERROR_DB;
    }

    return H_NO_ERROR;

}    //  eSaveIrregularForm()

bool CWordForm::bSaveStemToDb()
{
    shared_ptr<CSqlite> spDbHandle = nullptr;
    try
    {
        if (nullptr == m_pLexeme)
        {
            ERROR_LOG(L"No lexeme.");
            return false;
        }

        spDbHandle = m_pLexeme->spGetDb();
        if (nullptr == spDbHandle)
        {
            ERROR_LOG(L"No database access.");
            return false;
        }

        if (m_sStem.bIsEmpty())
        {
            ERROR_LOG(L"No stem.");
            return false;
        }

        bool bIgnoreOnConflict = true;
        spDbHandle->PrepareForInsert(L"stems", 1, bIgnoreOnConflict);
        spDbHandle->Bind(1, m_sStem);
        spDbHandle->InsertRow();
        spDbHandle->Finalize();
        if (m_llStemId < 1)
        {
            m_llStemId = -1;
            m_llStemId = spDbHandle->llGetLastKey();
        }
    }
    catch (CException& exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            spDbHandle->GetLastError(sError);
            sMsg += CEString(L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(spDbHandle->iGetLastError());
        ERROR_LOG(sMsg);

        return false;
    }

    return true;

}       // bSaveStemsToDb()

bool CWordForm::bSaveToDb()
{
    long long llStemDataId = -1;
    shared_ptr<CSqlite> spDbHandle = NULL;
    const StLexemeProperties& stLexemeProperties = m_pLexeme->stGetProperties();

    try
    {
        spDbHandle = m_pLexeme->spGetDb();
        if (nullptr == spDbHandle)
        {
            ERROR_LOG(L"No database access.");
            return false;
        }

        if (m_sStem.bIsEmpty())
        {
            ERROR_LOG(L"No stem.");
            return false;
        }

        if (0 == m_ullDbInsertHandle)
        {
            spDbHandle->uiPrepareForInsert(L"stem_data", 3, (sqlite3_stmt *&)m_ullDbInsertHandle, false);
        }
        spDbHandle->Bind(1, m_llStemId, m_ullDbInsertHandle);
        spDbHandle->Bind(2, sGramHash(), m_ullDbInsertHandle);
        spDbHandle->Bind(3, stLexemeProperties.llDescriptorId, m_ullDbInsertHandle);
        spDbHandle->InsertRow(m_ullDbInsertHandle);
        spDbHandle->Finalize(m_ullDbInsertHandle);
        llStemDataId = spDbHandle->llGetLastKey();

        spDbHandle->PrepareForInsert(L"wordforms", 2);
        spDbHandle->Bind(1, llStemDataId);
        spDbHandle->Bind(2, m_llEndingDataId);
        spDbHandle->InsertRow();
        spDbHandle->Finalize();
        long long llWordFormDbKey = spDbHandle->llGetLastKey();
        
        map<int, ET_StressType>::iterator itStress = m_mapStress.begin();
        for (; itStress != m_mapStress.end(); ++itStress)
        {
            spDbHandle->PrepareForInsert(L"stress_data", 4);
            spDbHandle->Bind(1, llWordFormDbKey);
            spDbHandle->Bind(2, (*itStress).first);
            bool bIsPrimary = ((*itStress).second == STRESS_PRIMARY) ? true : false;
            spDbHandle->Bind(3, bIsPrimary);
            spDbHandle->Bind(4, m_bIsVariant);
            spDbHandle->InsertRow();
            spDbHandle->Finalize();
        }
    }
    catch (CException& exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            spDbHandle->GetLastError(sError);
            sMsg += CEString(L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(spDbHandle->iGetLastError());
        ERROR_LOG(sMsg);

        return false;
    }

    return true;

}   //  bool bSaveToDb (...)

ET_ReturnCode CWordForm::eSaveTestData()
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (NULL == m_pLexeme)
    {
        return H_ERROR_POINTER;
    }

    auto spDb = m_pLexeme->spGetDb();
    CEString sHash = sGramHash();

    try
    {
        spDb->PrepareForInsert(L"test_data", 3);
        spDb->Bind(1, m_pLexeme->sHash());         // lexeme hash
        spDb->Bind(2, sHash);
        spDb->Bind(3, m_sWordForm);
        spDb->InsertRow();
        spDb->Finalize();

        auto llWordFormId = spDb->llGetLastKey();

        map<int, ET_StressType>::iterator itStress = m_mapStress.begin();
        for (; itStress != m_mapStress.end(); ++itStress)
        {
            spDb->PrepareForInsert(L"test_data_stress", 3);
            spDb->Bind(1, llWordFormId);
            spDb->Bind(2, (*itStress).first);
            ET_StressType eType = (*itStress).second ? STRESS_PRIMARY : STRESS_SECONDARY;
            spDb->Bind(3, eType);
            spDb->InsertRow();
            spDb->Finalize();
        }
    }
    catch (CException& exc)
    {
        spDb->RollbackTransaction();
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            spDb->GetLastError(sError);
            sMsg += L", error: ";
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(spDb->iGetLastError());
        ERROR_LOG(sMsg);

        return ET_ReturnCode(H_ERROR_DB);
    }

    return H_NO_ERROR;

}   //  eSaveTestData()

void CWordForm::Copy(const CWordForm& source)
{
    m_pLexeme = source.m_pLexeme;
    m_ullDbInsertHandle = source.m_ullDbInsertHandle;
    m_llDbKey = source.m_llDbKey;
    m_sWordForm = source.m_sWordForm;
    m_sStem = source.m_sStem;
    m_llStemId = source.m_llStemId;
    m_sEnding = source.m_sEnding;
    m_llEndingDataId = source.m_llEndingDataId;
    m_llLexemeId = source.m_llLexemeId;
    m_mapStress = source.m_mapStress;
    m_ePos = source.m_ePos;
    m_eCase = source.m_eCase;
    m_eNumber = source.m_eNumber;
    m_eSubparadigm = source.m_eSubparadigm;
    m_eGender = source.m_eGender;
    m_ePerson = source.m_ePerson;
    m_eAnimacy = source.m_eAnimacy;
    m_eReflexivity = source.m_eReflexivity;
    m_eAspect = source.m_eAspect;
    m_eStatus = source.m_eStatus;
    m_bIrregular = source.m_bIrregular;
    m_bIsEdited = source.m_bIsEdited;
    m_bIsDifficult = source.m_bIsDifficult;
    m_itStressPos = source.m_itStressPos;
    m_sLeadComment = source.m_sLeadComment;
    m_sTrailingComment = source.m_sTrailingComment;
}
