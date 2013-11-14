#include "StdAfx.h"
#include "WordForm.h"
#include "FormBuilderBaseConj.h"

ET_ReturnCode CFormBuilderBaseConj::eBuildVerbStems()
{
    ET_ReturnCode rc = H_NO_ERROR;

    StLexemeProperties stProperties = m_pLexeme->stGetPropertiesForWriteAccess();

    CEString& sInfinitive = stProperties.sSourceForm;
    CEString sGraphicStem = stProperties.sGraphicStem;                  // immutable
    CEString& s1SgStem = stProperties.s1SgStem;
    CEString& sInfStem = stProperties.sInfStem;
    CEString& s3SgStem = stProperties.s3SgStem;
    CEString sVerbStemAlternation = stProperties.sVerbStemAlternation;  // immutable

/*
    if (sInfinitive.b_EndsWith (L"ться") || 
        sInfinitive.b_EndsWith (L"тись") || 
        sInfinitive.b_EndsWith (L"чься"))
    {
        m_pLexeme->eo_Reflexive = REFL_YES;
    }
    else
    {
        m_pLexeme->eo_Reflexive = REFL_NO;
    }
*/

    sInfStem = stProperties.sGraphicStem;

    switch (m_pLexeme->iType())
    {
        case 1:         // делать, читать(ся), верстать, терять, жалеть, читать, стараться
        {
            s1SgStem = s3SgStem = sGraphicStem;
            break;
        }

        case 2:         // требовать, рисовать(ся), тушевать, малевать, ковать, жевать, клевать, радоваться
        {
            s1SgStem = s3SgStem = sGraphicStem.sErase (sInfStem.uiLength()-3, 3);
            if (sInfinitive.bEndsWith (L"овать") || sInfinitive.bEndsWith (L"оваться"))
            {
                s1SgStem += L"у";
                s3SgStem += L"у";
            }
            else if (sInfinitive.bEndsWith (L"евать") || sInfinitive.bEndsWith (L"еваться"))
            {
                if (s1SgStem.bEndsWithOneOf (L"шжчщц"))
                {
                    s1SgStem += L"у";
                    s3SgStem += L"у";
                }
                else
                {
                    s1SgStem += L"ю";
                    s3SgStem += L"ю";
                }
            }
            break;
        }

        case 3:         // Without circle: (вы)тянуть, тронуть, стукнуть, гнуть, повернуть, тянуть
        {               // With circle: вымокнуть, (за)вянуть, (по)гибнуть, стынуть, достигнуть 
                        //              (достичь), свыкнуться
            s1SgStem = s3SgStem = sGraphicStem.sErase (sInfStem.uiLength()-1, 1);
            break;
        }

        case 4:         // строить(ся), множить, грабить, тратить, смолить, крушить, томить, щадить, хоронить, 
                        // точить, ловить, просить, молиться

        case 5:         // (вы)стоять, выглядеть, слышать, видеть, кричать, бежать, звенеть, шуметь, висеть
        {
            rc = eStandardAlternation (s1SgStem);
            if (H_NO_ERROR != rc)
            {
                ERROR_LOG (L"hStandardAlternation() failed.");
                return rc;
            }

            s3SgStem = sGraphicStem.sErase (sInfStem.uiLength()-1, 1);
            break;
        }

        case 6:         // Without circle: сеять(ся), глаголать, колебать, сыпать, прятать, смеяться, ржать, 
                        //                 слать, стлать, трепать, скакать, казаться
                        // With circle: (вы)сосать(ся), жаждать, рвать(ся), ткать, лгать, брать, звать, стонать
        {
            if (1 != m_pLexeme->iStemAugment())
            {
                rc = eStandardAlternation (s1SgStem);
                if (H_NO_ERROR != rc)
                {
                    ERROR_LOG (L"hStandardAlternation() failed.");
                    return rc;
                }
            }
            else
            {
                s1SgStem = sGraphicStem;
                s1SgStem.sErase (s1SgStem.uiLength() - 1);
            }

            s3SgStem = s1SgStem;
            break;
        }

        case 7:         // (вы)грызть, лезть, сесть, красть(ся), ползти, везти, пасти, нести, блюсти, вести, 
                        // мести(сь), (у)честь, расти, грести
        {
            if (sInfStem.bEndsWith (L"с"))
            {
                if (m_pLexeme->sVerbStemAlternation().bIsEmpty())
                {
                    ATLASSERT(0);
                    ERROR_LOG (L"Missing stem alternation for type 7");
                    return H_ERROR_GENERAL;
                }

                s1SgStem = sInfStem.sSubstr (0, sInfStem.uiLength()-1) + sVerbStemAlternation;
            }
            else
            {
                ATLASSERT (sInfStem.bEndsWith (L"з"));
                s1SgStem = sInfStem;
            }

            s3SgStem = s1SgStem;
            break;
        }

        case 8:         // (вы)стричь, лечь, запрячь, беречь(ся), волочь, печь(ся), жечь, толочь, мочь
        {
            if (sVerbStemAlternation.bIsEmpty())
            {
                ATLASSERT(0);
                ERROR_LOG (L"Missing stem alternation for type 8");
                return H_ERROR_GENERAL;
            }

            s1SgStem = sInfStem += sVerbStemAlternation;
            if (L"к" == sVerbStemAlternation)
            {
                s3SgStem = sInfStem;
                s3SgStem.sErase (s3SgStem.uiLength() - 1);
                s3SgStem += L"ч";
            }
            else
            {
                if (L"г" == sVerbStemAlternation)
                {
                    s3SgStem = sInfStem;
                    s3SgStem.sErase (s3SgStem.uiLength() - 1);
                    s3SgStem += L"ж";
                }
                else
                {
                    ATLASSERT(0);
                    ERROR_LOG (L"Bad stem finale");
                    return H_ERROR_GENERAL;
                }
            }
            break;
        }

        case 9:             // (вы-, с)тереть(ся), запереть, умереть, отпереть
        {
            s1SgStem = s3SgStem = sInfStem.sSubstr (0, sInfStem.uiLength()-3) + L"р";
            break;
        }
        case 10:            // (вы)пороть, (у)колоть(ся), молоть, бороться
        {
            s1SgStem = s3SgStem = sGraphicStem.sErase (sInfStem.uiLength()-1, 1);
            break;
        }
        case 11:            // шить(ся), (вы-, до)бить(ся), лить(ся), пить(ся)
        {
            s1SgStem = s3SgStem = sGraphicStem.sErase (sInfStem.uiLength()-1, 1) + L"ь";
            break;
        }
        case 12:            // выть, крыть, мыть, ныть,...
        {
            if (sInfStem.bEndsWith (L"ы"))
            {
                s1SgStem = s3SgStem = sInfStem.sReplace (sInfStem.uiLength()-1, L'о');
            }
            else
            {
                s1SgStem = s3SgStem = sInfStem;
            }
            break;
        }
        case 13:            // давать(ся), оставаться
        {
            s1SgStem = s3SgStem = sInfStem.sErase (sInfStem.uiLength() - 2, 2);
            break;
        }
        case 14:            // мять(ся), жать(ся), сжать, отнять, занять, ...
        {
            if (sVerbStemAlternation.bIsEmpty())
            {
                if (17 == m_pLexeme->iSection())
                {
                    return H_NO_MORE; // внять, объять и т.п.
                }
                else
                {

                    ATLASSERT(0);
                    ERROR_LOG (L"Missing stem alternation for type 14");
                    return H_ERROR_GENERAL;
                }
            }

            if (L"н" != sVerbStemAlternation && L"м" != sVerbStemAlternation && L"им" != sVerbStemAlternation)
            {
                s1SgStem = s3SgStem = sVerbStemAlternation;
            }
            else
            {
                s1SgStem = s3SgStem = sGraphicStem.sErase (sInfStem.uiLength()-1, 1) + sVerbStemAlternation;
            }
            break;
        }
        case 15:            // одеть, достать, остаться
        {
            s1SgStem = s3SgStem = sInfStem + L"н";
            break;
        }
        case 16:            // жить(ся), плыть, слыть
        {
            s1SgStem = s3SgStem = sInfStem + L"в";
            break;
        }
        default:
        {
//            ATLASSERT(0);
//            ERROR_LOG (L"Unrecognized conjugation type.");
//            return H_ERROR_GENERAL;
            return H_NO_MORE;     // this is possible for some irregular verbs
        }
    }

    return rc;

}    //  eBuildVerbStems()

