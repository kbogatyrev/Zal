#ifndef FORMBUILDERVERBNONFINITE_H_INCLUDED
#define FORMBUILDERVERBNONFINITE_H_INCLUDED

//#include "resource.h"       // main symbols
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "Endings.h"
#include "FormBuilderBaseConj.h"

using namespace std;
using namespace std::tr1;

namespace Hlib
{

    class CFormBuilderNonFinite : public CFormBuilderConj
    {
    public:
        CFormBuilderNonFinite (CLexeme * pLexeme) : 
            CFormBuilderConj (pLexeme, SUBPARADIGM_UNDEFINED)
        {}

    public:
        ET_ReturnCode eBuild();

    private:
        ET_ReturnCode eBuildInfinitive();
        ET_ReturnCode eBuildPresentActiveParticiple();
        ET_ReturnCode eBuildPresentAdverbial();
        ET_ReturnCode eBuildPresentPassiveParticiple();
        ET_ReturnCode eBuildPastActiveParticiple();
        ET_ReturnCode eBuildPastAdverbial();
        ET_ReturnCode eBuildPastPassiveParticiple();

        ET_ReturnCode eGetParticipleStressPos (ET_Subparadigm eSubparadigm, vector<int>& vecPositions);

        int iStressPosFromTemplate (const CWordForm& wfTemplate, const CEString& sLemma, int iAt = 0);

        ET_ReturnCode ePresAdvType13();
        ET_ReturnCode ePresAdvGeneral();

        //
        // Regular derivation off of irregularly derived stem
        //
        ET_ReturnCode eDeriveIrregPresAdverbial();
        ET_ReturnCode eDeriveIrregPresActiveParticiple();
        ET_ReturnCode eDeriveIrregPresPassiveParticiple();

        ET_ReturnCode eDeriveIrregPastActPartAndAdverbial();
    
        ET_ReturnCode eBuildIrregParticipialFormsLong (ET_Subparadigm eSp);
    
        ET_ReturnCode eDeriveIrregPartPresPassShort (ET_Status eStatus);
        ET_ReturnCode eBuildIrregPartPastPassShort (bool& bIsVariant);
        ET_ReturnCode eDeriveIrregPartPastPassShort();

        bool bHasIrregPartPastPassShort();
    };

}       //  namespace Hlib

#endif      //  FORMBUILDERVERBNONFINITE_H_INCLUDED
