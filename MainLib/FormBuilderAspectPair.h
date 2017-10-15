#ifndef ASPECTPAIRBUILDER_H_INCLUDED
#define ASPECTPAIRBUILDER_H_INCLUDED

namespace Hlib
{
    class CLexeme;

	class CFormBuilderAspectPair
	{
    protected:
        enum class ExtraData
        {
            None,
            Vowel,
            Suffix,
            WholeWord
        };

    protected:
		CLexeme * m_pLexeme;
        ExtraData m_eExtraData;
        CEString m_sAspectPairSource;
        int m_iStressPos;

	public:
		CFormBuilderAspectPair(CLexeme * pLexeme) : m_pLexeme(pLexeme), m_eExtraData(ExtraData::None)
		{}

	protected:
		ET_ReturnCode eBuild();
		ET_ReturnCode eSvToNsvTypeI();
        ET_ReturnCode eBuildTypeIa(const CEString& sStem);
        ET_ReturnCode eBuildTypeIb(const CEString& sStem);
        ET_ReturnCode eBuildTypeIc();
		ET_ReturnCode eGetPairedLexeme();

    protected:

	};

}


#endif		// ASPECTPAIRBUILDER_H_INCLUDED