ET_ReturnCode CFormBuilderBaseConj::eStandardAlternation (CEString& sPresentStem)
{
    CEString& sVerbStemAlternation = m_pLexeme->sVerbStemAlternation();

    sPresentStem = m_pLexeme->sGraphicStem();
    sPresentStem.sErase (sPresentStem.uiLength()-1);

    CEString sFinale;
    if (sPresentStem.bEndsWith (L"ск"))
    {
        sFinale = L"ск";
    }
    else
    {
        if (sPresentStem.bEndsWith (L"ст"))
        {
            sFinale = L"ст";
        }
        else
        {
            sFinale = sPresentStem[sPresentStem.uiLength()-1];
        }
    }

    CEString sAlternant;
    bool bHasStandardAlternation = m_pLexeme->bFindStandardAlternation(sFinale, sAlternant);
    if (bHasStandardAlternation)
    {
        if (L"щ" == sVerbStemAlternation)
        {
            if (L"т" != sFinale)
            {
                ATLASSERT(0);
                CEString sMsg(L"Unexpected verb stem alternation: ");
                sMsg += sFinale + L".";
                ERROR_LOG (sMsg);
                return H_ERROR_UNEXPECTED;
            }

            sPresentStem = sPresentStem.sReplace (sPresentStem.uiLength()-1, 1, sVerbStemAlternation);
        }
        else
        {
            sPresentStem.sErase (sPresentStem.uiLength()- sFinale.uiLength());
            sPresentStem += sAlternant;
        }
    }

    return H_NO_ERROR;

}   //  eStandardAlternation()

