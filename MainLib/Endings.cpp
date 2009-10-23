#include "StdAfx.h"
#include "Endings.h"

HRESULT CT_Endings::h_AddEnding (const wstring& str_ending,
                                 ET_Number eo_number, 
                                 ET_Case eo_case, 
                                 ET_EndingStressType eo_stress)
{

    HRESULT h_r = h_HandleAddEnding (ENDING_CLASS_NOUN, 
                                     str_ending, 
                                     GENDER_UNDEFINED, 
                                     eo_number, 
                                     eo_case, 
                                     ANIM_UNDEFINED, 
                                     eo_stress);

    return h_r;

}   // h_AddEnding (...)

HRESULT CT_Endings::h_AddEnding (const wstring& str_ending,
                                 ET_Gender eo_gender,
                                 ET_Number eo_number, 
                                 ET_Case eo_case, 
                                 ET_Animacy eo_animacy, 
                                 ET_EndingStressType eo_stress)
{
    HRESULT h_r = h_HandleAddEnding (ENDING_CLASS_ADJECTIVE, 
                                     str_ending, 
                                     eo_gender, 
                                     eo_number, 
                                     eo_case, 
                                     eo_animacy, 
                                     eo_stress);
    return h_r;
}

HRESULT CT_Endings::h_HandleAddEnding (ET_EndingClass eo_class,
                                       const wstring& str_ending,
                                       ET_Gender eo_gender, 
                                       ET_Number eo_number, 
                                       ET_Case eo_case, 
                                       ET_Animacy eo_animacy, 
                                       ET_EndingStressType eo_stress)
{
    //
    // Gender
    //
    std::vector<ET_Gender> vec_gender;
    if (ENDING_CLASS_NOUN == eo_class)
    {
        vec_gender.push_back (GENDER_UNDEFINED);
    }
    else
    {
        if (ENDING_CLASS_ADJECTIVE)
        {
            if (GENDER_UNDEFINED == eo_gender)
            {
                for (int i_gen = (int)GENDER_UNDEFINED; i_gen < GENDER_COUNT; ++i_gen)
                {
                    vec_gender.push_back ((ET_Gender)i_gen);
                }
            }
            else
            {
                vec_gender.push_back (eo_gender);
            }
        }
    }

    //
    // Number
    //
    std::vector<ET_Number> vec_number;
    if (NUM_UNDEFINED == eo_number)
    {
        for (int i_num = (int)NUM_UNDEFINED; i_num < (int)NUM_COUNT; ++i_num)
        {
            vec_number.push_back ((ET_Number)i_num);
        }
    }
    else
    {
        vec_number.push_back (eo_number);
    }

    //
    // Case (must be defined for nouns)
    //
    if (CASE_UNDEFINED == eo_case)
    {
        ERROR_LOG (_T("Undefined case in noun ending."));
        return E_FAIL;
    }

    //
    // Animacy
    //
    std::vector<ET_Animacy> vec_animacy;
    if (ENDING_CLASS_NOUN == eo_class)
    {
        vec_animacy.push_back (ANIM_UNDEFINED);
    }
    else
    {
        if (ANIM_UNDEFINED == eo_animacy)
        {
            for (int i_anim = ANIM_UNDEFINED; i_anim < (int)ANIM_COUNT; ++i_anim)
            {
                vec_animacy.push_back ((ET_Animacy)i_anim);
            }
        }
        else
        {
            vec_animacy.push_back (eo_animacy);
        }
    }

    //
    // Ending stressed/unstressed
    //
    std::vector<ET_EndingStressType> vec_stress;
    if (ENDING_STRESS_UNDEFINED == eo_stress)
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
        vec_stress.push_back (eo_stress);
    }

    for (int i_g = 0; i_g < (int)vec_gender.size(); ++i_g)
        for (int i_n = 0; i_n < (int)vec_number.size(); ++i_n)
            for (int i_a = 0; i_a < (int)vec_animacy.size(); ++i_a)
                for (int i_s = 0; i_s < (int)vec_stress.size(); ++i_s)
                {
                    int i_key = arrHashKeys[vec_gender[i_g]][vec_number[i_n]][eo_case][vec_animacy[i_a]][vec_stress[i_s]];
                    mmap_Endings.insert (std::pair<int, wstring> (i_key, str_ending));
                }

    return S_OK;

}   //  h_HandleAddEnding (...)

int CT_Endings::i_GetNumOfEndings (ET_Gender eo_gender,
                                   ET_Number eo_number, 
                                   ET_Case eo_case, 
                                   ET_Animacy eo_animacy,
                                   ET_EndingStressType eo_stress)
{
    int i_key = arrHashKeys[eo_gender][eo_number][eo_case][eo_animacy][eo_stress];
    return mmap_Endings.count (i_key);
}

bool CT_Endings::b_GetEnding (wstring& str_ending,
                              ET_Gender eo_gender, 
                              ET_Number eo_number, 
                              ET_Case eo_case, 
                              ET_Animacy eo_animacy, 
                              ET_EndingStressType eo_stress,
                              int i_seqNum)
{
    int i_key = arrHashKeys[eo_gender][eo_number][eo_case][eo_animacy][eo_stress];
    ItPair pair_Range = mmap_Endings.equal_range (i_key);
    for (int i_item = 0; i_item < i_seqNum; ++i_item)
    {
        if (++pair_Range.first == pair_Range.second)
        {
            return false;
        }
    }

    str_ending = (*pair_Range.first).second;

    return true;
}
