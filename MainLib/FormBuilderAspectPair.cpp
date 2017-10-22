#include "EString.h"
#include "Lexeme.h"
#include "WordForm.h"
#include "FormBuilderAspectPair.h"

using namespace Hlib;

ET_ReturnCode CFormBuilderAspectPair::eBuild()
{
    StLexemeProperties stProperties = m_pLexeme->stGetProperties();
    if (!stProperties.bHasAspectPair)
    {
        return H_FALSE;
    }

    if (stProperties.sAspectPairData.bStartsWith(L"-") && stProperties.sAspectPairData.bEndsWith(L"-"))
    {
        m_eExtraData = ET_ExtraData::Vowel;
    }
    else if (stProperties.sAspectPairData.bStartsWith(L"-"))
    {
        m_eExtraData = ET_ExtraData::Suffix;
    }
    else
    {
        m_eExtraData = ET_ExtraData::WholeWord;
    }

    ET_ReturnCode eRet = H_NO_ERROR;
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    m_sSource = m_pLexeme->sSourceForm();
    if (m_sSource.bEndsWith(L"ся") || m_sSource.bEndsWith(L"сь"))
    {
        m_sReflexiveSuffix = m_sSource.sSubstr(m_sSource.uiLength()-3);
        m_sSource = m_sSource.sRemoveCharsFromEnd(2);
    }

    eRet = m_pLexeme->eGenerateParadigm();

    for (int iCount = 0; iCount < 2; ++iCount)
    {
        int iType = (0 == iCount) ? stProperties.iAspectPairType : stProperties.iAltAspectPairType;
        bool bIsVariant = (0 == iCount) ? false : true;

        switch (iType)
        {
            case 0:
            {
                if (0 == iCount)
                {
                    eRet = H_ERROR_UNEXPECTED;
                }
                else
                {
                    eRet = H_NO_ERROR;
                }
                break;
            }

            case -1:
            {
                eRet = eSvToNsvTypeI(bIsVariant);
                if (eRet != H_NO_ERROR)
                {
                    return eRet;
                }

                eRet = eFindStressPositionI(bIsVariant);
                if (eRet != H_NO_ERROR)
                {
                    return eRet;
                }

                if (ET_ExtraData::Vowel == m_eExtraData)
                {
                    eRet = eApplyVowelModification(bIsVariant);
                }

                break;
            }

            case -2:
            {
                eRet = eSvToNsvTypeII(bIsVariant);
                break;
            }

            case -3:
            {
                eRet = eSvToNsvTypeIII(bIsVariant);
                break;
            }

            case 1:
            case 5:
            case 6:
            {
                eRet = eNsvToSvTypes1_5_6(bIsVariant);
                break;
            }

            case 2:
            {
                eRet = eNsvToSvTypes2(bIsVariant);
                break;
            }

            case 3:
            {
                eRet = eNsvToSvTypes3(bIsVariant);
                break;
            }

            case 4:
            {
                eRet = eNsvToSvTypes4(bIsVariant);
                break;
            }

            case 7:
            {
                eRet = eNsvToSvTypes7(bIsVariant);
                break;
            }

            case 8:
            {
                eRet = eNsvToSvTypes8(bIsVariant);
                break;
            }

            case 9:
            {
                eRet = eNsvToSvTypes9(bIsVariant);
                break;
            }

            case 10:
            {
                eRet = eNsvToSvTypes10(bIsVariant);
                break;
            }

            case 14:
            {
                eRet = eNsvToSvTypes14(bIsVariant);
                break;
            }

            case 12:
            case 13:
            case 15:
            case 16:
            {
                eRet = eNsvToSvTypes11_12_13_15_16(bIsVariant);
                break;
            }

            default:
            {
                eRet = H_ERROR_UNEXPECTED;
            }
        }       // switch
    }       // for (int iCount = 0; iCount < 2; ++iCount)

    return eRet;

}       //  eBuild()

ET_ReturnCode CFormBuilderAspectPair::eGet1PersonWordForm()
{
    CGramHasher hasher1Sg(POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG,
        GENDER_UNDEFINED, PERSON_1, ANIM_UNDEFINED, m_pLexeme->eAspect(),
        m_pLexeme->eIsReflexive());

    CWordForm * pWordForm = NULL;
    ET_ReturnCode eRet = m_pLexeme->eWordFormFromHash(hasher1Sg.sGramHash(), 0, m_p1stPersonWordForm);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    if (NULL == pWordForm)
    {
        return H_ERROR_POINTER;
    }

    return H_NO_ERROR;

}       //  eGet1PersonWordForm()

