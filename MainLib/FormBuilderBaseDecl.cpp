#include "StdAfx.h"
#include "FormBuilderBaseDecl.h"

HRESULT CT_FormBuilderBaseDecl::h_HandleFleetingVowel (ET_Number eo_number,               // in
                                                       ET_Case eo_case,                   // in
                                                       ET_Gender eo_gender,               // in
                                                       ET_StressLocation eo_stressType, // in
                                                       ET_Subparadigm eo_subparadigm,     // in
                                                       const wstring& str_ending,         // in
                                                       wstring& str_lemma)                // out
{
    HRESULT h_r = S_OK;

    if (SUBPARADIGM_LONG_ADJ != eo_subparadigm && 
        SUBPARADIGM_SHORT_ADJ != eo_subparadigm && 
        SUBPARADIGM_UNDEFINED != eo_subparadigm &&
        SUBPARADIGM_NOUN != eo_subparadigm)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Illegal subparadigm.");
        return E_UNEXPECTED;
    }

    int i_type = pco_Lexeme->i_Type;
    wstring str_grStem = pco_Lexeme->str_GraphicStem;
    wstring str_inflection = pco_Lexeme->str_InflectionType;

    int i_lastVowel = str_grStem.find_last_of (str_Vowels);

    //
    // Fleeting vowel in initial form GDRL, p. 29-30
    //

    // ASSERT: last stem vowel is { о, е, ё, и } [also а, я "in several anomalous words"]
    if (GENDER_M == eo_gender ||
        (GENDER_F == eo_gender && 8 == pco_Lexeme->i_Type) ||
        L"мс" == str_inflection)
    {
// endings that preserve the fleeting vowel (all from the initial form except -ью):
// m.: NULL (сон, конец)
// f.: -ь, -ью (любовь)
// pron.: NULL, -ь, -й (весь, волчий)

        //
        // No vowel in ending: keep the fleeting vowel
        //
        if (str_ending.empty())
        {
            if (GENDER_M != eo_gender)
            {
                ATLASSERT(0);
                ERROR_LOG (L"Enexpected gender for null ending");
                return E_UNEXPECTED;
            }
            return S_OK;
        }

        if (L"ь" == str_ending)
        {
            if ((L"мс" != str_inflection) || 
                (GENDER_F != eo_gender || 8 != i_type))
            {
                ATLASSERT(0);
                ERROR_LOG (L"Enexpected type for ending 'ь'");
                return E_UNEXPECTED;

            }
            return S_OK;
        }

        if (L"ью" == str_ending)
        {
            if (GENDER_F != eo_gender || 8 != i_type)
            {
                ATLASSERT(0);
                ERROR_LOG (L"Enexpected main symbol for ending 'ью'");
                return E_UNEXPECTED;
            }
            return S_OK;
        }

        if (L"й" == str_ending)
        {
            if (L"мс" != str_inflection)
            {
                ATLASSERT(0);
                ERROR_LOG (L"Enexpected main symbol for ending 'й'");
                return E_UNEXPECTED;
            }
            return S_OK;
        }

        if (wstring::npos == i_lastVowel)
        {
            ERROR_LOG (L"Vowel expected.");
            str_lemma.clear();
            return E_UNEXPECTED;
        }

        if (str_grStem[i_lastVowel] == L'о')   // сон, любовь
        {
            str_lemma.erase (i_lastVowel, 1);
            return S_OK;
        }

        if (str_grStem[i_lastVowel] == L'и')   // волчий
        {
            if ((L"мс" != str_inflection) || (6 != i_type))
            {
                ERROR_LOG (L"Unexpected inflection type.");
                str_lemma.clear();
                return E_UNEXPECTED;
            }
            str_lemma[i_lastVowel] = L'ь';
            return S_OK;
        }

        if ((str_grStem[i_lastVowel] == L'е') || 
            (str_grStem[i_lastVowel] == L'ё'))
        {
            if (i_lastVowel > 0)
            {
                if (wstring::npos != str_Vowels.find (str_grStem[i_lastVowel-1]))
                {
                    str_lemma[i_lastVowel] = L'й';   // боец, паек
                    return S_OK;
                }
            }
            if (GENDER_M == eo_gender)
            {
                if (6 == i_type)                    // улей
                {
                    str_lemma = str_grStem;
                    str_lemma[i_lastVowel] = L'ь';
                    return S_OK;
                }
                if (3 == i_type)
                {
                    if ((i_lastVowel > 0) && 
                        (wstring::npos == wstring (str_ShSounds + L'ц').find (str_grStem[i_lastVowel-1])))
                    {
                        str_lemma[i_lastVowel] = L'ь';   // зверек
                        return S_OK;
                    }
                }
                if ((i_lastVowel > 0) && (L'л' == str_grStem[i_lastVowel-1]))
                {
                    str_lemma[i_lastVowel] = L'ь';       // лед, палец
                    return S_OK;
                }
            }
            
            str_lemma.erase (i_lastVowel, 1);   // default -- just remove the vowel

            return S_OK;
        
        }   // е or ё
    }

    //
    // Fleeting vowel is NOT in the initial form
    //
