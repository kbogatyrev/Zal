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
        ET_ExtraData m_eExtraData;
        CEString m_sSource;
        CEString m_sReflexiveSuffix;
        CEString m_sAspectPairSource;
        int m_iStressPos;

	public:
		CFormBuilderAspectPair(CLexeme * pLexeme) : m_pLexeme(pLexeme), m_p1stPersonWordForm(NULL), m_eExtraData(ET_ExtraData::None)
		{}

	protected:
		ET_ReturnCode eBuild();
		ET_ReturnCode eSvToNsvTypeI();
        ET_ReturnCode eBuildTypeIa(const CEString& sStem);
        ET_ReturnCode eBuildTypeIb(const CEString& sStem);
        ET_ReturnCode eBuildTypeIc();
        ET_ReturnCode eSvToNsvTypeII();
        ET_ReturnCode eSvToNsvTypeIII();
        ET_ReturnCode eFindStressPositionI();
        ET_ReturnCode eApplyVowelModification();
		ET_ReturnCode eGetPairedLexeme();
        ET_ReturnCode eGet1PersonWordForm();

    protected:

	};

}


#endif		// ASPECTPAIRBUILDER_H_INCLUDED