ET_ReturnCode CFormBuilderAspectPair::eGetInfinitiveWordForm()
{
    CGramHasher hasherInf(POS_VERB, SUBPARADIGM_INFINITIVE, CASE_UNDEFINED, NUM_UNDEFINED,
        GENDER_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, m_pLexeme->eAspect(),
        m_pLexeme->eIsReflexive());

    ET_ReturnCode eRet = m_pLexeme->eWordFormFromHash(hasherInf.sGramHash(), 0, m_pInfWordForm);

    if (eRet != H_NO_ERROR)
    {
        m_pInfWordForm = NULL;
        return eRet;
    }

    if (NULL == m_pInfWordForm)
    {
        return H_ERROR_POINTER;
    }

    return H_NO_ERROR;

}       // eGetInfinitiveWordForm()

ET_ReturnCode CFormBuilderAspectPair::eGetInfinitiveStressPos(int& iStressPos)
{
    ET_ReturnCode eRet = H_NO_ERROR;
    iStressPos = -1;

    if (NULL == m_pInfWordForm)
    {
        eRet = eGetInfinitiveWordForm();
        if (eRet != H_NO_ERROR)
        {
            return eRet;
        }
    }

    int iInfStressPos = -1;
    ET_StressType eType;
    eRet = m_pInfWordForm->eGetFirstStressPos(iInfStressPos, eType);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    while (eType != ET_StressType::STRESS_PRIMARY && H_NO_ERROR == eRet)
    {
        eRet = m_pInfWordForm->eGetNextStressPos(iInfStressPos, eType);
    }

    return eRet;

}       //  eGetInfinitiveStressPos()

ET_ReturnCode CFormBuilderAspectPair::eGetNsvDerivationType(CWordForm * pInfinitive, int& iType)
{
    iType = 0;

    if (NULL == pInfinitive)
    {
        return H_ERROR_POINTER;
    }

    CEString sInfinitive = pInfinitive->sWordForm();
    
    int iInfStressPos = -1;
    ET_ReturnCode eRet = eGetInfinitiveStressPos(iInfStressPos);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    if (iInfStressPos < 0)
    {
        return H_ERROR_UNEXPECTED;
    }

    if (sInfinitive.bEndsWith(L"ивать") || sInfinitive.bEndsWith(L"ывать"))
    {
        try
        {
            int iStressedSyllable = pInfinitive->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
            if (iStressedSyllable < pInfinitive->sWordForm().uiNSyllables()-2)
            {
                iType = -1;
                return H_NO_ERROR;
            }
        }
        catch (CException& ex)
        {
            return H_ERROR_UNEXPECTED;
        }
    }

    if ((sInfinitive.bEndsWith(L"ать") && !sInfinitive.bEndsWith(L"вать")) || sInfinitive.bEndsWith(L"ять"))
    {
        int iStressedSyllable = pInfinitive->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
        ASSERT(iStressedSyllable > 0);
        if (pInfinitive->sWordForm().uiNSyllables()-1 == iStressedSyllable) // end-stressed?
        {
            iType = -2;
            return H_NO_ERROR;
        }
    }

    if (sInfinitive.bEndsWith(L"вать"))
    {
        int iStressedSyllable = pInfinitive->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
        ASSERT(iStressedSyllable > 0);
        if (pInfinitive->sWordForm().uiNSyllables()-1 == iStressedSyllable) // end-stressed?
        {
            iType = -3;
            return H_NO_ERROR;
        }
    }

    return H_ERROR_UNEXPECTED;

}       //  eGetNsvDerivationType()

////////////////////////////////////////////////////////////////////////////////////////////////

ET_ReturnCode CFormBuilderAspectPair::eSvToNsvTypeI(bool bIsVariant)
{
    ET_ReturnCode eRet = H_NO_ERROR;

    CEString& sAspectPair = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
    ET_ExtraData eExtraData = bIsVariant ? m_eAltExtraData : m_eExtraData;    
    CEString sData;
    if (ET_ExtraData::WholeWord == eExtraData)
    {
        StLexemeProperties stProperties = m_pLexeme->stGetProperties();
        sData = bIsVariant ? stProperties.sAltAspectPairData : stProperties.sAltAspectPairData;

        if (!stProperties.bHasAspectPair || sData.bIsEmpty())
        {
            return H_ERROR_UNEXPECTED;
        }

        sAspectPair = sData;

        return H_NO_ERROR;
    }

    if (m_sSource.bEndsWith(L"ать") || m_sSource.bEndsWith(L"оть"))
    {
        CEString sStem = m_sSource.sRemoveCharsFromEnd(3);
        eRet = eBuildTypeIa(sStem, sAspectPair);
        return eRet;
    }
    else if (m_sSource.bEndsWith(L"нуть") && !m_sSource.bEndsWith(L"ьнуть"))
    {
        CEString sStem = m_sSource.sRemoveCharsFromEnd(4);
        eRet = eBuildTypeIa(sStem, sAspectPair);
        return eRet;
    }

    if (m_sSource.bEndsWith(L"ять"))
    {
        CEString sStem = m_sSource.sRemoveCharsFromEnd(3);
        eRet = eBuildTypeIb(sStem, sAspectPair);
        return eRet;
    }
    else if (m_sSource.bEndsWith(L"ьнуть"))
    {
        CEString sStem = m_sSource.sRemoveCharsFromEnd(5);
        eRet = eBuildTypeIb(sStem, sAspectPair);
        return eRet;
    }

    // All other stem types:
    eRet = eBuildTypeIc(sAspectPair);

    return eRet;

}       // eSvToNsvTypeI()

