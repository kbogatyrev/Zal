#include <memory>

#include "Transcriber.h"
#include "SqliteWrapper.h"

using namespace Hlib;

CTranscriber::CTranscriber(const CEString& sDbPath) : m_sDbPath(sDbPath) {}

ET_ReturnCode CTranscriber::eLoadTranscriptionRules()
{
    static const CEString sQuery
    (L"SELECT ti.input_chars, tr.stress, tr.left_contexts, tr.right_contexts, tr.target, tr.morpheme_type, \
    tr.subparadigm, tr.gramm_gender, tr.gramm_number, tr.gramm_case, tr.strength FROM transcription_inputs \
    AS ti INNER JOIN transcription_rules as tr ON ti.id = tr.input_id");

    try
    {
        unique_ptr<CSqlite> spDb = make_unique<CSqlite>();
        if (!spDb)
        {
            return H_ERROR_DB;
        }

        auto uiHandle = spDb->uiPrepareForSelect(sQuery);
        while (spDb->bGetRow())
        {
            CEString sInputs;
            spDb->GetData(0, sInputs);

            CEString sStressType;
            spDb->GetData(1, sStressType);

            CEString sLeftContexts;
            spDb->GetData(2, sLeftContexts);

            CEString sRightContexts;
            spDb->GetData(3, sRightContexts);

            CEString sTarget;
            spDb->GetData(4, sTarget);

            CEString sMorphemeType;
            spDb->GetData(5, sMorphemeType);

            CEString sSubparadigm;
            spDb->GetData(6, sSubparadigm);

            CEString sGender;
            spDb->GetData(7, sGender);

            CEString sNumber;
            spDb->GetData(8, sNumber);

            CEString sCase;
            spDb->GetData(9, sCase);

            CEString sStrength;
            spDb->GetData(9, sStrength);
        }
    }
    catch (CException & ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}       //  eLoadTranscriptionRules()

ET_ReturnCode CTranscriber::eFormatInputs(const CEString& sSource)
{
    //
    //  E.g., "a+a" --> "aa", "aa" --> vector of chars
    //

    if (sSource.uiFind(L"+") != ERelation::ecNotFound)
    {

    }
    else
    {
        for (int iAt = 0; iAt < sSource.uiLength(); ++iAt)
        {
//            m_vecInputs
        }
    }


    return H_NO_ERROR;
}

ET_ReturnCode CTranscriber::eFormatContexts(const CEString& sContext, EC_CONTEXT_TYPE eLeftOrRight)
{

    return H_NO_ERROR;
}