#ifndef ENDINGS_H_INCLUDED
#define ENDINGS_H_INCLUDED

using namespace std;

class CLexeme;

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
public:
    CEndings(CLexeme * pLexeme) : m_pLexeme(pLexeme)
    {}

    virtual ~CEndings()
    {}

    virtual void Reset()
    {
        m_mmEndings.clear();
    }

    virtual int iHash (const StEndingDescriptor&) = 0;

    int iCount (const StEndingDescriptor& stD)
    {
        int iH = iHash (stD);
        return m_mmEndings.count (iH);
    }

    virtual ET_ReturnCode eLoad() = 0;
    virtual ET_ReturnCode eAddEnding (const CEString&, const StEndingDescriptor&) = 0;

    ET_ReturnCode eGetEnding (const StEndingDescriptor& stDescriptor, int iSeqNum, CEString& sEnding)
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
                return H_ERROR_GENERAL;
            }
        }

        sEnding = (*pair_Range.first).second;

        return H_NO_ERROR;
    }

    ET_ReturnCode eGetEnding (const StEndingDescriptor& stD, CEString& s_)
    {
        return eGetEnding (stD, 0, s_);
    }

protected:
    void ReportDbError();
    std::multimap<int, CEString> m_mmEndings;

    typedef std::multimap<int, CEString> EndingsMultiMap;
    typedef pair<EndingsMultiMap::const_iterator, EndingsMultiMap::const_iterator> ItPair;

    CLexeme * m_pLexeme;
};

class CNounEndings : public CEndings
{
public:
    CNounEndings(CLexeme *);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding (const CEString&, const StEndingDescriptor&);

private:
    virtual int iHash (const StEndingDescriptor&);
};

class CAdjLongEndings : public CEndings
{
public:
    CAdjLongEndings(CLexeme *, ET_Subparadigm);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding (const CEString&, const StEndingDescriptor&);

private:
    virtual int iHash (const StEndingDescriptor&);

    ET_Subparadigm m_eSubparadigm;
};

class CAdjShortEndings : public CEndings
{
public:
    CAdjShortEndings(CLexeme * pLexeme);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding (const CEString&, const StEndingDescriptor&);

private:
    virtual int iHash (const StEndingDescriptor&);
};

class CPersonalEndings : public CEndings
{
public:
    CPersonalEndings(CLexeme * pLexeme);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding (const CEString&, const StEndingDescriptor&);

private:
    virtual int iHash (const StEndingDescriptor&);
};

#endif // ENDINGS_H_INCLUDED
