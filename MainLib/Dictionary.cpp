// Dictionary.cpp : Implementation of CDictionary

#include "stdafx.h"

#include "WordForm.h"
#include "Lexeme.h"
#include "Dictionary.h"

static CEString sQueryBase (L"SELECT \
headword.source, \
headword.id, \
headword.comment, \
descriptor.id, \
descriptor.graphic_stem, \
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
descriptor.assumed_forms, \
descriptor.yo_alternation, \
descriptor.o_alternation, \
descriptor.second_genitive, \
descriptor.has_aspect_pair, \
descriptor.has_questionable_forms, \
descriptor.has_irregular_forms, \
descriptor.has_deficiencies, \
descriptor.restricted_forms, \
descriptor.contexts, \
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
inflection.stem_augment \
FROM \
headword INNER JOIN descriptor ON descriptor.word_id = headword.id ");

CDictionary::~CDictionary()
{
    Clear();
    delete m_pDb;
}

ET_ReturnCode CDictionary::eSetDbPath (const CEString& sPath)
{
    m_sDbPath = sPath;

    try
    {
        m_pDb = new CSqlite (m_sDbPath);
        if (!m_pDb)
        {
            return H_ERROR_POINTER;
        }
    }
    catch (CException&)
    {
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}   //  setDbPath (...)

ET_ReturnCode CDictionary::eGetLexemeById (long iId)
{
    ET_ReturnCode rc = H_NO_ERROR;

    Clear();

    CEString sQuery (sQueryBase);
    sQuery += L"LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id ";
    sQuery += L"WHERE descriptor.id = ";
    sQuery += CEString::sToString (iId);
    sQuery += L";";

    unsigned int uiQueryHandle = 0;
    rc = eQueryDb (sQuery, uiQueryHandle);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    Clear();

    CLexeme * pLexeme = new CLexeme(this);
    if (NULL == pLexeme)
    {
        return H_ERROR_POINTER;
    }

    StLexemeProperties stProperties = pLexeme->stGetPropertiesForWriteAccess();
    rc = eGetSingleRow(stProperties, uiQueryHandle);
    if (H_NO_ERROR != rc)
    {
        delete pLexeme;
        pLexeme = NULL;
        ERROR_LOG (L"Error retrieving lexeme data.");
        return rc;
    }

    rc = eGetSingleRow(stProperties, uiQueryHandle);
    if (H_NO_MORE != rc)
    {
        delete pLexeme;
        pLexeme = NULL;
        ERROR_LOG (L"More than one row returned for a single lexeme ID or DB error.");
        return H_ERROR_UNEXPECTED;
    }

    m_vecLexemes.push_back(pLexeme);

    return rc;

}   //  eGetLexemeById (...)

ET_ReturnCode CDictionary::eGetLexemesByMd5 (const CEString& sMd5)
{
    ET_ReturnCode rc = H_NO_ERROR;

    Clear();

    CEString sQuery (sQueryBase);
    sQuery += L"INNER JOIN lexeme_hash_to_descriptor as l on l.descriptor_id=descriptor.id ";
    sQuery += L"LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id ";
    sQuery += L"WHERE lexeme_hash = \"";
    sQuery += sMd5;
    sQuery += L"\";";

    unsigned int uiQueryHandle = 0;
    rc = eQueryDb (sQuery, uiQueryHandle);
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
            ERROR_LOG (L"Error retrieving lexeme data.");
            return rc;
        }
    }
    return (H_NO_MORE == rc) ? H_NO_ERROR : rc;

}   //  eGetLexemesByMd5 (...)

