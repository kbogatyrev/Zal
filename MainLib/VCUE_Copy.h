// This is a part of the Active Template Library. 
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Active Template Library product.

// VCUE_Copy.h
//
// This file contains ATL-style copy policy classes
// ATL uses copy policy classes in its enumerator and collection interface implementations
//
//////////////////////////////////////////////////////////////////////

#if !defined(_GENERICCOPY_H___36A49827_B15B_11D2_BA63_00C04F8EC847___INCLUDED_)
#define _GENERICCOPY_H___36A49827_B15B_11D2_BA63_00C04F8EC847___INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <AtlCom.h>

namespace VCUE
{
	template <class DestinationType, class SourceType = DestinationType>
	class GenericCopy
	{
	public :
		typedef DestinationType	destination_type;
		typedef SourceType		source_type;

		static void init(destination_type* p)
		{
			_Copy<destination_type>::init(p);
		}
		static void destroy(destination_type* p)
		{
			_Copy<destination_type>::destroy(p);
		}
		static HRESULT copy(destination_type* pTo, const source_type* pFrom)
		{
			return _Copy<destination_type>::copy(pTo, const_cast<source_type*>(pFrom));
		}

	}; // class GenericCopy

	template <>
	class GenericCopy<BSTR>
	{
	public :
		typedef BSTR	destination_type;
		typedef BSTR	source_type;

		static void init(destination_type* p)
		{
			*p = NULL;
		}
		static void destroy(destination_type* p)
		{
			SysFreeString(*p);
			*p = NULL;
		}
		static HRESULT copy(destination_type* pTo, const source_type* pFrom)
		{
			*pTo = SysAllocStringByteLen(reinterpret_cast<const char*>(*pFrom), SysStringByteLen(*pFrom));
			if (*pTo)
				return S_OK;
			else
				return E_OUTOFMEMORY;
		}
	
	}; // class GenericCopy<BSTR>

	template <>
	class GenericCopy<VARIANT, BSTR>
	{
	public :
		typedef VARIANT	destination_type;
		typedef BSTR	source_type;

		static void init(destination_type* p)
		{
			GenericCopy<destination_type>::init(p);
		}
		static void destroy(destination_type* p)
		{
			GenericCopy<destination_type>::destroy(p);
		}
		static HRESULT copy(destination_type* pTo, const source_type* pFrom)
		{
			return CComVariant(*pFrom).Detach(pTo);
		}

	}; // class GenericCopy<VARIANT, BSTR>

	template <>
	class GenericCopy<BSTR, VARIANT>
	{
	public :
		typedef BSTR	destination_type;
		typedef VARIANT	source_type;

		static void init(destination_type* p)
		{
			GenericCopy<destination_type>::init(p);
		}
		static void destroy(destination_type* p)
		{
			GenericCopy<destination_type>::destroy(p);
		}
		static HRESULT copy(destination_type* pTo, const source_type* pFrom)
		{
			VARIANT var = {0};
			HRESULT hr = VariantChangeType(&var, const_cast<VARIANT*>(pFrom), 0, VT_BSTR);
			if (FAILED(hr))
				return hr;

			*pTo = var.bstrVal;
			return S_OK;
			// We deliberately do not free the variant.
		}

	}; // class GenericCopy<BSTR, VARIANT>

    //
    // Added by Sam Leventer
    //
	template <>
	class GenericCopy<VARIANT, CComVariant>
	{
	public :
		typedef VARIANT	destination_type;
		typedef CComVariant	source_type;

		static void init(destination_type* p)
		{
			GenericCopy<destination_type>::init(p);
		}
		static void destroy(destination_type* p)
		{
			GenericCopy<destination_type>::destroy(p);
		}
		static HRESULT copy(destination_type* pTo, const source_type* pFrom)
		{
			return CComVariant(*pFrom).Detach(pTo);
		}

	}; // class GenericCopy<VARIANT, CComVariant>

    //
    // Added by Sam Leventer
    //
	template <>
    class GenericCopy<VARIANT, CAdapt< ATL::CComBSTR> >
	{
	public :
		typedef VARIANT	destination_type;
        typedef CAdapt < ATL::CComBSTR >	source_type;

		static void init(destination_type* p)
		{
			GenericCopy<destination_type>::init(p);
		}
		static void destroy(destination_type* p)
		{
			GenericCopy<destination_type>::destroy(p);
		}
		static HRESULT copy(destination_type* pTo, const source_type* pFrom)
		{
			return CComVariant(*pFrom).Detach(pTo);
		}

	}; // class GenericCopy<VARIANT, BSTR>