ET_ReturnCode CFormBuilderBaseConj::eGetPastTenseStressTypes (ET_AccentType eAccentType,
                                                              ET_Number eNumber, 
                                                              ET_Gender eGender,
                                                              vector<ET_StressLocation>& vecStressType)
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (POS_VERB != m_pLexeme->ePartOfSpeech())
    {
        ATLASSERT(0);
        CEString sMsg (L"Unexpected part of speech value.");
        ERROR_LOG (sMsg);
        return H_ERROR_GENERAL;
    }

    vector<ET_StressLocation> vecStress;
    switch (eAccentType)
    {
        case AT_A:
        {
            vecStressType.push_back (STRESS_LOCATION_STEM);
            break;
        }
        case AT_B:
        {
            vecStressType.push_back (STRESS_LOCATION_ENDING);
            break;
        }
        case AT_C:
        {
            if (NUM_SG == eNumber && GENDER_N == eGender)
            {
                vecStressType.push_back (STRESS_LOCATION_STEM);
            }
            else if (NUM_PL == eNumber)
            {
                vecStressType.push_back (STRESS_LOCATION_STEM);
                break;
            }
            else
            {
                vecStressType.push_back (STRESS_LOCATION_ENDING);
            }
            break;
        }
        case AT_C2:
        {
            if (REFL_NO == m_pLexeme->eIsReflexive())
            {
                ATLASSERT(0);
                CEString sMsg (L"Unexpected part of speech value.");
                ERROR_LOG (sMsg);
                return H_ERROR_GENERAL;
            }
            else if (!(GENDER_F == eGender && NUM_SG == eNumber))
            {
                vecStressType.push_back (STRESS_LOCATION_STEM);
                vecStressType.push_back (STRESS_LOCATION_ENDING);
            }
            else
            {
                vecStressType.push_back (STRESS_LOCATION_ENDING);
            }
            break;
        }
        default:
        {
            ATLASSERT(0);
            ERROR_LOG (L"Unexpected past tense accent type.");
            return H_ERROR_GENERAL;
        }

    }   // switch (eAccentType)

    return H_NO_ERROR;

}   //  eGetPastTenseStressType (...)

