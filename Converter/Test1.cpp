#using <mscorlib.dll>

using namespace System;
using namespace DBHandlerLib;

#pragma managed

namespace Conv
{
int TestProc (String ^ str_, String ^ str_result)
{
    str_result = L"result";
    return -99;
}
}

public ref class CT_ConversionLibTest
{

public:
    CT_ConversionLibTest() {};
    ~CT_ConversionLibTest() {};
    int test();
    int testInsert (String ^ str_);

private:
    SqliteWrapperClass ^ po_sqlite;

};

int CT_ConversionLibTest::test()
{
    po_sqlite = gcnew SqliteWrapperClass();    
    po_sqlite->Open (gcnew String (L"test.db")); 
    po_sqlite->Exec (gcnew String (L"CREATE TABLE Example (Key TEXT, Description TEXT)"));

	return 0;
}

int CT_ConversionLibTest::testInsert (String ^ str_)
{
    String ^ sInsert = L"INSERT INTO Example VALUES ('";
    sInsert += str_;
    sInsert += L"', 'test')";
    po_sqlite->Exec (sInsert);

	return 0;
}
