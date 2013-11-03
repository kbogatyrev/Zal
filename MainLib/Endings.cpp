#include "StdAfx.h"
#include "Endings.h"

int CNounEndings::iHash (const StEndingDescriptor& stD)
{
    ATLASSERT (stD.eAnimacy == ANIM_UNDEFINED && stD.eGender == GENDER_UNDEFINED);

    int iKey = stD.eNumber * CASE_COUNT * STRESS_LOCATION_COUNT +
                stD.eCase * STRESS_LOCATION_COUNT +
                stD.eStress;

    return iKey;
}

ET_ReturnCode CNounEndings::eAddEnding (const CEString& sEnding, const StEndingDescriptor& stDescriptor)
{
    std::vector<ET_Number> vecNumber;
    if (NUM_UNDEFINED == stDescriptor.eNumber)
    {
        for (int iNum = (int)NUM_UNDEFINED; iNum < (int)NUM_COUNT; ++iNum)
        {
            vecNumber.push_back ((ET_Number)iNum);
        }
    }
    else
    {
        vecNumber.push_back (stDescriptor.eNumber);
    }

    //
    // Case must be defined for nouns
    //
    if (CASE_UNDEFINED == stDescriptor.eCase)
    {
        ERROR_LOG (L"Undefined case in noun ending.");
        return H_ERROR_INVALID_ARG;
    }

    std::vector<ET_StressLocation> vecStress;
    if (STRESS_LOCATION_UNDEFINED == stDescriptor.eStress)
    {
        for (int iStress = STRESS_LOCATION_UNDEFINED; 
             iStress < (int)STRESS_LOCATION_COUNT; 
             ++iStress)
        {
            vecStress.push_back ((ET_StressLocation)iStress);
        }
    }
    else
    {
        vecStress.push_back (stDescriptor.eStress);
    }

    for (int iN = 0; iN < (int)vecNumber.size(); ++iN)
        for (int iS = 0; iS < (int)vecStress.size(); ++iS)
        {
            StEndingDescriptor stD (vecNumber[iN], stDescriptor.eCase, vecStress[iS]);
            int iKey = iHash (stD);
            m_mmEndings.insert (std::pair<int, CEString> (iKey, sEnding));
        }

    return H_NO_ERROR;

}   // CNounEndings::eAddEnding (...)

////////////////////////////////////////////////////////////////////////////////////

int CAdjLongEndings::iHash (const StEndingDescriptor& stD)
{
    int iKey = stD.eGender * NUM_COUNT * CASE_COUNT * ANIM_COUNT * STRESS_LOCATION_COUNT +
               stD.eNumber * CASE_COUNT * ANIM_COUNT * STRESS_LOCATION_COUNT +
               stD.eCase * ANIM_COUNT * STRESS_LOCATION_COUNT +
               stD.eAnimacy * STRESS_LOCATION_COUNT +
               stD.eStress;

    return iKey;
}

ET_ReturnCode CAdjLongEndings::eAddEnding (const CEString& sEnding, const StEndingDescriptor& stDescriptor)
{
    //
    // Gender
    //
    std::vector<ET_Gender> vecGender;
    if (GENDER_UNDEFINED == stDescriptor.eGender)
    {
        for (int iGen = (int)GENDER_UNDEFINED; iGen < GENDER_COUNT; ++iGen)
        {
            vecGender.push_back ((ET_Gender)iGen);
        }
    }
    else
    {
        vecGender.push_back (stDescriptor.eGender);
    }

    //
    // Number
    //
    std::vector<ET_Number> vecNumber;
    if (NUM_UNDEFINED == stDescriptor.eNumber)
    {
        for (int iNum = (int)NUM_UNDEFINED; iNum < (int)NUM_COUNT; ++iNum)
        {
            vecNumber.push_back ((ET_Number)iNum);
        }
    }
    else
    {
        vecNumber.push_back (stDescriptor.eNumber);
    }

    //
    // Animacy
    //
    std::vector<ET_Animacy> vecAnimacy;
    if (ANIM_UNDEFINED == stDescriptor.eAnimacy)
    {
        for (int iAnim = ANIM_UNDEFINED; iAnim < (int)ANIM_COUNT; ++iAnim)
        {
            vecAnimacy.push_back ((ET_Animacy)iAnim);
        }
    }
    else
    {
        vecAnimacy.push_back (stDescriptor.eAnimacy);
    }

    //
    // Ending stressed/unstressed
    //
    std::vector<ET_StressLocation> vecStress;
    if (STRESS_LOCATION_UNDEFINED == stDescriptor.eStress)
    {
        for (int iStress = STRESS_LOCATION_UNDEFINED; 
             iStress < (int)STRESS_LOCATION_COUNT; 
             ++iStress)
        {
            vecStress.push_back ((ET_StressLocation)iStress);
        }
    }
    else
    {
        vecStress.push_back (stDescriptor.eStress);
    }

    for (int iG = 0; iG < (int)vecGender.size(); ++iG)
        for (int iN = 0; iN < (int)vecNumber.size(); ++iN)
            for (int iA = 0; iA < (int)vecAnimacy.size(); ++iA)
                for (int iS = 0; iS < (int)vecStress.size(); ++iS)
                {
                    StEndingDescriptor stD (vecGender[iG], 
                                            vecNumber[iN], 
                                            stDescriptor.eCase, 
                                            vecAnimacy[iA], 
                                            vecStress[iS]);
                    int iKey = iHash (stD);
                    m_mmEndings.insert (std::pair<int, CEString> (iKey, sEnding));
                }

    return H_NO_ERROR;

}   //  CAdjLongEndings::eAddEnding (...)

