#include "StdAfx.h"
#include "FormBuilderBaseDecl.h"

HRESULT CT_FormBuilderBaseDecl::h_FleetingVowelCheck (ET_Number eo_number,               // in
                                                      ET_Case eo_case,                   // in
                                                      ET_Gender eo_gender,               // in
                                                      ET_StressLocation eo_stressType,   // in
                                                      ET_Subparadigm eo_subparadigm,     // in
                                                      wstring& str_ending,               // in/out
                                                      wstring& str_lemma)                // out
{
    HRESULT h_r = S_OK;

//    if (SUBPARADIGM_LONG_ADJ != eo_subparadigm && 
//        SUBPARADIGM_SHORT_ADJ != eo_subparadigm && 
//        SUBPARADIGM_UNDEFINED != eo_subparadigm &&
//        SUBPARADIGM_NOUN != eo_subparadigm)
//    {
//        ATLASSERT(0);
//        wstring str_msg (L"Illegal subparadigm.");
//        ERROR_LOG (str_msg);
//        throw CT_Exception (E_INVALIDARG, str_msg);
//    }

    if (!b_FleetingVowel)
    {
        return S_OK;
    }

    int i_type = pco_Lexeme->i_Type;
    wstring& str_inflection = pco_Lexeme->str_InflectionType;

    //
    // Determine if the fleeting vowel is in initial form GDRL, p. 29-30
    //

    // Fl vowel is in the source form: m. (except for the likes of мужчина), f. type 8*,
    // and pronominal decl.
    if (POS_NOUN == pco_Lexeme->eo_PartOfSpeech)
    {
        if (GENDER_M == eo_gender || (GENDER_F == eo_gender && 8 == pco_Lexeme->i_Type))
        {
            h_FleetingVowelRemove (eo_gender, str_ending, str_lemma);
            return S_OK;
        }
    }
    if (pco_Lexeme->eo_PartOfSpeech == POS_PRONOUN_ADJ)
    {
        h_FleetingVowelRemove (eo_gender, str_ending, str_lemma);
        return S_OK;
    }

    if (2 == pco_Lexeme->i_Type && AT_A == pco_Lexeme->eo_AccentType1)
    {
        if (POS_NOUN == pco_Lexeme->eo_PartOfSpeech && 
            GENDER_F == eo_gender && 
            NUM_PL == eo_number && 
            CASE_GEN == eo_case)
        {                               // башен
            if (L"ь" != str_ending)
            {
                ATLASSERT(0);
                wstring str_msg (L"Unexpected ending.");
                ERROR_LOG (str_msg);
                throw CT_Exception (E_UNEXPECTED, str_msg);
            }
            str_ending.erase();
        }
        if (POS_ADJ == pco_Lexeme->eo_PartOfSpeech &&
            SUBPARADIGM_SHORT_ADJ == eo_subparadigm &&
            NUM_SG == eo_number &&
            GENDER_M == eo_gender)
        {
            if (L"ь" != str_ending)
            {
                ATLASSERT(0);
                wstring str_msg (L"Unexpected ending.");
                ERROR_LOG (str_msg);
                throw CT_Exception (E_UNEXPECTED, str_msg);
            }
            str_ending.erase();
        }
    }

    // Fl vowel is NOT in the source form: f. other than 8*, n., adj. declension,
    // nouns of the мн. type
    if (POS_NOUN == pco_Lexeme->eo_PartOfSpeech)
    {
        if ((GENDER_F == eo_gender && 8 != i_type) || GENDER_N == eo_gender)
        {
            h_FleetingVowelAdd (eo_number, eo_case, eo_gender, eo_stressType, eo_subparadigm, str_lemma);
            return S_OK;
        }
        if (L"мн." == pco_Lexeme->str_MainSymbol)
        {
            h_FleetingVowelAdd (eo_number, eo_case, eo_gender, eo_stressType, eo_subparadigm, str_lemma);
            return S_OK;
        }
    }
//    if (L"п" == str_inflection)
    if (SUBPARADIGM_SHORT_ADJ == eo_subparadigm || 
        SUBPARADIGM_PART_PRES_PASS_SHORT == eo_subparadigm ||
        SUBPARADIGM_PART_PAST_PASS_SHORT)
    {
        h_FleetingVowelAdd (eo_number, eo_case, eo_gender, eo_stressType, eo_subparadigm, str_lemma);
        return S_OK;
    }        

    ATLASSERT(0);
    wstring str_msg (L"Unable to determine fleeting vowel type.");
    ERROR_LOG (str_msg);
    throw CT_Exception (E_FAIL, str_msg);

}   //  h_FleetingVowelCheck (...)

