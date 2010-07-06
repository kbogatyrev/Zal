#include "StdAfx.h"
#include "WordForm.h"
#include "FormBuilderBaseConj.h"

HRESULT CT_FormBuilderBaseConj::h_BuildVerbStems()
{
    HRESULT h_r = S_OK;

    CT_ExtString& xstr_infinitive = pco_Lexeme->xstr_SourceForm;
    CT_ExtString& xstr_graphicStem = pco_Lexeme->xstr_GraphicStem;
    CT_ExtString& xstr_1SgStem = pco_Lexeme->xstr_1SgStem;
    CT_ExtString& xstr_infStem = pco_Lexeme->xstr_InfStem;
    CT_ExtString& xstr_3SgStem = pco_Lexeme->xstr_3SgStem;
    wstring& str_VerbStemAlternation = pco_Lexeme->str_VerbStemAlternation;

/*
    if (xstr_infinitive.b_EndsWith (L"ться") || 
        xstr_infinitive.b_EndsWith (L"тись") || 
        xstr_infinitive.b_EndsWith (L"чься"))
    {
        pco_Lexeme->eo_Reflexive = REFL_YES;
    }
    else
    {
        pco_Lexeme->eo_Reflexive = REFL_NO;
    }
*/

    xstr_infStem = pco_Lexeme->xstr_GraphicStem;

    switch (pco_Lexeme->i_Type)
    {
        case 1:         // делать, читать(ся), верстать, терять, жалеть, читать, стараться
        {
            xstr_1SgStem = xstr_3SgStem = xstr_graphicStem;
            break;
        }

        case 2:         // требовать, рисовать(ся), тушевать, малевать, ковать, жевать, клевать, радоваться
        {
            xstr_1SgStem = xstr_3SgStem = wstring (xstr_graphicStem.c_str()).erase (xstr_infStem.length()-3, 3);
            if (xstr_infinitive.b_EndsWith (L"овать") || xstr_infinitive.b_EndsWith (L"оваться"))
            {
                xstr_1SgStem += L"у";
                xstr_3SgStem += L"у";
            }
            else if (xstr_infinitive.b_EndsWith (L"евать") || xstr_infinitive.b_EndsWith (L"еваться"))
            {
                if (xstr_1SgStem.b_EndsWithOneOf (str_ShSounds + L'ц'))
                {
                    xstr_1SgStem += L"у";
                    xstr_3SgStem += L"у";
                }
                else
                {
                    xstr_1SgStem += L"ю";
                    xstr_3SgStem += L"ю";
                }
            }
            break;
        }

        case 3:         // Without circle: (вы)тянуть, тронуть, стукнуть, гнуть, повернуть, тянуть
        {               // With circle: вымокнуть, (за)вянуть, (по)гибнуть, стыть//стынуть, достигнуть 
                        //              (достичь), свыкнуться
            xstr_1SgStem = xstr_3SgStem = wstring (xstr_graphicStem.c_str()).erase (xstr_infStem.length()-1, 1);
            break;
        }

        case 4:         // строить(ся), множить, грабить, тратить, смолить, крушить, томить, щадить, хоронить, 
                        // точить, ловить, просить, молиться

        case 5:         // (вы)стоять, выглядеть, слышать, видеть, кричать, бежать, звенеть, шуметь, висеть
        {
            h_r = h_StandardAlternation (xstr_1SgStem);
            if (S_OK != h_r)
            {
                ERROR_LOG (L"h_StandardAlternation() failed.");
                return h_r;
            }

            xstr_3SgStem = wstring (xstr_graphicStem.c_str()).erase (xstr_infStem.length()-1, 1);
            break;
        }

        case 6:         // Without circle: сеять(ся), глаголать, колебать, сыпать, прятать, смеяться, ржать, 
                        //                 слать, стлать, трепать, скакать, казаться
                        // With circle: (вы)сосать(ся), жаждать, рвать(ся), ткать, лгать, брать, звать, стонать
        {
            if (1 == pco_Lexeme->i_StemAugment)
            {
                h_r = h_StandardAlternation (xstr_1SgStem);
                if (S_OK != h_r)
                {
                    ERROR_LOG (L"h_StandardAlternation() failed.");
                    return h_r;
                }
            }
            else
            {
                xstr_1SgStem = xstr_graphicStem;
                xstr_1SgStem.erase (xstr_1SgStem.length() - 1);
            }

            xstr_3SgStem = xstr_1SgStem;
            break;
        }

        case 7:         // (вы)грызть, лезть, сесть, красть(ся), ползти, везти, пасти, нести, блюсти, вести, 
                        // мести(сь), (у)честь, расти, грести
        {
            if (xstr_infStem.b_EndsWith (L"с"))
            {
                if (pco_Lexeme->str_VerbStemAlternation.empty())
                {
                    ATLASSERT(0);
                    ERROR_LOG (L"Missing stem alternation for type 7");
                    return E_FAIL;
                }

                xstr_1SgStem = xstr_infStem.substr (0, xstr_infStem.length()-1) + str_VerbStemAlternation;
            }
            else
            {
                ATLASSERT (xstr_infStem.b_EndsWith (L"з"));
                xstr_1SgStem = xstr_infStem;
            }

            xstr_3SgStem = xstr_1SgStem;
            break;
        }

        case 8:         // (вы)стричь, лечь, запрячь, беречь(ся), волочь, печь(ся), жечь, толочь, мочь
        {
            if (str_VerbStemAlternation.empty())
            {
                ATLASSERT(0);
                ERROR_LOG (L"Missing stem alternation for type 8");
                return E_FAIL;
            }

            xstr_1SgStem = xstr_infStem += str_VerbStemAlternation;
            if (L"к" == str_VerbStemAlternation)
            {
                xstr_3SgStem = xstr_infStem + L"ч";
            }
            else
            {
                if (L"г" == str_VerbStemAlternation)
                {
                    xstr_3SgStem = xstr_infStem + L"ж";
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
            xstr_1SgStem = xstr_3SgStem = xstr_infStem.substr (0,  xstr_infStem.length()-3) + L"р";
            break;
        }
        case 10:            // (вы)пороть, (у)колоть(ся), молоть, бороться
        {
            xstr_1SgStem = xstr_3SgStem = wstring (xstr_graphicStem.c_str()).erase (xstr_infStem.length()-1, 1);
            break;
        }
        case 11:            // шить(ся), (вы-, до)бить(ся), лить(ся), пить(ся)
        {
            xstr_1SgStem = 
                xstr_3SgStem = 
                    wstring (xstr_graphicStem.c_str()).erase (xstr_infStem.length()-1, 1) + L"ь";
            break;
        }
        case 12:            // выть, крыть, мыть, ныть,...
        {
            if (xstr_infStem.b_EndsWith (L"ы"))
            {
                xstr_1SgStem = xstr_3SgStem = wstring (xstr_infStem).replace (xstr_infStem.length()-1, 1, L'о', 1);
            }
            break;
        }
        case 13:            // давать(ся), оставаться
        {
            xstr_1SgStem = xstr_3SgStem = wstring (xstr_infStem.c_str()).erase (xstr_infStem.length()-2, 2);
            break;
        }
        case 14:            // мять(ся), жать(ся), сжать, отнять, занять, ...
        {
//&&&& #17
            if (str_VerbStemAlternation.empty())
            {
                    ATLASSERT(0);
                    ERROR_LOG (L"Missing stem alternation for type 14");
                    return E_FAIL;
            }

            if (L"н" != str_VerbStemAlternation && 
                L"м" != str_VerbStemAlternation && 
                L"им" != str_VerbStemAlternation)
            {
                xstr_1SgStem = xstr_3SgStem = str_VerbStemAlternation;
            }
            else
            {
                xstr_1SgStem = xstr_3SgStem = 
                    wstring (xstr_graphicStem.c_str()).erase (xstr_infStem.length()-1, 1) +
                    str_VerbStemAlternation;


            }
            break;
        }
        case 15:            // одеть, достать, остаться
        case 16:            // жить(ся), плыть, слыть
        {
            xstr_1SgStem = xstr_3SgStem = xstr_infStem;            
            break;
        }
        default:
        {
            ATLASSERT(0);
            ERROR_LOG (L"Unrecognized conjugation type.");
            return E_FAIL;
        }

    }

    return h_r;

}    //  h_BuildVerbStems()

HRESULT CT_FormBuilderBaseConj::h_StandardAlternation (CT_ExtString& xstr_presentStem)
{
    map<wstring, wstring>& map_StandardAlternations = pco_Lexeme->map_StandardAlternations;
    wstring& str_VerbStemAlternation = pco_Lexeme->str_VerbStemAlternation;

//    if (pco_Lexeme->xstr_GraphicStem.length() <= 3)
//    {
//        ATLASSERT(0);
//        ERROR_LOG (L"Graphic stem too short.");
//        return E_INVALIDARG;
//    }

    xstr_presentStem = pco_Lexeme->xstr_GraphicStem;

    wstring str_finale;
    if (xstr_presentStem.b_EndsWith (L"ск"))
    {
        str_finale = L"ск";
    }
    else
    {
        if (xstr_presentStem.b_EndsWith (L"ст"))
        {
            str_finale = L"ст";
        }
        else
        {
            str_finale = xstr_presentStem[xstr_presentStem.length()-2];
        }
    }

    xstr_presentStem.erase (xstr_presentStem.length()-1);
    map<wstring, wstring>::iterator it_alt = map_StandardAlternations.find (str_finale);
    if (map_StandardAlternations.end() != it_alt)
    {
        if (L"щ" == str_VerbStemAlternation)
        {
            ATLASSERT (L"т" == str_finale);
            xstr_presentStem = xstr_presentStem.replace (xstr_presentStem.length()-1, 1, str_VerbStemAlternation);
        }
        else
        {
            xstr_presentStem = xstr_presentStem.replace (xstr_presentStem.length()-1, 1, (*it_alt).second);
        }
    }

    return S_OK;

}   //  h_StandardAlternation()

HRESULT CT_FormBuilderBaseConj::h_GetPastTenseStressTypes (ET_AccentType eo_accentType,
                                                           ET_Number eo_number, 
                                                           ET_Gender eo_gender,
                                                           vector<ET_StressLocation>& vec_eoStressType)
{
    HRESULT h_r = S_OK;

    if (POS_VERB != pco_Lexeme->eo_PartOfSpeech)
    {
        ATLASSERT(0);
        wstring str_msg (L"Unexpected part of speech value.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    vector<ET_StressLocation> vec_stress;
    switch (eo_accentType)
    {
        case AT_A:
        {
            vec_eoStressType.push_back (STRESS_LOCATION_STEM);
            break;
        }
        case AT_B:
        {
            vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
            break;
        }
        case AT_C:
        {
            if (NUM_SG == eo_number && GENDER_N == eo_gender)
            {
                vec_eoStressType.push_back (STRESS_LOCATION_STEM);
            }
            else if (NUM_PL == eo_number)
            {
                vec_eoStressType.push_back (STRESS_LOCATION_STEM);
                break;
            }
            else
            {
                vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
            }
            break;
        }
        case AT_C2:
        {
            if (REFL_NO == pco_Lexeme->eo_Reflexive)
            {
                ATLASSERT(0);
                wstring str_msg (L"Unexpected part of speech value.");
                ERROR_LOG (str_msg);
                throw CT_Exception (E_FAIL, str_msg);
            }
            else if (!(GENDER_F == eo_gender && NUM_SG == eo_number))
            {
                vec_eoStressType.push_back (STRESS_LOCATION_STEM);
                vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
            }
            else
            {
                vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
            }
            break;
        }
        default:
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected past tense accent type.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_FAIL, str_msg);
        }

    }   // switch (eo_accentType)

    return S_OK;

}   //  h_GetPastTenseStressType (...)

HRESULT CT_FormBuilderBaseConj::h_GetStemStressPositions (const wstring& str_lemma, 
                                                          vector<int>& vec_iPosition)
{
    //
    // Find the sequence # of the stressed vowel in infinitive
    //
    vector<int>& vec_infStress = pco_Lexeme->vec_SourceStressPos; // alias for readability
    vector<int>::iterator it_infStresPos = vec_infStress.begin();
    for (; it_infStresPos != vec_infStress.end(); ++it_infStresPos)
    {
        CT_ExtString xstr_lemma (str_lemma);
        xstr_lemma.v_SetVowels (str_Vowels);
        int i_stemSyllables = xstr_lemma.i_NSyllables();
        if (i_stemSyllables < 1)
        {
            ATLASSERT(0);
            wstring str_msg (L"No vowels in verb stem.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_FAIL, str_msg);
        }

        if (*it_infStresPos >= i_stemSyllables)
        {
            vec_iPosition.push_back (i_stemSyllables - 1);  // last stem syllable
        }
        else
        {
            vec_iPosition.push_back (*it_infStresPos);    // same syllable as in infinitive
        }

    }   //  for (; it_sourceStressPos != vec_SourceStressPos.end(); ... )

    return S_OK;

}   //  h_GetStemStressPositions (...)

HRESULT CT_FormBuilderBaseConj::h_GetEndingStressPosition (const wstring& str_lemma, 
                                                           const wstring& str_ending,
                                                           int& i_position)
{
   HRESULT h_r = S_OK;

    if (POS_VERB != pco_Lexeme->eo_PartOfSpeech)
    {
        ATLASSERT(0);
        wstring str_msg (L"Unexpected part of speech.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_UNEXPECTED, str_msg);
    }

    bool b_retract = false;
    CT_ExtString xstr_ending (str_ending);
    xstr_ending.v_SetVowels (str_Vowels);
    if (0 == xstr_ending.i_NSyllables())
    {
        if (!(xstr_ending.length() == 0 || 
              L"ь" == xstr_ending || 
              L"й" == xstr_ending || 
              L"л" == xstr_ending))
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected non-syllabic ending.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_FAIL, str_msg);
        }
        b_retract = true;
    }
    else
    {
        if (L"те" == xstr_ending || L"ся" == xstr_ending || L"тесь" == xstr_ending)
        {
            b_retract = true;
        }
        else if (xstr_ending.b_EndsWith (L"те") || 
                 xstr_ending.b_EndsWith (L"ся") || 
                 xstr_ending.b_EndsWith (L"тесь"))
        {
            if (L'ь' == xstr_ending[0] || L'й' == xstr_ending[0])
            {
                b_retract = true;
            }
        }
    }

    CT_ExtString xstr_lemma (str_lemma);
    xstr_lemma.v_SetVowels (str_Vowels);
    if (!b_retract)
    {
        i_position = xstr_lemma.i_NSyllables();
    }
    else
    {
        i_position = xstr_lemma.i_NSyllables() - 1;
    }

    return h_r;

}   //  h_GetEndingStressPosition (...)

HRESULT CT_FormBuilderBaseConj::h_BuildPastTenseStem (wstring& str_lemma)
{
    HRESULT h_r = S_OK;

    CT_ExtString xstr_lemma;

    int i_type = pco_Lexeme->i_Type;
    if (7 == i_type || 8 == i_type)
    {
        xstr_lemma = pco_Lexeme->xstr_1SgStem;
        if (xstr_lemma.b_EndsWithOneOf (L"тд"))
        {
            xstr_lemma = xstr_lemma.erase (xstr_lemma.length()-1);
        }
    }
    else
    {
        xstr_lemma = pco_Lexeme->xstr_InfStem;
    }
    if (9 == i_type)
    {
        ATLASSERT (xstr_lemma.b_EndsWith (L"е"));
        xstr_lemma = xstr_lemma.erase (xstr_lemma.length()-1);
    }
    if (3 == i_type && 1 == pco_Lexeme->i_StemAugment)
    {
        ATLASSERT (xstr_lemma.b_EndsWith (L"ну"));
        xstr_lemma = xstr_lemma.erase (xstr_lemma.length()-2);
    }

    str_lemma = xstr_lemma.c_str();

    return h_r;

}   //  h_BuildPastTenseStem (...)

HRESULT CT_FormBuilderBaseConj::h_HandleYoAlternation (int i_stressSyll,
                                                       ET_Subparadigm eo_subParadigm, 
                                                       wstring& str_lemma)
{
    HRESULT h_r = S_OK;

    if (!pco_Lexeme->b_YoAlternation)
    {
        return S_OK;
    }

    if (SUBPARADIGM_PAST_TENSE != eo_subParadigm && 
        SUBPARADIGM_PART_PAST_ACT != eo_subParadigm &&
        SUBPARADIGM_PART_PAST_PASS_LONG != eo_subParadigm &&
        SUBPARADIGM_PART_PAST_PASS_SHORT != eo_subParadigm &&
        SUBPARADIGM_ADVERBIAL_PAST != eo_subParadigm &&
        SUBPARADIGM_PART_PRES_PASS_LONG != eo_subParadigm &&
        SUBPARADIGM_PART_PRES_PASS_SHORT != eo_subParadigm )
    {
        return S_OK;
    }

    if (SUBPARADIGM_PART_PAST_ACT == eo_subParadigm)
    {
        ATLASSERT (7 == pco_Lexeme->i_Type &&                  // GDRL, p. 85
        CT_ExtString (pco_Lexeme->xstr_SourceForm).b_EndsWith (L"сти") &&
        (L"т" == pco_Lexeme->str_VerbStemAlternation || 
         L"д" == pco_Lexeme->str_VerbStemAlternation));
    }

    CT_ExtString xstr_lemma (str_lemma);
    if (i_stressSyll >= xstr_lemma.i_NSyllables())
    {
        return S_FALSE;
    }

    int i_eOffset = str_lemma.rfind (_T("е"));    // last "e" in graphic stem (?)
    if (wstring::npos == i_eOffset)
    {
        ATLASSERT(0);
        wstring str_msg (L"Unstressed stem with yo alternation has no e.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    int i_stressPos = CT_ExtString (str_lemma).i_GetVowelPos (i_stressSyll);
    if (i_stressPos == i_eOffset)
    {
        str_lemma[i_stressPos] = L'ё';
    }

    return S_OK;

}   //  h_HandleYoAlternation (...)

HRESULT CT_FormBuilderBaseConj::h_Get1SgIrregular (ST_IrregularForm& st_if)
{
    HRESULT h_r = S_OK;

    map<int, ST_IrregularForm>& map_if = pco_Lexeme->map_IrregularForms;
    CT_GramHasher co_1SgHash (SUBPARADIGM_PRESENT_TENSE, 
                              NUM_SG, 
                              GENDER_UNDEFINED, 
                              PERSON_1, 
                              ANIM_UNDEFINED, 
                              CASE_UNDEFINED, 
                              pco_Lexeme->eo_Reflexive);

    map<int, ST_IrregularForm>::iterator it_1Sg = map_if.find (co_1SgHash.i_GramHash());
    if (map_if.end() == it_1Sg)
    {
        ATLASSERT(0);
        wstring str_msg (L"Irregular 1 Sg form not found in database.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    st_if = (*it_1Sg).second;

    return S_OK;

}   //  h_Get3SgIrregular (...)

HRESULT CT_FormBuilderBaseConj::h_Get3SgIrregular (ST_IrregularForm& st_if)
{
    HRESULT h_r = S_OK;

    map<int, ST_IrregularForm>& map_if = pco_Lexeme->map_IrregularForms;
    CT_GramHasher co_3SgHash (SUBPARADIGM_PRESENT_TENSE, 
                              NUM_SG, 
                              GENDER_UNDEFINED, 
                              PERSON_3, 
                              ANIM_UNDEFINED, 
                              CASE_UNDEFINED, 
                              pco_Lexeme->eo_Reflexive);

    map<int, ST_IrregularForm>::iterator it_3Sg = map_if.find (co_3SgHash.i_GramHash());
    if (map_if.end() == it_3Sg)
    {
        ATLASSERT(0);
        wstring str_msg (L"Irregular 3 Sg form not found in database.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    st_if = (*it_3Sg).second;

    return S_OK;

}   //  h_Get3SgIrregular (...)

HRESULT CT_FormBuilderBaseConj::h_Get3PlIrregular (ST_IrregularForm& st_if)
{
    HRESULT h_r = S_OK;

    map<int, ST_IrregularForm>& map_if = pco_Lexeme->map_IrregularForms;
    CT_GramHasher co_3PlHash (SUBPARADIGM_PRESENT_TENSE, 
                              NUM_PL, 
                              GENDER_UNDEFINED, 
                              PERSON_3, 
                              ANIM_UNDEFINED, 
                              CASE_UNDEFINED, 
                              pco_Lexeme->eo_Reflexive);

    map<int, ST_IrregularForm>::iterator it_3Pl = map_if.find (co_3PlHash.i_GramHash());
    if (map_if.end() == it_3Pl)
    {
        ATLASSERT(0);
        wstring str_msg (L"Irregular 3 Pl form not found in database.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    st_if = (*it_3Pl).second;

    return S_OK;

}   //  h_Get3PlIrregular (...)

bool CT_FormBuilderBaseConj::b_HasIrregularPresent()
{
    if (pco_Lexeme->map_IrregularForms.empty())
    {
        return false;
    }

    // Must have at least two forms: 1 sg and 3 sg (GDRL, p. 89)
    map<int, ST_IrregularForm>& map_if = pco_Lexeme->map_IrregularForms;
    CT_GramHasher co_1SgHash (SUBPARADIGM_PRESENT_TENSE, 
                              NUM_SG, 
                              GENDER_UNDEFINED, 
                              PERSON_1, 
                              ANIM_UNDEFINED, 
                              CASE_UNDEFINED, 
                              pco_Lexeme->eo_Reflexive);

    map<int, ST_IrregularForm>::iterator it_1Sg = map_if.find (co_1SgHash.i_GramHash());
    if (map_if.end() == it_1Sg)
    {
        return false;
    }

    CT_GramHasher co_3SgHash (SUBPARADIGM_PRESENT_TENSE, 
                              NUM_SG, 
                              GENDER_UNDEFINED, 
                              PERSON_3, 
                              ANIM_UNDEFINED, 
                              CASE_UNDEFINED, 
                              pco_Lexeme->eo_Reflexive);

    map<int, ST_IrregularForm>::iterator it_3Sg = map_if.find (co_3SgHash.i_GramHash());
    if (map_if.end() == it_3Sg)
    {
        ATLASSERT(0);
        wstring str_msg (L"3 Sg form not found in database.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    return true;

}   //  b_HasIrregularPresent()

bool CT_FormBuilderBaseConj::b_HasIrregularPast()
{
    if (pco_Lexeme->map_IrregularForms.empty())
    {
        return false;
    }

    // Must have at least two forms: m and f (GDRL, p. 89)
    map<int, ST_IrregularForm>& map_if = pco_Lexeme->map_IrregularForms;
    CT_GramHasher co_mSgHash (SUBPARADIGM_PAST_TENSE, 
                              NUM_SG, 
                              GENDER_M, 
                              PERSON_UNDEFINED, 
                              ANIM_UNDEFINED, 
                              CASE_UNDEFINED, 
                              pco_Lexeme->eo_Reflexive);

    map<int, ST_IrregularForm>::iterator it_mSg = map_if.find (co_mSgHash.i_GramHash());
    if (map_if.end() == it_mSg)
    {
        return false;
    }

    CT_GramHasher co_fSgHash (SUBPARADIGM_PAST_TENSE, 
                              NUM_SG, 
                              GENDER_F, 
                              PERSON_UNDEFINED, 
                              ANIM_UNDEFINED, 
                              CASE_UNDEFINED, 
                              pco_Lexeme->eo_Reflexive);

    map<int, ST_IrregularForm>::iterator it_fSg = map_if.find (co_fSgHash.i_GramHash());
    if (map_if.end() == it_fSg)
    {
        ATLASSERT(0);
        wstring str_msg (L"f Sg form not found in database.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    return true;

}   //  b_HasIrregularPast()

bool CT_FormBuilderBaseConj::b_HasIrregularPassivePartPast()
{
    if (pco_Lexeme->map_IrregularForms.empty())
    {
        return false;
    }

    // Must have at least one form: nom. sg. m. long (GDRL, p. 89)
    map<int, ST_IrregularForm>& map_if = pco_Lexeme->map_IrregularForms;
    CT_GramHasher co_mSgHash (SUBPARADIGM_PART_PAST_PASS_LONG, 
                              NUM_SG, 
                              GENDER_M, 
                              PERSON_UNDEFINED, 
                              ANIM_UNDEFINED, 
                              CASE_UNDEFINED, 
                              pco_Lexeme->eo_Reflexive);

    map<int, ST_IrregularForm>::iterator it_mSg = map_if.find (co_mSgHash.i_GramHash());
    if (map_if.end() == it_mSg)
    {
        return false;
    }

    return true;

}   //  b_HasIrregularPassivePartPast()

HRESULT CT_FormBuilderBaseConj::h_GetPastMIrregular (ST_IrregularForm& st_if)
{
    HRESULT h_r = S_OK;

    map<int, ST_IrregularForm>& map_if = pco_Lexeme->map_IrregularForms;
    CT_GramHasher co_mSgHash (SUBPARADIGM_PAST_TENSE, 
                              NUM_SG, 
                              GENDER_M, 
                              PERSON_UNDEFINED, 
                              ANIM_UNDEFINED, 
                              CASE_UNDEFINED, 
                              pco_Lexeme->eo_Reflexive);

    map<int, ST_IrregularForm>::iterator it_mSg = map_if.find (co_mSgHash.i_GramHash());
    if (map_if.end() == it_mSg)
    {
        ATLASSERT(0);
        wstring str_msg (L"Irregular m Sg form not found in database.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    st_if = (*it_mSg).second;

    return S_OK;

}   //  h_GetPastMIrregular (...)

HRESULT CT_FormBuilderBaseConj::h_GetPastFIrregular (ST_IrregularForm& st_if)
{
    HRESULT h_r = S_OK;

    map<int, ST_IrregularForm>& map_if = pco_Lexeme->map_IrregularForms;
    CT_GramHasher co_fSgHash (SUBPARADIGM_PAST_TENSE, 
                              NUM_SG, 
                              GENDER_F, 
                              PERSON_UNDEFINED, 
                              ANIM_UNDEFINED, 
                              CASE_UNDEFINED, 
                              pco_Lexeme->eo_Reflexive);

    map<int, ST_IrregularForm>::iterator it_fSg = map_if.find (co_fSgHash.i_GramHash());
    if (map_if.end() == it_fSg)
    {
        ATLASSERT(0);
        wstring str_msg (L"Irregular f Sg form not found in database.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    st_if = (*it_fSg).second;

    return S_OK;

}   //  h_GetPastFIrregular (...)

HRESULT CT_FormBuilderBaseConj::h_GetPastPartPassNSgIrregular (ST_IrregularForm& st_if)
{
    HRESULT h_r = S_OK;

    map<int, ST_IrregularForm>& map_if = pco_Lexeme->map_IrregularForms;
    CT_GramHasher co_partPassLong (SUBPARADIGM_PART_PAST_PASS_LONG, 
                                   NUM_SG, 
                                   GENDER_M, 
                                   PERSON_UNDEFINED, 
                                   ANIM_UNDEFINED, 
                                   CASE_NOM, 
                                   pco_Lexeme->eo_Reflexive);

    map<int, ST_IrregularForm>::iterator it_ppp = map_if.find (co_partPassLong.i_GramHash());
    if (map_if.end() == it_ppp)
    {
        ATLASSERT(0);
        wstring str_msg (L"Irregular part past passive form not found in database.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    st_if = (*it_ppp).second;

    return S_OK;

}   //  h_GetPastFIrregular (...)

/*
HRESULT CT_FormBuilderBaseConj::h_GetIrregularPresentActiveParticiple (ST_IrregularForm& st_if)
{
    HRESULT h_r = S_OK;

    map<int, ST_IrregularForm>& map_if = pco_Lexeme->map_IrregularForms;
    CT_GramHasher co_partHash (SUBPARADIGM_PART_PRES_ACT, 
                               NUM_SG, 
                               GENDER_M, 
                               PERSON_UNDEFINED, 
                               ANIM_UNDEFINED, 
                               CASE_NOM, 
                               pco_Lexeme->eo_Reflexive);

    map<int, ST_IrregularForm>::iterator it_part = map_if.find (co_partHash.i_GramHash());
    if (map_if.end() == it_part)
    {
        return S_FALSE;
    }

    st_if = (*it_part).second;

    return S_OK;

}   //  h_GetIrregularPresentActiveParticiple (...)
*/
