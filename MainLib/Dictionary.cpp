// Dictionary.cpp : Implementation of CDictionary

#include <ctime>
#include <cassert>
#include <algorithm>

#include "WordForm.h"
#include "Lexeme.h"
#include "Analytics.h"
#include "Parser.h"
#include "Verifier.h"
#include "Dictionary.h"

using namespace Hlib;

ET_ReturnCode CLexemeEnumerator::eReset()
{
    if (nullptr == m_pDictionary)
    {
        return H_ERROR_POINTER;
    }

    m_itCurrentLexeme = m_pDictionary->m_vecLexemes.begin();
    return H_NO_ERROR;
}

ET_ReturnCode CLexemeEnumerator::eGetFirstLexeme(ILexeme*& pLexeme)
{
    if (nullptr == m_pDictionary)
    {
        return H_ERROR_POINTER;
    }

    m_itCurrentLexeme = m_pDictionary->m_vecLexemes.begin();
    if (m_pDictionary->m_vecLexemes.end() == m_itCurrentLexeme)
    {
        return H_FALSE;
    }

    pLexeme = *m_itCurrentLexeme;

    return H_NO_ERROR;
}

ET_ReturnCode CLexemeEnumerator::eGetFirstLexeme(CLexeme*& pLexeme)
{
    if (nullptr == m_pDictionary)
    {
        return H_ERROR_POINTER;
    }

    m_itCurrentLexeme = m_pDictionary->m_vecLexemes.begin();
    if (m_pDictionary->m_vecLexemes.end() == m_itCurrentLexeme)
    {
        return H_FALSE;
    }

    pLexeme = *m_itCurrentLexeme;

    return H_NO_ERROR;
}

ET_ReturnCode CLexemeEnumerator::eGetNextLexeme(ILexeme*& pLexeme)
{
    if (nullptr == m_pDictionary)
    {
        return H_ERROR_POINTER;
    }

    if (m_itCurrentLexeme != m_pDictionary->m_vecLexemes.end())
    {
        ++m_itCurrentLexeme;
    }

    if (m_pDictionary->m_vecLexemes.end() == m_itCurrentLexeme)
    {
        return H_NO_MORE;
    }

    pLexeme = *m_itCurrentLexeme;

    return H_NO_ERROR;
}

