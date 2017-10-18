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
        m_sReflexiveSuffix = m_sSource.sSubstr(m_sSource.uiLength() - 3);
        m_sSource = m_sSource.sRemoveCharsFromEnd(2);
    }

    eRet = m_pLexeme->eGenerateParadigm();

    switch (stProperties.iAspectPairType)
	{
		case -1:
		{
			eRet = eSvToNsvTypeI();
            if (eRet != H_NO_ERROR)
            { 
                return eRet;
            }

            eRet = eFindStressPositionI();
            if (eRet != H_NO_ERROR)
            {
                return eRet;
            }
            
            if (ET_ExtraData::Vowel == m_eExtraData)
            {
                eRet = eApplyVowelModification();
            }

            break;
		}
        case -2:
        {
            eRet = eSvToNsvTypeII();
            break;
        }

        case -3:
        {
            eRet = eSvToNsvTypeIII();
            break;
        }

        case 1:
        case 5:
        case 6:
        {
            eRet = eNsvToSvTypes1_5_6();
            break;
        }
        
        default:
		{}
	}

    m_sAspectPairSource += m_sReflexiveSuffix;

	return eRet;

}       //  eBuild()

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
    CWordForm * pInfinitive = NULL;
    CGramHasher hasherInf(POS_VERB, SUBPARADIGM_INFINITIVE, CASE_UNDEFINED, NUM_UNDEFINED,
        GENDER_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, m_pLexeme->eAspect(),
        m_pLexeme->eIsReflexive());

    CWordForm * pWordForm = NULL;
    ET_ReturnCode eRet = m_pLexeme->eWordFormFromHash(hasherInf.sGramHash(), 0, pWordForm);

    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    if (NULL == pWordForm)
    {
        return H_ERROR_POINTER;
    }

    int iStressPos = -1;
    ET_StressType eType;
    eRet = pWordForm->eGetFirstStressPos(iStressPos, eType);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    while (eType != ET_StressType::STRESS_PRIMARY && H_NO_ERROR == eRet)
    {
        eRet = pWordForm->eGetNextStressPos(iStressPos, eType);
    }

    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    try
    {
        int iStressedSyllable = pWordForm->sWordForm().uiGetSyllableFromVowelPos(iStressPos);
        if (pWordForm->sWordForm().bStartsWith(L"вы") ||
            pWordForm->sWordForm().uiNSyllables() - 1 == iStressedSyllable)
        {
            CEString& sAspectPair = bIsVariant ? m_sAltAspectPairSource : m_sAspectPairSource;
            int& iStressPos = bIsVariant ? m_iAltStressPos : m_iStressPos;
            ASSERT(sAspectPair.bEndsWith(L"ивать") || sAspectPair.bEndsWith(L"ывать"));
            iStressPos = sAspectPair.uiGetVowelPos(sAspectPair.uiNSyllables() - 3);
        }
        else
        {
            iStressPos = iStressPos;
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
    m_iStressPos = sOutput.uiGetVowelPos(sOutput.uiNSyllables() - 1);

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
    m_iStressPos = sOutput.uiGetVowelPos(sOutput.uiNSyllables() - 1);

    return H_NO_ERROR;

}       //  eSvToNsvTypeIII()

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
}

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
}

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

    CEString sInfinitive = m_pInfWordForm->sWordForm();

    if (sInfinitive.bEndsWith(L"ивать") || sInfinitive.bEndsWith(L"ывать"))
    {
        try
        {
            int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
            if (iStressedSyllable < m_pInfWordForm->sWordForm().uiNSyllables() - 2)
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
            }

            iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable);

            return H_NO_ERROR;
        }
        catch (CException& ex)
        {
            return H_ERROR_UNEXPECTED;
        }
    }           //  unstressed -yvat'/-ivat'

    if ((sInfinitive.bEndsWith(L"ать") && !sInfinitive.bEndsWith(L"вать")) || sInfinitive.bEndsWith(L"ять"))
    {
        int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
        ASSERT(iStressedSyllable > 0);
        if (m_pInfWordForm->sWordForm().uiNSyllables()-1 == iStressedSyllable) // end-stressed?
        {
            sAspectPair = m_pInfWordForm->sWordForm();
            iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable-1);
        }
    }

    if (sInfinitive.bEndsWith(L"вать"))
    {
        int iStressedSyllable = m_pInfWordForm->sWordForm().uiGetSyllableFromVowelPos(iInfStressPos);
        ASSERT(iStressedSyllable > 0);
        if (m_pInfWordForm->sWordForm().uiNSyllables() - 1 == iStressedSyllable) // end-stressed?
        {
            sAspectPair = m_pInfWordForm->sWordForm().sRemoveCharsFromEnd(4) + L"ть";
            iStressPos = sAspectPair.uiGetVowelPos(iStressedSyllable - 1);
        }
    }

    return H_NO_ERROR;

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
        }
    }

    return eRet;
}


