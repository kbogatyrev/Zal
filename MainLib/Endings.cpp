#include "StdAfx.h"
#include "Endings.h"

int CT_NounEndings::i_Hash (const ST_EndingDescriptor& st_d)
{
    ATLASSERT (st_d.eo_Animacy == ANIM_UNDEFINED && st_d.eo_Gender == GENDER_UNDEFINED);

    int i_key = st_d.eo_Number * CASE_COUNT * ENDING_STRESS_COUNT +
                st_d.eo_Case * ENDING_STRESS_COUNT +
                st_d.eo_Stress;

    return i_key;
}

HRESULT CT_NounEndings::h_AddEnding (const wstring& str_ending, 
                                     const ST_EndingDescriptor& st_descriptor)
{
    std::vector<ET_Number> vec_number;
    if (NUM_UNDEFINED == st_descriptor.eo_Number)
    {
        for (int i_num = (int)NUM_UNDEFINED; i_num < (int)NUM_COUNT; ++i_num)
        {
            vec_number.push_back ((ET_Number)i_num);
        }
    }
    else
    {
        vec_number.push_back (st_descriptor.eo_Number);
    }

    //
    // Case must be defined for nouns
    //
    if (CASE_UNDEFINED == st_descriptor.eo_Case)
    {
        ERROR_LOG (_T("Undefined case in noun ending."));
        return E_INVALIDARG;
    }

    std::vector<ET_EndingStressType> vec_stress;
    if (ENDING_STRESS_UNDEFINED == st_descriptor.eo_Stress)
    {
        for (int i_stress = ENDING_STRESS_UNDEFINED; 
             i_stress < (int)ENDING_STRESS_COUNT; 
             ++i_stress)
        {
            vec_stress.push_back ((ET_EndingStressType)i_stress);
        }
    }
    else
    {
        vec_stress.push_back (st_descriptor.eo_Stress);
    }

    for (int i_n = 0; i_n < (int)vec_number.size(); ++i_n)
        for (int i_s = 0; i_s < (int)vec_stress.size(); ++i_s)
        {
            ST_EndingDescriptor st_d (vec_number[i_n], st_descriptor.eo_Case, vec_stress[i_s]);
            int i_key = i_Hash (st_d);
            mmap_Endings.insert (std::pair<int, wstring> (i_key, str_ending));
        }

    return S_OK;

}   // CT_NounEndings::h_AddEnding (...)

////////////////////////////////////////////////////////////////////////////////////

int CT_AdjLongEndings::i_Hash (const ST_EndingDescriptor& st_d)
{
    int i_key = st_d.eo_Gender * NUM_COUNT * CASE_COUNT * ANIM_COUNT * ENDING_STRESS_COUNT +
                st_d.eo_Number * CASE_COUNT * ANIM_COUNT * ENDING_STRESS_COUNT +
                st_d.eo_Case * ANIM_COUNT * ENDING_STRESS_COUNT +
                st_d.eo_Animacy * ENDING_STRESS_COUNT +
                st_d.eo_Stress;

    return i_key;
}

HRESULT CT_AdjLongEndings::h_AddEnding (const wstring& str_ending, 
                                        const ST_EndingDescriptor& st_descriptor)
{
    //
    // Gender
    //
    std::vector<ET_Gender> vec_gender;
    if (GENDER_UNDEFINED == st_descriptor.eo_Gender)
    {
        for (int i_gen = (int)GENDER_UNDEFINED; i_gen < GENDER_COUNT; ++i_gen)
        {
            vec_gender.push_back ((ET_Gender)i_gen);
        }
    }
    else
    {
        vec_gender.push_back (st_descriptor.eo_Gender);
    }

    //
    // Number
    //
    std::vector<ET_Number> vec_number;
    if (NUM_UNDEFINED == st_descriptor.eo_Number)
    {
        for (int i_num = (int)NUM_UNDEFINED; i_num < (int)NUM_COUNT; ++i_num)
        {
            vec_number.push_back ((ET_Number)i_num);
        }
    }
    else
    {
        vec_number.push_back (st_descriptor.eo_Number);
    }

    //
    // Animacy
    //
    std::vector<ET_Animacy> vec_animacy;
    if (ANIM_UNDEFINED == st_descriptor.eo_Animacy)
    {
        for (int i_anim = ANIM_UNDEFINED; i_anim < (int)ANIM_COUNT; ++i_anim)
        {
            vec_animacy.push_back ((ET_Animacy)i_anim);
        }
    }
    else
    {
        vec_animacy.push_back (st_descriptor.eo_Animacy);
    }

    //
    // Ending stressed/unstressed
    //
    std::vector<ET_EndingStressType> vec_stress;
    if (ENDING_STRESS_UNDEFINED == st_descriptor.eo_Stress)
    {
        for (int i_stress = ENDING_STRESS_UNDEFINED; 
             i_stress < (int)ENDING_STRESS_COUNT; 
             ++i_stress)
        {
            vec_stress.push_back ((ET_EndingStressType)i_stress);
        }
    }
    else
    {
        vec_stress.push_back (st_descriptor.eo_Stress);
    }

    for (int i_g = 0; i_g < (int)vec_gender.size(); ++i_g)
        for (int i_n = 0; i_n < (int)vec_number.size(); ++i_n)
            for (int i_a = 0; i_a < (int)vec_animacy.size(); ++i_a)
                for (int i_s = 0; i_s < (int)vec_stress.size(); ++i_s)
                {
                    ST_EndingDescriptor st_d (vec_gender[i_g], 
                                              vec_number[i_n], 
                                              st_descriptor.eo_Case, 
                                              vec_animacy[i_a], 
                                              vec_stress[i_s]);
                    int i_key = i_Hash (st_d);
                    mmap_Endings.insert (std::pair<int, wstring> (i_key, str_ending));
                }

    return S_OK;

}   //  CT_AdjLongEndings::h_AddEnding (...)