ET_ReturnCode CFormBuilderAspectPair::eBuildTypeIa(const CEString& sStem, CEString& sOutput)
{
    if (CEString::bIn(sStem[sStem.uiLength() - 1], CEString(g_szHushers) + CEString(L"кгх")))
    {
        sOutput = sStem + L"ивать";
    }
    else
    {
        sOutput = sStem + L"ывать";
    }

    return H_NO_ERROR;
}

ET_ReturnCode CFormBuilderAspectPair::eBuildTypeIb(const CEString& sStem, CEString& sOutput)
{
    sOutput = sStem + L"ивать";
    return H_NO_ERROR;
}

ET_ReturnCode CFormBuilderAspectPair::eBuildTypeIc(CEString& sOutput)
{
    ET_ReturnCode eRet = eGet1PersonWordForm();
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }
    
    if (NULL == m_p1stPersonWordForm)
    {
        return H_ERROR_POINTER;
    }

    if (L"у" == m_p1stPersonWordForm->sEnding())
    {
        if (m_p1stPersonWordForm->sLemma().bEndsWith(CEString(g_szHushers) + CEString(L"кгх")))
        {
            sOutput = m_p1stPersonWordForm->sLemma() + L"ивать";
        }
        else
        {
            sOutput = m_p1stPersonWordForm->sLemma() + L"ывать";
        }
    }

    return H_NO_ERROR;

}       //  eBuildTypeIc()

ET_ReturnCode CFormBuilderAspectPair::eFindStressPositionI(bool bIsVariant)
{
    ET_ReturnCode eRet = eGetInfinitiveWordForm();
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    int iSourceStressPos = -1;
    eRet = eGetInfinitiveStressPos(iSourceStressPos);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    try
    {
        int& iStressPos = bIsVariant ? m_iAltStressPos : m_iStressPos;
        int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iSourceStressPos);
        if (m_pInfWordForm->sWordForm().bStartsWith(L"вы") ||
            m_pInfWordForm->sWordForm().uiNSyllables()-1 == iStressedSyllable)
        {
            CEString& sAspectPair = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
            ASSERT(sAspectPair.bEndsWith(L"ивать") || sAspectPair.bEndsWith(L"ывать"));
            iStressPos = sAspectPair.uiGetVowelPos(sAspectPair.uiNSyllables() - 3);
        }
        else
        {
            iStressPos = iSourceStressPos;
        }
    }
    catch (CException& ex)
    {
        return H_ERROR_UNEXPECTED;
    }

    return H_NO_ERROR;

}       //  eFindStressPositionI()

ET_ReturnCode CFormBuilderAspectPair::eApplyVowelModification(bool bIsVariant)
{
    StLexemeProperties stProperties = m_pLexeme->stGetProperties();
    if (!stProperties.bHasAspectPair)
    {
        return H_ERROR_UNEXPECTED;
    }

    CEString& sData = bIsVariant ? stProperties.sAspectPairData : stProperties.sAltAspectPairData;
    if (sData.bIsEmpty())
    {
        return H_ERROR_UNEXPECTED;
    }

    if (!sData.bStartsWith(L"-") || !sData.bEndsWith(L"-") || sData.uiLength() > 4 || sData.uiLength() < 3)
    {
        return H_ERROR_UNEXPECTED;
    }

    sData = sData.sSubstr(1, sData.uiLength() - 2);

//    bool bStressed = false;
//    if (sData.bStartsWith(L"/"))
//    {
//        bStressed = true;
//        sData = sData.sSubstr(1);
//    }

    if (sData.uiLength() != 1)
    {
        return H_ERROR_UNEXPECTED;
    }

    if (sData != L'ё' && sData != L'о' && sData != L'а')
    {
        return H_ERROR_UNEXPECTED;
    }

    CEString& sAspectPair = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
    int& iStressPos = bIsVariant ? m_iAltStressPos : m_iStressPos;
    try
    {
        sAspectPair[iStressPos] = sData[0];
    }
    catch (...)
    {
        return H_ERROR_UNEXPECTED;
    }

    return H_NO_ERROR;

}       //  eApplyVowelModification()

