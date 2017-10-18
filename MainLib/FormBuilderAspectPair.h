#ifndef ASPECTPAIRBUILDER_H_INCLUDED
#define ASPECTPAIRBUILDER_H_INCLUDED

namespace Hlib
{
    class CLexeme;

	class CFormBuilderAspectPair
	{
    protected:
        enum class ET_ExtraData
        {
            None,
            Vowel,
            Suffix,
            WholeWord
        };

    protected:
		CLexeme * m_pLexeme;
        CWordForm * m_p1stPersonWordForm;
        CWordForm * m_pInfWordForm;
        ET_ExtraData m_eExtraData;
        ET_ExtraData m_eAltExtraData;
        CEString m_sComment;
        CEString m_sSource;
        CEString m_sReflexiveSuffix;
        CEString m_sAspectPairSource;
        CEString m_sAltAspectPairSource;
        int m_iStressPos;
        int m_iAltStressPos;

	public:
		CFormBuilderAspectPair(CLexeme * pLexeme) : 
            m_pLexeme(pLexeme), m_p1stPersonWordForm(NULL), m_pInfWordForm(NULL), m_eExtraData(ET_ExtraData::None)
		{}

	protected:
		ET_ReturnCode eBuild();
		ET_ReturnCode eSvToNsvTypeI(bool bIsVariant = false);
        ET_ReturnCode eBuildTypeIa(const CEString& sStem, CEString& sOutput);
        ET_ReturnCode eBuildTypeIb(const CEString& sStem, CEString& sOutput);
        ET_ReturnCode eBuildTypeIc(CEString& sOutput);
        ET_ReturnCode eSvToNsvTypeII(bool bIsVariant = false);
        ET_ReturnCode eSvToNsvTypeIII(bool bIsVariant = false);
        ET_ReturnCode eNsvToSvTypes1_5_6(bool bIsVariant = false);
        ET_ReturnCode eNsvToSvTypes2(bool bIsVariant = false);
        ET_ReturnCode eFindStressPositionI(bool bIsVariant = false);
        ET_ReturnCode eApplyVowelModification(bool bIsVariant = false);
		ET_ReturnCode eGetPairedLexeme();
        ET_ReturnCode eGet1PersonWordForm();
        ET_ReturnCode eGetInfinitiveWordForm();
        ET_ReturnCode eGetInfinitiveStressPos(int&);
    };
}


#endif		// ASPECTPAIRBUILDER_H_INCLUDED