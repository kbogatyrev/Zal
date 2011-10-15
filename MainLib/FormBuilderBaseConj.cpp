#include "StdAfx.h"
#include "WordForm.h"
#include "FormBuilderBaseConj.h"

HRESULT CFormBuilderBaseConj::hBuildVerbStems()
{
    HRESULT hr = S_OK;

    CEString& sInfinitive = m_pLexeme->m_sSourceForm;
    CEString sGraphicStem = m_pLexeme->m_sGraphicStem;                      // immutable
    CEString& s1SgStem = m_pLexeme->m_s1SgStem;
    CEString& sInfStem = m_pLexeme->m_sInfStem;
    CEString& s3SgStem = m_pLexeme->m_s3SgStem;
    CEString sVerbStemAlternation = m_pLexeme->m_sVerbStemAlternation;      // immutable

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

    sInfStem = m_pLexeme->m_sGraphicStem;

    switch (m_pLexeme->m_iType)
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
            hr = hStandardAlternation (s1SgStem);
            if (S_OK != hr)
            {
                ERROR_LOG (L"hStandardAlternation() failed.");
                return hr;
            }

            s3SgStem = sGraphicStem.sErase (sInfStem.uiLength()-1, 1);
            break;
        }

        case 6:         // Without circle: сеять(ся), глаголать, колебать, сыпать, прятать, смеяться, ржать, 
                        //                 слать, стлать, трепать, скакать, казаться
                        // With circle: (вы)сосать(ся), жаждать, рвать(ся), ткать, лгать, брать, звать, стонать
        {
            if (1 != m_pLexeme->m_iStemAugment)
            {
                hr = hStandardAlternation (s1SgStem);
                if (S_OK != hr)
                {
                    ERROR_LOG (L"hStandardAlternation() failed.");
                    return hr;
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
                if (m_pLexeme->m_sVerbStemAlternation.bIsEmpty())
                {
                    ATLASSERT(0);
                    ERROR_LOG (L"Missing stem alternation for type 7");
                    return E_FAIL;
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
                return E_FAIL;
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
                    return E_FAIL;
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
                if (17 == m_pLexeme->m_iSection)
                {
                    return S_FALSE; // внять, объять и т.п.
                }
                else
                {

                    ATLASSERT(0);
                    ERROR_LOG (L"Missing stem alternation for type 14");
                    return E_FAIL;
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
//            return E_FAIL;
            return S_FALSE;     // this is possible for some irregular verbs
        }
    }

    return hr;

}    //  hBuildVerbStems()

HRESULT CFormBuilderBaseConj::hStandardAlternation (CEString& sPresentStem)
{
    map<CEString, CEString>& m_mapStandardAlternations = m_pLexeme->m_mapStandardAlternations;
    CEString& sVerbStemAlternation = m_pLexeme->m_sVerbStemAlternation;

//    if (m_pLexeme->xstr_GraphicStem.length() <= 3)
//    {
//        ATLASSERT(0);
//        ERROR_LOG (L"Graphic stem too short.");
//        return E_INVALIDARG;
//    }

    sPresentStem = m_pLexeme->m_sGraphicStem;
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

    map<CEString, CEString>::iterator itAlt = m_mapStandardAlternations.find (sFinale);
    if (m_mapStandardAlternations.end() != itAlt)
    {
        if (L"щ" == sVerbStemAlternation)
        {
            ATLASSERT (L"т" == sFinale);
            sPresentStem = sPresentStem.sReplace (sPresentStem.uiLength()-1, 1, sVerbStemAlternation);
        }
        else
        {
            sPresentStem.sErase (sPresentStem.uiLength()-(*itAlt).first.uiLength());
            sPresentStem += (*itAlt).second;
        }
    }

    return S_OK;

}   //  hStandardAlternation()

HRESULT CFormBuilderBaseConj::hGetPastTenseStressTypes (ET_AccentType eAccentType,
                                                        ET_Number eNumber, 
                                                        ET_Gender eGender,
                                                        vector<ET_StressLocation>& vecStressType)
{
    HRESULT hr = S_OK;

    if (POS_VERB != m_pLexeme->m_ePartOfSpeech)
    {
        ATLASSERT(0);
        CEString sMsg (L"Unexpected part of speech value.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
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
            if (REFL_NO == m_pLexeme->m_eReflexive)
            {
                ATLASSERT(0);
                CEString sMsg (L"Unexpected part of speech value.");
                ERROR_LOG (sMsg);
                throw CException (E_FAIL, sMsg);
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
            CEString sMsg (L"Unexpected past tense accent type.");
            ERROR_LOG (sMsg);
            throw CException (E_FAIL, sMsg);
        }

    }   // switch (eAccentType)

    return S_OK;

}   //  hGetPastTenseStressType (...)

HRESULT CFormBuilderBaseConj::hGetStemStressPositions (CEString& sLemma, 
                                                       vector<int>& vecPosition)
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
            CEString sMsg (L"No vowels in verb stem.");
            ERROR_LOG (sMsg);
            throw CException (E_FAIL, sMsg);
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

    return S_OK;

}   //  hGetStemStressPositions (...)

HRESULT CFormBuilderBaseConj::hGetEndingStressPosition (CEString& sLemma, CEString& sEnding, int& iPosition)
{
   HRESULT hr = S_OK;

    if (POS_VERB != m_pLexeme->m_ePartOfSpeech)
    {
        ATLASSERT(0);
        CEString sMsg (L"Unexpected part of speech.");
        ERROR_LOG (sMsg);
        throw CException (E_UNEXPECTED, sMsg);
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
            throw CException (E_FAIL, sMsg);
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

    return hr;

}   //  hGetEndingStressPosition (...)

HRESULT CFormBuilderBaseConj::hFleetingVowelCheck (CEString& sVerbForm)
{
    if (!m_pLexeme->m_bFleetingVowel)
    {
        return S_OK;
    }

// types 5, 6, 7, 8, 9, 11, 14
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
        return S_OK;
    }

    if (sVerbForm.uiLength() < sPreverb.uiLength() + 2)
    {
        ATLASSERT(0);
        CEString sMsg (L"Stem too short.");
        ERROR_LOG (sMsg);
        throw CException (E_INVALIDARG, sMsg);
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

    return S_OK;

}   //  hFleetingVowelCheck (...)

HRESULT CFormBuilderBaseConj::hBuildPastTenseStem (CEString& sLemma)
{
    HRESULT hr = S_OK;

    int iType = m_pLexeme->m_iType;
    if (7 == iType || 8 == iType)
    {
        sLemma = m_pLexeme->m_s1SgStem;
        if (sLemma.bEndsWithOneOf (L"тд"))
        {
            sLemma = sLemma.sErase (sLemma.uiLength()-1);
        }
    }
    else
    {
        sLemma = m_pLexeme->m_sInfStem;
    }
    if (9 == iType)
    {
        ATLASSERT (sLemma.bEndsWith (L"е"));
        sLemma = sLemma.sErase (sLemma.uiLength()-1);
    }
    if (3 == iType && 1 == m_pLexeme->m_iStemAugment)
    {
        ATLASSERT (sLemma.bEndsWith (L"ну"));
        sLemma = sLemma.sErase (sLemma.uiLength()-2);
    }

    return hr;

}   //  hBuildPastTenseStem (...)

HRESULT CFormBuilderBaseConj::hHandleYoAlternation (int iStressSyll,
                                                    ET_Subparadigm eoSubParadigm, 
                                                    CEString& sLemma)
{
    HRESULT hr = S_OK;

    if (!m_pLexeme->m_bYoAlternation)
    {
        return S_OK;
    }

    if (SUBPARADIGM_PAST_TENSE != eoSubParadigm && 
        SUBPARADIGM_PART_PAST_ACT != eoSubParadigm &&
        SUBPARADIGM_PART_PAST_PASS_LONG != eoSubParadigm &&
        SUBPARADIGM_PART_PAST_PASS_SHORT != eoSubParadigm &&
        SUBPARADIGM_ADVERBIAL_PAST != eoSubParadigm &&
        SUBPARADIGM_PART_PRES_PASS_LONG != eoSubParadigm &&
        SUBPARADIGM_PART_PRES_PASS_SHORT != eoSubParadigm )
    {
        return S_OK;
    }

    if (SUBPARADIGM_PART_PAST_ACT == eoSubParadigm)
    {
        ATLASSERT (7 == m_pLexeme->m_iType &&                  // GDRL, p. 85
        m_pLexeme->m_sSourceForm.bEndsWith (L"сти") &&
        (L"т" == m_pLexeme->m_sVerbStemAlternation || 
         L"д" == m_pLexeme->m_sVerbStemAlternation));
    }

    if (iStressSyll >= sLemma.uiNSyllables())
    {
        return S_FALSE;
    }

    unsigned int uiEOffset = sLemma.uiRFind (L"е");    // last "e" in graphic stem (?)
	if (ecNotFound == uiEOffset)
    {
        ATLASSERT(0);
        CEString sMsg (L"Unstressed stem with yo alternation has no e.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }

    unsigned int uiStressPos = sLemma.uiGetVowelPos (iStressSyll);
    if (uiStressPos == uiEOffset)
    {
        sLemma[uiStressPos] = L'ё';
    }

    return S_OK;

}   //  hHandleYoAlternation (...)

HRESULT CFormBuilderBaseConj::hGet1SgIrregular (StIrregularForm& stIf)
{
    HRESULT hr = S_OK;

    multimap<int, StIrregularForm>& mmapIf = m_pLexeme->m_mmapIrregularForms;
    CGramHasher Sg1Hash (SUBPARADIGM_PRESENT_TENSE, 
                            NUM_SG, 
                            GENDER_UNDEFINED, 
                            PERSON_1, 
                            ANIM_UNDEFINED,
                            m_pLexeme->m_eAspect,
                            CASE_UNDEFINED, 
                            m_pLexeme->m_eReflexive);

    multimap<int, StIrregularForm>::iterator it1Sg = mmapIf.find (Sg1Hash.iGramHash());
    if (mmapIf.end() == it1Sg)
    {
        ATLASSERT(0);
        CEString sMsg (L"Irregular 1 Sg form not found in database.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }

    stIf = (*it1Sg).second;
    if (stIf.mapStress.empty())
    {
        CEString sWf (stIf.sForm);
        sWf.SetVowels (g_szVowels);
        if (sWf.uiGetNumOfSyllables() != 1)
        {
            ATLASSERT(0);
            ERROR_LOG (L"No stress position given for a polysyllabic irregular form.");
            return E_UNEXPECTED;
        }
        int iStressPos = sWf.uiFindFirstOf (g_szVowels);
        stIf.mapStress[iStressPos] = STRESS_PRIMARY;
    }

    return S_OK;

}   //  hGet1SgIrregular (...)

HRESULT CFormBuilderBaseConj::hGet3SgIrregular (StIrregularForm& stIf)
{
    HRESULT hr = S_OK;

    multimap<int, StIrregularForm>& mmapIf = m_pLexeme->m_mmapIrregularForms;
    CGramHasher Sg3Hash (SUBPARADIGM_PRESENT_TENSE, 
                         NUM_SG, 
                         GENDER_UNDEFINED, 
                         PERSON_3, 
                         ANIM_UNDEFINED,
                         m_pLexeme->m_eAspect,
                         CASE_UNDEFINED, 
                         m_pLexeme->m_eReflexive);

    multimap<int, StIrregularForm>::iterator it3Sg = mmapIf.find (Sg3Hash.iGramHash());
    if (mmapIf.end() == it3Sg)
    {
        ATLASSERT(0);
        CEString sMsg (L"Irregular 3 Sg form not found in database.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }

    stIf = (*it3Sg).second;
    if (stIf.mapStress.empty())
    {
        CEString sWf (stIf.sForm);
        sWf.SetVowels (g_szVowels);
        if (sWf.uiGetNumOfSyllables() != 1)
        {
            ATLASSERT(0);
            ERROR_LOG (L"No stress position given for a polysyllabic irregular form.");
            return E_UNEXPECTED;
        }
        int iStressPos = sWf.uiFindFirstOf (g_szVowels);
        stIf.mapStress[iStressPos] = STRESS_PRIMARY;
    }

    return S_OK;

}   //  hGet3SgIrregular (...)

HRESULT CFormBuilderBaseConj::hGet3PlIrregular (StIrregularForm& stIf)
{
    HRESULT hr = S_OK;

    multimap<int, StIrregularForm>& mmapIf = m_pLexeme->m_mmapIrregularForms;
    CGramHasher Pl3Hash (SUBPARADIGM_PRESENT_TENSE, 
                         NUM_PL, 
                         GENDER_UNDEFINED, 
                         PERSON_3, 
                         ANIM_UNDEFINED,
                         m_pLexeme->m_eAspect,
                         CASE_UNDEFINED, 
                         m_pLexeme->m_eReflexive);

    multimap<int, StIrregularForm>::iterator it3Pl = mmapIf.find (Pl3Hash.iGramHash());
    if (mmapIf.end() == it3Pl)
    {
        ATLASSERT(0);
        CEString sMsg (L"Irregular 3 Pl form not found in database.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }

    stIf = (*it3Pl).second;

    return S_OK;

}   //  hGet3PlIrregular (...)

bool CFormBuilderBaseConj::bHasIrregularPresent()
{
    if (m_pLexeme->m_mmapIrregularForms.empty())
    {
        return false;
    }

    // Must have at least two forms: 1 sg and 3 sg (GDRL, p. 89)
    multimap<int, StIrregularForm>& mmapIf = m_pLexeme->m_mmapIrregularForms;
    CGramHasher sg1Hash (SUBPARADIGM_PRESENT_TENSE, 
                         NUM_SG, 
                         GENDER_UNDEFINED, 
                         PERSON_1, 
                         ANIM_UNDEFINED, 
                         m_pLexeme->m_eAspect,
                         CASE_UNDEFINED, 
                         m_pLexeme->m_eReflexive);

    multimap<int, StIrregularForm>::iterator it1Sg = mmapIf.find (sg1Hash.iGramHash());
    if (mmapIf.end() == it1Sg)
    {
        return false;
    }

    CGramHasher Sg3Hash (SUBPARADIGM_PRESENT_TENSE, 
                         NUM_SG, 
                         GENDER_UNDEFINED, 
                         PERSON_3, 
                         ANIM_UNDEFINED,
                         m_pLexeme->m_eAspect,
                         CASE_UNDEFINED, 
                         m_pLexeme->m_eReflexive);

    multimap<int, StIrregularForm>::iterator it3Sg = mmapIf.find (Sg3Hash.iGramHash());
    if (mmapIf.end() == it3Sg)
    {
        return false;       // isolated forms are permitted
//        ATLASSERT(0);
//        wstring str_msg (L"3 Sg form not found in database.");
//        ERROR_LOG (str_msg);
//        throw CException (E_FAIL, str_msg);
    }

    return true;

}   //  bHasIrregularPresent()

bool CFormBuilderBaseConj::bHasIrregularPast()
{
    if (m_pLexeme->m_mmapIrregularForms.empty())
    {
        return false;
    }

    // Must have at least two forms: m and f (GDRL, p. 89)
    multimap<int, StIrregularForm>& mmapIf = m_pLexeme->m_mmapIrregularForms;
    CGramHasher mSgHash (SUBPARADIGM_PAST_TENSE, 
                         NUM_SG, 
                         GENDER_M, 
                         PERSON_UNDEFINED, 
                         ANIM_UNDEFINED,
                         m_pLexeme->m_eAspect,
                         CASE_UNDEFINED, 
                         m_pLexeme->m_eReflexive);

    multimap<int, StIrregularForm>::iterator itMSg = mmapIf.find (mSgHash.iGramHash());
    if (mmapIf.end() == itMSg)
    {
        return false;
    }

    CGramHasher FSgHash (SUBPARADIGM_PAST_TENSE, 
                         NUM_SG, 
                         GENDER_F, 
                         PERSON_UNDEFINED, 
                         ANIM_UNDEFINED,
                         m_pLexeme->m_eAspect,
                         CASE_UNDEFINED, 
                         m_pLexeme->m_eReflexive);

    multimap<int, StIrregularForm>::iterator itFSg = mmapIf.find (FSgHash.iGramHash());
    if (mmapIf.end() == itFSg)
    {
        return false;
//        ATLASSERT(0);
//        wstring str_msg (L"f Sg form not found in database.");
//        ERROR_LOG (str_msg);
//        throw CException (E_FAIL, str_msg);
    }

    return true;

}   //  bHasIrregularPast()

bool CFormBuilderBaseConj::bHasIrregularImperative()
{
    if (m_pLexeme->m_mmapIrregularForms.empty())
    {
        return false;
    }

    // Must have at least sg form
    multimap<int, StIrregularForm>& mmapIf = m_pLexeme->m_mmapIrregularForms;
    CGramHasher sg2Hash (SUBPARADIGM_IMPERATIVE, 
                         NUM_SG, 
                         GENDER_UNDEFINED, 
                         PERSON_2, 
                         ANIM_UNDEFINED, 
						 m_pLexeme->m_eAspect,
                         CASE_UNDEFINED, 
                         m_pLexeme->m_eReflexive);

    multimap<int, StIrregularForm>::iterator itSg2 = mmapIf.find (sg2Hash.iGramHash());
    if (mmapIf.end() == itSg2)
    {
        return false;
    }

    return true;

}   //  bHasIrregularImperative

HRESULT CFormBuilderBaseConj::hGetPastMIrregular (StIrregularForm& stIf)
{
    HRESULT hr = S_OK;

    multimap<int, StIrregularForm>& mmapIf = m_pLexeme->m_mmapIrregularForms;
    CGramHasher mSgHash (SUBPARADIGM_PAST_TENSE, 
                         NUM_SG, 
                         GENDER_M, 
                         PERSON_UNDEFINED, 
                         ANIM_UNDEFINED,
                         m_pLexeme->m_eAspect,
                         CASE_UNDEFINED, 
                         m_pLexeme->m_eReflexive);

    multimap<int, StIrregularForm>::iterator itMSg = mmapIf.find (mSgHash.iGramHash());
    if (mmapIf.end() == itMSg)
    {
        ATLASSERT(0);
        CEString sMsg (L"Irregular m Sg form not found in database.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }

    stIf = (*itMSg).second;

    return S_OK;

}   //  hGetPastMIrregular (...)

/*
HRESULT CFormBuilderBaseConj::hGetPastFIrregular (StIrregularForm& stIf)
{
    HRESULT hr = S_OK;

    multimap<int, StIrregularForm>& mmapIf = m_pLexeme->m_mmapIrregularForms;
    CGramHasher fSgHash (SUBPARADIGM_PAST_TENSE, 
                         NUM_SG, 
                         GENDER_F, 
                         PERSON_UNDEFINED, 
                         ANIM_UNDEFINED,
                         m_pLexeme->m_eAspect,
                         CASE_UNDEFINED, 
                         m_pLexeme->m_eReflexive);

    multimap<int, StIrregularForm>::iterator itFSg = mmapIf.find (fSgHash.i\GramHash());
    if (mmapIf.end() == itfSg)
    {
        ATLASSERT(0);
        CEString sMsg (L"Irregular f Sg form not found in database.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }

    stIf = (*itfSg).second;

    return S_OK;

}   //  hGetPastFIrregular (...)

HRESULT CFormBuilderBaseConj::hGetPastPartActIrregular (StIrregularForm& stIf)
{
    HRESULT hr = S_OK;

    multimap<int, StIrregularForm>& mmapIf = m_pLexeme->m_mmapIrregularForms;
    CGramHasher partPastAct (SUBPARADIGM_PART_PAST_ACT, 
                             NUM_UNDEFINED, 
                             GENDER_UNDEFINED, 
                             PERSON_UNDEFINED, 
                             ANIM_UNDEFINED,
                             m_pLexeme->m_eAspect,
                             CASE_UNDEFINED, 
                             m_pLexeme->m_eReflexive);
    multimap<int, StIrregularForm>::iterator itPpa = mmapIf.find (partPastAct.iGramHash());
    if (mmapIf.end() == itPpa)
    {
        ATLASSERT(0);
        CEString sMsg (L"Irregular part past active form not found in database.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }

    stIf = (*itPpa).second;

    return S_OK;

}   //  hGetPastPartActIrregular (...)
*/