// fem nouns except i-stems: G Pl (and A PL if animate)
// adjectives (non-pronominal paradigm) short form m
// nouns with main symbol "mn."
    if ((GENDER_F == eo_gender && 8 != i_type && NUM_PL == eo_number) || 
        (GENDER_N == eo_gender && NUM_PL == eo_number) || 
        (L"п" == str_inflection || L"мн." == pco_Lexeme->str_MainSymbol))
    {
        if (POS_NOUN == eo_subparadigm)
        {
            if (eo_case == CASE_ACC)
            {

                if (pco_Endings->i_Count (ST_EndingDescriptor (GENDER_UNDEFINED, 
                                                               NUM_PL, 
                                                               CASE_ACC, 
                                                               ANIM_UNDEFINED, 
                                                               eo_stressType)) != 1)
                {
                    ERROR_LOG (L"Wrong number of endings.");
                }
                std::wstring str_endingPlAcc;
                h_r = pco_Endings->h_GetEnding (ST_EndingDescriptor (GENDER_UNDEFINED, 
                                                                     NUM_PL, 
                                                                     eo_case, 
                                                                     ANIM_UNDEFINED, 
                                                                     eo_stressType), 
                                                                     str_endingPlAcc);
                if (S_OK != h_r)
                {
                    ERROR_LOG (L"GetEnding() failed.");
                }

                std::wstring str_endingPlGen;
                h_r = pco_Endings->h_GetEnding (ST_EndingDescriptor (GENDER_UNDEFINED, 
                                                                     NUM_PL, 
                                                                     CASE_GEN, 
                                                                     ANIM_UNDEFINED, 
                                                                     eo_stressType),
                                                                     str_endingPlGen);
                if (S_OK != h_r)
                {
                    ERROR_LOG (L"GetEnding() failed.");
                }

                if (str_endingPlAcc != str_endingPlGen)
                {
                    return S_OK;
                }
            }
            else
            {
                if (eo_case != CASE_GEN)
                {
                    return S_OK;
                }
            }
        }

        if (L"п" == str_inflection)
        {
            if (SUBPARADIGM_SHORT_ADJ != eo_subparadigm || GENDER_M != eo_gender)
            {
                return S_OK;
            }
        }

        if (((GENDER_F == eo_gender) || (GENDER_N == eo_gender)) && (6 == i_type))
        {
            if (STRESS_LOCATION_ENDING == eo_stressType)
            {
                str_lemma[str_lemma.length()-1] = L'е';   // статей, питей
            }
            else
            {
                str_lemma[str_lemma.length()-1] = L'и';   // гостий, ущелий
            }
            return S_OK;
        }

        int i_lastConsonant = str_grStem.find_last_of (str_Consonants);
        if (i_lastConsonant > 0)
        {
            if ((L'ь' == str_grStem[i_lastConsonant-1]) || 
                (L'й' == str_grStem[i_lastConsonant-1]))
            {
                if (L'ц' == str_grStem[i_lastConsonant])
                {
                    str_lemma[i_lastConsonant-1] = L'е';    // колец
                    return S_OK;
                }

                if (STRESS_LOCATION_ENDING == eo_stressType)
                {
                    str_lemma[i_lastConsonant-1] = L'ё';     // серёг, каём
                    return S_OK;
                }
                else
                {
                    str_lemma[i_lastConsonant] = L'е';       // шпилек, чаек, писем
                    return S_OK;
                }
            }
        }
        if (i_lastConsonant > 0)
        {
            if (L'к' == str_grStem[i_lastConsonant-1] || 
                L'г' == str_grStem[i_lastConsonant-1] ||
                L'х' == str_grStem[i_lastConsonant-1])
            {
                str_lemma.insert (i_lastConsonant, 1, L'о');  // кукол, окон, мягок
                return S_OK;
            }
            if (L'к' == str_grStem[i_lastConsonant] ||
                L'г' == str_grStem[i_lastConsonant] ||
                L'х' == str_grStem[i_lastConsonant])
            {
                if (3 != i_type)
                {
                    ERROR_LOG (_T("Unexpected type."));
                    return E_FAIL;
                }
                if (wstring::npos == wstring (str_ShSounds + L'ц').find (str_grStem[i_lastConsonant-1]))
                {
                    str_lemma.insert (i_lastConsonant, 1, L'о');  // сказок, ведерок, краток, долог
                    return S_OK;
                }
            }
            if (L'ц' == str_grStem[i_lastConsonant])
            {
                str_lemma.insert (i_lastConsonant, 1, L'е');      // овец
                return S_OK;
            }

            if (STRESS_LOCATION_ENDING == eo_stressType)
            {
                if (wstring::npos != str_ShSounds.find (str_grStem[i_lastConsonant-1]))
                {
                    str_lemma.insert (i_lastConsonant, 1, L'о');    // кишок
                    return S_OK;
                }
                else
                {
                    str_lemma.insert (i_lastConsonant, 1, L'ё');    // сестёр, хитёр
                    return S_OK;
                }
            }
            else
            {
                str_lemma.insert (i_lastConsonant, 1, L'е');       // сосен, чисел, ножен, верен
            }
        }
    }
    return S_OK;

}   //  h_HandleFleetingVowel (...)

