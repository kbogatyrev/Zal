/**

*/

#pragma once

#include "StdAfx.h"

using namespace std;

class  ExtString : public wstring
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
	
	vector<ST_Token> vo_Tokens_;

	vector<wstring> vo_LastTokenizedContents_;
		// ^-- a snapshot of string as of last tokenization; 1st
		// element is the STL string, 2nd element is a concatenation
		// of separators in the order of declaration
		//

public:

	/**
	* @desc Default constructor
	*/
	ExtString();
	
	/**
	* @desc Construct ext. string object using STL strings
	*
	* @param str_text
	*      Out: string contents
	*
	* @param str_break
	*      Out: whitespace char(s)
	*
	* @param str_break
	*      Out: tab char(s)
	*
	* @param str_escape
	*      Out: escape char(s)
	*
	* @param str_punctuation
	*      Out: punctuation marks
	*/
	ExtString (const wstring& str_text,
			   const wstring& str_break = L"",
			   const wstring& str_tab = L"",
			   const wstring& str_escape = L"",
			   const wstring& str_punctuation = L"");

	/**
	* @desc Construct ext. string object using C strings
	*
	* @param qcz_text
	*      Out: string contents
	*
	* @param qcz_break
	*      Out: whitespace char(s)
	*
	* @param qcz_break
	*      Out: tab char(s)
	*
	* @param qcz_escape
	*      Out: escape char(s)
	*
	* @param qcz_punctuation
	*      Out: punctuation marks
	*/
	ExtString (const wchar_t * qcz_text, 
			   const wchar_t * qcz_break = L"",
			   const wchar_t * qcz_tab = L"",
			   const wchar_t * qcz_escape = L"",
			   const wchar_t * qcz_punctuation = L"");

	/**
	* @desc Copy constructor
	*/
	ExtString (const ExtString& extstr);

	/**
	* @desc Non-virtual destructor: this is not a base class
	*/
	~ExtString();

private:
	void v_Null_();
	void v_CopyAttributes (const ExtString&);
	void v_DeepCopy_ (const ExtString&);
	void v_Synchronize_();
	bool b_Tokenize_ (const et_TokenType,  vector<ST_Token>&);
	bool b_ExtractTextTokens_ (vector<ST_Token>&);
	bool b_ExtractSeparators_ (const et_TokenType, 
							   const wstring&, 
							   vector<ST_Token>&);
public:
	/**
	* @desc Assignment operators
	*/
	ExtString& operator = (const ExtString& estr_);
	ExtString& operator = (const wstring& str_);
	ExtString& operator = (const wchar_t * pcchr_);

