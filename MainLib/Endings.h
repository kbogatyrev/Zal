#pragma once
#include "resource.h"       // main symbols
#include "MainLib_i.h"

using namespace std;

struct StEndingDescriptor
{
    int iInflectionType;
    ET_Gender eGender;
    ET_Number eNumber;
    ET_Case eCase;
    ET_Animacy eAnimacy;
    ET_Person ePerson;
    ET_StressLocation eStress;
    ET_StemAuslaut eStemAuslaut;

    StEndingDescriptor() : iInflectionType (INFLECTION_TYPE_UNDEFINED),
                           eGender (GENDER_UNDEFINED), 
                           eNumber (NUM_UNDEFINED), 
                           eCase (CASE_UNDEFINED), 
                           eAnimacy (ANIM_UNDEFINED), 
                           ePerson (PERSON_UNDEFINED),
                           eStress (STRESS_LOCATION_UNDEFINED),
                           eStemAuslaut (STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for nouns
    StEndingDescriptor (ET_Number eN, 
                         ET_Case eC, 
                         ET_StressLocation eS) :
                            iInflectionType (INFLECTION_TYPE_UNDEFINED),
                            eGender (GENDER_UNDEFINED), 
                            eNumber (eN),
                            eCase (eC), 
                            eAnimacy (ANIM_UNDEFINED), 
                            ePerson (PERSON_UNDEFINED),
                            eStress (eS),
                            eStemAuslaut (STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for long adjectves
    StEndingDescriptor (ET_Gender eG,
                         ET_Number eN, 
                         ET_Case eC,
                         ET_Animacy eA,
                         ET_StressLocation eS) :
                            iInflectionType (INFLECTION_TYPE_UNDEFINED),
                            eGender (eG), 
                            eNumber (eN),
                            eCase (eC),
                            eAnimacy (eA),
                            ePerson (PERSON_UNDEFINED),
                            eStress (eS),
                            eStemAuslaut (STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for short adjectives
    StEndingDescriptor (ET_Gender eG,
                         ET_Number eN,
                         ET_StressLocation eS) :
                            iInflectionType (INFLECTION_TYPE_UNDEFINED),
                            eGender (eG), 
                            eNumber (eN),
                            eCase (CASE_UNDEFINED), 
                            eAnimacy (ANIM_UNDEFINED), 
                            ePerson (PERSON_UNDEFINED),
                            eStress (eS),
                            eStemAuslaut (STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for personal forms
    StEndingDescriptor (int iInflectionType,
                         ET_Person eP,
                         ET_Number eN,
                         ET_StressLocation eS,
                         ET_StemAuslaut eSa) :
                            iInflectionType (iInflectionType),
                            eGender (GENDER_UNDEFINED),
                            eNumber (eN),
                            eCase (CASE_UNDEFINED),
                            eAnimacy (ANIM_UNDEFINED),
                            ePerson (eP), 
                            eStress (eS),
                            eStemAuslaut (eSa)
    {}

    CEString sToString() const
    {
        CEString s_ (L"Inflection type = ");
        s_ += CEString::sToString (iInflectionType);
        s_ += L"; Gender = ";
        s_ += CEString::sToString (eGender);
        s_ += L"; Number = ";
        s_ += CEString::sToString (eNumber);
        s_ += L"; Case = ";
        s_ += CEString::sToString (eCase);
        s_ += L"; Animacy = ";
        s_ += CEString::sToString (eAnimacy);
        s_ += L"; Person = ";
        s_ += CEString::sToString (ePerson);
        s_ += L"; Stress = ";
        s_ += CEString::sToString (eStress);
        s_ += L"; StemAuslaut = ";
        s_ += CEString::sToString (eStemAuslaut);

        return s_;
    }

};

class CEndings
{
protected:
    typedef std::multimap<int, CEString> EndingsMultiMap;
    typedef pair<EndingsMultiMap::const_iterator, EndingsMultiMap::const_iterator> ItPair;

public:
    CEndings()
    {}

    virtual ~CEndings()
    {}

    virtual void v_Reset()
    {
        m_mmEndings.clear();
    }

    virtual int iHash (const StEndingDescriptor&) = 0;

    int iCount (const StEndingDescriptor& stD)
    {
        int iH = iHash (stD);
        return m_mmEndings.count (iH);
    }

    virtual HRESULT hAddEnding (const CEString&, const StEndingDescriptor&) = 0;

    HRESULT hGetEnding (const StEndingDescriptor& stDescriptor, int iSeqNum, CEString& sEnding)
    {
        int iKey = iHash (stDescriptor);

        ItPair pair_Range = m_mmEndings.equal_range (iKey);
        for (int iItem = 0; iItem < iSeqNum; ++iItem)
        {
            if (++pair_Range.first == pair_Range.second)
            {
                CEString sMsg (L"Error getting ending from hash for: ");
                sMsg += stDescriptor.sToString();
                ERROR_LOG (sMsg);
                throw CException (E_FAIL, sMsg);
            }
        }

        sEnding = (*pair_Range.first).second;

        return S_OK;
    }

    HRESULT hGetEnding (const StEndingDescriptor& stD, CEString& s_)
    {
        return hGetEnding (stD, 0, s_);
    }

protected:
    std::multimap<int, CEString> m_mmEndings;

};

class CNounEndings : public CEndings
{
    virtual HRESULT hAddEnding (const CEString&, const StEndingDescriptor&);

private:
    virtual int iHash (const StEndingDescriptor&);
};

class CAdjLongEndings : public CEndings
{
    virtual HRESULT hAddEnding (const CEString&, const StEndingDescriptor&);

private:
    virtual int iHash (const StEndingDescriptor&);
};

class CAdjShortEndings : public CEndings
{
    virtual HRESULT hAddEnding (const CEString&, const StEndingDescriptor&);

private:
    virtual int iHash (const StEndingDescriptor&);
};

class CPersonalEndings : public CEndings
{
    virtual HRESULT hAddEnding (const CEString&, const StEndingDescriptor&);

private:
    virtual int iHash (const StEndingDescriptor&);
};