HRESULT CT_FormBuilderBaseDecl::h_FleetingVowelAdd (ET_Number eo_number,               // in
                                                    ET_Case eo_case,                   // in
                                                    ET_Gender eo_gender,               // in
                                                    ET_StressLocation eo_stressType,   // in
                                                    ET_Subparadigm eo_subparadigm,     // in
                                                    wstring& str_lemma)                // out
{
    HRESULT h_r = S_OK;

    wstring& str_inflection = pco_Lexeme->str_InflectionType;
    int i_type = pco_Lexeme->i_Type;
    CT_ExtString xstr_grStem;
    if (SUBPARADIGM_PART_PRES_PASS_SHORT == eo_subparadigm ||
        SUBPARADIGM_PART_PAST_PASS_SHORT)
    {
        xstr_grStem = str_lemma;
    }
    else
    {
        xstr_grStem = pco_Lexeme->xstr_GraphicStem;
    }

    b_FleetingVowelAdded = false;

    // Fem nouns except i-stems: G Pl (and A PL if animate);
    // adjectives (non-pronominal paradigm) short form m;
    // nouns with main symbol "mn."

    if (POS_NOUN == pco_Lexeme->eo_PartOfSpeech)
    {
        if (NUM_PL != eo_number)
        {
            return S_OK;
        }

        if (eo_case == CASE_ACC)
        {

            if (pco_Endings->i_Count (ST_EndingDescriptor (GENDER_UNDEFINED, 
                                                           NUM_PL, 
                                                           CASE_ACC, 
                                                           ANIM_UNDEFINED, 
                                                           eo_stressType)) != 1)
            {
                wstring str_msg (L"Wrong number of endings.");
                ERROR_LOG (str_msg);
                throw CT_Exception (E_UNEXPECTED, str_msg);
            }

            std::wstring str_endingPlAcc;
            h_r = pco_Endings->h_GetEnding (ST_EndingDescriptor (GENDER_UNDEFINED, 
                                                                 NUM_PL, 
                                                                 eo_case, 
                                                                 ANIM_UNDEFINED, 
                                                                 eo_stressType), 
                                                                 str_endingPlAcc);

            std::wstring str_endingPlGen;
            h_r = pco_Endings->h_GetEnding (ST_EndingDescriptor (GENDER_UNDEFINED, 
                                                                 NUM_PL, 
                                                                 CASE_GEN, 
                                                                 ANIM_UNDEFINED, 
                                                                 eo_stressType),
                                                                 str_endingPlGen);

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

//    if (L"п" == str_inflection)
    if (SUBPARADIGM_SHORT_ADJ == eo_subparadigm || 
        SUBPARADIGM_PART_PRES_PASS_SHORT == eo_subparadigm ||
        SUBPARADIGM_PART_PAST_PASS_SHORT)
    {
//        if (SUBPARADIGM_SHORT_ADJ != eo_subparadigm || GENDER_M != eo_gender)
        if (GENDER_M != eo_gender)
        {
            return S_OK;
        }
    }

    b_FleetingVowelAdded = true;
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

    int i_lastConsonant = xstr_grStem.find_last_of (str_Consonants);
    if (i_lastConsonant <= 0)
    {
        ATLASSERT(0);
        wstring str_msg (L"Can't find last consonant.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_UNEXPECTED, str_msg);
    }

    if ((L'ь' == xstr_grStem[i_lastConsonant-1]) || 
        (L'й' == xstr_grStem[i_lastConsonant-1]))
    {
        if (L'ц' == xstr_grStem[i_lastConsonant])
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
            str_lemma[i_lastConsonant-1] = L'е';       // шпилек, чаек, писем
            return S_OK;
        }
    }
    if (L'к' == xstr_grStem[i_lastConsonant-1] || 
        L'г' == xstr_grStem[i_lastConsonant-1] ||
        L'х' == xstr_grStem[i_lastConsonant-1])
    {
        str_lemma.insert (i_lastConsonant, 1, L'о');  // кукол, окон, мягок
        return S_OK;
    }
    if (L'к' == xstr_grStem[i_lastConsonant] ||
        L'г' == xstr_grStem[i_lastConsonant] ||
        L'х' == xstr_grStem[i_lastConsonant])
    {
        if (3 != i_type)
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected type.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_FAIL, str_msg);
        }
        if (wstring::npos == wstring (str_ShSounds + L'ц').find (xstr_grStem[i_lastConsonant-1]))
        {
            str_lemma.insert (i_lastConsonant, 1, L'о');  // сказок, ведерок, краток, долог
            return S_OK;
        }
    }
    if (L'ц' == xstr_grStem[i_lastConsonant])
    {
        str_lemma.insert (i_lastConsonant, 1, L'е');      // овец
        return S_OK;
    }

    if (STRESS_LOCATION_ENDING == eo_stressType)
    {
        if (wstring::npos != str_ShSounds.find (xstr_grStem[i_lastConsonant-1]))
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

    return S_OK;

}   //  h_FleetingVowelAdd (...)

HRESULT CT_FormBuilderBaseDecl::h_FleetingVowelRemove (ET_Gender eo_gender,               // in
                                                       const wstring& str_ending,         // in
                                                       wstring& str_lemma)                // out
{
    HRESULT h_r = S_OK;

    wstring& str_inflection = pco_Lexeme->str_InflectionType;
    int i_type = pco_Lexeme->i_Type;
    CT_ExtString& xstr_grStem = pco_Lexeme->xstr_GraphicStem;

    // Forms that have the fleeting vowel (all from the initial form except -ью):
    // m.: NULL (сон, конец)
    // m.: -ь (кашель)
    // f.: -ь, -ью (любовь)
    // pron.: NULL, -ь, -й (весь, волчий)

    // ASSERT: last stem vowel is { о, е, ё, и } [also а, я "in several anomalous words"]

    //
    // No vowel in ending: keep the fleeting vowel
    //
    if (str_ending.empty())
    {
        if (GENDER_M != eo_gender)
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected gender for null ending.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_UNEXPECTED, str_msg);
        }
        return S_OK;
    }

    if (L"ь" == str_ending)
    {
    //    if (L"мс" != str_inflection && !(GENDER_F == eo_gender && 8 == i_type))
    //    {
    //        ATLASSERT(0);
    //        wstring str_msg (L"Unexpected type for ending 'ь'."); .. NO!! cf кашель
    //        ERROR_LOG (str_msg);
    //        throw CT_Exception (E_UNEXPECTED, str_msg);
    //    }
        return S_OK;
    }

    if (L"ью" == str_ending)
    {
        if (GENDER_F != eo_gender || 8 != i_type)
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected type for ending 'ью'.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_UNEXPECTED, str_msg);
        }
        return S_OK;
    }

    if (L"й" == str_ending)
    {
        if (L"мс-п" != str_inflection)
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected type for ending 'й'.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_UNEXPECTED, str_msg);
        }
        return S_OK;
    }

    int i_lastVowel = xstr_grStem.find_last_of (str_Vowels);

    if (wstring::npos == i_lastVowel)
    {
        str_lemma.clear();
        ATLASSERT(0);
        wstring str_msg (L"Vowel expected.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_UNEXPECTED, str_msg);
    }

    if (xstr_grStem[i_lastVowel] == L'о')   // сон, любовь
    {
        b_FleetingVowelAdded = false;
        str_lemma.erase (i_lastVowel, 1);
        return S_OK;
    }

    if (xstr_grStem[i_lastVowel] == L'и' ||
        (xstr_grStem[i_lastVowel] == L'е' && POS_PRONOUN_ADJ == pco_Lexeme->eo_PartOfSpeech))   // чей, волчий
    {
        if ((L"мс-п" != str_inflection) || (1 != i_type && 2 != i_type && 6 != i_type))
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected inflection type.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_UNEXPECTED, str_msg);
        }
        b_FleetingVowelAdded = false;
        if (1 == i_type)        // один
        {
            str_lemma.erase(i_lastVowel, 1);
        }
        else if (2 == i_type)   // весь
        {
            str_lemma.erase(i_lastVowel, 1);
            // Palatalize the ending
            if (L"о" == str_ending)
            {
                //str_ending = L"ё";
            }
            if (L"а" == str_ending)
            {
                //str_ending = L"я";
            }
        }
        else if (6 == i_type)   // чей, волчий
        {
            str_lemma[i_lastVowel] = L'ь';
        }
        return S_OK;
    }

    if ((xstr_grStem[i_lastVowel] == L'е') || (xstr_grStem[i_lastVowel] == L'ё'))
    {
        b_FleetingVowelAdded = false;

        if (i_lastVowel > 0)
        {
            if (wstring::npos != str_Vowels.find (xstr_grStem[i_lastVowel-1]))
            {
                str_lemma[i_lastVowel] = L'й';   // боец, паек
                return S_OK;
            }
        }
        if (GENDER_M == eo_gender)
        {
            if (6 == i_type)                    // улей
            {
                str_lemma = xstr_grStem.c_str();
                str_lemma[i_lastVowel] = L'ь';
                return S_OK;
            }
            if (3 == i_type)
            {
                if ((i_lastVowel > 0) && 
                    (wstring::npos == wstring (str_ShSounds + L'ц').find (xstr_grStem[i_lastVowel-1])))
                {
                    str_lemma[i_lastVowel] = L'ь';   // зверек
                    return S_OK;
                }
            }
            if ((i_lastVowel > 0) && (L'л' == xstr_grStem[i_lastVowel-1]))
            {
                str_lemma[i_lastVowel] = L'ь';       // лед, палец
                return S_OK;
            }
        }
        
        str_lemma.erase (i_lastVowel, 1);   // default -- just remove the vowel

        return S_OK;
    
    }   // е or ё

    ATLASSERT(0);
    wstring str_msg (L"Function logic error.");
    ERROR_LOG (str_msg);
    throw CT_Exception (E_UNEXPECTED, str_msg);

}   //  h_FleetingVowelRemove (...)

