/**

*/

#pragma once

#include "StdAfx.h"
#include "atlrx.h"

using namespace std;

class  CT_ExtString : public wstring
{
public:

	//
	// Helper objects
	//
	enum et_TokenType
	{
		ec_TokenTypeFront	= 10001,
		ec_TokenText,				 // single word
		ec_TokenSpace,				 // sequence of break chars
		ec_TokenTab,				 // sequence of tabs
		ec_TokenPunctuation,		 // sequence of punct. marks i.e. ; or ?!...
        ec_TokenRegexMatch,
		ec_TokenTypeBack	= ec_TokenPunctuation + 1
	};

	struct ST_Token
	{
		et_TokenType eo_TokenType;
		int i_Offset;
		int i_Length;

    	ST_Token()
	    {
		    eo_TokenType = ec_TokenTypeFront;
		    i_Offset = -1;
		    i_Length = -1;
	    }

	    //
	    // These operators are needed so we can sort tokens:
	    //
	    bool operator < (const ST_Token& st_token) const
	    {
		    return i_Offset < st_token.i_Offset;
	    }

	    bool operator == (const ST_Token& st_token) const
	    {
		    bool b_type = (eo_TokenType == st_token.eo_TokenType);
		    bool b_offset = (i_Offset == st_token.i_Offset);
		    bool b_length = (i_Length == st_token.i_Length);
	
		    return (b_type && b_offset && b_length);
	    }

    };

private:
	//
	// Non-public data members:
	//
	wstring str_Break_;
	wstring str_Tab_;
	wstring str_Escape_;
	wstring str_Punctuation_;
    wstring str_Regex_;
	
	vector<ST_Token> vo_Tokens_;

	vector<wstring> vo_LastTokenizedContents_;
		// ^-- a snapshot of string as of last tokenization; 1st
		// element is the STL string, 2nd element is a concatenation
		// of separators in the order of declaration
		//

public:

	CT_ExtString();
	
	CT_ExtString (const wstring& str_text,
			      const wstring& str_break = L"",
			      const wstring& str_tab = L"",
			      const wstring& str_escape = L"",
			      const wstring& str_punctuation = L"");

	CT_ExtString (const wchar_t * qcz_text, 
			      const wchar_t * qcz_break = L"",
			      const wchar_t * qcz_tab = L"",
			      const wchar_t * qcz_escape = L"",
			      const wchar_t * qcz_punctuation = L"");

	CT_ExtString (const CT_ExtString& extstr);

	~CT_ExtString();

private:
	void v_Null_();
	void v_CopyAttributes (const CT_ExtString&);
	void v_DeepCopy_ (const CT_ExtString&);
	void v_Synchronize_();
	bool b_Tokenize_ (const et_TokenType,  vector<ST_Token>&);
	bool b_ExtractTextTokens_ (vector<ST_Token>&);
	bool b_ExtractSeparators_ (const et_TokenType, 
							   const wstring&, 
							   vector<ST_Token>&);
    bool b_Regex_();

public:
	CT_ExtString& operator = (const CT_ExtString& estr_);
	CT_ExtString& operator = (const wstring& str_);
	CT_ExtString& operator = (const wchar_t * pcchr_);

public:
    void v_RegexMatch (const wstring& str_regex);

	void v_ToLower();
	void v_ToUpper();
	void v_ToTitle();

    void v_TrimLeft();
	void v_TrimLeft (const wstring& str_charsToTrim);
	void v_TrimRight();
	void v_TrimRight (const wstring& str_charsToTrim);
	void v_Trim();
	void v_Trim (const wstring& str_charsToTrim);

    int i_CompareNoCase (const wstring& str) const;

    wstring str_GetField (const int i_at, 
						  const et_TokenType eo_type = ec_TokenText);
	wstring str_GetField (const int i_offset,
						  const int i_at, 
						  const et_TokenType eo_type = ec_TokenText);

    bool b_GetField (const int i_at,
                     const et_TokenType eo_type,
                     ST_Token& );
	bool b_GetField (const int i_offset,
			         int i_at,
					 const et_TokenType eo_type,
                     ST_Token& );

    et_TokenType eo_GetTokenType (const int i_at);
	et_TokenType eo_GetTokenType (const int i_offset, const int i_at);

    bool b_GetToken (const int i_at, ST_Token&);
	bool b_GetToken (const int i_offset, const int i_at, ST_Token&);

    wstring str_GetToken (const int i_at);

    wstring str_GetRegexMatch (const int i_at);

    int i_GetNumOfFields (const et_TokenType eo_type = ec_TokenText);
	int i_GetNumOfFields (const int i_offset, 
						  const int i_length,
						  const CT_ExtString::et_TokenType eo_type = CT_ExtString::ec_TokenText);
	
    int i_NFields (const CT_ExtString::et_TokenType eo_type = CT_ExtString::ec_TokenText)
    { 	
        v_Synchronize_(); 
        return i_GetNumOfFields (eo_type); 
    }

	int i_NFields (const int i_offset, 
				   const int i_length,
				   const CT_ExtString::et_TokenType eo_type = CT_ExtString::ec_TokenText)
    { 
        v_Synchronize_(); 
        return i_GetNumOfFields (i_offset, i_length, eo_type); 
    }
	
    int i_GetNumOfTokens()
    { 	
        v_Synchronize_(); 
        return vo_Tokens_.size(); 
    }
	
    int i_NTokens()
    { 	
        v_Synchronize_(); 
        return vo_Tokens_.size(); 
    }

    int i_GetNumOfRegexMatches()
    {
        i_NRegexMatches();
    }

    int i_NRegexMatches()
    {
        v_Synchronize_(); 
        return i_GetNumOfFields (ec_TokenRegexMatch); 
    }

	int const i_GetFieldLength (const int i_at,
								const et_TokenType eo_type = ec_TokenText);
	int const i_GetFieldLength (const int i_offset, 
								const int i_at,
								const CT_ExtString::et_TokenType eo_type = 
                                        CT_ExtString::ec_TokenText);
	
    void v_SetBreakChars (const wstring& str_break)
    { 
        str_Break_ = str_break; 
    }

	void v_SetTabChars (const wstring& str_tab)
    {
        str_Tab_ = str_tab; 
    }

	void v_SetEscChars (const wstring& str_escape)
    {
        str_Escape_ = str_escape; 
    }

	void v_SetPunctChars (const wstring& str_punctuation)
    {
        str_Punctuation_ = str_punctuation; 
    }
};
