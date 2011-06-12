#pragma once
#include "FormBuilderBaseDecl.h"

using namespace std;
using namespace std::tr1;

class CFormBuilderShortAdj : public CFormBuilderBaseDecl
{
public:

    //
    // Use with adjectives
    //
    CFormBuilderShortAdj (CLexeme * pLexeme) : 
        CFormBuilderBaseDecl (pLexeme, SUBPARADIGM_SHORT_ADJ),
        m_sLemma (pLexeme->m_sGraphicStem),
        m_iType (pLexeme->m_iType),
        m_eAccentType1 (pLexeme->m_eAccentType1),
        m_eAccentType2 (pLexeme->m_eAccentType2),
        m_bFleetingVowel (pLexeme->m_bFleetingVowel),
        m_mapCommonDeviations (pLexeme->m_mapCommonDeviations)
    {
        if (AT_UNDEFINED == m_eAccentType2)
        {
            switch (m_eAccentType1)
            {
                case AT_A:
                {
                    m_eAccentType2 = AT_A;
                    break;
                }
                case AT_A1:
                {
                    m_eAccentType2 = AT_A1;
                    break;
                }
                case AT_B:
                {
                    m_eAccentType2 = AT_B;
                    break;
                }
                default:
                {
                    ATLASSERT(0);
                    CEString sMsg (L"Accent type undefined for short forms.");
                    ERROR_LOG (sMsg);
                    throw CException (E_FAIL, sMsg);
                }
            }       // switch
        }
        m_pEndings = new CAdjShortEndings();
    }

    //
    // Use with participles
    //
    CFormBuilderShortAdj (CLexeme * pLexeme, 
                          bool bYoAlternation, 
                          const CEString& sLemma,
                          ET_Subparadigm eSubparadigm,
                          ET_AccentType eAccentType1,
                          ET_AccentType eAccentType2,
                          vector<int>& vecStressPos,
                          bool bFleetingVowel,
                          int iDeviation = -1,
                          ET_Status eStatus = STATUS_COMMON) : 
        CFormBuilderBaseDecl (pLexeme, eSubparadigm, bFleetingVowel, bYoAlternation), 
        m_sLemma (sLemma), 
        m_iType (1),
        m_vecStressPos (vecStressPos),
        m_eAccentType1 (eAccentType1),
        m_eAccentType2 (eAccentType2),
        m_bFleetingVowel (bFleetingVowel),
        m_eStatus (eStatus)
    {
        m_pEndings = new CAdjShortEndings();
        if (iDeviation > 0)
        {
            if (iDeviation != 2)
            {
                ATLASSERT(0);
                CESttring sMsg (L"Unexpected common deviation in short participle.");
                ERROR_LOG (sMsg);
                throw CException (E_INVALIDARG, sMsg);
            }
            m_mapCommonDeviations[iDeviation] = false; // not optional
        }
    }

    HRESULT hGetEndings();

    HRESULT hStressOnEnding (ET_Number eNumber, ET_Gender eGender);

    HRESULT hGetStressTypes (ET_Number eNumber, 
                             ET_Gender eGender, 
                             vector<ET_StressLocation>& vecStressType);

    HRESULT hCreateFormTemplate (const CEString& sLemma,
                                 const CEString& sEnding,
                                 ET_Gender eGender,
                                 ET_Number eNumber,
                                 CComObject<CWordForm> *& pWordForm);

    HRESULT hCreateWordForm (ET_Number eNumber, 
                             ET_Gender eGender, 
                             ET_StressLocation eStressType,
                             const CEString& sEnding,
                             CEString& sLemma);

    HRESULT hHandleDeviations (CWordForm *);

    HRESULT hCheckIrregularForms (ET_Gender eGender, ET_Number eNumber, bool& bHandled);

    HRESULT hBuild();

private:
    CEString m_sLemma;
    int m_iType;
    vector<int> m_vecStressPos;        // Participles only
    ET_AccentType m_eAccentType1, m_eAccentType2;
    bool m_bFleetingVowel;
    map<int, bool> m_mapCommonDeviations;
    ET_Status m_eoStatus;
};