HRESULT CT_FormBuilderBaseDecl::h_GetStemStressPositions (const wstring& str_lemma, 
                                                          ET_Subparadigm eo_subParadigm,
                                                          vector<int>& vec_iPositions)
{
    HRESULT h_r = S_OK;

    CT_ExtString xstr_lemma (str_lemma);
    xstr_lemma.v_SetVowels (str_Vowels);

    int i_nSyllablesInStem = pco_Lexeme->xstr_GraphicStem.i_NSyllables();

    vector<int>::iterator it_pos = pco_Lexeme->vec_SourceStressPos.begin();
    for (; it_pos != pco_Lexeme->vec_SourceStressPos.end() && 
           *it_pos >= 0 &&          // that should be always true? 
           *it_pos < i_nSyllablesInStem; 
         ++it_pos)
    {
        vec_iPositions.push_back (*it_pos);
    }

    if (pco_Lexeme->vec_SourceStressPos.end() == it_pos)
    {
        return S_OK;
    }

    //
    // Source stress is on ending:
    //
	int i_nSyll = xstr_lemma.i_GetNumOfSyllables();
    if (i_nSyll < 1)
    {
        ATLASSERT(0);
        wstring str_msg (L"i_NSyllables() failed.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    //
    // Trivial
    //
    if (1 == i_nSyll)
    {
        vec_iPositions.push_back (0);
        return S_OK;
    }

    //
    // 2 or more syllables
    //
    ET_AccentType eo_accentType = (SUBPARADIGM_SHORT_ADJ == eo_subParadigm ||
                                   SUBPARADIGM_PART_PRES_PASS_SHORT == eo_subParadigm ||
                                   SUBPARADIGM_PART_PAST_PASS_SHORT == eo_subParadigm)
                                    ? pco_Lexeme->eo_AccentType2 
                                    : pco_Lexeme->eo_AccentType1;
    switch (eo_accentType)
    {
        case AT_A:
        case AT_A1:
        case AT_B:
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected accent type.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_UNEXPECTED, str_msg);
        }
        case AT_B1:
        {
            if (b_FleetingVowelAdded)
            {
                vec_iPositions.push_back (i_nSyll - 2);
            }
            else
            {
                vec_iPositions.push_back (i_nSyll - 1);
            }
            return S_OK;
        }
        case AT_C:
        case AT_C1:
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected accent type.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_UNEXPECTED, str_msg);
        }
        case AT_D:
        case AT_D1:
        {
            vec_iPositions.push_back (i_nSyll - 1);
            return S_OK;
        }
        case AT_E:
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected accent type.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_UNEXPECTED, str_msg);
        }
        case AT_F:
        case AT_F1:
        case AT_F2:
        {
            vec_iPositions.push_back (0);
            return S_OK;
        }
        default:
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected accent type.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_FAIL, str_msg);
        }
    }

    return S_OK;

}   //  h_GetStemStressPositions (...)

