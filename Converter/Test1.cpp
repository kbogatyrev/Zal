#include <msclr\marshal.h>
#include <string>

#using <mscorlib.dll>
using namespace msclr::interop;
using namespace std;
using namespace System;
using namespace DBHandlerLib;

#pragma managed

public ref class CT_ConversionLibTest
{

public:
    CT_ConversionLibTest() {};
    ~CT_ConversionLibTest() {};
    int test();
    int testInsert (String ^ dnstr_);
    int testStringConversion (String ^ dnstr_);

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

int CT_ConversionLibTest::testInsert (String ^ dnstr_)
{
    String ^ dnstr_Insert = L"INSERT INTO Example VALUES ('";
    dnstr_Insert += dnstr_;
    dnstr_Insert += L"', 'test')";
    po_sqlite->Exec (dnstr_Insert);

	return 0;
}

int CT_ConversionLibTest::testStringConversion (String ^ dnstr_)
{
    //
    // Method 1:
    //
    marshal_context ^ co_context = gcnew marshal_context();
    const wchar_t * sz_result;
    sz_result = co_context->marshal_as<const wchar_t *> (dnstr_);
    wstring wstr_(sz_result);
    delete co_context;

    //
    // Method2:
    //
//    CString cstr_ (dotNetStr_);

	return 0;
}