ET_ReturnCode CDictionary::eGetLexemesByGraphicStem (const CEString& sHeadword)
{
    ET_ReturnCode rc = H_NO_ERROR;

    Clear();

    CEString sQuery (sQueryBase);
    sQuery += L"LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id ";
    sQuery += L"WHERE descriptor.graphic_stem = \"";
    sQuery += sHeadword;
    sQuery += L"\";";

    unsigned int uiQueryHandle = 0;
    rc = eQueryDb (sQuery, uiQueryHandle);
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
        if (H_NO_ERROR == rc)
        {
            m_vecLexemes.push_back(pLexeme);
        }
        if (H_NO_ERROR != rc && H_NO_MORE != rc)
        {
            Clear();
            delete pLexeme;
            pLexeme = NULL;
            ERROR_LOG (L"Error retrieving lexeme data.");
            return rc;
        }

        m_vecLexemes.push_back(pLexeme);
    }
    return (H_NO_ERROR == rc) ? H_NO_ERROR : rc;
}

ET_ReturnCode CDictionary::eGetLexemesByInitialForm (const CEString& sStem)
{
    ET_ReturnCode rc = H_NO_ERROR;

    Clear();

    CEString sQuery (sQueryBase);
    sQuery += L"LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id ";
    sQuery += L"WHERE headword.source = \"";
    sQuery += sStem;
    sQuery += L"\";";

    unsigned int uiQueryHandle = 0;
    rc = eQueryDb (sQuery, uiQueryHandle);
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
            Clear();
            delete pLexeme;
            pLexeme = NULL;
            ERROR_LOG (L"Error retrieving lexeme data.");
            return rc;
        }
    }
    return (H_NO_ERROR == rc) ? H_NO_ERROR : rc;
}

void CDictionary::Clear()
{
    vector<CLexeme *>::iterator it_ = m_vecLexemes.begin();
    for (; it_ != m_vecLexemes.end(); ++it_)
    {
        delete *it_;
    }
    m_vecLexemes.clear();
}

CSqlite * CDictionary::pGetDbHandle()
{
    if (m_pDb)
    {
        return m_pDb;
    }
    throw CException (H_ERROR_POINTER, L"Database handle not initialized.");
}


//
// Generate all word forms and store them in DB
//
ET_ReturnCode CDictionary::eGenerateAllForms(CProgressCallback& progress)
{
    if (!m_pDb)
    {
        return H_ERROR_POINTER;
    }

    ET_ReturnCode rc = H_NO_ERROR;

    int iLexemes = 0;
    rc = eCountLexemes(iLexemes);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    unsigned int uiQueryHandle = 0;
    CEString sQuery (sQueryBase);
    sQuery += L"LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id;";
    rc = eQueryDb (sQuery, uiQueryHandle);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    int iPercentDone = 0;
    for (int iRow = 0; H_NO_ERROR == rc; ++iRow)
    {
        CLexeme * pLexeme = new CLexeme(this);
        rc = eGetSingleRow(pLexeme->stGetPropertiesForWriteAccess(), uiQueryHandle);
        if (H_NO_ERROR != rc && H_NO_MORE != rc)
        {
            pLexeme = NULL;
            ERROR_LOG (L"Error retrieving lexeme data.");
            return rc;
        }

        //
        // Generate word forms
        //
        pLexeme->eGenerateParadigm();

        //
        // Update progress
        //
        if ((H_NO_ERROR == rc || 0 == pLexeme) && (iRow % 100 == 0))
        {
            int iPd = (int) (((double)iRow/(double)iLexemes) * 100);
            if (iPd > iPercentDone)
            {
                iPercentDone = min (iPd, 100);
                progress (iPercentDone);
            }
        }
    }

    return (H_NO_MORE == rc) ? H_NO_ERROR : rc;

}    //  eGenerateAllForms()

