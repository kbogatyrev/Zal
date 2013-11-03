#pragma once
#include "resource.h"       // main symbols
//#include "MainLib_i.h"

#include "VCUE_Collection.h"
#include "VCUE_Copy.h"

#include "WordForm.h"
#include "GramHasher.h"

using namespace std;
using namespace std::tr1;

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

namespace WordFormVector
{
// *** This was taken from ATLCollections Sample
// I'm not using some of the typedefs listed below 
// as they only make this convoluted code even more obscure.
// However, the MSFT's typedef names appear in comments. 
//                                                  -- kb

// We always need to provide the following information
//typedef std::vector<CComVariant> ContainerType;
//typedef VARIANT	ExposedType;
//typedef IEnumVARIANT EnumeratorInterface;

// Typically the copy policy can be calculated from the typedefs defined above:
//typedef VCUE::GenericCopy<ExposedType, ContainerType::value_type> CopyType;
// However, we may want to use a different class, as in this case:
//typedef VCUE::MapCopy<ContainerType, ExposedType>				CopyType;
	// (The advantage of MapCopy is that we don't need to provide implementations 
	//  of GenericCopy for all the different pairs of key and value types)

// Now we have all the information we need to fill in the template arguments on the implementation classes

//
// Vector enumerator
//
typedef CComEnumOnSTL <IEnumVARIANT, 
                       &__uuidof(IEnumVARIANT), 
                       VARIANT,
                       VCUE::GenericCopy<VARIANT, 
                                         std::vector<CComVariant>::value_type>,
                       std::vector<CComVariant> > VectorEnumeratorType;

//
// Vector-based container
//
typedef VCUE::ICollectionOnSTLCopyImpl <IFormFinder,
                                        std::vector<CComVariant>, 
                                        VARIANT,
                                        VCUE::GenericCopy<VARIANT, 
                                                          std::vector<CComVariant>::value_type>,
                                        VectorEnumeratorType> VectorCollection;

}   // namespace WordFormVector

using namespace WordFormVector;

class ATL_NO_VTABLE CFormDescriptor :
	public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CFormDescriptor, &CLSID_ZalFormDescriptor>,
    public IDispatchImpl<VectorCollection, &IID_IFormFinder, &LIBID_MainLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
friend class CLexeme;

public:
	CFormDescriptor()
	{
	}

//DECLARE_REGISTRY_RESOURCEID(IDR_LEXEME)
    DECLARE_NO_REGISTRY()

    BEGIN_COM_MAP(CFormDescriptor)
	    COM_INTERFACE_ENTRY(IFormFinder)
//        COM_INTERFACE_ENTRY(IError)
        COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
        return S_OK;
    }

	void FinalRelease()
	{}

// IFormFinder
    STDMETHOD (FindForms) ();
    STDMETHOD (put_PartOfSpeech) (ET_PartOfSpeech);
    STDMETHOD (put_Subparadigm) (ET_Subparadigm);
    STDMETHOD (put_Case) (ET_Case);
    STDMETHOD (put_Number) (ET_Number);
    STDMETHOD (put_Gender) (ET_Gender);
//    STDMETHOD (put_Tense) (ET_Tense);
    STDMETHOD (put_Person) (ET_Person);
    STDMETHOD (put_Animacy) (ET_Animacy);
    STDMETHOD (put_Reflexivity) (ET_Reflexive);
//    STDMETHOD (put_Voice) (ET_Voice);
    STDMETHOD (put_Aspect) (ET_Aspect);

protected:
    void FormsFromHash();

private:
    CGramHasher m_Hasher;
    CLexeme * m_pLexeme;

};

OBJECT_ENTRY_AUTO(__uuidof(ZalFormDescriptor), CFormDescriptor)