public:
	/**
	* @desc Convert text to all-lower case
	*
	* @return void
	*/
	void v_ToLower();

	/**
	* @desc Convert text to all-upper case
	*
	* @return void
	*/
	void v_ToUpper();

	/**
	* @desc Capitalize each word
	*
	* @return void
	*/
	void v_ToTitle();

	/**
	* @desc Trim whitespace on the left
	*
	* @return void
	*/
	void v_TrimLeft();

	/**
	* @desc Trim given characters on the left
	*
	* @return void
	*
	* @param str_charsToTrim
	*	Out: characters to trim
	*/
	void v_TrimLeft (const wstring& str_charsToTrim);

	/**
	* @desc Trim whitespace on the right
	*
	* @return void
	*/
	void v_TrimRight();

	/**
	* @desc Trim given characters on the right
	*
	* @return void
	*
	* @param str_charsToTrim
	*	Out: characters to trim
	*/
	void v_TrimRight (const wstring& str_charsToTrim);
	
	/**
	* @desc Trim whitespace on the left and onthe right
	*
	* @return void
	*/
	void v_Trim();
	
	/**
	* @desc Trim given characters on the left and on the right
	*
	* @return void
	*
	* @param str_charsToTrim
	*	Out: characters to trim
	*/
	void v_Trim (const wstring& str_charsToTrim);
	
	/**
	* @desc Lexicographic comparisopn without regard to case
	*
	* @return int (< 0: this sting precedes; = 0: equal; > 0 this string follows)
	*
	* @param str 
	*	Out: String object to compare with
	*/
	int i_CompareNoCase (const wstring& str) const;

	/**
	* @desc Get token of the given type with the given index
	*
	* @return STL string (token text)
	*
	* @param i_at
	*	Out: index
	*
	* @param eo_Type
	*	Out: token type, text assumed
	*/
	wstring str_GetField (const int i_at, 
						  const et_TokenType eo_type = ec_TokenText);

	/**
	* @desc Return token of a given type at the position i_at 
	*       counting from the given string offset
	*
	* @return STL string (token text)
	*
	* @param i_offset
	*	Out: index
	*
	* @param i_at
	*	Out: index
	*
	* @param eo_Type
	*	Out: token type, text assumed
	*/
	wstring str_GetField (const int i_offset,
						  const int i_at, 
						  const et_TokenType eo_type = ec_TokenText);

	/**
	* @desc Return token of a given type at the given index
	*
	* @return Toke struct
	*
	* @param i_at
	*	Out: index
	*
	* @param eo_Type
	*	Out: token type, text assumed
	*/
	const ST_Token& st_GetField (const int i_at,
								 const et_TokenType eo_type = ec_TokenText);

	/**
	* @desc Return token of a given type at the position 
	*       i_at counting from the given string offset
	*
	* @return Token struct
	*
	* @param i_offset
	*	Out: index
	*
	* @param i_at
	*	Out: index
	*
	* @param eo_Type
	*	Out: token type, text assumed
	*/
	const ST_Token& st_GetField (const int i_offset,
								 int i_at,
								 const et_TokenType eo_type = ec_TokenText);

	/**
	* @desc Determine type of a given token
	*
	* @return et_TokenType
	*
	* @param i_at
	*	Out: index
	*/
	et_TokenType eo_GetTokenType (const int i_at);

	/**
	* @desc Determine type of a given token counting from the 
	*       specified string offset
	*
	* @return et_TokenType
	*
	* @param i_offset
	*	Out: index
	*
	* @param i_at
	*	Out: index
	*/
	et_TokenType eo_GetTokenType (const int i_offset, const int i_at);

	/**
	* @desc Return token at position i_at
	*
	* @return Token struct
	*
	* @param i_offset
	*	Out: index
	*
	* @param i_at
	*	Out: index
	*
	* @param eo_Type
	*	Out: token type, text assumed
	*/
	const ST_Token& st_GetToken (const int i_at);

	/**
	* @desc Return token at position i_at counting from the given string offset
	*
	* @return Token struct
	*
	* @param i_offset
	*	Out: index
	*
	* @param i_at
	*	Out: index
	*/
	const ST_Token& st_GetToken (const int i_offset, const int i_at);

	/**
	* @desc Return number of tokens of a given type
	*
	* @return integer
	*
	* @param eo_Type
	*	Out: token type, text assumed
	*
	*/
	int i_GetNumOfFields (const et_TokenType eo_type = ec_TokenText);
	

	/**
	* @desc Return number of tokens of a given type in a given substring
	*
	* @return integer
	*
	* @param i_offset
	*	Out: substring offset
	*
	* @param i_length
	*	Out: substring length
	*
	* @param eo_Type
	*	Out: token type, text assumed
	*
	*/
	int i_GetNumOfFields (const int i_offset, 
						  const int i_length,
						  const ExtString::et_TokenType eo_type = ExtString::ec_TokenText);

	/**
	* @desc same as i_GetNumOfFields
	*
	*/
	int i_NFields (const ExtString::et_TokenType eo_type = ExtString::ec_TokenText);

	/**
	* @desc same as i_GetNumOfFields
	*
	*/
	int i_NFields (const int i_offset, 
				   const int i_length,
				   const ExtString::et_TokenType eo_type = ExtString::ec_TokenText);

	/**
	* @desc Return total number of tokens
	*
	* @return integer
	*
	*/
	int i_GetNumOfTokens();

	/**
	* @desc Same as i_GetNumOfTokens();
	*/
	int i_NTokens();

	/**
	* @desc Return text length of a token of the given type at the given offset
	*
	* @return integer
	*
	* @param i_at
	*	Out: index
	*
	* @param eo_Type
	*	Out: token type, text assumed
	*
	*/
	int const i_GetFieldLength (const int i_at,
								const et_TokenType eo_type = ec_TokenText);

	/**
	* @desc Return text length of a token of the given type at the position i_at 
	*       counting from the given string offset
	*
	* @return integer
	*
	* @param i_offset
	*	Out: index
	*
	* @param i_at
	*	Out: index
	*
	* @param eo_Type
	*	Out: token type, text assumed
	*/
	int const i_GetFieldLength (const int i_offset, 
								const int i_at,
								const ExtString::et_TokenType eo_type = 
													ExtString::ec_TokenText);

	/**
	* @desc Set whitespace character(s)
	*
	* @return void
	*
	* @param str_break
	*	Out: whitespace characters as an STL string
	*
	*/
	void v_SetBreakChars (const wstring& str_break);

	/**
	* @desc Set tab(s)
	*
	* @return void
	*
	* @param str_tab
	*	Out: tab characters as an STL string
	*
	*/
	void v_SetTabChars (const wstring& str_tab);

	/**
	* @desc Set escape character(s)
	*
	* @return void
	*
	* @param str_escape
	*	Out: escape characters as an STL string
	*
	*/
	void v_SetEscChars (const wstring& str_escape);

	/**
	* @desc Set punctuation marks
	*
	* @return void
	*
	* @param str_punctuation
	*	Out: punctuation marks as an STL string
	*
	*/
	void v_SetPunctChars (const wstring& str_punctuation);
};

inline const ExtString::ST_Token& ExtString::st_GetToken (const int i_at)
{	v_Synchronize_(); return vo_Tokens_[i_at]; }
inline int ExtString::i_GetNumOfTokens() 
 { 	v_Synchronize_(); return vo_Tokens_.size(); }
inline int ExtString::i_NTokens() 
 { 	v_Synchronize_(); return vo_Tokens_.size(); }
inline int ExtString::i_NFields (const et_TokenType eo_type)
 { 	v_Synchronize_(); return i_GetNumOfFields (eo_type); }
inline int ExtString::i_NFields (const int i_offset, const int i_length, 
 const et_TokenType eo_type) { v_Synchronize_(); 
 return i_GetNumOfFields (i_offset, i_length, eo_type); }
inline void ExtString::v_SetBreakChars (const wstring& str_break) 
{ str_Break_ = str_break; }
inline void ExtString::v_SetTabChars (const wstring& str_tab) 
{ str_Tab_ = str_tab; }
inline void ExtString::v_SetEscChars (const wstring& str_escape) 
{ str_Escape_ = str_escape; }
inline void ExtString::v_SetPunctChars (const wstring& str_punctuation) 
{ str_Punctuation_ = str_punctuation; }
