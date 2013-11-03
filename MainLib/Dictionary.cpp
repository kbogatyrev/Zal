// Dictionary.cpp : Implementation of CDictionary

#include "stdafx.h"
#include "Dictionary.h"
#include "Lexeme.h"
//#include "Verifier.h"

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

ET_ReturnCode CDictionary::eSetDbPath (const CEString& sPath)
{
    m_sDbPath = sPath;

    m_pDb = new CSqlite (m_sDbPath);
    if (!m_pDb)
    {
        return H_ERROR_POINTER;
    }

    return H_NO_ERROR;

}   //  setDbPath (...)

ET_ReturnCode CDictionary::eGetLexemeById (long iId, CLexeme *& pLexeme)
{
    ET_ReturnCode rc = H_NO_ERROR;

    pLexeme = 0;

    CEString sQuery (sQueryBase);
    sQuery += L"LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id ";
    sQuery += L"WHERE descriptor.id = ";
    sQuery += CEString::sToString (iId);
    sQuery += L";";

    rc = eQueryDb (sQuery);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    rc = eGetSingleRow(pLexeme);
    if (H_NO_ERROR != rc || 0 == pLexeme)
    {
        pLexeme = NULL;
        ERROR_LOG (L"Error retrieving lexeme data.");
        return rc;
    }

    rc = eGetSingleRow(pLexeme);
    if (H_NO_MORE != rc)
    {
        pLexeme = NULL;
        ERROR_LOG (L"Either more than one row returned for a single lexeme ID or DB error.");
        return H_ERROR_UNEXPECTED;
    }

    return rc;

}   //  eGetLexeme (...)