HRESULT CT_FormBuilderBaseDecl::h_GetEndingStressPosition (const wstring& str_lemma, 
                                                           const wstring& str_ending,
                                                           int& i_position)
{
    CT_ExtString xstr_ending (str_ending);
    CT_ExtString xstr_lemma (str_lemma);
    if (xstr_ending.i_NSyllables() > 0)
    {
        i_position = xstr_lemma.i_NSyllables();
    }
    else
    {
        i_position = xstr_lemma.i_NSyllables() - 1;
        if (i_position < 0)
        {
            ATLASSERT(0);
            wstring str_msg (L"Warning: can't find stressed vowel.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_FAIL, str_msg);
        }
    }

    return S_OK;

}   //  h_GetEndingStressPosition (...)

HRESULT CT_FormBuilderBaseDecl::h_HandleYoAlternation (ET_StressLocation eo_stressType, 
                                                       int i_stressedSyll,
                                                       wstring& str_lemma)
{
    HRESULT h_r = S_OK;

    if (!b_YoAlternation)
    {
        return S_OK;
    }

    if (STRESS_LOCATION_UNDEFINED == eo_stressType)
    {
        ATLASSERT(0);
        wstring str_msg (L"Undefined stress type.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_INVALIDARG, str_msg);
    }

    wstring str_graphicStem = (POS_VERB == pco_Lexeme->eo_PartOfSpeech) 
        ? str_lemma // for participles, graphic stem is participial stem
        : pco_Lexeme->xstr_GraphicStem.c_str();

    int i_yoOffset = str_graphicStem.find (_T("ё"));
    if (std::wstring::npos != i_yoOffset)
    {
        // Graphic stem contains yo: replace with e if stress is on ending
        if (STRESS_LOCATION_ENDING == eo_stressType)
        {
            if (i_stressedSyll >= CT_ExtString (str_lemma).i_NSyllables())
            {
                str_lemma[i_yoOffset] = L'е';        //  yo --> e: ежа, щелочей, тяжела
            }
        }
    }
    else
    {
        // Graphic stem has no yo (must have at least on e)
        if (STRESS_LOCATION_STEM == eo_stressType)
        {
            int i_eOffset = str_graphicStem.rfind (_T("е"));    // last "e" in graphic stem (?)
            if (wstring::npos == i_eOffset)
            {
                if (POS_VERB != pco_Lexeme->eo_PartOfSpeech) 
                {
                    ATLASSERT(0);
                    wstring str_msg (L"Unstressed stem with yo alternation has no e.");
                    ERROR_LOG (str_msg);
                    throw CT_Exception (E_FAIL, str_msg);
                }
                else
                {
                    return S_OK;
                }
            }

            int i_stressPos = CT_ExtString (str_lemma).i_GetVowelPos (i_stressedSyll);
            if (i_stressPos < 0 || i_stressPos > (int)str_lemma.length())
            {
                ATLASSERT(0);
                wstring str_msg (L"Bad stress position.");
                ERROR_LOG (str_msg);
                throw CT_Exception (E_FAIL, str_msg);
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