ET_ReturnCode CFormBuilderAspectPair::eSvToNsvTypeII(bool bIsVariant)
{
    ET_ReturnCode eRet = H_NO_ERROR;

    CEString& sOutput = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
    ET_ExtraData eExtraData = bIsVariant ? m_eAltExtraData : m_eExtraData;
    if (ET_ExtraData::WholeWord == eExtraData)
    {
        StLexemeProperties stProperties = m_pLexeme->stGetProperties();
        CEString sData = bIsVariant ? stProperties.sAltAspectPairData : stProperties.sAspectPairData;
        if (!stProperties.bHasAspectPair || sData.bIsEmpty())
        {
            return H_ERROR_UNEXPECTED;
        }

        sOutput = stProperties.sAspectPairData;

        return H_NO_ERROR;
    }

    if (m_sSource.bEndsWith(L"ать"))
    {
        sOutput = m_sSource;
    }
    else if (m_sSource.bEndsWith(L"нуть"))
    {
        sOutput = m_sSource.sRemoveCharsFromEnd(3) + L"ать";
    }
    else
    {
        eRet = eGet1PersonWordForm();
        if (eRet != H_NO_ERROR)
        {
            return eRet;
        }

        if (NULL == m_p1stPersonWordForm)
        {
            return H_ERROR_POINTER;
        }

        if (m_p1stPersonWordForm->sEnding().bEndsWith(L"у"))
        {
            sOutput = m_p1stPersonWordForm->sLemma() + L"ать";
        }
        else
        {
            ASSERT(m_p1stPersonWordForm->sEnding() == L"ю");
            sOutput = m_p1stPersonWordForm->sLemma() + L"ять";
        }
    }

    int& iStressPos = bIsVariant ? m_iAltStressPos : m_iStressPos;
    iStressPos = sOutput.uiGetVowelPos(sOutput.uiNSyllables() - 1);

    return eRet;

}       //  eSvToNsvTypeII()

ET_ReturnCode CFormBuilderAspectPair::eSvToNsvTypeIII(bool bIsVariant)
{
    ET_ReturnCode eRet = H_NO_ERROR;

    CEString& sOutput = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
    ET_ExtraData eExtraData = bIsVariant ? m_eAltExtraData : m_eExtraData;
    if (ET_ExtraData::WholeWord == eExtraData)        
    {
        StLexemeProperties stProperties = m_pLexeme->stGetProperties();
        CEString sData = bIsVariant ? stProperties.sAltAspectPairData : stProperties.sAspectPairData;
        if (!stProperties.bHasAspectPair || sData.bIsEmpty())
        {
            return H_ERROR_UNEXPECTED;
        }

        sOutput = stProperties.sAspectPairData;

        return H_NO_ERROR;
    }

    if (m_sSource.bEndsWith(L"нуть"))
    {
        sOutput = m_sSource.sRemoveCharsFromEnd(3) + L"вать";
    }
    else
    {
        sOutput = m_sSource.sRemoveCharsFromEnd(2) + L"вать";
    }

    int& iStressPos = bIsVariant ? m_iStressPos : m_iAltStressPos;
    iStressPos = sOutput.uiGetVowelPos(sOutput.uiNSyllables() - 1);

    return H_NO_ERROR;

}       //  eSvToNsvTypeIII()

ET_ReturnCode CFormBuilderAspectPair::eNsvToSvTypes1_5_6(bool bIsVariant)
{
    ET_ReturnCode eRet = H_NO_ERROR;
    CEString& sAspectPair = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
    int& iStressPos = bIsVariant ? m_iAltStressPos : m_iStressPos;

    if (NULL == m_pInfWordForm)
    {
        eRet = eGetInfinitiveWordForm();
        if (eRet != H_NO_ERROR)
        {
            return eRet;
        }
    }

    int iInfStressPos = -1;
    eRet = eGetInfinitiveStressPos(iInfStressPos);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    if (iInfStressPos < 0)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iStressedSyllable = -1;
    try
    {
        iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
    }
    catch (CException& ex)
    {
        return H_ERROR_UNEXPECTED;
    }

    CEString sInfinitive = m_pInfWordForm->sWordForm();

    int iDerivationType = 0;
    eRet = eGetNsvDerivationType(m_pInfWordForm, iDerivationType);
    if (eRet != H_NO_ERROR || iDerivationType < -3 || iDerivationType > -1)
    {
        return H_ERROR_UNEXPECTED;
    }

    if (-1 == iDerivationType)      //  unstressed -yvat'/-ivat'
    {
        if (sInfinitive.bEndsWith(L"ывать"))
        {
            sAspectPair = sInfinitive.sRemoveCharsFromEnd(5) + L"ать";
        }
        else
        {
            ASSERT(sInfinitive.bEndsWith(L"ивать"));
            CEString sStem = sInfinitive.sRemoveCharsFromEnd(5);
            if (CEString::bIn(sStem[sStem.uiLength() - 1], CEString(g_szHushers) + CEString(L"кгх")))
            {
                sAspectPair = sStem + L"ать";
            }
            else
            {
                sAspectPair = sStem + L"ять";
            }
        }

        iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable);
        return H_NO_ERROR;

    }           //  unstressed -yvat'/-ivat'

    if (-2 == iDerivationType)
    {
        int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
        ASSERT(iStressedSyllable > 0);
        if (m_pInfWordForm->sWordForm().uiNSyllables()-1 == iStressedSyllable) // end-stressed?
        {
            sAspectPair = m_pInfWordForm->sWordForm();
            iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable-1);
            return H_NO_ERROR;
        }
    }

    if (sInfinitive.bEndsWith(L"вать"))
    {
        int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
        ASSERT(iStressedSyllable > 0);
        if (m_pInfWordForm->sWordForm().uiNSyllables()-1 == iStressedSyllable) // end-stressed?
        {
            sAspectPair = m_pInfWordForm->sWordForm().sRemoveCharsFromEnd(4) + L"ть";
            iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable-1);
            return H_NO_ERROR;
        }
    }

    return H_ERROR_UNEXPECTED;

}       //  eNsvToSvTypes1_5_6()

