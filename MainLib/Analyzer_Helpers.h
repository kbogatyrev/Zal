#include "stdafx.h"
#include "MainLib_i.h"

using namespace std;
using namespace std::tr1;

struct stStemLinks
{
public:
    int iStemId;
    __int64 llLexemeId;
    int iEndingsLink;
    int iStressPos;
    CEString sLemma;
};

struct stInflexion
{
    CEString sEnding;
    int iGram;
    int iStress;

    friend bool operator< (stInflexion& soInfl1, stInflexion& soInfl2);
    friend bool operator> (stInflexion& soInfl1, stInflexion& soInfl2);
    friend bool operator== (stInflexion& soInfl1, stInflexion& soInfl2);
};

class CEndingsTable
{
private:
    bool bSorted;

public:
    vector<stInflexion> m_vecEndings;

    int m_iLexemeCount, m_iCutRight;
    CEString m_sStemFinal, m_sLemmaFinal;
    vector<int> m_vecFind(CEString sEnding, int iStressPos = -2);
    vector<int> vecDbId(CSqlite* pDbHandle); // Tries to find the table in the db, returns -1 if there's no such table
    int iDbId(vector<CEndingsTable>& vecDb);

    CEndingsTable();
    CEndingsTable(const CEndingsTable& Source);
    ~CEndingsTable();
    void Clear();
    void vSort(); // Sorts all inflexions (ordered by ending, then by iStress, then by iGram)
    int iAddEnding(int iHash, CEString sEnding, int iStressPos);
    int iAddEnding(long lHash, CEString sEnding, int iStressPos);
    int iDbWrite(CSqlite* pDbHandle, bool bSearchFirst = true, int iSubtableId = -1); // Write the whole table to the db, return subtable_id
    int iDbWrite(vector<CEndingsTable>& vecDb, bool bSearchFirst = true);
};
