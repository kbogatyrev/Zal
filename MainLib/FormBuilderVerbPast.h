#ifndef FORMBUILDERVERBPAST_H_INCLUDED
#define FORMBUILDERVERBPAST_H_INCLUDED

#include "FormBuilderBaseConj.h"

using namespace std;
using namespace std::tr1;

namespace Hlib
{
    class CFormBuilderPast : public CFormBuilderConj
    {
        struct stStressData
        {
            ET_StressLocation eType;
            int iPosition;

            stStressData (ET_StressLocation eType, int iPos) : eType (eType), iPosition (iPos)
            {}
        };

    public:
        CFormBuilderPast (CLexeme * pLexeme) : CFormBuilderConj (pLexeme, SUBPARADIGM_PAST_TENSE)
        {}

    public:
        ET_ReturnCode eGetEnding (CEString& sLemma, ET_Number eNumber, ET_Gender eGender, CEString& sEnding);

        ET_ReturnCode eGetStressPositions (const CEString& sLemma,
                                           const CEString& sEnding,
                                           ET_Number eNumber, 
                                           ET_Gender eGender, 
                                           vector<int>& vecStressPositions);

        ET_ReturnCode eCreateFormTemplate (const CEString& sLemma,
                                           const CEString& sEnding,
                                           ET_Number eNumber, 
                                           ET_Gender eGender, 
                                           CWordForm *& pWordForm);

        ET_ReturnCode eAssemble (CWordForm * pWordForm, int iStressPos, CEString& sLemma, CEString& sEnding);

        ET_ReturnCode eBuild();

        ET_ReturnCode eBuildIrregular (bool&);

        ET_ReturnCode eRetractStressToPreverb (CWordForm *, bool bIsOptional);

    };

}       //  namespace Hlib

#endif      //  FORMBUILDERVERBPAST_H_INCLUDED