	template <class MapType, class DestinationType = MapType::referent_type>
	class MapCopy
	{
	public :
		typedef DestinationType			destination_type;
		typedef typename MapType::value_type		source_type;
		
		typedef MapType					map_type;
		typedef typename MapType::referent_type	pseudosource_type;

		static void init(destination_type* p)
		{
			GenericCopy<destination_type, pseudosource_type>::init(p);
		}
		static void destroy(destination_type* p)
		{
			GenericCopy<destination_type, pseudosource_type>::destroy(p);
		}
		static HRESULT copy(destination_type* pTo, const source_type* pFrom)
		{
			return GenericCopy<destination_type, pseudosource_type>::copy(pTo, &(pFrom->second));
		}

	}; // class MapCopy

    //
    // From VCUE_CopyString.h
    //
	template <>
	class GenericCopy<std::string>
	{
	public :
		typedef std::string	destination_type;
		typedef std::string	source_type;

		static void init(destination_type* )
		{
		}
		static void destroy(destination_type* )
		{
		}
		static HRESULT copy(destination_type* pTo, const source_type* pFrom)
		{
			*pTo = *pFrom;
			return S_OK;
		}
	
	}; // class GenericCopy<std::string>

	template <>
	class GenericCopy<VARIANT, std::string>
	{
	public :
		typedef VARIANT		destination_type;
		typedef std::string	source_type;

		static void init(destination_type* p)
		{
			GenericCopy<destination_type>::init(p);
		}
		static void destroy(destination_type* p)
		{
			GenericCopy<destination_type>::destroy(p);
		}
		static HRESULT copy(destination_type* pTo, const source_type* pFrom)
		{
			return CComVariant(pFrom->c_str()).Detach(pTo);
		}

	}; // class GenericCopy<VARIANT, std::string>

	template <>
	class GenericCopy<BSTR, std::string>
	{
	public :
		typedef BSTR		destination_type;
		typedef std::string	source_type;

		static void init(destination_type* p)
		{
			GenericCopy<destination_type>::init(p);
		}
		static void destroy(destination_type* p)
		{
			GenericCopy<destination_type>::destroy(p);
		}
		static HRESULT copy(destination_type* pTo, const source_type* pFrom)
		{
			*pTo = CComBSTR(pFrom->c_str()).Detach();
			if (*pTo)
				return S_OK;
			else
				return E_OUTOFMEMORY;
		}

	}; // class GenericCopy<BSTR, std::string>

    //
    // 16 bit version of the above
    //
	template <>
	class GenericCopy<std::wstring>
	{
	public :
		typedef std::wstring	destination_type;
		typedef std::wstring	source_type;

		static void init(destination_type* )
		{
		}
		static void destroy(destination_type* )
		{
		}
		static HRESULT copy(destination_type* pTo, const source_type* pFrom)
		{
			*pTo = *pFrom;
			return S_OK;
		}
	
	}; // class GenericCopy<std::wstring>

	template <>
	class GenericCopy<VARIANT, std::wstring>
	{
	public :
		typedef VARIANT		destination_type;
		typedef std::wstring	source_type;

		static void init(destination_type* p)
		{
			GenericCopy<destination_type>::init(p);
		}
		static void destroy(destination_type* p)
		{
			GenericCopy<destination_type>::destroy(p);
		}
		static HRESULT copy(destination_type* pTo, const source_type* pFrom)
		{
			return CComVariant(pFrom->c_str()).Detach(pTo);
		}

	}; // class GenericCopy<VARIANT, std::wstring>

	template <>
	class GenericCopy<BSTR, std::wstring>
	{
	public :
		typedef BSTR		destination_type;
		typedef std::wstring	source_type;

		static void init(destination_type* p)
		{
			GenericCopy<destination_type>::init(p);
		}
		static void destroy(destination_type* p)
		{
			GenericCopy<destination_type>::destroy(p);
		}
		static HRESULT copy(destination_type* pTo, const source_type* pFrom)
		{
			*pTo = CComBSTR(pFrom->c_str()).Detach();
			if (*pTo)
				return S_OK;
			else
				return E_OUTOFMEMORY;
		}

	}; // class GenericCopy<BSTR, std::wstring>


}; // namespace VCUE

#endif // !defined(_GENERICCOPY_H___36A49827_B15B_11D2_BA63_00C04F8EC847___INCLUDED_)
