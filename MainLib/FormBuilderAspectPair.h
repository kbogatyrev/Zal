#ifndef ASPECTPAIRBUILDER_H_INCLUDED
#define ASPECTPAIRBUILDER_H_INCLUDED

namespace Hlib
{
    class CDictionary;
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
        CDictionary * m_pDictionary;
        bool m_bPairedLexemesAcquired;
        bool m_bNoPairedLexemes;
        vector<CLexeme> m_vecPairedLexemes;
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
        bool m_bBuilt;
        bool m_bError;

	public:
        CFormBuilderAspectPair(CDictionary * pDictionary, CLexeme * pLexeme) :
            m_pDictionary(pDictionary), m_bPairedLexemesAcquired(false), m_bNoPairedLexemes(false), m_pLexeme(pLexeme),
            m_p1stPersonWordForm(NULL), m_pInfWordForm(NULL), m_eExtraData(ET_ExtraData::None), m_bBuilt(false), m_bError(false)
		{}
        ET_ReturnCode eBuild();
//        unsigned int uiNPairedLexemes();
//        ET_ReturnCode eGetFirstPairedLexeme(ILexeme *&);
//        ET_ReturnCode eGetNextPairedLexeme(ILexeme *&);
        ET_ReturnCode eGetPairedInfinitive(CEString&, int& iStressPos);
        ET_ReturnCode eGetAltPairedInfinitive(CEString&, int& iStressPos);

    protected:      // helpers
        ET_ReturnCode eFindStressPositionI(bool bIsVariant = false);
        ET_ReturnCode eExtractStressMark(bool bIsVariant, const CEString& sData);
        ET_ReturnCode eGet1PersonWordForm();
        ET_ReturnCode eGetInfinitiveWordForm();
        ET_ReturnCode eGetInfinitiveStressPos(int&);
        ET_ReturnCode eGetNsvDerivationType(CWordForm * pInfinitive, int& iType);
        ET_ReturnCode eGetPairedLexemes();

	protected:      // build
		ET_ReturnCode eSvToNsvTypeI(bool bIsVariant = false);
        ET_ReturnCode eBuildTypeIa(const CEString& sStem, CEString& sOutput);
        ET_ReturnCode eBuildTypeIb(const CEString& sStem, CEString& sOutput);
        ET_ReturnCode eBuildTypeIc(CEString& sOutput);
        ET_ReturnCode eSvToNsvTypeII(bool bIsVariant = false);
        ET_ReturnCode eSvToNsvTypeIII(bool bIsVariant = false);
        ET_ReturnCode eNsvToSvTypes1_5_6(bool bIsVariant = false);
        ET_ReturnCode eNsvToSvTypes2(bool bIsVariant = false);
        ET_ReturnCode eNsvToSvTypes3(bool bIsVariant = false);
        ET_ReturnCode eNsvToSvTypes4(bool bIsVariant = false);
        ET_ReturnCode eNsvToSvTypes7(bool bIsVariant = false);
        ET_ReturnCode eNsvToSvTypes8(bool bIsVariant = false);
        ET_ReturnCode eNsvToSvTypes9(bool bIsVariant = false);
        ET_ReturnCode eNsvToSvTypes10(bool bIsVariant = false);
        ET_ReturnCode eNsvToSvTypes14(bool bIsVariant = false);
        ET_ReturnCode eNsvToSvTypes11_12_13_15_16(bool bIsVariant = false);
        ET_ReturnCode eApplyVowelModification(bool bIsVariant = false);
    };
}

#endif		// ASPECTPAIRBUILDER_H_INCLUDED