/*
ET_ReturnCode CFormBuilderBaseConj::eGetStemStressPositions (CEString& sLemma, vector<int>& vecPosition)
{
    //
    // Find the sequence # of the stressed vowel in infinitive
    //
    vector<int>& vecInfStress = m_pLexeme->m_vecSourceStressPos; // alias for readability
    vector<int>::iterator itInfStresPos = vecInfStress.begin();
    for (; itInfStresPos != vecInfStress.end(); ++itInfStresPos)
    {
        sLemma.SetVowels (g_szVowels);
        int iStemSyllables = sLemma.uiNSyllables();
        if (iStemSyllables < 1)
        {
            ATLASSERT(0);
            ERROR_LOG (L"No vowels in verb stem.");
            return H_ERROR_GENERAL;
        }

        if (*itInfStresPos >= iStemSyllables)
        {
            vecPosition.push_back (iStemSyllables - 1);  // last stem syllable
        }
        else
        {
            vecPosition.push_back (*itInfStresPos);    // same syllable as in infinitive
        }

    }   //  for (; it_sourceStressPos != vec_SourceStressPos.end(); ... )

    return H_NO_ERROR;

}   //  hGetStemStressPositions (...)
*/

ET_ReturnCode CFormBuilderBaseConj::eGetEndingStressPosition (CEString& sLemma, CEString& sEnding, int& iPosition)
{
   ET_ReturnCode rc = H_NO_ERROR;

    if (POS_VERB != m_pLexeme->ePartOfSpeech())
    {
        ATLASSERT(0);
        ERROR_LOG (L"Unexpected part of speech.");
        return H_ERROR_UNEXPECTED;
    }

    bool bRetract = false;
    sEnding.SetVowels (g_szVowels);
    if (0 == sEnding.uiNSyllables())
    {
        if (!(sEnding.uiLength() == 0 || L"ь" == sEnding || L"й" == sEnding || L"л" == sEnding))
        {
            ATLASSERT(0);
            CEString sMsg (L"Unexpected non-syllabic ending.");
            ERROR_LOG (sMsg);
            throw CException (H_ERROR_GENERAL, sMsg);
        }
        bRetract = true;
    }
    else
    {
        if (L"те" == sEnding || L"ся" == sEnding || L"тесь" == sEnding)
        {
            bRetract = true;
        }
        else if (sEnding.bEndsWith (L"те") || sEnding.bEndsWith (L"ся") || sEnding.bEndsWith (L"тесь"))
        {
            if (L'ь' == sEnding[0] || L'й' == sEnding[0])
            {
                bRetract = true;
            }
        }
    }

    sLemma.SetVowels (g_szVowels);
    if (!bRetract)
    {
        iPosition = sLemma.uiNSyllables();
    }
    else
    {
        iPosition = sLemma.uiNSyllables() - 1;
    }

    return rc;

}   //  eGetEndingStressPosition (...)

