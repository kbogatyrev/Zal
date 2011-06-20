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

class CEndingsTable
{
public:
    vector<CEString> m_vecEndings;
    vector<int> m_vecGram;
    vector<int> m_vecStress;

    int m_iLexemeCount, m_iCutRight;
    CEString m_sStemFinal, m_sLemmaFinal;
    vector<int> m_vecFind (CEString sEnding, int iStressPos = -2);
    vector<int> vecDbId (CSqlite* pDbHandle); // Tries to find the table in the db, returns -1 if there's no such table

    CEndingsTable();
    ~CEndingsTable();
    void Clear();
    int iAddEnding (int iHash, CEString sEnding, int iStressPos);
    int iAddEnding (long lHash, CEString sEnding, int iStressPos);
    int iDbWrite (CSqlite* pDbHandle); // Write the whole table to the db, return subtable_id
};