ET_ReturnCode CFormBuilderAspectPair::eNsvToSvTypes2(bool bIsVariant)
{
    ET_ReturnCode eRet = H_NO_ERROR;
    CEString& sAspectPair = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
    int& iStressPos = bIsVariant ? m_iAltStressPos : m_iStressPos;

    if (NULL == m_pInfWordForm)
    {
        eRet = eGetInfinitiveWordForm();
        if (eRet != H_NO_ERROR)
        {
            return eRet;
        }
    }

    int iInfStressPos = -1;
    eRet = eGetInfinitiveStressPos(iInfStressPos);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    if (iInfStressPos < 0)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iDerivationType = 0;
    eRet = eGetNsvDerivationType(m_pInfWordForm, iDerivationType);
    if (eRet != H_NO_ERROR || iDerivationType < -3 || iDerivationType > -1)
    {
        return H_ERROR_UNEXPECTED;
    }

    if (-1 == iDerivationType)      //  unstressed -yvat'/-ivat'
    {
        CEString sInfinitive = m_pInfWordForm->sWordForm();

        if (sInfinitive.bEndsWith(L"овывать") && !sInfinitive.bEndsWith(L"цовывать"))
        {
            int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
            ASSERT(iStressedSyllable > 0);
            if (m_pInfWordForm->sWordForm().uiNSyllables() - 3 == iStressedSyllable) // -Ovyvat'
            {
                sAspectPair = m_pInfWordForm->sWordForm().sRemoveCharsFromEnd(7);
                sAspectPair += L"овать";
                iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable - 1);
                return H_NO_ERROR;
            }
        }

        if (sInfinitive.bEndsWith(L"цовывать"))
        {
            int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
            ASSERT(iStressedSyllable > 0);
            if (m_pInfWordForm->sWordForm().uiNSyllables() - 3 == iStressedSyllable)
            {
                sAspectPair = m_pInfWordForm->sWordForm().sRemoveCharsFromEnd(8);
                sAspectPair += L"цевать";
                iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable - 1);
                return H_NO_ERROR;
            }
        }

        if (sInfinitive.bEndsWith(L"ёвывать"))
        {
            int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
            ASSERT(iStressedSyllable > 0);
            if (m_pInfWordForm->sWordForm().uiNSyllables() - 3 == iStressedSyllable)
            {
                sAspectPair = m_pInfWordForm->sWordForm().sRemoveCharsFromEnd(7);
                sAspectPair += L"евать";
                iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable - 1);
                return H_NO_ERROR;
            }
        }
    }

    return H_ERROR_UNEXPECTED;

}       //  eNsvToSvTypes2()