////////////////////////////////////////////////////////////////////////////////////

int CAdjShortEndings::iHash (const StEndingDescriptor& stD)
{
    int iKey = stD.eGender * NUM_COUNT * STRESS_LOCATION_COUNT +
               stD.eNumber * STRESS_LOCATION_COUNT +
               stD.eStress;
    return iKey;
}

ET_ReturnCode CAdjShortEndings::eAddEnding (const CEString& sEnding, const StEndingDescriptor& stDescriptor)
{
    //
    // Gender
    //
    std::vector<ET_Gender> vecGender;
    if (GENDER_UNDEFINED == stDescriptor.eGender)
    {
        for (int iGen = (int)GENDER_UNDEFINED; iGen < GENDER_COUNT; ++iGen)
        {
            vecGender.push_back ((ET_Gender)iGen);
        }
    }
    else
    {
        vecGender.push_back (stDescriptor.eGender);
    }

    //
    // Number
    //
    if (NUM_UNDEFINED == stDescriptor.eNumber)
    {
        ERROR_LOG (L"Undefined number in short form ending.");
        return H_ERROR_INVALID_ARG;
    }

    //
    // Ending stressed/unstressed
    //
    std::vector<ET_StressLocation> vecStress;
    if (STRESS_LOCATION_UNDEFINED == stDescriptor.eStress)
    {
        for (int iStress = STRESS_LOCATION_UNDEFINED; 
             iStress < (int)STRESS_LOCATION_COUNT; 
             ++iStress)
        {
            vecStress.push_back ((ET_StressLocation)iStress);
        }
    }
    else
    {
        vecStress.push_back (stDescriptor.eStress);
    }

    for (int iG = 0; iG < (int)vecGender.size(); ++iG)
        for (int iS = 0; iS < (int)vecStress.size(); ++iS)
        {
            StEndingDescriptor stD (vecGender[iG], stDescriptor.eNumber, vecStress[iS]);
            int iKey = iHash (stD);
            m_mmEndings.insert (std::pair<int, CEString> (iKey, sEnding));
        }

    return H_NO_ERROR;

}   //  CAdjShortEndings::hAddEnding (...)


////////////////////////////////////////////////////////////////////////////////////

int CPersonalEndings::iHash (const StEndingDescriptor& stD)
{
    int iKey = stD.iInflectionType * PERSON_COUNT * NUM_COUNT * STRESS_LOCATION_COUNT * STEM_AUSLAUT_COUNT +
               stD.ePerson * NUM_COUNT * STRESS_LOCATION_COUNT * STEM_AUSLAUT_COUNT +
               stD.eNumber * STRESS_LOCATION_COUNT * STEM_AUSLAUT_COUNT +
               stD.eStress * STEM_AUSLAUT_COUNT + stD.eStemAuslaut;
    return iKey;
}

ET_ReturnCode CPersonalEndings::eAddEnding (const CEString& sEnding, const StEndingDescriptor& stDescriptor)
{
    //
    // Person
    //
    std::vector<ET_Person> vecPerson;
    if (PERSON_UNDEFINED == stDescriptor.ePerson)
    {
        ERROR_LOG (L"Undefined person.");
        return H_ERROR_INVALID_ARG;
    }
    else
    {
        vecPerson.push_back (stDescriptor.ePerson);
    }

    //
    // Number
    //
    if (NUM_UNDEFINED == stDescriptor.eNumber)
    {
        ERROR_LOG (L"Undefined number in personal ending.");
        return H_ERROR_INVALID_ARG;
    }

    //
    // Ending stressed/unstressed
    //
    std::vector<ET_StressLocation> vecStress;
    if (STRESS_LOCATION_UNDEFINED == stDescriptor.eStress)
    {
        for (int iStress = STRESS_LOCATION_UNDEFINED; 
             iStress < (int)STRESS_LOCATION_COUNT; 
             ++iStress)
        {
            vecStress.push_back ((ET_StressLocation)iStress);
        }
    }
    else
    {
        vecStress.push_back (stDescriptor.eStress);
    }

    //
    // Ending after sh/not after sh
    //
    std::vector<ET_StemAuslaut> vecStemAuslaut;
    if (STEM_AUSLAUT_UNDEFINED == stDescriptor.eStemAuslaut)
    {
        for (int iStemAuslaut = STEM_AUSLAUT_UNDEFINED; 
             iStemAuslaut < (int)STEM_AUSLAUT_COUNT; 
             ++iStemAuslaut)
        {
            vecStemAuslaut.push_back ((ET_StemAuslaut)iStemAuslaut);
        }
    }
    else
    {
        vecStemAuslaut.push_back (stDescriptor.eStemAuslaut);
    }

    for (int iS = 0; iS < (int)vecStress.size(); ++iS)
        for (int iSa = 0; iSa < (int)vecStemAuslaut.size(); ++iSa)
        {
            StEndingDescriptor stD (stDescriptor.iInflectionType,
                                    stDescriptor.ePerson, 
                                    stDescriptor.eNumber, 
                                    vecStress[iS], 
                                    vecStemAuslaut[iSa]);
            int iKey = iHash (stD);
            m_mmEndings.insert (std::pair<int, CEString> (iKey, sEnding));
        }

    return H_NO_ERROR;

}   //  CPersonalEndings::eAddEnding (...)
