#pragma once
#include "resource.h"       // main symbols
#include "MainLib_i.h"

using namespace std;

struct ST_Ending
{
    ST_Ending()
    {
        eo_Gender = GENDER_UNDEFINED;
        eo_Number = NUM_UNDEFINED;
        eo_Case = CASE_UNDEFINED;
        eo_Animacy = ANIM_UNDEFINED;
        eo_Stress = ENDING_STRESS_UNDEFINED;
    }

    ST_Ending (const ST_Ending& st_rhs) : 
        str_Ending (st_rhs.str_Ending), 
        eo_Gender (st_rhs.eo_Gender), 
        eo_Number (st_rhs.eo_Number), 
        eo_Case (st_rhs.eo_Case),
        eo_Animacy (st_rhs.eo_Animacy),
        eo_Stress (st_rhs.eo_Stress)
    {}

    wstring str_Ending;    
    ET_Gender eo_Gender;  // adj only
    ET_Number eo_Number;
    ET_Case eo_Case;
    ET_Animacy eo_Animacy;
    ET_EndingStressType eo_Stress;
};

class CT_Endings
{
typedef std::multimap<int, wstring> EndingsMultiMap;
typedef pair<EndingsMultiMap::const_iterator, EndingsMultiMap::const_iterator> ItPair;

public:
    CT_Endings()
    {
        int i_key = 0;

        for (int i_gender = 0; i_gender < GENDER_COUNT; ++i_gender)
            for (int i_num = 0; i_num < NUM_COUNT; ++i_num)
                for (int i_case = 0; i_case < CASE_COUNT; ++i_case)
                    for (int i_animacy = 0; i_animacy < ANIM_COUNT; ++i_animacy)
                        for (int i_eStress = 0; i_eStress < ENDING_STRESS_COUNT; ++i_eStress)
        {
            arrHashKeys[i_gender][i_num][i_case][i_animacy][i_eStress] = i_key++;
        }
    }

    void v_Reset()
    {
        mmap_Endings.clear();
    }

    HRESULT h_AddEnding (const wstring& str_ending,
                         ET_Number eo_number, 
                         ET_Case eo_case, 
                         ET_EndingStressType eo_stress);

    HRESULT h_AddEnding (const wstring& str_ending,
                         ET_Gender eo_gender, 
                         ET_Number eo_number, 
                         ET_Case eo_case, 
                         ET_Animacy eo_animacy, 
                         ET_EndingStressType eo_stress);

    int i_GetNumOfEndings (ET_Gender eo_gender,
                           ET_Number eo_number, 
                           ET_Case eo_case, 
                           ET_Animacy eo_animacy,
                           ET_EndingStressType eo_stress);

    bool b_GetEnding  (wstring& str_ending,
                       ET_Gender eo_gender, 
                       ET_Number eo_number, 
                       ET_Case eo_case, 
                       ET_Animacy eo_animacy,
                       ET_EndingStressType eo_stress,
                       int i_seqNum = 0);

private:
    HRESULT h_HandleAddEnding (ET_EndingClass eo_class,
                               const wstring& str_ending,
                               ET_Gender eo_gender, 
                               ET_Number eo_number, 
                               ET_Case eo_case, 
                               ET_Animacy eo_animacy, 
                               ET_EndingStressType eo_stress);

    int arrHashKeys[GENDER_COUNT][NUM_COUNT][CASE_COUNT][ANIM_COUNT][ENDING_STRESS_COUNT];
    std::multimap<int, wstring> mmap_Endings;

};  //  class CT_Endings
