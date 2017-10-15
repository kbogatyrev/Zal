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
        m_eExtraData = ExtraData::Vowel;
    }
    else if (stProperties.sAspectPairData.bStartsWith(L"-"))
    {
        m_eExtraData = ExtraData::Suffix;
    }
    else
    {
        m_eExtraData = ExtraData::WholeWord;
    }

	ET_ReturnCode eRet = H_NO_ERROR;
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }
    
    eRet = m_pLexeme->eGenerateParadigm();

    switch (stProperties.iAspectPairType)
	{
		case -1:
		{
			eRet = eSvToNsvTypeI();
			break;
		}

		default:
		{}
	}

	return H_NO_ERROR;

}       //  eBuild()

ET_ReturnCode CFormBuilderAspectPair::eSvToNsvTypeI()
{
    ET_ReturnCode eRet = H_NO_ERROR;

    CEString sSource = m_pLexeme->sSourceForm();

    if (sSource.bEndsWith(L"ать") || sSource.bEndsWith(L"оть"))
    {
        CEString sStem = sSource.sRemoveCharsFromEnd(3);
        eRet = eBuildTypeIa(sStem);
        return eRet;
    }
    else if (sSource.bEndsWith(L"нуть") && !sSource.bEndsWith(L"ьнуть"))
    {
        CEString sStem = sSource.sRemoveCharsFromEnd(4);
        eRet = eBuildTypeIa(sStem);
        return eRet;
    }

    if (sSource.bEndsWith(L"ять"))
    {
        CEString sStem = sSource.sRemoveCharsFromEnd(3);
        eRet = eBuildTypeIb(sStem);
        return eRet;
    }
    else if (sSource.bEndsWith(L"ьнуть"))
    {
        CEString sStem = sSource.sRemoveCharsFromEnd(5);
        eRet = eBuildTypeIb(sStem);
        return eRet;
    }

    // All other stem types:
    eRet = eBuildTypeIc();

    return eRet;

}       // eSvToNsvTypeI()

ET_ReturnCode CFormBuilderAspectPair::eBuildTypeIa(const CEString& sStem)
{
    if (CEString::bIn(sStem[sStem.uiLength() - 1], CEString(g_szHushers) + CEString(L"кгх")))
    {
        m_sAspectPairSource = sStem + L"ивать";
    }
    else
    {
        m_sAspectPairSource = sStem + L"ывать";
    }

    return H_NO_ERROR;
}

ET_ReturnCode CFormBuilderAspectPair::eBuildTypeIb(const CEString& sStem)
{
    m_sAspectPairSource = sStem + L"ивать";
    return H_NO_ERROR;
}

ET_ReturnCode CFormBuilderAspectPair::eBuildTypeIc()
{
    CWordForm * p1stPersonSgFuture = NULL;
    CGramHasher hasher1Sg(POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG,
        GENDER_UNDEFINED, PERSON_1, ANIM_UNDEFINED, m_pLexeme->eAspect(),
        m_pLexeme->eIsReflexive());

    CWordForm * pWordForm = NULL;
    ET_ReturnCode eRet = m_pLexeme->eWordFormFromHash(hasher1Sg.sGramHash(), 0, pWordForm);

    if (L"у" == pWordForm->sEnding())
    {
        if (pWordForm->sLemma().bEndsWith(CEString(g_szHushers) + CEString(L"кгх")))
        {
            m_sAspectPairSource = pWordForm->sLemma() + L"ивать";
        }
        else
        {
            m_sAspectPairSource = pWordForm->sLemma() + L"ывать";
        }
    }

    return H_NO_ERROR;
}