//
// This is only needed when the initial form has ending stress
//
HRESULT CT_FormBuilderBaseDecl::h_GetStemStressPositions (const wstring& str_lemma, 
                                                          ET_Subparadigm eo_subParadigm,
                                                          vector<int>& vec_iPositions)
{
    HRESULT h_r = S_OK;

    vector<int>::iterator it_pos = pco_Lexeme->vec_SourceStressPos.begin();
    for (; it_pos != pco_Lexeme->vec_SourceStressPos.end() && 
           *it_pos >= 0 && 
           *it_pos < (int)pco_Lexeme->str_GraphicStem.length(); 
           ++it_pos)
    {
        vec_iPositions.push_back (*it_pos);
    }

    if (pco_Lexeme->vec_SourceStressPos.end() == it_pos)
    {
        return S_OK;
    }

    CT_ExtString xstr_stem (pco_Lexeme->str_GraphicStem);
    xstr_stem.v_SetVowels (str_Vowels);
    int i_syllables = xstr_stem.i_NSyllables();
    if (i_syllables < 1)
    {
        ATLASSERT (0);
        ERROR_LOG (L"i_NSyllables() failed.");
        return E_FAIL;
    }

    int i_lastVowelPos = xstr_stem.i_GetVowelPos (i_syllables - 1);

    //
    // Trivial
    //
    if (1 == i_syllables)
    {
        vec_iPositions.push_back (i_lastVowelPos);
        return S_OK;
    }

    //
    // 2 or more syllables
    //
    ET_AccentType eo_accentType = (SUBPARADIGM_SHORT_ADJ == eo_subParadigm) 
                                    ? pco_Lexeme->eo_AccentType2 
                                    : pco_Lexeme->eo_AccentType1;
    switch (eo_accentType)
    {
        case AT_A:
        case AT_A1:
        case AT_B:
        {
            ATLASSERT (0);
            ERROR_LOG (L"Unexpected accent type.");
            return E_UNEXPECTED;
        }
        case AT_B1:
        {
            vec_iPositions.push_back (i_lastVowelPos);
            return S_OK;
        }
        case AT_C:
        case AT_C1:
        {
            ATLASSERT (0);
            ERROR_LOG (L"Unexpected accent type.");
            return E_UNEXPECTED;
        }
        case AT_D:
        case AT_D1:
        {
            vec_iPositions.push_back (i_lastVowelPos);
            return S_OK;
        }
        case AT_E:
        {
            ATLASSERT (0);
            ERROR_LOG (L"Unexpected accent type.");
            return E_UNEXPECTED;
        }
        case AT_F:
        case AT_F1:
        case AT_F2:
        {
            unsigned int ui_firstVowel = xstr_stem.find_first_of (str_Vowels, 0);
            if (wstring::npos == ui_firstVowel)
            {
                ATLASSERT (0);
                ERROR_LOG (L"No vowels.");
                return E_FAIL;
            }
            vec_iPositions.push_back ((int)ui_firstVowel);
            return S_OK;
        }
        default:
        {
            ATLASSERT (0);
            ERROR_LOG (L"Unexpected accent type.");
            return E_UNEXPECTED;
        }
    }

    return S_OK;

}   //  h_GetStemStressPositions (...)