ET_ReturnCode CFormBuilderAspectPair::eNsvToSvTypes3(bool bIsVariant)
{
    ET_ReturnCode eRet = H_NO_ERROR;
    CEString& sAspectPair = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
    int& iStressPos = bIsVariant ? m_iAltStressPos : m_iStressPos;

    if (NULL == m_pInfWordForm)
    {
        eRet = eGetInfinitiveWordForm();
        if (eRet != H_NO_ERROR)
        {
            return eRet;
        }
    }

    int iInfStressPos = -1;
    eRet = eGetInfinitiveStressPos(iInfStressPos);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    if (iInfStressPos < 0)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iDerivationType = 0;
    eRet = eGetNsvDerivationType(m_pInfWordForm, iDerivationType);
    if (eRet != H_NO_ERROR || iDerivationType < -3 || iDerivationType > -1)
    {
        return H_ERROR_UNEXPECTED;
    }

    if (-1 == iDerivationType)      //  unstressed -yvat'/-ivat'
    {
        CEString sInfinitive = m_pInfWordForm->sWordForm();
        if (sInfinitive.bEndsWith(L"ивать") || sInfinitive.bEndsWith(L"ывать"))
        {
            try
            {
                int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
                if (iStressedSyllable < m_pInfWordForm->sWordForm().uiNSyllables() - 2)
                {
                    CEString sStem = sInfinitive.sRemoveCharsFromEnd(5);
                    sAspectPair = sStem + L"нуть";
                    iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable);
                }

                return H_NO_ERROR;
            }
            catch (CException& ex)
            {
                return H_ERROR_UNEXPECTED;
            }
        }           //  unstressed -yvat'/-ivat'
    }

    if (-2 == iDerivationType)
    {
        CEString sInfinitive = m_pInfWordForm->sWordForm();
        int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
        if (m_pInfWordForm->sWordForm().uiNSyllables()-1 == iStressedSyllable) // end-stressed?
        {
            CEString sStem = sInfinitive.sRemoveCharsFromEnd(3);
            sAspectPair = sStem + L"нуть";
            iStressedSyllable = m_pInfWordForm->sWordForm().uiNSyllables()-2;
            ASSERT(iStressedSyllable > 0);
            iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable);
            return H_NO_ERROR;
        }
    }

    if (-3 == iDerivationType)
    {
        int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
        ASSERT(iStressedSyllable > 0);
        if (m_pInfWordForm->sWordForm().uiNSyllables() - 1 == iStressedSyllable) // end-stressed?
        {
            sAspectPair = m_pInfWordForm->sWordForm().sRemoveCharsFromEnd(4) + L"ть";
            iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable - 1);
            return H_NO_ERROR;
        }
    }

    return H_ERROR_UNEXPECTED;

}       //  eNsvToSvTypes3()

ET_ReturnCode CFormBuilderAspectPair::eNsvToSvTypes4(bool bIsVariant)
{
    ET_ReturnCode eRet = H_NO_ERROR;
    CEString& sAspectPair = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
    int& iStressPos = bIsVariant ? m_iAltStressPos : m_iStressPos;

    if (NULL == m_pInfWordForm)
    {
        eRet = eGetInfinitiveWordForm();
        if (eRet != H_NO_ERROR)
        {
            return eRet;
        }
    }

    int iInfStressPos = -1;
    eRet = eGetInfinitiveStressPos(iInfStressPos);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    if (iInfStressPos < 0)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iDerivationType = 0;
    eRet = eGetNsvDerivationType(m_pInfWordForm, iDerivationType);
    if (eRet != H_NO_ERROR || iDerivationType < -3 || iDerivationType > -1)
    {
        return H_ERROR_UNEXPECTED;
    }
    
    CEString sInfinitive = m_pInfWordForm->sWordForm();
    int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
    ASSERT(iStressedSyllable >= 0);

    if (-1 == iDerivationType)
    {
        CEString sStem = sInfinitive.sRemoveCharsFromEnd(5);
        sAspectPair = sStem + L"ить";
        iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable);
        return H_NO_ERROR;
    }           //  unstressed -yvat'/-ivat'

    if (-2 == iDerivationType)
    {
        CEString sStem = sInfinitive.sRemoveCharsFromEnd(3);
        sAspectPair = sStem + L"ить";
        iStressedSyllable = m_pInfWordForm->sWordForm().uiNSyllables()-2;
        ASSERT(iStressedSyllable > 0);
        iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable);

        return H_NO_ERROR;
    }

    return H_ERROR_UNEXPECTED;

}       //  eNsvToSvTypes4()

ET_ReturnCode CFormBuilderAspectPair::eNsvToSvTypes7(bool bIsVariant)
{
    ET_ReturnCode eRet = H_NO_ERROR;
    CEString& sAspectPair = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
    int& iStressPos = bIsVariant ? m_iAltStressPos : m_iStressPos;

    if (NULL == m_pInfWordForm)
    {
        eRet = eGetInfinitiveWordForm();
        if (eRet != H_NO_ERROR)
        {
            return eRet;
        }
    }

    int iInfStressPos = -1;
    eRet = eGetInfinitiveStressPos(iInfStressPos);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    if (iInfStressPos < 0)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iDerivationType = 0;
    eRet = eGetNsvDerivationType(m_pInfWordForm, iDerivationType);
    if (eRet != H_NO_ERROR || iDerivationType < -3 || iDerivationType > -1)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
    CEString sInfinitive = m_pInfWordForm->sWordForm();

    if (-1 == iDerivationType)
    {
        if (sInfinitive.bEndsWith(L"зывать"))
        {
            ASSERT(sInfinitive.uiLength() > 6);
            sAspectPair = sInfinitive.sRemoveCharsFromEnd(6) + L"зть";
            iStressPos = iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable);
            return H_NO_ERROR;
        }

        if (sInfinitive.bEndsWith(L"тывать") || sInfinitive.bEndsWith(L"дывать") || sInfinitive.bEndsWith(L"сывать") || sInfinitive.bEndsWith(L"бывать"))
        {
            ASSERT(sInfinitive.uiLength() > 6);
            sAspectPair = sInfinitive.sRemoveCharsFromEnd(6) + L"сть";
            iStressPos = iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable);
            return H_NO_ERROR;
        }
    }

    if (-2 == iDerivationType)
    {
        if (sInfinitive.bEndsWith(L"зать"))
        {
            ASSERT(sInfinitive.uiLength() > 4);
            sAspectPair = sInfinitive.sRemoveCharsFromEnd(4) + L"зть";
            iStressPos = iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable-1);
            return H_NO_ERROR;
        }

        if (sInfinitive.bEndsWith(L"тать") || sInfinitive.bEndsWith(L"дать") || sInfinitive.bEndsWith(L"сать") || sInfinitive.bEndsWith(L"бать"))
        {
            ASSERT(sInfinitive.uiLength() > 4);
            sAspectPair = sInfinitive.sRemoveCharsFromEnd(4) + L"сть";
            iStressPos = iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable-1);
            return H_NO_ERROR;
        }
    }

    return H_ERROR_UNEXPECTED;

}       //  eNsvToSvTypes7()

