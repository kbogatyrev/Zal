#include "stdafx.h"
#include "Analyzer_Helpers.h"

CEndingsTable::CEndingsTable()
{
    m_sStemFinale = m_sLemmaFinale = L"";
    m_iCutRight = m_iLexemeCount = 0;
}

CEndingsTable::~CEndingsTable()
{
    Clear();
}

void CEndingsTable::Clear()
{
    m_vecEndings.clear();
    m_vecGram.clear();
    m_vecStress.clear();
    m_sStemFinale = m_sLemmaFinale = L"";
    m_iCutRight = m_iLexemeCount = 0;
}

int CEndingsTable::iAddEnding (int iHash, CEString sEnding, int iStressPos)
{
    for (int i = 0; i < m_vecEndings.size() && i < m_vecGram.size() && i < m_vecStress.size(); i++)
    {
        if (m_vecEndings[i] == sEnding &&
            m_vecGram[i] == iHash &&
            m_vecStress[i] == iStressPos)
        {
            return 0;      // We don't want to write several identical endings
        }
    }
    m_vecEndings.push_back (sEnding);
    m_vecGram.push_back (iHash);
    m_vecStress.push_back (iStressPos);
    return 0;
}

int CEndingsTable::iAddEnding (long l_Hash, CEString sEnding, int iStressPos)
{
    // TODO: replace int with long in the whole class
    m_vecEndings.push_back (sEnding);
    m_vecGram.push_back ((int)l_Hash);
    m_vecStress.push_back (iStressPos);
    return 0;
}

vector<int> CEndingsTable::vecDbId (CSqlite* pco_dbHandle)
// Tries to find the table in the db, returns -1 if there's no such table.
// If everything goes well, the output vector will contain just 1 element. But who knows what can happen.
{
    vector<int> vecSubtableId, vecEntriesCount, vecResult;
    if (pco_dbHandle == NULL ||
        m_vecEndings.empty() || m_vecGram.empty() || m_vecStress.empty() ||
        m_vecEndings.size() != m_vecGram.size() || m_vecEndings.size() != m_vecStress.size())
    {
        return vecResult;
    }
    
    CEString sQuery, sCount, sGram, sStress;
    int iEntriesCount, iSubtableId;

    vector<CEString>::iterator iter_endings = m_vecEndings.begin();
    vector<int>::iterator iter_gram = m_vecGram.begin();
    vector<int>::iterator iter_stress = m_vecStress.begin();
    sGram = CEString::sToString (*iter_gram);
    sStress = CEString::sToString (*iter_stress);
    sQuery += L"Select * from endings_for_analysis as a0 where ending = \"" 
        + *iter_endings + L"\" and gram_hash = " + sGram 
        + L" and stress_pos = " + sStress;
    ++iter_gram;
    ++iter_endings;
    ++iter_stress;
    if (m_vecEndings.size() > 1)
    {
        for (int i_ = 1;
            iter_endings != m_vecEndings.end();
            ++iter_endings, ++iter_gram, ++iter_stress, ++i_)
        {
            sCount = CEString::sToString (i_);
            sGram = CEString::sToString (*iter_gram);
            sStress = CEString::sToString (*iter_stress);
            sQuery += L" and exists (select * from endings_for_analysis as a" 
                + sCount + L" where ending = \"" + *iter_endings 
                + L"\" and gram_hash = " + sGram 
                + L" and stress_pos = " + sStress
                + L" and a" + sCount + L".subtable_id = a0.subtable_id)";
        }
    }
    pco_dbHandle->PrepareForSelect(sQuery);
    while (pco_dbHandle->bGetRow())
    {
        pco_dbHandle->GetData(1, iSubtableId);
        vecSubtableId.push_back(iSubtableId);
    }
    pco_dbHandle->Finalize();

    // Now that we've learned the ids of possible subtables we must ensure they 
    // don't contain any entries except those specified by the input vectors.
    for (vector<int>::iterator iter_subtable_id = vecSubtableId.begin();
        iter_subtable_id != vecSubtableId.end();
        ++iter_subtable_id)
    {
        sQuery = L"Select entries_count from endings_meta where subtable_id = " 
            + CEString::sToString (*iter_subtable_id);
        pco_dbHandle->PrepareForSelect(sQuery);
        while (pco_dbHandle->bGetRow())
        {
            pco_dbHandle->GetData(0, iEntriesCount);
            vecEntriesCount.push_back(iEntriesCount);
        }
        pco_dbHandle->Finalize();
        if (vecEntriesCount.empty() == false
            && *(vecEntriesCount.begin()) == m_vecGram.size())
        {
            vecResult.push_back(*iter_subtable_id);
            break;
        }
        vecEntriesCount.clear();
    }
    return vecResult;
}