ET_ReturnCode CFormBuilderBaseConj::eFleetingVowelCheck (CEString& sVerbForm)
{
    if (!m_pLexeme->bFleetingVowel())
    {
        return H_NO_ERROR;
    }

// types 5, 6, 7, 8, 9, 11, 14

/*
    CEString sPreverb;
    bool bPreverb = false;
    bool bVoicing = false;

    vector<CEString>::iterator itP = m_pLexeme->m_vecAlternatingPreverbs.begin();
    for (; itP != m_pLexeme->m_vecAlternatingPreverbs.end()&&!bPreverb; ++itP)
    {
        if (sVerbForm.bStartsWith (*itP))
        {
            sPreverb = *itP;
            bPreverb = true;
        }
    }

    if (!bPreverb)
    {
        itP = m_pLexeme->m_vecAlternatingPreverbsWithVoicing.begin();
        for (; itP != m_pLexeme->m_vecAlternatingPreverbsWithVoicing.end()&&!bPreverb; ++itP)
        {
            if (sVerbForm.bStartsWith (*itP))
            {
                sPreverb = *itP;    
                bPreverb = true;
                bVoicing = true;
            }
        }
    }

    if (!bPreverb)
    {
        return H_NO_ERROR;
    }
*/
    ET_ReturnCode rc = H_NO_ERROR;

    CEString sPreverb;
    bool bVoicing = false;
    rc = m_pLexeme->eGetAlternatingPreverb (sVerbForm, sPreverb, bVoicing);
    if (H_FALSE == rc)
    {
        return H_NO_ERROR;
    }

    if (H_NO_ERROR != rc)
    {
        return rc;
    }

    if (sVerbForm.uiLength() < sPreverb.uiLength() + 2)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Stem too short.");
        return H_ERROR_INVALID_ARG;
    }

    // o after prefix?
    if (sVerbForm[sPreverb.uiLength()-1] == L'о')
    {
        // Remove o if followed by CV
        if (CEString::bIn (sVerbForm[sPreverb.uiLength()], g_szConsonants) && 
            CEString::bIn (sVerbForm[sPreverb.uiLength()+1], g_szVowels))
        {
            sVerbForm.sErase (sPreverb.uiLength()-1);
            if (bVoicing)
            {
                wchar_t chrStemAnlaut = sVerbForm[sPreverb.uiLength()];
                if (CEString::bIn (chrStemAnlaut, g_szNonVoicedConsonants))
                {
                    sVerbForm[sPreverb.uiLength()] = L'з';
                }
                else
                {
                    sVerbForm[sPreverb.uiLength()] = L'с';
                }
            }
        }
    }
    else
    {
        // Insert o if followed by CC or Cь
        if (CEString::bIn (sVerbForm[sPreverb.uiLength()], g_szConsonants) &&
            (L'ь' == sVerbForm[sPreverb.uiLength()+1] || 
             CEString::bIn (sVerbForm[sPreverb.uiLength()+1], g_szConsonants)))
        {
            sVerbForm.sInsert (sPreverb.uiLength(), L'о');
            if (bVoicing)
            {
                sVerbForm[sPreverb.uiLength()-1] = L'з';  // force voicing
            }
        }
    }

    return H_NO_ERROR;

}   //  hFleetingVowelCheck (...)

ET_ReturnCode CFormBuilderBaseConj::eBuildPastTenseStem (CEString& sLemma)
{
    ET_ReturnCode hr = H_NO_ERROR;

    int iType = m_pLexeme->iType();
    if (7 == iType || 8 == iType)
    {
        sLemma = m_pLexeme->s1SgStem();
        if (sLemma.bEndsWithOneOf (L"тд"))
        {
            sLemma = sLemma.sErase (sLemma.uiLength()-1);
        }
    }
    else
    {
        sLemma = m_pLexeme->sInfStem();
    }
    if (9 == iType)
    {
        ATLASSERT (sLemma.bEndsWith (L"е"));
        sLemma = sLemma.sErase (sLemma.uiLength()-1);
    }
    if (3 == iType && 1 == m_pLexeme->iStemAugment())
    {
        ATLASSERT (sLemma.bEndsWith (L"ну"));
        sLemma = sLemma.sErase (sLemma.uiLength()-2);
    }

    return hr;

}   //  eBuildPastTenseStem (...)

