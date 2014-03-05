#ifndef FORMBUILDERADJLONG_H_INCLUDED
#define FORMBUILDERADJLONG_H_INCLUDED

#include "WordForm.h"
#include "FormBuilderBaseDecl.h"

using namespace std;
using namespace std::tr1;

namespace Hlib
{

class CFormBuilderLongAdj : public CFormBuilderBaseDecl
{
public:
    CFormBuilderLongAdj (CLexeme * pLexeme,
                         const CEString& sLemma,
                         ET_AccentType eAccentType, 
                         ET_Subparadigm eSubparadigm,
                         int iStemStressPos = -1,
                         ET_Status eStatus = STATUS_COMMON,
                         bool bIrregular = false) 
        : CFormBuilderBaseDecl (pLexeme, SUBPARADIGM_LONG_ADJ),
          m_sLemma (sLemma),
          m_eAccentType (eAccentType),
          m_eSubparadigm (eSubparadigm),
          m_iStemStressPos (iStemStressPos),
          m_eStatus (eStatus),
          m_bIrregular (bIrregular)
    {}

protected:
    int iGramHash (ET_PartOfSpeech ePos, ET_Subparadigm eSubparadigm, ET_Case eCase, 
                    ET_Number eNum, ET_Gender eGender, ET_Animacy eAnimacy, 
                    ET_Reflexive eRefl);

    int iGramHashNSgMLong();
    ET_ReturnCode eGetStressPositions (const CEString&, ET_StressLocation, vector<int>& vecStressPos);
    ET_ReturnCode eCreateFormTemplate (ET_Gender, ET_Number, ET_Case, ET_Animacy, const CEString& sEnding, __int64 llEndingKey, CWordForm *&); 
    ET_ReturnCode eHandleCommonDeviations (CWordForm *);

public:
    ET_ReturnCode eBuild();
    ET_ReturnCode eBuildParticiple();

private:
    CEString m_sLemma;
    ET_AccentType m_eAccentType;
    ET_Subparadigm m_eSubparadigm;
    int m_iStemStressPos;        // participles only
    ET_Status m_eStatus;
    bool m_bIrregular;
};

}   // namespace Hlib

#endif  //  FORMBUILDERADJLONG_H_INCLUDED