ET_ReturnCode CFormBuilderAspectPair::eNsvToSvTypes8(bool bIsVariant)
{
    ET_ReturnCode eRet = H_NO_ERROR;
    CEString& sAspectPair = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
    int& iStressPos = bIsVariant ? m_iAltStressPos : m_iStressPos;

    if (NULL == m_pInfWordForm)
    {
        eRet = eGetInfinitiveWordForm();
        if (eRet != H_NO_ERROR)
        {
            return eRet;
        }
    }

    int iInfStressPos = -1;
    eRet = eGetInfinitiveStressPos(iInfStressPos);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    if (iInfStressPos < 0)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iDerivationType = 0;
    eRet = eGetNsvDerivationType(m_pInfWordForm, iDerivationType);
    if (eRet != H_NO_ERROR || iDerivationType < -3 || iDerivationType > -1)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
    CEString sInfinitive = m_pInfWordForm->sWordForm();

    if (-1 == iDerivationType)
    {
        if (sInfinitive.bEndsWith(L"кивать") || sInfinitive.bEndsWith(L"гивать"))
        {
            ASSERT(sInfinitive.uiLength() > 6);
            sAspectPair = sInfinitive.sRemoveCharsFromEnd(6) + L"чь";
            iStressPos = iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable);
            return H_NO_ERROR;
        }
    }

    if (-2 == iDerivationType)
    {
        if (sInfinitive.bEndsWith(L"кать") || sInfinitive.bEndsWith(L"гать"))
        {
            ASSERT(sInfinitive.uiLength() > 4);
            sAspectPair = sInfinitive.sRemoveCharsFromEnd(4) + L"чь";
            iStressPos = iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable-1);
            return H_NO_ERROR;
        }
    }

    return H_ERROR_UNEXPECTED;

}       //  eNsvToSvTypes8()

ET_ReturnCode CFormBuilderAspectPair::eNsvToSvTypes9(bool bIsVariant)
{
    ET_ReturnCode eRet = H_NO_ERROR;
    CEString& sAspectPair = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
    int& iStressPos = bIsVariant ? m_iAltStressPos : m_iStressPos;

    if (NULL == m_pInfWordForm)
    {
        eRet = eGetInfinitiveWordForm();
        if (eRet != H_NO_ERROR)
        {
            return eRet;
        }
    }

    int iInfStressPos = -1;
    eRet = eGetInfinitiveStressPos(iInfStressPos);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    if (iInfStressPos < 0)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iDerivationType = 0;
    eRet = eGetNsvDerivationType(m_pInfWordForm, iDerivationType);
    if (eRet != H_NO_ERROR || iDerivationType < -3 || iDerivationType > -1)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
    CEString sInfinitive = m_pInfWordForm->sWordForm();

    if (-2 == iDerivationType)
    {
        if (sInfinitive.bEndsWith(L"ирать"))
        {
            ASSERT(sInfinitive.uiLength() > 5);
            sAspectPair = sInfinitive.sRemoveCharsFromEnd(5) + L"ереть";
            iStressPos = iStressPos = sAspectPair.uiGetVowelPos(sInfinitive.uiNSyllables()-1);
            return H_NO_ERROR;
        }
    }

    return H_ERROR_UNEXPECTED;

}       //  eNsvToSvTypes9()