ET_ReturnCode CFormBuilderBaseConj::eHandleYoAlternation (int iStressSyll, ET_Subparadigm eoSubParadigm, CEString& sLemma)
{
    ET_ReturnCode hr = H_NO_ERROR;

    if (!m_pLexeme->bYoAlternation())
    {
        return H_NO_ERROR;
    }

    if (SUBPARADIGM_PAST_TENSE != eoSubParadigm && 
        SUBPARADIGM_PART_PAST_ACT != eoSubParadigm &&
        SUBPARADIGM_PART_PAST_PASS_LONG != eoSubParadigm &&
        SUBPARADIGM_PART_PAST_PASS_SHORT != eoSubParadigm &&
        SUBPARADIGM_ADVERBIAL_PAST != eoSubParadigm &&
        SUBPARADIGM_PART_PRES_PASS_LONG != eoSubParadigm &&
        SUBPARADIGM_PART_PRES_PASS_SHORT != eoSubParadigm )
    {
        return H_NO_ERROR;
    }

    if (SUBPARADIGM_PART_PAST_ACT == eoSubParadigm)
    {
        ATLASSERT (7 == m_pLexeme->iType() &&                  // GDRL, p. 85
        m_pLexeme->sSourceForm().bEndsWith (L"сти") &&
        (L"т" == m_pLexeme->sVerbStemAlternation() || 
         L"д" == m_pLexeme->sVerbStemAlternation()));
    }

    if (iStressSyll >= sLemma.uiNSyllables())
    {
        return H_NO_MORE;
    }

    unsigned int uiEOffset = sLemma.uiRFind (L"е");    // last "e" in graphic stem (?)
    if (ecNotFound == uiEOffset)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Unstressed stem with yo alternation has no e.");
        return H_ERROR_GENERAL;
    }

    unsigned int uiStressPos = sLemma.uiGetVowelPos (iStressSyll);
    if (uiStressPos == uiEOffset)
    {
        sLemma[uiStressPos] = L'ё';
    }

    return H_NO_ERROR;

}   //  eHandleYoAlternation (...)

bool CFormBuilderBaseConj::bHasIrregularPresent()
{
    try
    {
        if (!m_pLexeme->bHasIrregularForms())
        {
            return false;
        }

        // Must have at least two forms: 1 sg and 3 sg (GDRL, p. 89)
        CGramHasher sg1Hash (SUBPARADIGM_PRESENT_TENSE, 
                             NUM_SG, 
                             GENDER_UNDEFINED, 
                             PERSON_1, 
                             ANIM_UNDEFINED, 
                             m_pLexeme->eAspect(),
                             CASE_UNDEFINED, 
                             m_pLexeme->eIsReflexive());
    
        if (!m_pLexeme->bHasIrregularForm(sg1Hash.iGramHash()))
        {
            return false;
        }

        CGramHasher sg3Hash (SUBPARADIGM_PRESENT_TENSE, 
                             NUM_SG, 
                             GENDER_UNDEFINED, 
                             PERSON_3, 
                             ANIM_UNDEFINED,
                             m_pLexeme->eAspect(),
                            CASE_UNDEFINED, 
                            m_pLexeme->eIsReflexive());
    
        if (!m_pLexeme->bHasIrregularForm(sg3Hash.iGramHash()))
        {
            return false;   // isolated forms are permitted
        }
    }
    catch (CException& ex)
    {
        ATLASSERT(0);
        CEString sMsg (L"Error checking for irregular present forms of ");
        sMsg += m_pLexeme->sInfinitive();
        sMsg += L"; ";
        sMsg += ex.szGetDescription();
        ERROR_LOG (sMsg);
        throw (ex);
    }

    return true;

}   //  bHasIrregularPresent()