HRESULT CT_FormBuilderBaseDecl::h_GetEndingStressPosition (const wstring& str_lemma, 
                                                           const wstring& str_ending,
                                                           int& i_position)
{
    int i_endingVowel = str_ending.find_first_of (str_Vowels);
    if (wstring::npos != i_endingVowel)
    {
        i_position = str_lemma.length() + i_endingVowel;
    }
// &&&& pronominal decl -ogo -ego -omu -emu + samoyo, eyo, neyo
    else
    {
        int i_position = str_lemma.find_last_of (str_Vowels);
        if (wstring::npos == i_position)
        {
            ERROR_LOG (L"Warning: can't find stressed vowel.");
            i_position = -1;
            return E_FAIL;                        
        }
    }

    return S_OK;

}   //  h_GetEndingStressPosition (...)

HRESULT CT_FormBuilderBaseDecl::h_HandleYoAlternation (ET_StressLocation eo_stressType, 
                                                       int i_stressPos,
                                                       ET_Subparadigm eo_subParadigm, 
                                                       wstring& str_lemma)
{
    HRESULT h_r = S_OK;

// TODO: do we need the test below??
//    if (!b_YoAlternation && !b_OAlternation)
//    {
//        ERROR_LOG (L"No o/yo alternation.");
//        return E_UNEXPECTED;                        
//    }

    if (STRESS_LOCATION_UNDEFINED == eo_stressType)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Undefined stress type.");
        return E_INVALIDARG;
    }

    int i_yoOffset = pco_Lexeme->str_GraphicStem.find (_T("ё"));
    if (std::wstring::npos != i_yoOffset)
    {
        // Graphic stem contains yo: replace with e if stress is on ending
        if (STRESS_LOCATION_ENDING == eo_stressType)
        {
            str_lemma[i_yoOffset] = L'е';        //  yo --> e: ежа, щелочей, тяжела
        }
    }
    else
    {
        // Graphic stem has no yo (must have at least on e)
        if (STRESS_LOCATION_STEM == eo_stressType)
        {
            int i_eOffset = pco_Lexeme->str_GraphicStem.rfind (_T("е"));    // last "e" in graphic stem (?)
            if (wstring::npos == i_eOffset)
            {
                ATLASSERT(0);
                ERROR_LOG (L"Unstressed stem with yo alternation has no e.");
                return E_FAIL;
            }

            // Replace stressed e with yo:
            if (i_eOffset == i_stressPos)
            {
                str_lemma[i_eOffset] = L'ё';
            }
        }
    }

    return h_r;

}   //  h_HandleYoAlternation (...)