int CEndingsTable::iDbWrite(CSqlite* pco_dbHandle)
// If a table already exists, return its subtable_id.
// If not, write the whole table to the db, then return its subtable_id.
// Return -1 or -2 on error.
{
    if (pco_dbHandle == NULL)
    {
        return -1;
    }
    int i_inserted = 0, iSubtableId;
    vector<int> vec_search_result;
    vec_search_result = vecDbId(pco_dbHandle);
    if (!vec_search_result.empty())
    {
        return *(vec_search_result.begin());
        // TODO: What if it contains more than one element?
    }

    // If we've reached this mark, it means that there's no such table in the db yet.
    // We should get the number of the last table in the db and save our table there.
    vector<CEString>::iterator iter_Ending;
    vector<int>::iterator iter_Gram;
    vector<int>::iterator iter_Stress;
    iSubtableId = pco_dbHandle->iLastID(L"endings_meta") + 1;

    for (iter_Ending = m_vecEndings.begin(),
            iter_Gram = m_vecGram.begin(),
            iter_Stress = m_vecStress.begin();
        (iter_Ending != m_vecEndings.end()) &&
            (iter_Gram != m_vecGram.end()) &&
            (iter_Stress != m_vecStress.end());
        ++iter_Ending, ++iter_Gram, ++iter_Stress, ++i_inserted)
    {
        // For each tuple <ending, grammatical parameters, stress position>,
        // insert it into the endings table.
        pco_dbHandle->PrepareForInsert(L"endings_for_analysis", 4);
        pco_dbHandle->Bind(1, iSubtableId);  // 0-based
        pco_dbHandle->Bind(2, *iter_Ending);
        pco_dbHandle->Bind(3, *iter_Gram);
        pco_dbHandle->Bind(4, *iter_Stress);
        pco_dbHandle->InsertRow();
        pco_dbHandle->Finalize();
    }
    // Now, write auxiliary information to endings_meta
    pco_dbHandle->PrepareForInsert(L"endings_meta", 2);
    pco_dbHandle->Bind(1, iSubtableId);
    pco_dbHandle->Bind(2, i_inserted);
    pco_dbHandle->InsertRow();
    pco_dbHandle->Finalize();
    return iSubtableId;
}

vector<int> CEndingsTable::m_vecFind(CEString str_ending, int iStressPos)
{
    vector<int> vec_i_result;
    if (m_vecEndings.empty() || m_vecGram.empty() || m_vecStress.empty())
    {
        return vec_i_result;
    }
    vector<CEString>::iterator iter_endings;
    vector<int>::iterator iter_stress, iter_hash;
    for (iter_endings = m_vecEndings.begin(),
            iter_stress = m_vecStress.begin(),
            iter_hash = m_vecGram.begin();
         iter_endings != m_vecEndings.end();
         ++iter_endings, ++iter_stress, ++iter_hash)
    {
        if ((iStressPos < -1 || (iStressPos >= -1 && *iter_stress == iStressPos)) &&
            *iter_endings == str_ending)
        {
            vec_i_result.push_back(*iter_hash);
        }
    }
    return vec_i_result;
}