bool CFormBuilderBaseConj::bHasIrregularPast()
{
    try
    {
        if (!m_pLexeme->bHasIrregularForms())
        {
            return false;
        }

        // Must have at least two forms: m and f (GDRL, p. 89)
        CGramHasher mSgHash (SUBPARADIGM_PAST_TENSE, 
                             NUM_SG, 
                             GENDER_M, 
                             PERSON_UNDEFINED, 
                             ANIM_UNDEFINED,
                             m_pLexeme->eAspect(),
                             CASE_UNDEFINED, 
                             m_pLexeme->eIsReflexive());

        if (!m_pLexeme->bHasIrregularForm(mSgHash.iGramHash()))
        {
            return false;
        }

        CGramHasher fSgHash (SUBPARADIGM_PAST_TENSE, 
                             NUM_SG, 
                             GENDER_F, 
                             PERSON_UNDEFINED, 
                             ANIM_UNDEFINED,
                             m_pLexeme->eAspect(),
                             CASE_UNDEFINED, 
                             m_pLexeme->eIsReflexive());

        if (!m_pLexeme->bHasIrregularForm(fSgHash.iGramHash()))
        {
            return false;   // isolated forms are permitted?
        }
    }
    catch (CException ex)
    {
        ATLASSERT(0);
        CEString sMsg (L"Error checking for irregular past forms of ");
        sMsg += m_pLexeme->sInfinitive();
        sMsg += L"; ";
        sMsg += ex.szGetDescription();
        ERROR_LOG (sMsg);
    }

    return true;

}   //  bHasIrregularPast()

bool CFormBuilderBaseConj::bHasIrregularImperative()
{
    try
    {
        if (!m_pLexeme->bHasIrregularForms())
        {
            return false;
        }

        // Must have at least sg form
        CGramHasher sg2Hash (SUBPARADIGM_IMPERATIVE, 
                             NUM_SG, 
                             GENDER_UNDEFINED, 
                             PERSON_2, 
                             ANIM_UNDEFINED, 
                             m_pLexeme->eAspect(),
                             CASE_UNDEFINED, 
                             m_pLexeme->eIsReflexive());
            
        return m_pLexeme->bHasIrregularForm(sg2Hash.iGramHash());
    }
    catch (CException ex)
    {
        ATLASSERT(0);
        CEString sMsg (L"Error checking for irregular imperative forms of ");
        sMsg += m_pLexeme->sInfinitive();
        sMsg += L"; ";
        sMsg += ex.szGetDescription();
        ERROR_LOG (sMsg);
        throw CException (H_ERROR_GENERAL, sMsg);
    }

}   //  bHasIrregularImperative

//
// Get irregular form; fill stress pos if it is monosyllabic
//
ET_ReturnCode CFormBuilderBaseConj::eGetIrregularForms (int iHash, vector<CWordForm *>& vecForms)
{
    ET_ReturnCode rc = H_NO_ERROR;
    
    map<CWordForm *, bool> mapIrregularForms;
    rc = m_pLexeme->eGetIrregularForms(iHash, mapIrregularForms);
    if (rc != H_NO_ERROR)
    {
        return rc;
    }

    map<CWordForm *, bool>::iterator it = mapIrregularForms.begin();
    for (; it != mapIrregularForms.end(); ++it)
    {
        CWordForm * pWf = it->first;
        if (pWf->m_mapStress.empty())
        {
            CEString sWf (pWf->m_sWordForm);
            sWf.SetVowels (g_szVowels);
            if (sWf.uiGetNumOfSyllables() != 1)
            {
                ATLASSERT(0);
                CEString sMsg(L"No stress position given for a polysyllabic irregular form.");
                ERROR_LOG(sMsg);
                throw CException(H_ERROR_UNEXPECTED, sMsg);
            }
            int iStressPos = sWf.uiFindFirstOf (g_szVowels);
            pWf->m_mapStress[iStressPos] = STRESS_PRIMARY;
            vecForms.push_back(pWf);
        }
    }

    return H_NO_ERROR;

}   //  eGetIrregularForms (...)