ET_ReturnCode CLexemeEnumerator::eGetNextLexeme(CLexeme*& pLexeme)
{
    if (nullptr == m_pDictionary)
    {
        return H_ERROR_POINTER;
    }

    if (m_itCurrentLexeme != m_pDictionary->m_vecLexemes.end())
    {
        ++m_itCurrentLexeme;
    }

    if (m_pDictionary->m_vecLexemes.end() == m_itCurrentLexeme)
    {
        return H_NO_MORE;
    }

    pLexeme = *m_itCurrentLexeme;

    return H_NO_ERROR;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static CEString sQueryBase(L"SELECT \
                            headword.source, \
                            headword.id, \
                            headword.comment, \
                            headword.variant_comment, \
                            headword.plural_of, \
                            headword.usage, \
                            headword.variant, \
                            headword.see_ref, \
                            headword.back_ref, \
                            headword.source_entry_id, \
                            descriptor.id, \
                            descriptor.graphic_stem, \
                            descriptor.graphic_stem2, \
                            descriptor.is_variant, \
                            descriptor.main_symbol, \
                            descriptor.part_of_speech, \
                            descriptor.is_plural_of, \
                            descriptor.is_intransitive, \
                            descriptor.is_reflexive, \
                            descriptor.main_symbol_plural_of, \
                            descriptor.alt_main_symbol, \
                            descriptor.inflection_type, \
                            descriptor.comment, \
                            descriptor.alt_main_symbol_comment, \
                            descriptor.alt_inflection_comment, \
                            descriptor.verb_stem_alternation, \
                            descriptor.part_past_pass_zhd, \
                            descriptor.section, \
                            descriptor.no_comparative, \
                            descriptor.no_long_forms, \
                            descriptor.assumed_forms, \
                            descriptor.yo_alternation, \
                            descriptor.o_alternation, \
                            descriptor.second_genitive, \
                            descriptor.is_impersonal, \
                            descriptor.is_iterative, \
                            descriptor.has_aspect_pair, \
                            descriptor.has_difficult_forms, \
                            descriptor.has_missing_forms, \
                            descriptor.has_irregular_forms, \
                            descriptor.irregular_forms_lead_comment, \
                            descriptor.restricted_contexts, \
                            descriptor.contexts, \
                            descriptor.cognate, \
                            descriptor.trailing_comment, \
                            inflection.id, \
                            inflection.is_primary, \
                            inflection.inflection_type, \
                            inflection.accent_type1, \
                            inflection.accent_type2, \
                            inflection.short_form_restrictions, \
                            inflection.past_part_restrictions, \
                            inflection.no_short_form, \
                            inflection.no_past_part, \
                            inflection.fleeting_vowel, \
                            inflection.stem_augment, \
                            inflection.inflected_parts, \
                            inflection.is_second_part \
                            FROM \
                            headword INNER JOIN descriptor ON descriptor.word_id = headword.id ");

CDictionary::CDictionary() //: m_spDb(NULL), m_pParser(NULL), m_pAnalytics(NULL), m_pVerifier(NULL)
{}

CDictionary::~CDictionary()
{
    Clear();
//    delete m_spDb;
    delete m_pVerifier;
}

ET_ReturnCode CDictionary::eSetDbPath(const CEString& sPath)
{
    m_sDbPath = sPath;
    try
    {
//        delete m_spDb;   // needed for ref counting

        m_spDb = make_shared<CSqlite>(m_sDbPath);
        if (!m_spDb)
        {
            return H_ERROR_DB;
        }
    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}   //  setDbPath (...)

CEString CDictionary::sGetDbPath()
{
    return m_sDbPath;
}

ET_ReturnCode CDictionary::eCreateLexemeForEdit(ILexeme *& pLexeme)
{
    if (nullptr == m_spDb)
    {
        pLexeme = NULL;
        return H_ERROR_POINTER;
    }

    pLexeme = new CLexeme(this);
    if (NULL == pLexeme)
    {
        return H_ERROR_POINTER;
    }

    return H_NO_ERROR;
}

ET_ReturnCode CDictionary::eCopyLexemeForEdit(const ILexeme * pSource, ILexeme *& pCopy)
{
    if (nullptr == m_spDb)
    {
        pSource = NULL;
        return H_ERROR_POINTER;
    }

    const CLexeme * pLexeme = dynamic_cast<const CLexeme *>(pSource);
    pCopy = new CLexeme(*pLexeme);
    if (NULL == pCopy)
    {
        return H_ERROR_POINTER;
    }

    pCopy->SetDictionary(this);

    return H_NO_ERROR;
}

ET_ReturnCode CDictionary::eGetLexemeById(long long llId, ILexeme *& pILexeme)
{
    ET_ReturnCode rc = H_NO_ERROR;
    CEString sQuery(sQueryBase);
    sQuery += L"LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id ";
    sQuery += L"WHERE descriptor.id = ";
    sQuery += CEString::sToString(llId);
    sQuery += L";";

    uint64_t uiQueryHandle = 0;
    rc = eQueryDb(sQuery, uiQueryHandle);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }
    
    CLexeme * pLexeme = new CLexeme(this);
    if (NULL == pLexeme)
    {
        return H_ERROR_POINTER;
    }

    StLexemeProperties& stProperties = pLexeme->stGetPropertiesForWriteAccess();
    rc = eGetSingleRow(stProperties, uiQueryHandle);
    if (H_NO_ERROR != rc)
    {
        delete pLexeme;
        pLexeme = NULL;
        ERROR_LOG(L"Error retrieving lexeme data.");
        return rc;
    }

    rc = eGetSingleRow(stProperties, uiQueryHandle);
    if (H_NO_MORE != rc)
    {
        delete pLexeme;
        pLexeme = NULL;
        ERROR_LOG(L"More than one row returned for a single lexeme ID or DB error.");
        return H_ERROR_UNEXPECTED;
    }

    m_spDb->Finalize(uiQueryHandle);

    m_vecLexemes.push_back(pLexeme);

    pILexeme = pLexeme;

    return rc;

}   //  eGetLexemeById (...)

ET_ReturnCode CDictionary::eGetLexemesByHash(const CEString& sMd5)
{
    ET_ReturnCode rc = H_NO_ERROR;
    CEString sQuery(sQueryBase);
    sQuery += L"INNER JOIN lexeme_hash_to_descriptor as lhd on lhd.descriptor_id=descriptor.id ";
    sQuery += L"LEFT OUTER JOIN inflection ON lhd.inflection_id = inflection.id ";
    sQuery += L"WHERE lexeme_hash = \"";
    sQuery += sMd5;
    sQuery += L"\";";

    uint64_t uiQueryHandle = 0;
    rc = eQueryDb(sQuery, uiQueryHandle);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    while (H_NO_ERROR == rc)
    {
        CLexeme * pLexeme = new CLexeme(this);
        if (NULL == pLexeme)
        {
            return H_ERROR_POINTER;
        }

        rc = eGetSingleRow(pLexeme->stGetPropertiesForWriteAccess(), uiQueryHandle);
        if (H_NO_ERROR == rc)
        {
            m_vecLexemes.push_back(pLexeme);
        }
        if (H_NO_ERROR != rc && H_NO_MORE != rc)
        {
            delete pLexeme;
            pLexeme = NULL;
            ERROR_LOG(L"Error retrieving lexeme data.");
            return rc;
        }
    }

    m_spDb->Finalize(uiQueryHandle);

    return (H_NO_MORE == rc) ? H_NO_ERROR : rc;

}   //  eGetLexemesByMd5 (...)

ET_ReturnCode CDictionary::eGetLexemesByGraphicStem(const CEString& sHeadword)
{
    ET_ReturnCode rc = H_NO_ERROR;
    CEString sQuery(sQueryBase);
    sQuery += L"LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id ";
    sQuery += L"WHERE descriptor.graphic_stem = \"";
    sQuery += sHeadword;
    sQuery += L"\";";

    uint64_t uiQueryHandle = 0;
    rc = eQueryDb(sQuery, uiQueryHandle);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    while (H_NO_ERROR == rc)
    {
        CLexeme * pLexeme = new CLexeme(this);
        if (NULL == pLexeme)
        {
            Clear();
            return H_ERROR_POINTER;
        }

        rc = eGetSingleRow(pLexeme->stGetPropertiesForWriteAccess(), uiQueryHandle);
        if (H_NO_ERROR != rc && H_NO_MORE != rc)
        {
            Clear();
            delete pLexeme;
            pLexeme = NULL;
            ERROR_LOG(L"Error retrieving lexeme data.");
            return rc;
        }

        m_vecLexemes.push_back(pLexeme);
    }

    m_spDb->Finalize(uiQueryHandle);

    return (H_NO_ERROR == rc) ? H_NO_ERROR : rc;
}

ET_ReturnCode CDictionary::eGetLexemesByInitialForm(const CEString& sSource)
{
    ET_ReturnCode rc = H_NO_ERROR;
    CEString sQuery(sQueryBase);
    sQuery += L"LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id ";
    sQuery += L"WHERE headword.source = \"";
    sQuery += sSource;
    sQuery += L"\";";

    uint64_t uiQueryHandle = 0;
    rc = eQueryDb(sQuery, uiQueryHandle);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    bool bFound = false;
    while (H_NO_ERROR == rc)
    {
        CLexeme * pLexeme = new CLexeme(this);
        if (NULL == pLexeme)
        {
            return H_ERROR_POINTER;
        }

        rc = eGetSingleRow(pLexeme->stGetPropertiesForWriteAccess(), uiQueryHandle);
        if (H_NO_ERROR == rc)
        {
            bFound = true;
            m_vecLexemes.push_back(pLexeme);
        }
        if (H_NO_ERROR != rc && H_NO_MORE != rc)
        {
            Clear();
            delete pLexeme;
            pLexeme = NULL;
            ERROR_LOG(L"Error retrieving lexeme data.");
            return rc;
        }
    }

    m_spDb->Finalize(uiQueryHandle);

    if (H_NO_MORE == rc && !bFound)
    {
        return H_FALSE;
    }

    return rc;

}   //  eGetLexemesByInitialForm()

void CDictionary::Clear()
{
    vector<CLexeme *>::iterator itL = m_vecLexemes.begin();
    for (; itL != m_vecLexemes.end(); ++itL)
    {
        delete *itL;
    }
    m_vecLexemes.clear();
}

ET_ReturnCode CDictionary::Clear(ILexeme * pLexeme)
{
    vector<CLexeme *>::iterator itL = m_vecLexemes.begin();
    for (; itL != m_vecLexemes.end(); ++itL)
    {
        if (*itL == pLexeme)
        {
            delete *itL;
            m_vecLexemes.erase(itL);
            return H_NO_ERROR;
        }
    }

    return H_ERROR_UNEXPECTED;
}

ET_ReturnCode CDictionary::eCreateLexemeEnumerator(ILexemeEnumerator*& le)
{
    le = new CLexemeEnumerator(this);
    if (!le)
    {
        ERROR_LOG(L"Error retrieving ILexemeEnumerator.");
        return H_ERROR_POINTER;
    }

    return H_NO_ERROR;
}

void CDictionary::DeleteLexemeEnumerator(ILexemeEnumerator* pLe)
{
    delete pLe;
}

ET_ReturnCode CDictionary::eGetParser(shared_ptr<IParser>& spParser)
{
    if (nullptr == m_spDb)
    {
        m_pVerifier = nullptr;
        ERROR_LOG(L"Error retrieving IParser interface.");
        return H_ERROR_POINTER;
    }

    if (nullptr == m_spParser)
    {
        m_spParser = make_shared<CParser>(m_spDb);
    }

    m_spParser->SetDb(m_spDb);

    spParser = m_spParser;

    return H_NO_ERROR;
}

ET_ReturnCode CDictionary::eGetAnalytics(IAnalytics*& pAnalytics)
{
    if (nullptr == m_spDb)
    {
        m_pAnalytics = NULL;
        ERROR_LOG(L"Error retrieving IAnalytics interface.");
        return H_ERROR_POINTER;
    }

    if (nullptr == m_spParser)
    {
        m_spParser = make_shared<CParser>(m_spDb);
    }

    if (NULL == m_pAnalytics)
    {
        m_pAnalytics = new CAnalytics(m_spDb, m_spParser);
        pAnalytics = m_pAnalytics;
    }

    return H_NO_ERROR;
}

ET_ReturnCode CDictionary::eGetVerifier(IVerifier *& pVerifier)
{
    if (NULL == m_spDb)
    {
        m_pVerifier = NULL;
        ERROR_LOG(L"Error retrieving IVerifier interface.");
        return H_ERROR_POINTER;
    }

    if (NULL == m_pVerifier)
    {
        m_pVerifier = new CVerifier(this);
    }

    pVerifier = m_pVerifier;

    return H_NO_ERROR;
}

ET_ReturnCode CDictionary::eExportTestData(const CEString& sPath, PROGRESS_CALLBACK_CLR pProgressCallback)
{
    if (NULL == m_spDb)
    {
        ERROR_LOG(L"Unable to obtain database handle.");
        return H_ERROR_POINTER;
    }

    bool bRet = true;

    vector<CEString> vecTables;
    vecTables.push_back(L"test_data");
    vecTables.push_back(L"test_data_stress");
    
    try
    {
        bRet = m_spDb->bExportTables(sPath, vecTables, pProgressCallback);
    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    return bRet ? H_NO_ERROR : H_ERROR_GENERAL;

}       //  eExportTestData()

ET_ReturnCode CDictionary::eImportTestData(const CEString& sPath, PROGRESS_CALLBACK_CLR pProgressCallback)
{
    if (NULL == m_spDb)
    {
        ERROR_LOG(L"Unable to obtain database handle.");
        return H_ERROR_POINTER;
    }

    CEString sQuery = L"DELETE FROM test_data_stress";
    m_spDb->Delete(sQuery);
    sQuery = L"DELETE FROM test_data";
    m_spDb->Delete(sQuery);

    bool bRet = true;

    vector<CEString> vecTables;
    vecTables.push_back(L"test_data");
    vecTables.push_back(L"test_data_stress");

    try
    {
        bool bAutoincrement = false;
        bRet = m_spDb->bImportTables(sPath, bAutoincrement, pProgressCallback);
    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    if (!bRet)
    {
        return H_ERROR_GENERAL;
    }

//    ET_ReturnCode rc = ePopulateHashToDescriptorTable(pProgressCallback);

    pProgressCallback(100, true);

    return H_NO_ERROR;

}       //  eImportTestData()

shared_ptr<CSqlite> CDictionary::spGetDbHandle()
{
    if (m_spDb)
    {
        return m_spDb;
    }
    throw CException(H_ERROR_POINTER, L"Database handle not initialized.");
}

//
// Generate all wordforms for lexemes in m_vecLexemes
//
ET_ReturnCode CDictionary::eGenerateFormsForSelectedLexemes()
{
    if (!m_spDb)
    {
        return H_ERROR_DB;
    }

    ET_ReturnCode rc = H_NO_ERROR;


//    m_spDb->BeginTransaction();

    vector<CLexeme *>::iterator itLexeme = m_vecLexemes.begin();
    for (; itLexeme != m_vecLexemes.end(); ++itLexeme)
    {
        //
        // Generate word forms
        //

        try
        {
            (*itLexeme)->eGenerateParadigm();

            //if (H_NO_ERROR != rc)
            //{
            //    ERROR_LOG(L"Error generating paradigm.")
            //}
            ET_ReturnCode eRet = (*itLexeme)->eSaveStemsToDb();
            if (H_NO_ERROR != eRet)
            {
                ERROR_LOG(L"Error saving stems to database.")
            }

            (*itLexeme)->eAssignStemIds();

            eRet = (*itLexeme)->eSaveWordFormsToDb();
            if (H_NO_ERROR != eRet)
            {
                ERROR_LOG(L"Error saving word forms to database.")
            }

        }
        catch (CException& ex)
        {
            ERROR_LOG(ex.szGetDescription());
            //            return H_EXCEPTION;
        }

    }       //  for (int iRow = 0; iRow < iLexemes; ++iRow)

//    auto itEnd = m_vecLexemes.end();
//    for (auto itL = m_vecLexemes.begin(); itL != itEnd; ++itL)
//    {
//        itEnd = remove_if(m_vecLexemes.begin(), m_vecLexemes.end(), [](CLexeme* l) { return 2 == l->iInflectedParts() && l->bIsSecondPart(); });
//        m_vecLexemes.erase(itEnd, m_vecLexemes.end());
//    }

//    m_spDb->CommitTransaction();

    return (H_NO_MORE == rc) ? H_NO_ERROR : rc;

}    //  eGenerateFormsForSelectedLexemes()

//
// Generate all word forms and store them in DB
//
ET_ReturnCode CDictionary::eGenerateAllForms()
{
    if (!m_spDb)
    {
        return H_ERROR_DB;
    }

    ET_ReturnCode rc = H_NO_ERROR;

    rc = ePopulateStemsTable();
    CLogger::pGetInstance()->Flush();
    rc = ePopulateWordFormDataTables();
    CLogger::pGetInstance()->Flush();

    return rc;

}       //  eGenerateAllForms()

ET_ReturnCode CDictionary::ePopulateStemsTable()
{
    if (!m_spDb)
    {
        return H_ERROR_DB;
    }

    ET_ReturnCode rc = H_NO_ERROR;

    uint64_t uiQueryHandle = 0;
    CEString sQuery(sQueryBase);
    sQuery += L"LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id;";
    rc = eQueryDb(sQuery, uiQueryHandle);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    int iRows = 0;

    m_spDb->BeginTransaction();

    int iPercentDone = 0;
    bool bMoreData = true;
    for (int iRow = 0; bMoreData; ++iRow)
    {
        CLexeme lexeme(this);
        try
        {
            rc = eGetSingleRow(lexeme.stGetPropertiesForWriteAccess(), uiQueryHandle);
            if (H_NO_ERROR != rc)
            {
                if (H_NO_MORE == rc)
                {
                    bMoreData = false;
                }
                else
                {
                    CEString sMsg(L"Error retrieving lexeme data: ");
                    sMsg += lexeme.sSourceForm();
                    sMsg += L".";
                    ERROR_LOG(sMsg);
                    //                return rc;
                }
                continue;
            }
        }
        catch (CException& ex)
        {
            ERROR_LOG(ex.szGetDescription());
        }

        //
        // Generate word forms
        //

        try
        {
            rc = lexeme.eGenerateParadigm();
            if (H_NO_ERROR != rc)
            {
                CEString sMsg(L"Error generating paradigm for ");
                sMsg += lexeme.sSourceForm();
                ERROR_LOG(L"Error generating paradigm.")
            }

            ET_ReturnCode eRet = lexeme.eSaveStemsToDb();
        }
        catch (CException& ex)
        {
            ERROR_LOG(ex.szGetDescription());
            //            return H_EXCEPTION;
        }

        //
        // Update progress
        //
        if ((H_NO_ERROR == rc) && (iRow > 0) && (iRow % 1000 == 0))
        {
            m_spDb->CommitTransaction();
            m_spDb->BeginTransaction();
            CEString sMsg = CEString::sToString(iRow);
            sMsg += L" rows";
            ERROR_LOG(sMsg);
            std::wcout << wstring(sMsg) << std::endl;
        }

    }       //  for (...)

    m_spDb->CommitTransaction();

//    CLexeme lexeme(this);

    return (H_NO_MORE == rc) ? H_NO_ERROR : rc;

}       //  ePopulateStemsTable()

ET_ReturnCode CDictionary::ePopulateWordFormDataTables()
{
    if (!m_spDb)
    {
        return H_ERROR_DB;
    }

    ET_ReturnCode rc = H_NO_ERROR;

    uint64_t uiQueryHandle = 0;
    CEString sQuery(sQueryBase);
    sQuery += L"LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id;";
    rc = eQueryDb(sQuery, uiQueryHandle);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    std::clock_t totalTime = 0;

    int iPercentDone = 0;
    bool bMoreData = true;
    vector<CLexeme> vecLexemes;
    for (int iRow = 0; bMoreData; ++iRow)
    {
        CLexeme lexeme(this);
        try
        {
            rc = eGetSingleRow(lexeme.stGetPropertiesForWriteAccess(), uiQueryHandle);
            if (H_NO_ERROR == rc)
            {
                lexeme.eGenerateParadigm();
                lexeme.eAssignStemIds();
                vecLexemes.push_back(lexeme);
            }
            else
            {
                if (H_NO_MORE == rc)
                {
                    bMoreData = false;
                }
                else
                {
                    ERROR_LOG(L"Error retrieving lexeme data.");
                    continue;
                }
            }
        }
        catch (CException& ex)
        {
            ERROR_LOG(ex.szGetDescription());
        }

        if (iRow > 0 && (iRow % 1000 == 0 || !bMoreData))
        {
            clock_t dbProcTime = clock();

            m_spDb->BeginTransaction();

            for (vector<CLexeme>::iterator itLexeme = vecLexemes.begin(); itLexeme != vecLexemes.end(); ++itLexeme)
            {
                for (unsigned int uiWf = 0; uiWf < (*itLexeme).uiTotalWordForms(); ++uiWf)
                {
                    CWordForm * pWf = NULL;
                    rc = (*itLexeme).eGetWordForm(uiWf, pWf);
                    if (rc != H_NO_ERROR)
                    {
                        continue;
                    }

                    if (0 == pWf->m_llStemId)
                    {
                        CEString sMsg(L"Unable to find stem id for \"");
                        sMsg += pWf->m_sStem;
                        sMsg += L'"';
                        sMsg += L" lexeme = " + lexeme.sSourceForm();
                        ERROR_LOG(sMsg);

                        continue;
                    }
                    pWf->bSaveToDb();
                }
            }

            m_spDb->CommitTransaction();

            totalTime += clock() - dbProcTime;
            double dDuration = (clock() - dbProcTime) / (double)CLOCKS_PER_SEC;
            CEString sDurationMsg(L"Row ");
            sDurationMsg += CEString::sToString(iRow);
            sDurationMsg += L"; ";
            sDurationMsg += CEString::sToString(dDuration);
            sDurationMsg += L" seconds total";
//            CLogger::bWriteLog(wstring(sDurationMsg));
            std::wcout << wstring(sDurationMsg) << std::endl;

            vecLexemes.clear();

        }       //  if (iRow > 0 && (iRow % 1000 == 0 || !bMoreData))

    }       //  for ...

    return H_NO_ERROR;

}       //  ePopulateWordFormDataTables()

ET_ReturnCode CDictionary::ePopulateHashToDescriptorTable(PROGRESS_CALLBACK_CLR pProgressCLR, PROGRESS_CALLBACK_PYTHON pProgressPython)
{
    if (!m_spDb)
    {
        return H_ERROR_DB;
    }

    ET_ReturnCode rc = H_NO_ERROR;

    __int64 llCount = 0;
    try
    {
        CEString 
            sCountQuery(L"SELECT COUNT() FROM headword INNER JOIN descriptor ON descriptor.word_id = headword.id LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id; ");
        uint64_t uiCountQueryHandle = 0;
        rc = eQueryDb(sCountQuery, uiCountQueryHandle);
        if (H_NO_ERROR != rc)
        {
            return rc;
        }

        bool bRet = m_spDb->bGetRow(uiCountQueryHandle);
        if (!bRet)
        {
            return H_ERROR_UNEXPECTED;
        }

        m_spDb->GetData(0, llCount, uiCountQueryHandle);
        if (H_NO_ERROR != rc)
        {
            return rc;
        }
    }
    catch (CException& ex)
    {
        HandleDbException(ex);
        rc = H_ERROR_DB;
    }

    try
    {
        m_spDb->ClearTable(L"lexeme_hash_to_descriptor");
    }
    catch (CException& exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            m_spDb->GetLastError(sError);
            sMsg += CEString(L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(m_spDb->iGetLastError());
        ERROR_LOG(sMsg);

        return H_ERROR_DB;
    }

    uint64_t uiQueryHandle = 0;
    CEString sQuery(sQueryBase);
    sQuery += L"LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id;";
    rc = eQueryDb(sQuery, uiQueryHandle);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    map<CEString, StLexemeProperties> mapHashToProperties;

    std::clock_t totalTime = 0;
    int iPercentDone = 0;
    bool bMoreData = true;
    for (int iRow = 0; bMoreData; ++iRow)
    {
        clock_t dbProcTime = clock();

        CLexeme lexeme(this);
        try
        {
            rc = eGetSingleRow(lexeme.stGetPropertiesForWriteAccess(), uiQueryHandle);
            if (rc != H_NO_ERROR)
            {
                if (H_NO_MORE == rc)
                {
                    bMoreData = false;
                }
                else
                {
                    ERROR_LOG(L"Error retrieving lexeme data.");
                    continue;
                }
            }

            CEString sHash = lexeme.sHash();
            mapHashToProperties[sHash] = lexeme.stGetProperties();

            if (!bMoreData || (iRow > 0 && (iRow % 1000 == 0)))
            { 
                m_spDb->BeginTransaction();

                for (auto itLex = mapHashToProperties.begin(); itLex != mapHashToProperties.end(); ++itLex)
                {
                    uint64_t uiInsertHandle = 0;

                    try
                    {
                        if (0 == uiInsertHandle)
                        {
                            m_spDb->uiPrepareForInsert(L"lexeme_hash_to_descriptor", 3, (sqlite3_stmt *&)uiInsertHandle, false);
                        }
                        m_spDb->Bind(1, (*itLex).first, uiInsertHandle);
                        m_spDb->Bind(2, (*itLex).second.llDescriptorId, uiInsertHandle);
                        m_spDb->Bind(3, (*itLex).second.iInflectionId, uiInsertHandle);
                        m_spDb->InsertRow(uiInsertHandle);
                        m_spDb->Finalize(uiInsertHandle);
                    }
                    catch (CException& exc)
                    {
                        CEString sMsg(exc.szGetDescription());
                        CEString sError;
                        try
                        {
                            m_spDb->GetLastError(sError);
                            sMsg += CEString(L", error %d: ");
                            sMsg += sError;
                        }
                        catch (...)
                        {
                            sMsg = L"Apparent DB error ";
                        }

                        sMsg += CEString::sToString(m_spDb->iGetLastError());
                        ERROR_LOG(sMsg);

                        continue;
                    }
                }       //  for (auto itLex = mapHashToProperties.begin(); ...)

                m_spDb->CommitTransaction();

                mapHashToProperties.clear();
            
            }       //  if (!bMoreData || (iRow > 0 && (iRow % 100 == 0)))
        }
        catch (CException& ex)
        {
            ERROR_LOG(ex.szGetDescription());
        }

        if (iRow > 0 && (iRow % 1000 == 0 || !bMoreData))
        {
            int iPercentDone = min(100, (int)(100 * (double)iRow / (double)llCount));

            totalTime += clock() - dbProcTime;
            dbProcTime = clock();
            double dDuration = totalTime / (double)CLOCKS_PER_SEC;
            if (pProgressPython)
            {
                pProgressPython(iPercentDone, false, iRow, dDuration);
            }

//            CEString sMsg(L"------------ Saving lexeme hashes ------------- \r\n");
//            sMsg += CEString::sToString(iRow);
            //CLogger::bWriteLog(wstring(xxxx));
//            ERROR_LOG(sMsg)
//                std::wcout << wstring(sMsg) << std::endl;

//            CEString sDurationMsg(L"Row ");
//            sDurationMsg += CEString::sToString(iRow);
//            sDurationMsg += L"; ";
//            sDurationMsg += CEString::sToString(dDuration);
//            sDurationMsg += L" seconds total";
            //            CLogger::bWriteLog(wstring(sDurationMsg));
//            std::wcout << wstring(sDurationMsg) << std::endl;
//            totalTime = 0;

        }
    }       //  for ...

    return H_NO_ERROR;

}       //  ePopulateHashToDescriptorTable()

ET_ReturnCode CDictionary::eCountLexemes(int64_t& iLexemes)
{
    ET_ReturnCode rc = H_NO_ERROR;

    iLexemes = -1;

    if (!m_spDb)
    {
        return H_ERROR_DB;
    }

    CEString sQuery(L"SELECT descriptor.id, COUNT(*) FROM descriptor;");

    try
    {

        uint64_t uiQueryHandle = 0;
        rc = eQueryDb(sQuery, uiQueryHandle);
        if (H_NO_ERROR != rc)
        {
            return rc;
        }

        bool bRet = m_spDb->bGetRow(uiQueryHandle);
        if (!bRet)
        {
            return H_ERROR_UNEXPECTED;
        }

        m_spDb->GetData(1, iLexemes, uiQueryHandle);
        if (H_NO_ERROR != rc)
        {
            return rc;
        }

    }
    catch (CException& ex)
    {
        HandleDbException(ex);
        rc = H_ERROR_DB;
    }

    return rc;

}   //  eCountLexemes()

int CDictionary::nLexemesFound()
{
    return (int)m_vecLexemes.size();
}

//
// Helpers
//
ET_ReturnCode CDictionary::eQueryDb(const CEString& sSelect, uint64_t& uiQueryHandle)
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (NULL == m_spDb)
    {
        ASSERT(0);
        ERROR_LOG(L"DB pointer is NULL.");
        return H_ERROR_POINTER;
    }

    try
    {
        uiQueryHandle = m_spDb->uiPrepareForSelect(sSelect);
    }
    catch (CException& ex)
    {
        HandleDbException(ex);
        rc = H_ERROR_DB;
    }

    return rc;
}

ET_ReturnCode CDictionary::eGetSingleRow(StLexemeProperties& stProperties, uint64_t uiQueryHandle)
{
    if (NULL == m_spDb)
    {
        ASSERT(0);
        ERROR_LOG(L"DB pointer is NULL.");
        return H_ERROR_POINTER;
    }

    ET_ReturnCode rc = H_NO_ERROR;

    try
    {
        if (m_spDb->bGetRow(uiQueryHandle))
        {

/*
        static CEString sQueryBase(L"SELECT headword.source, headword.id, headword.comment, headword.variant_comment, headword.plural_of, 
                                                        1             2               3                     4                       5
                                            headword.usage, headword.variant, headword.see_ref, headword.back_ref, headword.source_entry_id, 
                                                       6                7                 8                  9                    10          
                                            descriptor.id, descriptor.graphic_stem, descriptor.graphic_stem2, descriptor.is_variant, 
                                                       11                   12                       13                      14
                                            descriptor.main_symbol, descriptor.part_of_speech, descriptor.is_plural_of, descriptor.is_intransitive, 
                                                           15                        16                        17                         18
                                            descriptor.is_reflexive, descriptor.main_symbol_plural_of, descriptor.alt_main_symbol, descriptor.inflection_type,
                                                             19                        20                                 21                         22
                                            descriptor.comment, descriptor.alt_main_symbol_comment, descriptor.alt_inflection_comment, 
                                                          23                          24                                25
                                            descriptor.verb_stem_alternation, descriptor.part_past_pass_zhd, descriptor.section, descriptor.no_comparative, 
                                                                 26                             27                         28                      29
                                            descriptor.no_long_forms, descriptor.assumed_forms, descriptor.yo_alternation, descriptor.o_alternation, 
                                                             30                         31                        32                         33                                            
                                            descriptor.second_genitive, descriptor.is_impersonal, descriptor.is_iterative, descriptor.has_aspect_pair, 
                                                             34                          35                        36                      37
                                            descriptor.has_difficult_forms, descriptor.has_missing_forms, descriptor.has_irregular_forms, 
                                                               38                            39                             40
                                            descriptor.irregular_forms_lead_comment, descriptor.restricted_contexts, descriptor.contexts, descriptor.cognate, 
                                                                      41                                 42                        43                   44
                                            descriptor.trailing_comment, inflection.id, inflection.is_primary, inflection.inflection_type, inflection.accent_type1, 
                                                               45                   46                  47                      48                           49
                                            inflection.accent_type2, inflection.short_form_restrictions, inflection.past_part_restrictions, inflection.no_short_form, 
                                                            50                           51                                   52                             53
                                            inflection.no_past_part, inflection.fleeting_vowel, inflection.stem_augment, inflection.inflected_parts, 
                                                            54                         55                      56                         57
                                            inflection.is_second_part 
                                                            58
*/
            m_spDb->GetData(0, stProperties.sSourceForm, uiQueryHandle);                         //  1 source
            stProperties.llHeadwordId = -1;
            m_spDb->GetData(1, stProperties.llHeadwordId, uiQueryHandle);                        //  2 plural_of
            m_spDb->GetData(2, stProperties.sHeadwordComment, uiQueryHandle);                    //  3 comment
            m_spDb->GetData(3, stProperties.sHeadwordVariantComment, uiQueryHandle);             //  4 variant_comment
            m_spDb->GetData(4, stProperties.sPluralOf, uiQueryHandle);                           //  5 plural_of
            m_spDb->GetData(5, stProperties.sUsage, uiQueryHandle);                              //  6 usage
            m_spDb->GetData(6, stProperties.sHeadwordVariant, uiQueryHandle);                    //  7 variant
            m_spDb->GetData(7, stProperties.sSeeRef, uiQueryHandle);                             //  8 see_ref
            m_spDb->GetData(8, stProperties.sBackRef, uiQueryHandle);                            //  9 back_ref
            m_spDb->GetData(9, stProperties.llSourceEntryId, uiQueryHandle);                     // 10 source_entry_id
            m_spDb->GetData(10, stProperties.llDescriptorId, uiQueryHandle);                     // 11 id
            m_spDb->GetData(11, stProperties.sGraphicStem, uiQueryHandle);                       // 12 graphic_stem
            m_spDb->GetData(12, stProperties.sGraphicStem2, uiQueryHandle);                      // 13 graphic_stem2
            m_spDb->GetData(13, stProperties.bIsVariant, uiQueryHandle);                         // 14 is_variant
            m_spDb->GetData(14, stProperties.sMainSymbol, uiQueryHandle);                        // 15 main_symbol
            m_spDb->GetData(15, (int&)stProperties.ePartOfSpeech, uiQueryHandle);                // 16 part_of_speech
            m_spDb->GetData(16, stProperties.bIsPluralOf, uiQueryHandle);                        // 17 is_plural_of
            bool bIntransitive = false;
            m_spDb->GetData(17, bIntransitive, uiQueryHandle);                                   // 18 is_intransitive; "нп" == !bTransitive
            stProperties.bTransitive = !bIntransitive;
            bool bReflexive = false;
            m_spDb->GetData(18, bReflexive, uiQueryHandle);                                      // 19 is_reflexive
            if (POS_VERB == stProperties.ePartOfSpeech)
            {
                stProperties.eReflexive = bReflexive ? REFL_YES : REFL_NO;
            }
            else
            {
                stProperties.eReflexive = REFL_UNDEFINED;
            }
            m_spDb->GetData(19, stProperties.sMainSymbolPluralOf, uiQueryHandle);                // 20 main_symbol_plural_of
            m_spDb->GetData(20, stProperties.sAltMainSymbol, uiQueryHandle);                     // 21 alt_main_symbol
            m_spDb->GetData(21, stProperties.sInflectionType, uiQueryHandle);                    // 22 inflection_type
            m_spDb->GetData(22, stProperties.sComment, uiQueryHandle);                           // 23 comment
            m_spDb->GetData(23, stProperties.sAltMainSymbolComment, uiQueryHandle);              // 24 alt_main_symbol_comment
            m_spDb->GetData(24, stProperties.sAltInflectionComment, uiQueryHandle);              // 25 alt_inflection_comment
            m_spDb->GetData(25, stProperties.sVerbStemAlternation, uiQueryHandle);               // 26 verb_stem_alternation
            m_spDb->GetData(26, stProperties.bPartPastPassZhd, uiQueryHandle);                   // 27 part_past_pass_zhd
            m_spDb->GetData(27, stProperties.iSection, uiQueryHandle);                           // 28 section
            m_spDb->GetData(28, stProperties.bNoComparative, uiQueryHandle);                     // 29 no_comparative
            m_spDb->GetData(29, stProperties.bNoLongForms, uiQueryHandle);                       // 30 
            m_spDb->GetData(30, stProperties.bAssumedForms, uiQueryHandle);                      // 31
            m_spDb->GetData(31, stProperties.bYoAlternation, uiQueryHandle);                     // 32
            m_spDb->GetData(32, stProperties.bOAlternation, uiQueryHandle);                      // 33
            m_spDb->GetData(33, stProperties.bSecondGenitive, uiQueryHandle);                    // 34
            m_spDb->GetData(34, stProperties.bIsImpersonal, uiQueryHandle);                      // 35
            m_spDb->GetData(35, stProperties.bIsIterative, uiQueryHandle);                       // 36
            m_spDb->GetData(36, stProperties.bHasAspectPair, uiQueryHandle);                     // 37
            m_spDb->GetData(37, stProperties.bHasDifficultForms, uiQueryHandle);                 // 38
            m_spDb->GetData(38, stProperties.bHasMissingForms, uiQueryHandle);                   // 39
            m_spDb->GetData(39, stProperties.bHasIrregularForms, uiQueryHandle);                 // 40
            m_spDb->GetData(40, stProperties.sIrregularFormsLeadComment, uiQueryHandle);         // 41
            m_spDb->GetData(41, stProperties.sRestrictedForms, uiQueryHandle);                   // 42
            m_spDb->GetData(42, stProperties.sContexts, uiQueryHandle);                          // 43
            m_spDb->GetData(43, stProperties.sCognate, uiQueryHandle);                           // 44
            m_spDb->GetData(44, stProperties.sTrailingComment, uiQueryHandle);                   // 45
            stProperties.iInflectionId = -1;
            m_spDb->GetData(45, stProperties.iInflectionId, uiQueryHandle);                      // 46
            m_spDb->GetData(46, stProperties.bPrimaryInflectionGroup, uiQueryHandle);            // 47
            m_spDb->GetData(47, stProperties.iType, uiQueryHandle);                              // 48
            m_spDb->GetData(48, (int&)stProperties.eAccentType1, uiQueryHandle);                 // 49
            m_spDb->GetData(49, (int&)stProperties.eAccentType2, uiQueryHandle);                 // 50
            m_spDb->GetData(50, stProperties.bShortFormsRestricted, uiQueryHandle);              // 51
            if (L"м" == stProperties.sMainSymbol)
            {
                stProperties.bShortFormsRestricted = false;     // nominalized adjectives: text parser may copy that from source
            }
            m_spDb->GetData(51, stProperties.bPastParticipleRestricted, uiQueryHandle);          // 52
            m_spDb->GetData(52, stProperties.bShortFormsIncomplete, uiQueryHandle);              // 53
            if (L"м" == stProperties.sMainSymbol)
            {
                stProperties.bShortFormsIncomplete = false;     // nominalized adjectives: text parser may copy that from source
            }
            m_spDb->GetData(53, stProperties.bNoPassivePastParticiple, uiQueryHandle);           // 54
            m_spDb->GetData(54, stProperties.bFleetingVowel, uiQueryHandle);                     // 55
            m_spDb->GetData(55, stProperties.iStemAugment, uiQueryHandle);                       // 56
            m_spDb->GetData(56, stProperties.iInflectedParts, uiQueryHandle);                    // 57
            m_spDb->GetData(57, stProperties.bIsSecondPart, uiQueryHandle);                      // 58

            CEString sStressQuery(L"SELECT stress_position, is_primary FROM stress WHERE is_variant = \"");
            sStressQuery += L"0\" AND headword_id = ";
            sStressQuery += CEString::sToString(stProperties.llHeadwordId);

            uint64_t uiStressHandle = m_spDb->uiPrepareForSelect(sStressQuery);
            while (m_spDb->bGetRow(uiStressHandle))
            {
                int iPos = -1;
                bool bPrimary = false;
                m_spDb->GetData(0, iPos, uiStressHandle);
                m_spDb->GetData(1, bPrimary, uiStressHandle);
                stProperties.sSourceForm.SetVowels(g_szRusVowels);
                int iStressedSyll = stProperties.sSourceForm.uiGetSyllableFromVowelPos(iPos);
                if (bPrimary)
                {
                    stProperties.vecSourceStressPos.push_back(iStressedSyll);
                }
                else
                {
                    stProperties.vecSecondaryStressPos.push_back(iStressedSyll);
                }
            }
            m_spDb->Finalize(uiStressHandle);

            if (stProperties.vecSourceStressPos.empty())
            {
                CEString sHwCommentQuery(L"SELECT comment FROM headword WHERE id = ");
                sHwCommentQuery += CEString::sToString(stProperties.llHeadwordId);

                uint64_t uiHwCommentHandle = m_spDb->uiPrepareForSelect(sHwCommentQuery);
                while (m_spDb->bGetRow(uiHwCommentHandle))
                {
                    CEString sHwComment;
                    m_spDb->GetData(0, sHwComment, uiHwCommentHandle);
                    if (L"_без удар._" == sHwComment)
                    {
                        stProperties.bIsUnstressed = true;
                    }
                }
                m_spDb->Finalize(uiHwCommentHandle);

                if (!stProperties.bIsUnstressed && !(POS_PARTICLE == stProperties.ePartOfSpeech) &&
                    1 == stProperties.sSourceForm.uiGetNumOfSyllables())
                {
                    ASSERT(1 == stProperties.sSourceForm.uiGetNumOfSyllables());
                    stProperties.vecSourceStressPos.push_back(0);
                }
            }

            CEString sHomonymsQuery(L"SELECT homonym_number FROM homonyms WHERE headword_id = ");
            sHomonymsQuery += CEString::sToString(stProperties.llHeadwordId);

            uint64_t uiHomonymsHandle = m_spDb->uiPrepareForSelect(sHomonymsQuery);
            while (m_spDb->bGetRow(uiHomonymsHandle))
            {
                int iHomonym = -1;
                m_spDb->GetData(0, iHomonym, uiHomonymsHandle);
                stProperties.vecHomonyms.push_back(iHomonym);
            }
            m_spDb->Finalize(uiHomonymsHandle);

            CEString sDeviationQuery(L"SELECT deviation_type, is_optional FROM common_deviation WHERE inflection_id = ");
            sDeviationQuery += CEString::sToString(stProperties.iInflectionId);

            uint64_t uiDeviationHandle = m_spDb->uiPrepareForSelect(sDeviationQuery);
            while (m_spDb->bGetRow(uiDeviationHandle))
            {
                int iType = -1;
                bool bOptional = false;
                m_spDb->GetData(0, iType, uiDeviationHandle);
                m_spDb->GetData(1, bOptional, uiDeviationHandle);
                stProperties.mapCommonDeviations[iType] = bOptional;
            }
            m_spDb->Finalize(uiDeviationHandle);

            if (POS_NOUN == stProperties.ePartOfSpeech)
            {
                rc = eGetP2Data(stProperties.llDescriptorId, stProperties);
                if (H_NO_ERROR != rc)
                {
                    ERROR_LOG(L"hGetP2Data() failed.");
                    return rc;
                }
            }

            if (POS_VERB == stProperties.ePartOfSpeech)
            {
                if (stProperties.bHasAspectPair)
                {
                    CEString sAspectPairQuery(L"SELECT type, data, is_variant, comment FROM aspect_pair WHERE descriptor_id = ");
                    sAspectPairQuery += CEString::sToString(stProperties.llDescriptorId);

                    uint64_t uiAspectPairHandle = m_spDb->uiPrepareForSelect(sAspectPairQuery);
                    while (m_spDb->bGetRow(uiAspectPairHandle))
                    {
                        int iType = 0;
                        bool bIsVariant = false;
                        CEString sData;
                        m_spDb->GetData(0, iType, uiAspectPairHandle);
                        m_spDb->GetData(1, sData, uiAspectPairHandle);
                        m_spDb->GetData(2, bIsVariant, uiAspectPairHandle);
                        m_spDb->GetData(3, stProperties.sAltAspectPairComment, uiAspectPairHandle);

                        if (bIsVariant)
                        {
                            stProperties.iAltAspectPairType = iType;
                            stProperties.sAltAspectPairData = sData;
                        }
                        else
                        {
                            stProperties.iAspectPairType = iType;
                            stProperties.sAspectPairData = sData;
                        }
                    }
                    m_spDb->Finalize(uiAspectPairHandle);
                }
            }
        }   // if (m_spDb->b_GetRow())
        else
        {
            rc = H_NO_MORE;
        }

    }
    catch (CException& ex)
    {
        HandleDbException(ex);
        rc = H_EXCEPTION;
    }

    return rc;

}    //  eGetSingleRow (CLexeme *& pLexeme)

ET_ReturnCode CDictionary::eGetP2Data(__int64 llLexemeId, StLexemeProperties& stProperties)
{
    if (NULL == m_spDb)
    {
        ASSERT(0);
        ERROR_LOG(L"DB pointer is NULL.");
        return H_ERROR_POINTER;
    }

    try
    {
        CEString sQuery(L"SELECT is_optional, preposition FROM second_locative WHERE descriptor_id = ");
        sQuery += CEString::sToString(llLexemeId);
        sQuery += L";";

        uint64_t uiHandle = m_spDb->uiPrepareForSelect(sQuery);

        stProperties.bSecondPrepositional = false;

        if (m_spDb->bGetRow(uiHandle))
        {
            stProperties.bSecondPrepositional = true;
            m_spDb->GetData(0, stProperties.bSecondPrepositionalOptional, uiHandle);
            m_spDb->GetData(1, stProperties.sP2Preposition, uiHandle);
        }

        if (m_spDb->bGetRow(uiHandle))
        {
            ASSERT(0);
            CEString sMsg(L"More than one P2 record for ");
            sMsg += stProperties.sSourceForm;
            ERROR_LOG(sMsg);
            //        return E_FAIL;
        }

        m_spDb->Finalize(uiHandle);
    }
    catch (CException& ex)
    {
        HandleDbException(ex);
        return H_ERROR_DB;
    }

    return H_NO_ERROR;

}   // eGetP2Data (...)

//static CEString sHeadwordInsertQuery(L"INSERT source, plural_of, comment, usage, variant, variant_comment, see_ref, back_ref ");
//static CEString sHomonyms(L"INSERT headword_id, homonym_number, is_variant");
//static CEString sStressInsertQuery(L"INSERT headword_id, stress_position, is_primary, is_variant");
//static CEString sDescriptorInsertQuery(L"INSERT word_id, graphic_stem, is_variant, main_symbol, part_of_speech, is_plural_of, \
//                                        is_intransitive, is_reflexive, main_symbol_plural_of, alt_main_symbol, inflection_type, \
//                                        comment, alt_main_symbol_comment, alt_inflection_comment, verb_stem_alternation, \
//                                        part_past_pass_zhd, section, no_comparative, assumed_forms, yo_alternation, o_alternation, \
//                                        second_genitive, is_impersonal, is_iterative, has_aspect_pair, has_difficulties, \
//                                        difficult_and_missing_forms, has_irregular_forms, irregular_forms_lead_comment, \
//                                        has_deficiencies, restricted_contexts, contexts, cognate, trailing_comment ");
//static CEString sInflectionInsertQuery(L"INSERT descriptor_id, is_primary, inflection_type, accent_type1, accent_type2, short_from_restrictions, \
//                                        past_part_restrictions, no_short_forms, no_past_part, fleeting_vowel, stem_augment, inflected_parts ");
/**
ET_ReturnCode CDictionary::eDeleteLexeme(ILexeme * pLexeme)
{
    const StLexemeProperties& stProperties = pLexeme->stGetProperties();

    try
    {
        if (NULL == m_spDb)
        {
            ASSERT(0);
            ERROR_LOG(L"DB pointer is NULL.");
            return H_ERROR_POINTER;
        }

        CEString sQuery = L"SELECT word_id FROM descriptor WHERE id = " + CEString::sToString(stProperties.llDescriptorId);
        m_spDb->PrepareForSelect(sQuery);
        bool bFound = false;
        int iWordId = -1;
        while (m_spDb->bGetRow())
        {
            if (bFound)
            {
                ERROR_LOG(L"Warning: multiple word ID's.");
                break;
            }
            m_spDb->GetData(0, iWordId);
            if (iWordId >= 0)
            {
                bFound = true;
            }
        }
        m_spDb->Finalize();

        vector<CEString> vecTables{ L"aspect_pair", L"second_genitive", L"second_locative", L"inflection", L"descriptor"};

        for (CEString sTable : vecTables)
        {
            CEString sDeleteStmt = L"DELETE FROM " + sTable + L" WHERE id = ";
            sDeleteStmt += CEString::sToString(stProperties.llDescriptorId);
            sDeleteStmt += L";";
            m_spDb->Exec(sDeleteStmt);
        }

//        do
        {
            CEString sSelectIrregularStmt = L"SELECT id FROM irregular_forms WHERE descriptor_id = " + 
                CEString::sToString(stProperties.llDescriptorId) + L";";
            m_spDb->PrepareForSelect(sSelectIrregularStmt);
            vector<int> vecIrregularFormIds;
            while (m_spDb->bGetRow())
            {
                int iIrregularFormId = -1;
                m_spDb->GetData(0, iIrregularFormId);
                vecIrregularFormIds.push_back(iIrregularFormId);
            }
            m_spDb->Finalize();

            for (auto sId : vecIrregularFormIds)
            {
                CEString sDeleteStmt = L"DELETE FROM irregular_stress WHERE id = ";
                sDeleteStmt += CEString::sToString(stProperties.llDescriptorId);
                sDeleteStmt += L";";
            }
        }

        while (m_spDb->bGetRow())
        {
            if (bFound)
            {
                ERROR_LOG(L"Warning: multiple word ID's.");
                break;
            }
            m_spDb->GetData(0, iWordId);
            if (iWordId >= 0)
            {
                bFound = true;
            }
        }
        m_spDb->Finalize();

        bool bDeleteHeadword = false;
        sQuery = L"SELECT 1 FROM descriptor WHERE word_id = " + CEString::sToString(iWordId) + L";";
        m_spDb->PrepareForSelect(sQuery);
        if (!m_spDb->bGetRow())
        {
            bDeleteHeadword = true;
        }
        m_spDb->Finalize();

        if (bDeleteHeadword)
        {
            CEString sDeleteStmt = L"DELETE FROM stress WHERE headword_id = ";
            sDeleteStmt += CEString::sToString(iWordId);
            sDeleteStmt += L";";
            m_spDb->Exec(sDeleteStmt);

            sDeleteStmt = L"DELETE FROM homonyms WHERE headword_id = ";
            sDeleteStmt += CEString::sToString(iWordId);
            sDeleteStmt += L";";
            m_spDb->Exec(sDeleteStmt);

            sDeleteStmt = L"DELETE FROM headword WHERE id = ";
            sDeleteStmt += CEString::sToString(iWordId);
            sDeleteStmt += L";";
            m_spDb->Exec(sDeleteStmt);
        }
    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}        //  eDeleteLexeme()
*/

ET_ReturnCode CDictionary::eSaveHeadword(ILexeme * pLexeme)
{
    ET_ReturnCode eRet = H_NO_ERROR;

    try
    {
        m_spDb->BeginTransaction();

        StLexemeProperties& stProperties = pLexeme->stGetPropertiesForWriteAccess();
        long long llOldDescriptorId = stProperties.llDescriptorId;
        long long llOldHeadwordId = -1;
        
        bool bNewWordId = (H_FALSE == eRet) ? true : false;

        if (!bNewWordId)
        {
            CEString sDeleteStressQuery(L"DELETE FROM stress WHERE headword_id = ");
            sDeleteStressQuery += CEString::sToString(llOldHeadwordId);
            m_spDb->Exec(sDeleteStressQuery);

            CEString sDeleteHomonymsQuery(L"DELETE FROM homonyms WHERE headword_id = ");
            sDeleteHomonymsQuery += CEString::sToString(llOldHeadwordId);
            m_spDb->Exec(sDeleteHomonymsQuery);

            CEString sDeleteHeadwordQuery(L"DELETE FROM headword WHERE id = ");
            sDeleteHeadwordQuery += CEString::sToString(llOldHeadwordId);
            m_spDb->Exec(sDeleteHeadwordQuery);
        }

        unsigned long long llInsertHandle = 0;
        m_spDb->uiPrepareForInsert(L"headword", 10, (sqlite3_stmt *&)llInsertHandle);

        m_spDb->Bind(1, stProperties.sSourceForm, llInsertHandle);
        m_spDb->Bind(2, stProperties.sPluralOf, llInsertHandle);
        m_spDb->Bind(3, stProperties.sComment, llInsertHandle);
        m_spDb->Bind(4, stProperties.sUsage, llInsertHandle);
        m_spDb->Bind(5, stProperties.sHeadwordVariant, llInsertHandle);
        m_spDb->Bind(6, stProperties.sHeadwordVariantComment, llInsertHandle);
        m_spDb->Bind(7, stProperties.sSeeRef, llInsertHandle);
        m_spDb->Bind(8, stProperties.sBackRef, llInsertHandle);
        m_spDb->Bind(9, stProperties.llSourceEntryId, llInsertHandle);
        m_spDb->Bind(10, true, llInsertHandle);                   // is_edited

        m_spDb->InsertRow(llInsertHandle);

        if (bNewWordId)
        {
            assert(llOldHeadwordId < 0);
        }

        long long llNewHeadwordId = m_spDb->llGetLastKey();

        m_spDb->Finalize(llInsertHandle);

        if (stProperties.vecHomonyms.size() > 0)
        {
            llInsertHandle = 0;
            m_spDb->uiPrepareForInsertOrReplace(L"homonyms", 4, (sqlite3_stmt *&)llInsertHandle);

            for (auto iHomonym : stProperties.vecHomonyms)
            {
                m_spDb->Bind(1, llNewHeadwordId, llInsertHandle);
                m_spDb->Bind(2, iHomonym, llInsertHandle);
                m_spDb->Bind(3, false, llInsertHandle);      // is_variant
                m_spDb->Bind(4, true, llInsertHandle);       // is_edited
            }

            m_spDb->InsertRow(llInsertHandle);
            m_spDb->Finalize(llInsertHandle);
        }

        stProperties.sSourceForm.SetVowels(g_szRusVowels);

        llInsertHandle = 0;
        if (stProperties.vecSourceStressPos.size() > 0)
        {
            m_spDb->uiPrepareForInsert(L"stress", 5, (sqlite3_stmt *&)llInsertHandle);

            for (auto iStressPos : stProperties.vecSourceStressPos)
            {
                m_spDb->Bind(1, llNewHeadwordId, llInsertHandle);
                m_spDb->Bind(2, iStressPos, llInsertHandle);
                m_spDb->Bind(3, true, llInsertHandle);       // primary
                m_spDb->Bind(4, false, llInsertHandle);      // not a variant
                m_spDb->Bind(5, true, llInsertHandle);       // is_edited
                m_spDb->InsertRow(llInsertHandle);
            }

            for (auto iStressPos : stProperties.vecSecondaryStressPos)
            {
                m_spDb->Bind(1, llNewHeadwordId, llInsertHandle);
                m_spDb->Bind(2, iStressPos, llInsertHandle);
                m_spDb->Bind(3, false, llInsertHandle);       // secondary
                m_spDb->Bind(4, false, llInsertHandle);       // not a variant
                m_spDb->Bind(5, true, llInsertHandle);        // is_edited
                m_spDb->InsertRow(llInsertHandle);
            }

            for (auto iStressPos : stProperties.vecSourceVariantStressPos)
            {
                m_spDb->Bind(1, llNewHeadwordId, llInsertHandle);
                m_spDb->Bind(2, iStressPos, llInsertHandle);
                m_spDb->Bind(3, true, llInsertHandle);       // primary
                m_spDb->Bind(4, true, llInsertHandle);       // variant
                m_spDb->Bind(5, true, llInsertHandle);       // is_edited
                m_spDb->InsertRow(llInsertHandle);
            }

            for (auto iStressPos : stProperties.vecSecondaryVariantStressPos)
            {
                m_spDb->Bind(1, llNewHeadwordId, llInsertHandle);
                m_spDb->Bind(2, iStressPos, llInsertHandle);
                m_spDb->Bind(3, false, llInsertHandle);       // primary
                m_spDb->Bind(4, true, llInsertHandle);        // variant
                m_spDb->Bind(5, true, llInsertHandle);        // is_edited
                m_spDb->InsertRow(llInsertHandle);
            }

            m_spDb->Finalize(llInsertHandle);
        }

        CEString sDescriptorUpdateQuery(L"UPDATE descriptor SET word_id = ");
        sDescriptorUpdateQuery += CEString::sToString(llNewHeadwordId);
        sDescriptorUpdateQuery += L" WHERE id = ";
        sDescriptorUpdateQuery += CEString::sToString(stProperties.llDescriptorId);
        m_spDb->Exec(sDescriptorUpdateQuery);

        m_spDb->CommitTransaction();
    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}       //  eSaveHeadword()

ET_ReturnCode CDictionary::eSaveAspectPairInfo(ILexeme * pLexeme)
{
    ET_ReturnCode eRet = H_NO_ERROR;

    try
    {
        m_spDb->BeginTransaction();

        StLexemeProperties& stProperties = pLexeme->stGetPropertiesForWriteAccess();

        CEString sDeleteAspectPairQuery(L"DELETE FROM aspect_pair WHERE descriptor_id = ");
        sDeleteAspectPairQuery += CEString::sToString(stProperties.llDescriptorId);
        m_spDb->Exec(sDeleteAspectPairQuery);

        unsigned long long llInsertHandle = 0;
        m_spDb->uiPrepareForInsert(L"aspect_pair", 6, (sqlite3_stmt *&)llInsertHandle);

        m_spDb->Bind(1, stProperties.llDescriptorId, llInsertHandle);
        m_spDb->Bind(2, stProperties.iAspectPairType, llInsertHandle);
        m_spDb->Bind(3, stProperties.sAltAspectPairData, llInsertHandle);
        m_spDb->Bind(4, stProperties.sAltAspectPairComment, llInsertHandle);                          
        m_spDb->Bind(5, false, llInsertHandle);                  // is variant -- currently not supported by UI
        m_spDb->Bind(6, true, llInsertHandle);                   // is_edited

        m_spDb->InsertRow(llInsertHandle);
        m_spDb->Finalize(llInsertHandle);
        m_spDb->CommitTransaction();
    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}       //  eSaveAspectPairInfo()

ET_ReturnCode CDictionary::eSaveP2Info(ILexeme * pLexeme)
{
    ET_ReturnCode eRet = H_NO_ERROR;

    try
    {
        m_spDb->BeginTransaction();

        StLexemeProperties& stProperties = pLexeme->stGetPropertiesForWriteAccess();

        CEString sDeleteAspectPairQuery(L"DELETE FROM second_locative WHERE descriptor_id = ");
        sDeleteAspectPairQuery += CEString::sToString(stProperties.llDescriptorId);
        m_spDb->Exec(sDeleteAspectPairQuery);

        unsigned long long llInsertHandle = 0;
        m_spDb->uiPrepareForInsert(L"second_locative", 4, (sqlite3_stmt *&)llInsertHandle);

        m_spDb->Bind(1, stProperties.llDescriptorId, llInsertHandle);
        m_spDb->Bind(2, stProperties.bSecondPrepositionalOptional, llInsertHandle);
        m_spDb->Bind(3, stProperties.sP2Preposition, llInsertHandle);
        m_spDb->Bind(4, true, llInsertHandle);                   // is_edited

        m_spDb->InsertRow(llInsertHandle);
        m_spDb->Finalize(llInsertHandle);
        m_spDb->CommitTransaction();
    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    return H_NO_ERROR;
}

ET_ReturnCode CDictionary::eSaveCommonDeviation(ILexeme * pLexeme)
{
    ET_ReturnCode eRet = H_NO_ERROR;

    try
    {
        m_spDb->BeginTransaction();

        StLexemeProperties& stProperties = pLexeme->stGetPropertiesForWriteAccess();

        if (stProperties.iInflectionId < 0)
        {
            ERROR_LOG(L"Illegal inflection key.");
            return H_ERROR_DB;
        }

        CEString sDeleteCommonDeviationQuery(L"DELETE FROM common_deviation WHERE inflection_id = ");
        sDeleteCommonDeviationQuery += CEString::sToString(stProperties.iInflectionId);
        m_spDb->Exec(sDeleteCommonDeviationQuery);

        for (auto pairDeviation : stProperties.mapCommonDeviations)
        {
            unsigned long long llInsertHandle = 0;
            m_spDb->uiPrepareForInsert(L"common_deviation", 4, (sqlite3_stmt *&)llInsertHandle);

            m_spDb->Bind(1, stProperties.iInflectionId, llInsertHandle);
            m_spDb->Bind(2, pairDeviation.first, llInsertHandle);
            m_spDb->Bind(3, pairDeviation.second, llInsertHandle);
            m_spDb->Bind(4, true, llInsertHandle);                   // is_edited

            m_spDb->InsertRow(llInsertHandle);
            m_spDb->Finalize(llInsertHandle);
        }

        m_spDb->CommitTransaction();

    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}       //  eSaveCommonDeviation()

ET_ReturnCode CDictionary::eSaveInflectionInfo(ILexeme * pLexeme)
{
    ET_ReturnCode eRet = H_NO_ERROR;
//                                                                 1                  2                              3                        4                      5
//    CREATE TABLE inflection(id INTEGER PRIMARY KEY ASC, descriptor_id INTEGER, is_primary BOOLEAN DEFAULT(0), inflection_type INTEGER, accent_type1 INTEGER, accent_type2 INTEGER, 
//           6                                              7                                     8                                 9                                   10
//    short_form_restrictions BOOLEAN DEAFULT(0), past_part_restrictions BOOLEAN DEAFULT(0), no_short_form BOOLEAN DEAFULT(0), no_past_part BOOLEAN DEAFULT(0), fleeting_vowel BOOLEAN DEFAULT(0), 
//         11                              12                                   13                                 14
//    stem_augment BOOLEAN DEFAULT(0), inflected_parts BOOLEAN DEFAULT(0), is_second_part BOOLEAN DEFAULT(0), is_edited BOOLEAN DEFAULT(0));

    try
    {
        m_spDb->BeginTransaction();

        StLexemeProperties& stProperties = pLexeme->stGetPropertiesForWriteAccess();

        if (stProperties.iInflectionId < 0)
        {
            ERROR_LOG(L"Illegal inflection key.");
            return H_ERROR_DB;
        }

        CEString sDeleteInflectionQuery(L"DELETE FROM inflection WHERE descriptor_id = ");
        sDeleteInflectionQuery += CEString::sToString(stProperties.llDescriptorId);
        m_spDb->Exec(sDeleteInflectionQuery);

        unsigned long long llInsertHandle = 0;
        m_spDb->uiPrepareForInsert(L"inflection", 14, (sqlite3_stmt *&)llInsertHandle);

        m_spDb->Bind(1, stProperties.llDescriptorId, llInsertHandle);
        m_spDb->Bind(2, stProperties.bPrimaryInflectionGroup, llInsertHandle);
        m_spDb->Bind(3, stProperties.iType, llInsertHandle);
        m_spDb->Bind(4, stProperties.eAccentType1, llInsertHandle);
        m_spDb->Bind(5, stProperties.eAccentType2, llInsertHandle);
        m_spDb->Bind(6, stProperties.bShortFormsRestricted, llInsertHandle);
        m_spDb->Bind(7, stProperties.bPastParticipleRestricted, llInsertHandle);
        m_spDb->Bind(8, stProperties.bShortFormsIncomplete, llInsertHandle);
        m_spDb->Bind(9, stProperties.bNoPassivePastParticiple, llInsertHandle);
        m_spDb->Bind(10, stProperties.bFleetingVowel, llInsertHandle);
        m_spDb->Bind(11, stProperties.iStemAugment, llInsertHandle);
        m_spDb->Bind(12, stProperties.iInflectedParts, llInsertHandle);
        m_spDb->Bind(13, stProperties.bIsSecondPart, llInsertHandle);
        m_spDb->Bind(14, true, llInsertHandle);                   // is_edited

        m_spDb->InsertRow(llInsertHandle);
        m_spDb->Finalize(llInsertHandle);

        m_spDb->CommitTransaction();

    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}   //  eSaveInflectionInfo()

ET_ReturnCode CDictionary::eSaveDescriptorInfo(ILexeme * pLexeme)
{
//    CREATE TABLE descriptor(id INTEGER PRIMARY KEY ASC, word_id INTEGER, graphic_stem TEXT, graphic_stem2 TEXT, 
//                            0                              1                  2                   3                      
//    is_variant BOOLEAN DEFAULT(0), main_symbol TEXT, part_of_speech INTEGER, is_plural_of BOOLEAN DEFAULT(0), 
//         4                                5                 6                     7
//    is_intransitive BOOLEAN DEFAULT(0), is_reflexive BOOLEAN DEFAULT(0), main_symbol_plural_of TEXT, 
//           8                                 9                                  10
//    alt_main_symbol TEXT, inflection_type TEXT, comment TEXT, alt_main_symbol_comment TEXT, alt_inflection_comment TEXT, 
//            11                   12                13                   14                           15
//    verb_stem_alternation TEXT, part_past_pass_zhd BOOLEAN DEFAULT(0), section INTEGER, no_comparative BOOLEAN DEFAULT(0), 
//             16                         17                                18                 19
//    no_long_forms BOOLEAN DEFAULT(0), assumed_forms BOOLEAN DEFAULT(0), yo_alternation BOOLEAN DEFAULT(0), o_alternation BOOLEAN DEFAULT(0), 
//            20                               21                              22                                    23
//    second_genitive BOOLEAN DEFAULT(0), is_impersonal BOOLEAN DEFAULT(0), is_iterative BOOLEAN DEFAULT(0), has_aspect_pair BOOLEAN DEFAULT(0), 
//            24                                 25                              26                                    27
//    has_difficult_forms BOOLEAN DEFAULT(0), has_missing_forms BOOLEAN DEFAULT(0), has_irregular_forms BOOLEAN DEFAULT(0), 
//            28                                 29                                        30
//    irregular_forms_lead_comment TEXT, restricted_contexts TEXT, contexts TEXT, cognate TEXT, trailing_comment TEXT, 
//            31                                 32                    33            34             35
//    is_edited BOOLEAN DEFAULT(0), FOREIGN KEY(word_id) REFERENCES headword(id));
//        36

    StLexemeProperties& stProperties = pLexeme->stGetPropertiesForWriteAccess();

    try
    {
        m_spDb->BeginTransaction();
        
        if (stProperties.llDescriptorId >= 0)
        {
            CEString sDeleteQuery(L"DELETE FROM descriptor WHERE id = ");
            sDeleteQuery += CEString::sToString(stProperties.llDescriptorId);
            m_spDb->Exec(sDeleteQuery);
        }

//        pLexeme->eMakeGraphicStem();

        long long llOldDescriptorId = stProperties.llDescriptorId;

        long long llInsertHandle = 0;
        m_spDb->uiPrepareForInsert(L"descriptor", 36, (sqlite3_stmt *&)llInsertHandle);

        m_spDb->Bind(1, stProperties.llHeadwordId, llInsertHandle);          //  1 word_id
        m_spDb->Bind(2, stProperties.sGraphicStem, llInsertHandle);          //  2 graphic_stem
        m_spDb->Bind(3, stProperties.sGraphicStem2, llInsertHandle);         //  3 graphic_stem2
        m_spDb->Bind(4, stProperties.bIsVariant, llInsertHandle);            //  4 is_variant
        m_spDb->Bind(5, stProperties.sMainSymbol, llInsertHandle);           //  5 main_symbol
        m_spDb->Bind(6, stProperties.ePartOfSpeech, llInsertHandle);         //  6 part_of_speech
        m_spDb->Bind(7, stProperties.bIsPluralOf, llInsertHandle);           //  7 is_plural_of
        m_spDb->Bind(8, !stProperties.bTransitive, llInsertHandle);          //  8 inflection_type
        bool bIsRefexive = stProperties.eReflexive ? REFL_YES : REFL_NO;
        m_spDb->Bind(9, bIsRefexive, llInsertHandle);                            //  9 is_reflexive
        m_spDb->Bind(10, stProperties.sMainSymbolPluralOf, llInsertHandle);      // 10 main_symbol_plural_of
        m_spDb->Bind(11, stProperties.sAltMainSymbol, llInsertHandle);           // 11 alt_main_symbol
        m_spDb->Bind(12, stProperties.sInflectionType, llInsertHandle);          // 12 inflection_type
        m_spDb->Bind(13, stProperties.sComment, llInsertHandle);                 // 13 comment
        m_spDb->Bind(14, stProperties.sAltMainSymbolComment, llInsertHandle);    // 14 alt_main_symbol_comment
        m_spDb->Bind(15, stProperties.sAltInflectionComment, llInsertHandle);    // 15 alt_inflection_comment
        m_spDb->Bind(16, stProperties.sVerbStemAlternation, llInsertHandle);     // 16 verb_stem_alternation
        m_spDb->Bind(17, stProperties.bPartPastPassZhd, llInsertHandle);         // 17 part_past_pass_zhd
        m_spDb->Bind(18, stProperties.iSection, llInsertHandle);                 // 18 section
        m_spDb->Bind(19, stProperties.bNoComparative, llInsertHandle);           // 19 no_comparative
        m_spDb->Bind(20, stProperties.bNoLongForms, llInsertHandle);             // 20 no_long_forms
        m_spDb->Bind(21, stProperties.bAssumedForms, llInsertHandle);            // 21 assumed_forms
        m_spDb->Bind(22, stProperties.bYoAlternation, llInsertHandle);           // 22 yo_alternation
        m_spDb->Bind(23, stProperties.bOAlternation, llInsertHandle);            // 23 o_alternation
        m_spDb->Bind(24, stProperties.bSecondGenitive, llInsertHandle);          // 24 second_genitive
        m_spDb->Bind(25, stProperties.bIsImpersonal, llInsertHandle);            // 25 is_impersonal
        m_spDb->Bind(26, stProperties.bIsIterative, llInsertHandle);             // 26 is_iterative
        m_spDb->Bind(27, stProperties.bHasAspectPair, llInsertHandle);           // 27 has_aspect_pair
        m_spDb->Bind(28, stProperties.bHasDifficultForms, llInsertHandle);       // 28 has_difficult_forms
        m_spDb->Bind(29, stProperties.bHasMissingForms, llInsertHandle);         // 29 has_missing_forms
        m_spDb->Bind(30, stProperties.bHasIrregularForms, llInsertHandle);       // 30 has_irregular_forms
        m_spDb->Bind(31, stProperties.sIrregularFormsLeadComment, llInsertHandle);    //  31 irregular_forms_lead_comment
        m_spDb->Bind(32, stProperties.sRestrictedForms, llInsertHandle);         //  32 restricted_contexts
        m_spDb->Bind(33, stProperties.sContexts, llInsertHandle);                //  33 contexts
        m_spDb->Bind(34, stProperties.sCognate, llInsertHandle);                 //  34 cognate
        m_spDb->Bind(35, stProperties.sTrailingComment, llInsertHandle);         //  35 trailing_comment
        m_spDb->Bind(36, true, llInsertHandle);                                  //  36 is_edited

        m_spDb->InsertRow(llInsertHandle);

        stProperties.llDescriptorId = m_spDb->llGetLastKey();

        m_spDb->Finalize(llInsertHandle);

        CEString sInflectionUpdateQuery(L"UPDATE inflection SET descriptor_id = ");
        sInflectionUpdateQuery += CEString::sToString(stProperties.llDescriptorId);
        sInflectionUpdateQuery += L" WHERE descriptor_id = ";
        sInflectionUpdateQuery += CEString::sToString(llOldDescriptorId);
        m_spDb->Exec(sInflectionUpdateQuery);

        m_spDb->CommitTransaction();
    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}       //  eSaveDescriptorInfo()

/*
ET_ReturnCode CDictionary::eSaveMissingFormHashes(ILexeme* pLexeme)
{
//  CREATE TABLE nonexistent_forms (id INTEGER PRIMARY KEY ASC, descriptor_id INTEGER, gram_hash TEXT, is_edited BOOLEAN DEFAULT (0), FOREIGN KEY (descriptor_id) REFERENCES descriptor (id));

    ET_ReturnCode eRet = H_NO_ERROR;

    vector <CEString> vecMissingFormHashes;

    try
    {
        CEString sHash;
        eRet = pLexeme->eGetFirstMissingFormHash(sHash);
        if (H_FALSE == eRet)
        {
            return H_NO_ERROR;  // No missing forms: that's most common case
        }

        if (eRet != H_NO_ERROR || sHash.bIsEmpty())
        {
            ERROR_LOG(L"Error getting gramm hash for the first missing form.");
            return eRet;
        }

        vecMissingFormHashes.push_back(sHash);

        while (H_NO_ERROR == eRet)
        {
            eRet = pLexeme->eGetFirstMissingFormHash(sHash);
            if (H_FALSE == eRet)
            {
                continue;
            }

            if (eRet != H_NO_ERROR || sHash.bIsEmpty())
            {
                ERROR_LOG(L"Error getting gramm hash for the first missing form.");
                return eRet;
            }

            vecMissingFormHashes.push_back(sHash);
        }

        if (NULL == m_spDb)
        {
            ERROR_LOG(L"Bad DB handle.");
            return H_ERROR_UNEXPECTED;
        }

        m_spDb->BeginTransaction();

        CEString sDeleteQuery(L"DELETE FROM nonexistent_forms WHERE descriptor_id = ");
        sDeleteQuery += CEString::sToString(pLexeme->llLexemeId());
        m_spDb->Exec(sDeleteQuery);

        for (auto sHash : vecMissingFormHashes)
        {
            m_spDb->PrepareForInsert(L"nonexistent_forms", 3);

            m_spDb->Bind(1, pLexeme->llLexemeId());
            m_spDb->Bind(2, sHash);
            m_spDb->Bind(3, true);       // is_edited

            m_spDb->InsertRow();
            m_spDb->Finalize();
        }

        m_spDb->CommitTransaction();

    }
    catch (CException & ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}       //  eSaveMissingFormHashes()
*/

ET_ReturnCode CDictionary::eGetWordIdFromLemexeId(long long llDescriptorId, long long& llWordId)
{
    ET_ReturnCode eRet = H_FALSE;

    try
    {
        CEString sQuery = L"SELECT word_id FROM descriptor WHERE id = " + CEString::sToString(llDescriptorId);
        m_spDb->PrepareForSelect(sQuery);
        bool bFound = false;
        while (m_spDb->bGetRow())
        {
            if (bFound)
            {
                ERROR_LOG(L"Warning: multiple word ID's.");
                break;
            }
            m_spDb->GetData(0, llWordId);
            if (llWordId >= 0)
            {
                eRet = H_NO_ERROR;
                bFound = true;
            }
        }
    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    return eRet;
}

ET_ReturnCode CDictionary::eMarkLexemeAsEdited(ILexeme * pLexeme)
{
    long long llDescriptorId = pLexeme->llLexemeId();

    if (llDescriptorId < 0)
    {
        ERROR_LOG(L"Illegal descriptor ID.");
        return H_ERROR_UNEXPECTED;
    }

    if (NULL == m_spDb)
    {
        ASSERT(0);
        ERROR_LOG(L"DB pointer is NULL.");
        return H_ERROR_POINTER;
    }

    CEString sQuery(L"SELECT lexeme_hash FROM lexeme_hash_to_descriptor WHERE descriptor_id = ");
    sQuery += CEString::sToString(llDescriptorId);

    CEString sLexemeHash;
    try
    {
        uint64_t uiHandle = m_spDb->uiPrepareForSelect(sQuery);
        if (m_spDb->bGetRow(uiHandle))
        {
            m_spDb->GetData(0, sLexemeHash, uiHandle);
        }

        if (m_spDb->bGetRow(uiHandle))
        {
            ASSERT(0);
            CEString sMsg(L"More than one lexeme hash record for descriptor id ");
            sMsg += CEString::sToString(llDescriptorId);
            ERROR_LOG(sMsg);
        }

        m_spDb->Finalize(uiHandle);
    }
    catch (CException& ex)
    {
        HandleDbException(ex);
        return H_ERROR_DB;
    }

    uint64_t uiInsertHandle = 0;

    try
    {
        m_spDb->uiPrepareForInsert(L"edited_lexemes", 1, (sqlite3_stmt *&)uiInsertHandle, false);
        m_spDb->Bind(1, sLexemeHash, uiInsertHandle);
        m_spDb->InsertRow(uiInsertHandle);
        m_spDb->Finalize(uiInsertHandle);
    }
    catch (CException& exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            m_spDb->GetLastError(sError);
            sMsg += CEString(L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(m_spDb->iGetLastError());
        ERROR_LOG(sMsg);
    }

    return H_NO_ERROR;

}       //  eMarkLexemeAsEdited()

void CDictionary::HandleDbException(CException& ex)
{
    CEString sMsg(ex.szGetDescription());
    CEString sError;
    try
    {
        m_spDb->GetLastError(sError);
        sMsg += CEString(L", error description: ");
        sMsg += sError;
    }
    catch (...)
    {
        sMsg = L"Apparent DB error ";
    }

    sMsg += L", error code = ";
    sMsg += CEString::sToString(m_spDb->iGetLastError());
    ERROR_LOG(sMsg);

}