////////////////////////////////////////////////////////////////////////////////////

int CT_AdjShortEndings::i_Hash (const ST_EndingDescriptor& st_d)
{
    int i_key = st_d.eo_Gender * NUM_COUNT * ENDING_STRESS_COUNT +
                st_d.eo_Number * ENDING_STRESS_COUNT +
                st_d.eo_Stress;
    return i_key;
}

HRESULT CT_AdjShortEndings::h_AddEnding (const wstring& str_ending, 
                                         const ST_EndingDescriptor& st_descriptor)
{
    //
    // Gender
    //
    std::vector<ET_Gender> vec_gender;
    if (GENDER_UNDEFINED == st_descriptor.eo_Gender)
    {
        for (int i_gen = (int)GENDER_UNDEFINED; i_gen < GENDER_COUNT; ++i_gen)
        {
            vec_gender.push_back ((ET_Gender)i_gen);
        }
    }
    else
    {
        vec_gender.push_back (st_descriptor.eo_Gender);
    }

    //
    // Number
    //
    if (NUM_UNDEFINED == st_descriptor.eo_Number)
    {
        ERROR_LOG (_T("Undefined number in short form ending."));
        return E_INVALIDARG;
    }

    //
    // Ending stressed/unstressed
    //
    std::vector<ET_EndingStressType> vec_stress;
    if (ENDING_STRESS_UNDEFINED == st_descriptor.eo_Stress)
    {
        for (int i_stress = ENDING_STRESS_UNDEFINED; 
             i_stress < (int)ENDING_STRESS_COUNT; 
             ++i_stress)
        {
            vec_stress.push_back ((ET_EndingStressType)i_stress);
        }
    }
    else
    {
        vec_stress.push_back (st_descriptor.eo_Stress);
    }

    for (int i_g = 0; i_g < (int)vec_gender.size(); ++i_g)
        for (int i_s = 0; i_s < (int)vec_stress.size(); ++i_s)
        {
            ST_EndingDescriptor st_d (vec_gender[i_g], st_descriptor.eo_Number, vec_stress[i_s]);
            int i_key = i_Hash (st_d);
            mmap_Endings.insert (std::pair<int, wstring> (i_key, str_ending));
        }

    return S_OK;

}   //  CT_AdjShortEndings::h_AddEnding (...)


////////////////////////////////////////////////////////////////////////////////////

int CT_PersonalEndings::i_Hash (const ST_EndingDescriptor& st_d)
{
    int i_key = st_d.eo_Person * NUM_COUNT * ENDING_STRESS_COUNT +
                st_d.eo_Number * ENDING_STRESS_COUNT +
                st_d.eo_Stress;
    return i_key;
}

HRESULT CT_PersonalEndings::h_AddEnding (const wstring& str_ending, 
                                         const ST_EndingDescriptor& st_descriptor)
{
    //
    // Person
    //
    std::vector<ET_Person> vec_person;
    if (PERSON_UNDEFINED == st_descriptor.eo_Person)
    {
        ERROR_LOG (_T("Undefined person."));
        return E_INVALIDARG;
    }
    else
    {
        vec_person.push_back (st_descriptor.eo_Person);
    }

    //
    // Number
    //
    if (NUM_UNDEFINED == st_descriptor.eo_Number)
    {
        ERROR_LOG (_T("Undefined number in personal ending."));
        return E_INVALIDARG;
    }

    //
    // Ending stressed/unstressed
    //
    std::vector<ET_EndingStressType> vec_stress;
    if (ENDING_STRESS_UNDEFINED == st_descriptor.eo_Stress)
    {
        for (int i_stress = ENDING_STRESS_UNDEFINED; 
             i_stress < (int)ENDING_STRESS_COUNT; 
             ++i_stress)
        {
            vec_stress.push_back ((ET_EndingStressType)i_stress);
        }
    }
    else
    {
        vec_stress.push_back (st_descriptor.eo_Stress);
    }

    for (int i_s = 0; i_s < (int)vec_stress.size(); ++i_s)
    {
        ST_EndingDescriptor st_d (st_descriptor.eo_Person, st_descriptor.eo_Number, vec_stress[i_s]);
        int i_key = i_Hash (st_d);
        mmap_Endings.insert (std::pair<int, wstring> (i_key, str_ending));
    }

    return S_OK;

}   //  CT_PersonalEndings::h_AddEnding (...)