ET_ReturnCode CDictionary::eGetLexemesByMd5 (const CEString& sMd5)
{
    ET_ReturnCode rc = H_NO_ERROR;

    CEString sQuery (sQueryBase);
    sQuery += L"INNER JOIN lexeme_hash_to_descriptor as l on l.descriptor_id=descriptor.id ";
    sQuery += L"LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id ";
    sQuery += L"WHERE lexeme_hash = \"";
    sQuery += sMd5;
    sQuery += L"\";";

    rc = eQueryDb (sQuery);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    m_vecLexemes.clear();

    while (H_NO_ERROR == rc)
    {
        CLexeme * pLexeme = 0;
        rc = eGetSingleRow(pLexeme);
        if (H_NO_ERROR == rc || 0 == pLexeme)
        {
            m_vecLexemes.push_back(pLexeme);
        }
        if (H_NO_ERROR != rc && H_NO_MORE != rc)
        {
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

    CEString sQuery (sQueryBase);
    sQuery += L"LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id ";
    sQuery += L"WHERE descriptor.graphic_stem = \"";
    sQuery += sHeadword;
    sQuery += L"\";";

    rc = eQueryDb (sQuery);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    m_vecLexemes.clear();

    while (H_NO_ERROR == rc)
    {
        CLexeme * pLexeme = 0;
        rc = eGetSingleRow(pLexeme);
        if (H_NO_ERROR == rc || 0 == pLexeme)
        {
            m_vecLexemes.push_back(pLexeme);
        }
        if (H_NO_ERROR != rc && H_NO_MORE != rc)
        {
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

    CEString sQuery (sQueryBase);
    sQuery += L"LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id ";
    sQuery += L"WHERE headword.source = \"";
    sQuery += OLE2W (sStem);
    sQuery += L"\";";

    rc = eQueryDb (sQuery);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    m_vecLexemes.clear();

    while (H_NO_ERROR == rc)
    {
        CLexeme * pLexeme = 0;
        rc = eGetSingleRow(pLexeme);
        if (H_NO_ERROR == rc || 0 == pLexeme)
        {
            m_vecLexemes.push_back(pLexeme);
        }
        if (H_NO_ERROR != rc && H_NO_ERROR != rc)
        {
            pLexeme = NULL;
            ERROR_LOG (L"Error retrieving lexeme data.");
            return rc;
        }
    }
    return (H_NO_ERROR == rc) ? H_NO_ERROR : rc;
}

void CDictionary::Clear()
{
    m_vecLexemes.clear();
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

    CEString sQuery (sQueryBase);
    sQuery += L";";
    rc = eQueryDb (sQuery);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    int iPercentDone = 0;
    for (int iRow = 0; H_NO_ERROR == rc; ++iRow)
    {
        CLexeme * pLexeme = 0;
        rc = eGetSingleRow(pLexeme);
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
    iLexemes = -1;

    if (!m_pDb)
    {
        return H_ERROR_POINTER;
    }
    
    CEString sQuery (L"SELECT descriptor.id FROM descriptor COUNT(*);");
    ET_ReturnCode rc = eQueryDb (sQuery);
    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    m_pDb->GetData(0, iLexemes);

    return H_NO_ERROR;
}


//
// Helpers
//

ET_ReturnCode CDictionary::eQueryDb (const CEString& sSelect)
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (NULL == m_pDb)
    {
        ATLASSERT(0);
        ERROR_LOG (L"DB pointer is NULL.");
        return H_ERROR_POINTER;
    }

    try
    {
        m_pDb->PrepareForSelect (sSelect);
    }
    catch (CException& ex)
    {
        HandleDbException(ex);
        rc = H_ERROR_DB;
    }

    return rc;
}

ET_ReturnCode CDictionary::eGetSingleRow (CLexeme *& pLexeme)
{
    if (NULL == m_pDb)
    {
        ATLASSERT(0);
        ERROR_LOG (L"DB pointer is NULL.");
        return H_ERROR_POINTER;
    }

    ET_ReturnCode rc = H_NO_ERROR;

    try
    {
        if (m_pDb->bGetRow())
        {
            pLexeme = new CLexeme;
            if (NULL == pLexeme)
            {
                ERROR_LOG (L"Unable to instantiate CLexeme.");
                return H_ERROR_POINTER;
            }

            pLexeme->SetDb (m_sDbPath);

            m_pDb->GetData (0, pLexeme->m_sSourceForm);
            int iHeadwordId = -1;
            m_pDb->GetData (1, iHeadwordId);
            m_pDb->GetData (2, pLexeme->m_sHeadwordComment);
            m_pDb->GetData (3, pLexeme->m_iDbKey);
            m_pDb->GetData (4, pLexeme->m_sGraphicStem);
            m_pDb->GetData (5, pLexeme->m_bIsVariant);
            m_pDb->GetData (6, pLexeme->m_sMainSymbol);
            m_pDb->GetData (7, (int&)pLexeme->m_ePartOfSpeech);
            m_pDb->GetData (8, pLexeme->m_bIsPluralOf);
            m_pDb->GetData (9, pLexeme->m_bTransitive);
            pLexeme->m_bTransitive = !pLexeme->m_bTransitive;     // "нп" == !b_transitive
            bool bReflexive = false;
            m_pDb->GetData (10, bReflexive);
            if (POS_VERB == pLexeme->m_ePartOfSpeech)
            {
                pLexeme->m_eReflexive = bReflexive ? REFL_YES : REFL_NO;
            }
            else
            {
                pLexeme->m_eReflexive = REFL_UNDEFINED;
            }
            m_pDb->GetData (11, pLexeme->m_sMainSymbolPluralOf);
            m_pDb->GetData (12, pLexeme->m_sAltMainSymbol);
            m_pDb->GetData (13, pLexeme->m_sInflectionType);
            m_pDb->GetData (14, pLexeme->m_sComment);
            m_pDb->GetData (15, pLexeme->m_sAltMainSymbolComment);
            m_pDb->GetData (16, pLexeme->m_sAltInflectionComment);
            m_pDb->GetData (17, pLexeme->m_sVerbStemAlternation);
            m_pDb->GetData (18, pLexeme->m_bPartPastPassZhd);
            m_pDb->GetData (19, pLexeme->m_iSection);
            m_pDb->GetData (20, pLexeme->m_bNoComparative);
            m_pDb->GetData (21, pLexeme->m_bAssumedForms);
            m_pDb->GetData (22, pLexeme->m_bYoAlternation);
            m_pDb->GetData (23, pLexeme->m_bOAlternation);
            m_pDb->GetData (24, pLexeme->m_bSecondGenitive);
            m_pDb->GetData (25, pLexeme->m_bHasAspectPair);
//            m_pDb->GetData (25, pLexeme->m_iAspectPairType);
//            m_pDb->GetData (26, pLexeme->m_sAspectPairComment);
            m_pDb->GetData (26, pLexeme->m_sQuestionableForms);
            m_pDb->GetData (27, pLexeme->m_bHasIrregularForms);
            m_pDb->GetData (28, pLexeme->m_bHasDeficiencies);
            m_pDb->GetData (29, pLexeme->m_sRestrictedFroms);
            m_pDb->GetData (30, pLexeme->m_sContexts);
            m_pDb->GetData (31, pLexeme->m_sTrailingComment);
            int iInflectionId = -1;
            m_pDb->GetData (32, pLexeme->m_iInflectionId);
            m_pDb->GetData (33, pLexeme->m_bPrimaryInflectionGroup);
            m_pDb->GetData (34, pLexeme->m_iType);
            m_pDb->GetData (35, (int&)pLexeme->m_eAccentType1);
            m_pDb->GetData (36, (int&)pLexeme->m_eAccentType2);
            m_pDb->GetData (37, pLexeme->m_bShortFormsRestricted);
            m_pDb->GetData (38, pLexeme->m_bPastParticipleRestricted);
            m_pDb->GetData (39, pLexeme->m_bNoShortForms);
            m_pDb->GetData (40, pLexeme->m_bNoPastParticiple);
            m_pDb->GetData (41, pLexeme->m_bFleetingVowel);
            m_pDb->GetData (42, pLexeme->m_iStemAugment);
            
            CEString sStressQuery (L"SELECT stress_position, is_primary FROM stress WHERE headword_id = ");
            sStressQuery += CEString::sToString (iHeadwordId);

            unsigned int uiStressHandle = m_pDb->uiPrepareForSelect (sStressQuery);
            while (m_pDb->bGetRow (uiStressHandle))
            {
                int iPos = -1;
                bool bPrimary = false;
                m_pDb->GetData (0, iPos, uiStressHandle);
                m_pDb->GetData (1, bPrimary, uiStressHandle);
                int iStressedSyll = pLexeme->m_sSourceForm.uiGetSyllableFromVowelPos (iPos);
                if (bPrimary)
                {
                    pLexeme->m_vecSourceStressPos.push_back (iStressedSyll);
                }
                else
                {
                    pLexeme->m_vecSecondaryStressPos.push_back (iStressedSyll);
                }
            }
            m_pDb->Finalize (uiStressHandle);

            if (pLexeme->m_vecSourceStressPos.empty())
            {
                ATLASSERT (pLexeme->m_vecSecondaryStressPos.empty());
                CEString sHwCommentQuery (L"SELECT comment FROM headword WHERE id = ");
                sHwCommentQuery += CEString::sToString (iHeadwordId);

                unsigned int uiHwCommentHandle = m_pDb->uiPrepareForSelect (sHwCommentQuery);
                while (m_pDb->bGetRow (uiHwCommentHandle))
                {
                    CEString sHwComment;
                    m_pDb->GetData (0, sHwComment, uiHwCommentHandle);
                    if (L"_без удар._" == sHwComment)
                    {
                        pLexeme->m_bIsUnstressed = true;
                    }
                }
                m_pDb->Finalize (uiHwCommentHandle);

                if (!pLexeme->m_bIsUnstressed && !(POS_PARTICLE == pLexeme->m_ePartOfSpeech))
                {
                    ATLASSERT (1 == pLexeme->m_sSourceForm.uiGetNumOfSyllables());
                    pLexeme->m_vecSourceStressPos.push_back (0);
                }
            }

            CEString sHomonymsQuery (L"SELECT homonym_number FROM homonyms WHERE headword_id = ");
            sHomonymsQuery += CEString::sToString (iHeadwordId);

            unsigned int uiHomonymsHandle = m_pDb->uiPrepareForSelect (sHomonymsQuery);
            while (m_pDb->bGetRow (uiHomonymsHandle))
            {
                int iHomonym = -1;
                m_pDb->GetData (0, iHomonym, uiHomonymsHandle);
                pLexeme->m_vecHomonyms.push_back (iHomonym);
            }
            m_pDb->Finalize (uiHomonymsHandle);

            CEString sDeviationQuery 
                (L"SELECT deviation_type, is_optional FROM common_deviation WHERE inflection_id = ");
            sDeviationQuery += CEString::sToString (pLexeme->m_iInflectionId);

            unsigned int uiDeviationHandle = m_pDb->uiPrepareForSelect (sDeviationQuery);
            while (m_pDb->bGetRow (uiDeviationHandle))
            {
                int iType = -1;
                bool bOptional = false;
                m_pDb->GetData (0, iType, uiDeviationHandle);
                m_pDb->GetData (1, bOptional, uiDeviationHandle);
                pLexeme->m_mapCommonDeviations[iType] = bOptional;
            }
            m_pDb->Finalize (uiDeviationHandle);

            if (POS_NOUN == pLexeme->m_ePartOfSpeech)
            {
                rc = eGetL2Data (pLexeme->m_iDbKey, *pLexeme);
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

ET_ReturnCode CDictionary::eGetL2Data (__int64 llLexemeId, CLexeme& lexeme)
{
    if (NULL == m_pDb)
    {
        ATLASSERT(0);
        ERROR_LOG (L"DB pointer is NULL.");
        return H_ERROR_POINTER;
    }

    try
    {
        CEString sQuery (L"SELECT is_optional, preposition FROM second_locative WHERE descriptor_id = ");
        sQuery += CEString::sToString (llLexemeId);
        sQuery += L";";

        unsigned int uiHandle = m_pDb->uiPrepareForSelect (sQuery);

        lexeme.m_bSecondLocative = false;

        if (m_pDb->bGetRow (uiHandle))
        {
            lexeme.m_bSecondLocative = true;
            m_pDb->GetData (0, lexeme.m_bSecondLocativeOptional, uiHandle);
            m_pDb->GetData (1, lexeme.m_sLoc2Preposition, uiHandle);
        }

        if (m_pDb->bGetRow (uiHandle))
        {
            ATLASSERT(0);
            CEString sMsg (L"More than one Loc2 record for ");
            sMsg += lexeme.m_sSourceForm;
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
        sMsg += CEString (L", error %d: ");
        sMsg += sError;
    }
    catch (...)
    {
        sMsg = L"Apparent DB error ";
    }

    CString csMsg;
    csMsg.Format (sMsg, m_pDb->iGetLastError());
    ERROR_LOG ((LPCTSTR)csMsg);

}