ET_ReturnCode CDictionary::eCountLexemes(int& iLexemes)
{
    ET_ReturnCode rc = H_NO_ERROR;

    iLexemes = -1;

    if (!m_pDb)
    {
        return H_ERROR_POINTER;
    }
    
    CEString sQuery (L"SELECT descriptor.id, COUNT(*) FROM descriptor;");

    try
    {

    unsigned int uiQueryHandle = 0;
    rc = eQueryDb (sQuery, uiQueryHandle);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    bool bRet = m_pDb->bGetRow(uiQueryHandle);
    if (!bRet)
    {
        return H_ERROR_UNEXPECTED;
    }

    m_pDb->GetData(0, iLexemes, uiQueryHandle);
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

ET_ReturnCode CDictionary::eGetFirstLexeme (ILexeme *& pLexeme)
{
    m_itCurrentLexeme = m_vecLexemes.begin();
    if (m_vecLexemes.end() == m_itCurrentLexeme)
    {
        return H_FALSE;
    }

    pLexeme = *m_itCurrentLexeme;

    return H_NO_ERROR;
}

ET_ReturnCode CDictionary::eGetNextLexeme (ILexeme *& pLexeme)
{
    if (m_itCurrentLexeme != m_vecLexemes.end())
    {
        ++m_itCurrentLexeme;
    }

    if (m_vecLexemes.end() == m_itCurrentLexeme)
    {
        return H_NO_MORE;
    }

    pLexeme = *m_itCurrentLexeme;

    return H_NO_ERROR;
}


//
// Helpers
//

ET_ReturnCode CDictionary::eQueryDb (const CEString& sSelect, unsigned int& uiQueryHandle)
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (NULL == m_pDb)
    {
        assert(0);
        ERROR_LOG (L"DB pointer is NULL.");
        return H_ERROR_POINTER;
    }

    try
    {
        uiQueryHandle = m_pDb->uiPrepareForSelect (sSelect);
    }
    catch (CException& ex)
    {
        HandleDbException(ex);
        rc = H_ERROR_DB;
    }

    return rc;
}

ET_ReturnCode CDictionary::eGetSingleRow (StLexemeProperties& stProperties, unsigned int uiQueryHandle)
{
    if (NULL == m_pDb)
    {
        assert(0);
        ERROR_LOG (L"DB pointer is NULL.");
        return H_ERROR_POINTER;
    }

    ET_ReturnCode rc = H_NO_ERROR;

    try
    {
        if (m_pDb->bGetRow(uiQueryHandle))
        {
            m_pDb->GetData (0, stProperties.sSourceForm, uiQueryHandle);
            int iHeadwordId = -1;
            m_pDb->GetData (1, iHeadwordId, uiQueryHandle);
            m_pDb->GetData (2, stProperties.sHeadwordComment, uiQueryHandle);
            m_pDb->GetData (3, stProperties.iDbKey, uiQueryHandle);
            m_pDb->GetData (4, stProperties.sGraphicStem, uiQueryHandle);
            m_pDb->GetData (5, stProperties.bIsVariant, uiQueryHandle);
            m_pDb->GetData (6, stProperties.sMainSymbol, uiQueryHandle);
            m_pDb->GetData (7, (int&)stProperties.ePartOfSpeech, uiQueryHandle);

            m_pDb->GetData (8, stProperties.bIsPluralOf, uiQueryHandle);
            bool bIntransitive = false;
            m_pDb->GetData (9, bIntransitive, uiQueryHandle);        // "нп" == !bTransitive
            stProperties.bTransitive = !bIntransitive;
            bool bReflexive = false;
            m_pDb->GetData (10, bReflexive, uiQueryHandle);
            if (POS_VERB == stProperties.ePartOfSpeech)
            {
                stProperties.eReflexive = bReflexive ? REFL_YES : REFL_NO;
            }
            else
            {
                stProperties.eReflexive = REFL_UNDEFINED;
            }
            m_pDb->GetData (11, stProperties.sMainSymbolPluralOf, uiQueryHandle);
            m_pDb->GetData (12, stProperties.sAltMainSymbol, uiQueryHandle);
            m_pDb->GetData (13, stProperties.sInflectionType, uiQueryHandle);
            m_pDb->GetData (14, stProperties.sComment, uiQueryHandle);
            m_pDb->GetData (15, stProperties.sAltMainSymbolComment, uiQueryHandle);
            m_pDb->GetData (16, stProperties.sAltInflectionComment, uiQueryHandle);
            m_pDb->GetData (17, stProperties.sVerbStemAlternation, uiQueryHandle);
            m_pDb->GetData (18, stProperties.bPartPastPassZhd, uiQueryHandle);
            m_pDb->GetData (19, stProperties.iSection, uiQueryHandle);
            m_pDb->GetData (20, stProperties.bNoComparative, uiQueryHandle);
            m_pDb->GetData (21, stProperties.bAssumedForms, uiQueryHandle);
            m_pDb->GetData (22, stProperties.bYoAlternation, uiQueryHandle);
            m_pDb->GetData (23, stProperties.bOAlternation, uiQueryHandle);
            m_pDb->GetData (24, stProperties.bSecondGenitive, uiQueryHandle);
            m_pDb->GetData (25, stProperties.bHasAspectPair, uiQueryHandle);
//            m_pDb->GetData (25, stProperties.iAspectPairType);
//            m_pDb->GetData (26, stProperties.sAspectPairComment);
            m_pDb->GetData (26, stProperties.sQuestionableForms, uiQueryHandle);
            m_pDb->GetData (27, stProperties.bHasIrregularForms, uiQueryHandle);
            m_pDb->GetData (28, stProperties.bHasDeficiencies, uiQueryHandle);
            m_pDb->GetData (29, stProperties.sRestrictedFroms, uiQueryHandle);
            m_pDb->GetData (30, stProperties.sContexts, uiQueryHandle);
            m_pDb->GetData (31, stProperties.sTrailingComment, uiQueryHandle);
            stProperties.iInflectionId = -1;
            m_pDb->GetData (32, stProperties.iInflectionId, uiQueryHandle);
            m_pDb->GetData (33, stProperties.bPrimaryInflectionGroup, uiQueryHandle);
            m_pDb->GetData (34, stProperties.iType, uiQueryHandle);
            m_pDb->GetData (35, (int&)stProperties.eAccentType1, uiQueryHandle);
            m_pDb->GetData (36, (int&)stProperties.eAccentType2, uiQueryHandle);
            m_pDb->GetData (37, stProperties.bShortFormsRestricted, uiQueryHandle);
            m_pDb->GetData (38, stProperties.bPastParticipleRestricted, uiQueryHandle);
            m_pDb->GetData (39, stProperties.bNoShortForms, uiQueryHandle);
            m_pDb->GetData (40, stProperties.bNoPastParticiple, uiQueryHandle);
            m_pDb->GetData (41, stProperties.bFleetingVowel, uiQueryHandle);
            m_pDb->GetData (42, stProperties.iStemAugment, uiQueryHandle);
            
            CEString sStressQuery (L"SELECT stress_position, is_primary FROM stress WHERE headword_id = ");
            sStressQuery += CEString::sToString (iHeadwordId);

            unsigned int uiStressHandle = m_pDb->uiPrepareForSelect (sStressQuery);
            while (m_pDb->bGetRow (uiStressHandle))
            {
                int iPos = -1;
                bool bPrimary = false;
                m_pDb->GetData (0, iPos, uiStressHandle);
                m_pDb->GetData (1, bPrimary, uiStressHandle);
                int iStressedSyll = stProperties.sSourceForm.uiGetSyllableFromVowelPos (iPos);
                if (bPrimary)
                {
                    stProperties.vecSourceStressPos.push_back (iStressedSyll);
                }
                else
                {
                    stProperties.vecSecondaryStressPos.push_back (iStressedSyll);
                }
            }
            m_pDb->Finalize (uiStressHandle);

            if (stProperties.vecSourceStressPos.empty())
            {
                assert (stProperties.vecSecondaryStressPos.empty());
                CEString sHwCommentQuery (L"SELECT comment FROM headword WHERE id = ");
                sHwCommentQuery += CEString::sToString (iHeadwordId);

                unsigned int uiHwCommentHandle = m_pDb->uiPrepareForSelect (sHwCommentQuery);
                while (m_pDb->bGetRow (uiHwCommentHandle))
                {
                    CEString sHwComment;
                    m_pDb->GetData (0, sHwComment, uiHwCommentHandle);
                    if (L"_без удар._" == sHwComment)
                    {
                        stProperties.bIsUnstressed = true;
                    }
                }
                m_pDb->Finalize (uiHwCommentHandle);

                if (!stProperties.bIsUnstressed && !(POS_PARTICLE == stProperties.ePartOfSpeech))
                {
                    assert (1 == stProperties.sSourceForm.uiGetNumOfSyllables());
                    stProperties.vecSourceStressPos.push_back (0);
                }
            }

            CEString sHomonymsQuery (L"SELECT homonym_number FROM homonyms WHERE headword_id = ");
            sHomonymsQuery += CEString::sToString (iHeadwordId);

            unsigned int uiHomonymsHandle = m_pDb->uiPrepareForSelect (sHomonymsQuery);
            while (m_pDb->bGetRow (uiHomonymsHandle))
            {
                int iHomonym = -1;
                m_pDb->GetData (0, iHomonym, uiHomonymsHandle);
                stProperties.vecHomonyms.push_back (iHomonym);
            }
            m_pDb->Finalize (uiHomonymsHandle);

            CEString sDeviationQuery 
                (L"SELECT deviation_type, is_optional FROM common_deviation WHERE inflection_id = ");
            sDeviationQuery += CEString::sToString (stProperties.iInflectionId);

            unsigned int uiDeviationHandle = m_pDb->uiPrepareForSelect (sDeviationQuery);
            while (m_pDb->bGetRow (uiDeviationHandle))
            {
                int iType = -1;
                bool bOptional = false;
                m_pDb->GetData (0, iType, uiDeviationHandle);
                m_pDb->GetData (1, bOptional, uiDeviationHandle);
                stProperties.mapCommonDeviations[iType] = bOptional;
            }
            m_pDb->Finalize (uiDeviationHandle);

            if (POS_NOUN == stProperties.ePartOfSpeech)
            {
                rc = eGetL2Data (stProperties.iDbKey, stProperties);
                if (H_NO_ERROR != rc)
                {
                    ERROR_LOG (L"hGetL2Data() failed.");
                    return rc;
                }
            }
        
        }   // if (m_pDb->b_GetRow())
        else
        {
            rc = H_NO_MORE;
        }
        
    }
    catch (CException& ex)
    {
        HandleDbException(ex);
        rc = H_ERROR_DB;
    }

    return rc;

}    //  eGetSingleRow (CLexeme *& pLexeme)

ET_ReturnCode CDictionary::eGetL2Data (__int64 llLexemeId, StLexemeProperties& stProperties)
{
    if (NULL == m_pDb)
    {
        assert(0);
        ERROR_LOG (L"DB pointer is NULL.");
        return H_ERROR_POINTER;
    }

    try
    {
        CEString sQuery (L"SELECT is_optional, preposition FROM second_locative WHERE descriptor_id = ");
        sQuery += CEString::sToString (llLexemeId);
        sQuery += L";";

        unsigned int uiHandle = m_pDb->uiPrepareForSelect (sQuery);

        stProperties.bSecondLocative = false;

        if (m_pDb->bGetRow (uiHandle))
        {
            stProperties.bSecondLocative = true;
            m_pDb->GetData (0, stProperties.bSecondLocativeOptional, uiHandle);
            m_pDb->GetData (1, stProperties.sLoc2Preposition, uiHandle);
        }

        if (m_pDb->bGetRow (uiHandle))
        {
            assert(0);
            CEString sMsg (L"More than one Loc2 record for ");
            sMsg += stProperties.sSourceForm;
            ERROR_LOG (sMsg);
    //        return E_FAIL;
        }

        m_pDb->Finalize (uiHandle);
    }
    catch (CException& ex)
    {
        HandleDbException(ex);
        return H_ERROR_DB;
    }

    return H_NO_ERROR;

}   // eGetL2Data (...)

void CDictionary::HandleDbException (CException& ex)
{
    CEString sMsg (ex.szGetDescription());
    CEString sError;
    try
    {
        m_pDb->GetLastError (sError);
        sMsg += CEString (L", error description: ");
        sMsg += sError;
    }
    catch (...)
    {
        sMsg = L"Apparent DB error ";
    }

    sMsg += L", error code = ";
    sMsg += CEString::sToString(m_pDb->iGetLastError());
    ERROR_LOG (sMsg);

}