ET_ReturnCode CFormBuilderAspectPair::eNsvToSvTypes10(bool bIsVariant)
{
    ET_ReturnCode eRet = H_NO_ERROR;
    CEString& sAspectPair = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
    int& iStressPos = bIsVariant ? m_iAltStressPos : m_iStressPos;

    if (NULL == m_pInfWordForm)
    {
        eRet = eGetInfinitiveWordForm();
        if (eRet != H_NO_ERROR)
        {
            return eRet;
        }
    }

    int iInfStressPos = -1;
    eRet = eGetInfinitiveStressPos(iInfStressPos);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    if (iInfStressPos < 0)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iDerivationType = 0;
    eRet = eGetNsvDerivationType(m_pInfWordForm, iDerivationType);
    if (eRet != H_NO_ERROR || iDerivationType < -3 || iDerivationType > -1)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
    CEString sInfinitive = m_pInfWordForm->sWordForm();

    if (-1 == iDerivationType)
    {
        if (sInfinitive.bEndsWith(L"алывать"))
        {
            ASSERT(sInfinitive.uiLength() > 7);
            sAspectPair = sInfinitive.sRemoveCharsFromEnd(7) + L"олоть";
            iStressPos = iStressPos = sAspectPair.uiGetVowelPos(sInfinitive.uiNSyllables()-1);
            return H_NO_ERROR;
        }
        if (sInfinitive.bEndsWith(L"арывать"))
        {
            ASSERT(sInfinitive.uiLength() > 7);
            sAspectPair = sInfinitive.sRemoveCharsFromEnd(7) + L"ороть";
            iStressPos = iStressPos = sAspectPair.uiGetVowelPos(sInfinitive.uiNSyllables()-1);
            return H_NO_ERROR;
        }
    }

    return H_ERROR_UNEXPECTED;

}       //  eNsvToSvTypes10()

ET_ReturnCode CFormBuilderAspectPair::eNsvToSvTypes14(bool bIsVariant)
{
    ET_ReturnCode eRet = H_NO_ERROR;
    CEString& sAspectPair = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
    int& iStressPos = bIsVariant ? m_iAltStressPos : m_iStressPos;

    if (NULL == m_pInfWordForm)
    {
        eRet = eGetInfinitiveWordForm();
        if (eRet != H_NO_ERROR)
        {
            return eRet;
        }
    }

    int iInfStressPos = -1;
    eRet = eGetInfinitiveStressPos(iInfStressPos);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    if (iInfStressPos < 0)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iDerivationType = 0;
    eRet = eGetNsvDerivationType(m_pInfWordForm, iDerivationType);
    if (eRet != H_NO_ERROR || iDerivationType < -3 || iDerivationType > -1)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
    CEString sInfinitive = m_pInfWordForm->sWordForm();

    if (-2 == iDerivationType)
    {
        if (sInfinitive.bEndsWith(L"имать") || sInfinitive.bEndsWith(L"инать"))
        {
            if (sInfinitive.uiNSyllables()-1 == iStressedSyllable || sInfinitive.uiNSyllables()-2 == iStressedSyllable)
            {
                ASSERT(sInfinitive.uiLength() > 5);
                CEString sStem = sInfinitive.sRemoveCharsFromEnd(5);
                if (CEString::bIn(sStem[sStem.uiLength() - 1], CEString(g_szHushers)))
                {
                    sAspectPair = sStem + L"ать";
                }
                else
                {
                    sAspectPair = sStem + L"ять";
                }
                iStressPos = iStressPos = sAspectPair.uiGetVowelPos(sInfinitive.uiNSyllables()-1);
                return H_NO_ERROR;
            }
        }
    }

    return H_ERROR_UNEXPECTED;

}       //  eNsvToSvTypes14()

ET_ReturnCode CFormBuilderAspectPair::eNsvToSvTypes11_12_13_15_16(bool bIsVariant)
{
    ET_ReturnCode eRet = H_NO_ERROR;
    CEString& sAspectPair = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
    int& iStressPos = bIsVariant ? m_iAltStressPos : m_iStressPos;

    if (NULL == m_pInfWordForm)
    {
        eRet = eGetInfinitiveWordForm();
        if (eRet != H_NO_ERROR)
        {
            return eRet;
        }
    }

    int iInfStressPos = -1;
    eRet = eGetInfinitiveStressPos(iInfStressPos);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    if (iInfStressPos < 0)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iDerivationType = 0;
    eRet = eGetNsvDerivationType(m_pInfWordForm, iDerivationType);
    if (eRet != H_NO_ERROR || iDerivationType < -3 || iDerivationType > -1)
    {
        return H_ERROR_UNEXPECTED;
    }

    int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
    CEString sInfinitive = m_pInfWordForm->sWordForm();

    if (-3 == iDerivationType)
    {
        if (sInfinitive.bEndsWith(L"вать"))
        {
            if (sInfinitive.uiNSyllables()-1 == iStressedSyllable)
            {
                ASSERT(sInfinitive.uiLength() > 3);
                sAspectPair = sInfinitive.sRemoveCharsFromEnd(3) + L"ть";
                iStressPos = iStressPos = sAspectPair.uiGetVowelPos(sInfinitive.uiNSyllables()-1);
                return H_NO_ERROR;
            }
        }
    }
}       //  eNsvToSvTypes11_12_13_15_16()
