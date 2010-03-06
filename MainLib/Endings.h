#pragma once
#include "resource.h"       // main symbols
#include "MainLib_i.h"

using namespace std;

struct ST_EndingDescriptor
{
    int i_InflectionType;
    ET_Gender eo_Gender;
    ET_Number eo_Number;
    ET_Case eo_Case;
    ET_Animacy eo_Animacy;
    ET_Person eo_Person;
    ET_StressLocation eo_Stress;
    ET_StemAuslaut eo_StemAuslaut;

    ST_EndingDescriptor() : i_InflectionType (INFLECTION_TYPE_UNDEFINED),
                            eo_Gender (GENDER_UNDEFINED), 
                            eo_Number (NUM_UNDEFINED), 
                            eo_Case (CASE_UNDEFINED), 
                            eo_Animacy (ANIM_UNDEFINED), 
                            eo_Person (PERSON_UNDEFINED),
                            eo_Stress (STRESS_LOCATION_UNDEFINED),
                            eo_StemAuslaut (STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for nouns
    ST_EndingDescriptor (ET_Number eo_n, 
                         ET_Case eo_c, 
                         ET_StressLocation eo_s) :
                            i_InflectionType (INFLECTION_TYPE_UNDEFINED),
                            eo_Gender (GENDER_UNDEFINED), 
                            eo_Number (eo_n),
                            eo_Case (eo_c), 
                            eo_Animacy (ANIM_UNDEFINED), 
                            eo_Person (PERSON_UNDEFINED),
                            eo_Stress (eo_s),
                            eo_StemAuslaut (STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for long adjectves
    ST_EndingDescriptor (ET_Gender eo_g,
                         ET_Number eo_n, 
                         ET_Case eo_c,
                         ET_Animacy eo_a,
                         ET_StressLocation eo_s) :
                            i_InflectionType (INFLECTION_TYPE_UNDEFINED),
                            eo_Gender (eo_g), 
                            eo_Number (eo_n),
                            eo_Case (eo_c),
                            eo_Animacy (eo_a),
                            eo_Person (PERSON_UNDEFINED),
                            eo_Stress (eo_s),
                            eo_StemAuslaut (STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for short adjectives
    ST_EndingDescriptor (ET_Gender eo_g,
                         ET_Number eo_n,
                         ET_StressLocation eo_s) :
                            i_InflectionType (INFLECTION_TYPE_UNDEFINED),
                            eo_Gender (eo_g), 
                            eo_Number (eo_n),
                            eo_Case (CASE_UNDEFINED), 
                            eo_Animacy (ANIM_UNDEFINED), 
                            eo_Person (PERSON_UNDEFINED),
                            eo_Stress (eo_s),
                            eo_StemAuslaut (STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for personal forms
    ST_EndingDescriptor (int i_inflectionType,
                         ET_Person eo_p,
                         ET_Number eo_n,
                         ET_StressLocation eo_s,
                         ET_StemAuslaut eo_sa) :
                            i_InflectionType (i_inflectionType),
                            eo_Gender (GENDER_UNDEFINED),
                            eo_Number (eo_n),
                            eo_Case (CASE_UNDEFINED),
                            eo_Animacy (ANIM_UNDEFINED),
                            eo_Person (eo_p), 
                            eo_Stress (eo_s),
                            eo_StemAuslaut (eo_sa)
    {}

    wstring str_ToString() const
    {
        wstring str_ (L"Inflection type = ");
        str_ += CT_ExtString::str_ToString (i_InflectionType);
        str_ += L"; Gender = ";
        str_ += CT_ExtString::str_ToString (eo_Gender);
        str_ += L"; Number = ";
        str_ += CT_ExtString::str_ToString (eo_Number);
        str_ += L"; Case = ";
        str_ += CT_ExtString::str_ToString (eo_Case);
        str_ += L"; Animacy = ";
        str_ += CT_ExtString::str_ToString (eo_Animacy);
        str_ += L"; Person = ";
        str_ += CT_ExtString::str_ToString (eo_Person);
        str_ += L"; Stress = ";
        str_ += CT_ExtString::str_ToString (eo_Stress);
        str_ += L"; StemAuslaut = ";
        str_ += CT_ExtString::str_ToString (eo_StemAuslaut);

        return str_;
    }

};

class CT_Endings
{
protected:
    typedef std::multimap<int, wstring> EndingsMultiMap;
    typedef pair<EndingsMultiMap::const_iterator, EndingsMultiMap::const_iterator> ItPair;

public:
    CT_Endings()
    {}

    virtual ~CT_Endings()
    {}

    virtual void v_Reset()
    {
        mmap_Endings.clear();
    }

    virtual int i_Hash (const ST_EndingDescriptor&) = 0;

    int i_Count (const ST_EndingDescriptor& st_d)
    {
        int i_h = i_Hash (st_d);
        return mmap_Endings.count (i_h);
    }

    virtual HRESULT h_AddEnding (const wstring&, const ST_EndingDescriptor&) = 0;

    HRESULT h_GetEnding (const ST_EndingDescriptor& st_descriptor, int i_seqNum, wstring& str_ending)
    {
        int i_key = i_Hash (st_descriptor);

        ItPair pair_Range = mmap_Endings.equal_range (i_key);
        for (int i_item = 0; i_item < i_seqNum; ++i_item)
        {
            if (++pair_Range.first == pair_Range.second)
            {
                wstring str_msg (L"Error getting ending from hash for: ");
                str_msg += st_descriptor.str_ToString();
                ERROR_LOG (str_msg);
                return E_FAIL;
            }
        }

        str_ending = (*pair_Range.first).second;

        return S_OK;
    }

    HRESULT h_GetEnding (const ST_EndingDescriptor& st_d, wstring& str_)
    {
        return h_GetEnding (st_d, 0, str_);
    }

protected:
    std::multimap<int, wstring> mmap_Endings;

};

class CT_NounEndings : public CT_Endings
{
    virtual HRESULT h_AddEnding (const wstring&, const ST_EndingDescriptor&);

private:
    virtual int i_Hash (const ST_EndingDescriptor&);
};

class CT_AdjLongEndings : public CT_Endings
{
    virtual HRESULT h_AddEnding (const wstring&, const ST_EndingDescriptor&);

private:
    virtual int i_Hash (const ST_EndingDescriptor&);
};

class CT_AdjShortEndings : public CT_Endings
{
    virtual HRESULT h_AddEnding (const wstring&, const ST_EndingDescriptor&);

private:
    virtual int i_Hash (const ST_EndingDescriptor&);
};

class CT_PersonalEndings : public CT_Endings
{
    virtual HRESULT h_AddEnding (const wstring&, const ST_EndingDescriptor&);

private:
    virtual int i_Hash (const ST_EndingDescriptor&);
};
