/**


*/

#include "StdAfx.h"
#include "ExtString.h"

//
// Ctors
//
CT_ExtString::CT_ExtString()
{
	v_Null_();

	str_Break_ = L" ";
	str_Tab_ = L"\t";
	str_Punctuation_ = L".,;:?!\'\"";
}

CT_ExtString::CT_ExtString (const wstring& str_text,
					        const wstring& str_break,
					        const wstring& str_tab,
					        const wstring& str_escape,
					        const wstring& str_punctuation)
{
	v_Null_();

	assign (str_text);
	str_Break_ = str_break;
	str_Tab_ = str_tab;
	str_Escape_ = str_escape;
	str_Punctuation_ = str_punctuation;
}

CT_ExtString::CT_ExtString (const wchar_t * qcz_text,
					        const wchar_t * qcz_break,
					        const wchar_t * qcz_tab,
					        const wchar_t * qcz_escape,
					        const wchar_t * qcz_punctuation)
{
	v_Null_();

	assign (qcz_text);
	str_Break_ = qcz_break;
	str_Tab_ = qcz_tab;
	str_Escape_ = qcz_escape;
	str_Punctuation_ = qcz_punctuation;
}

CT_ExtString::CT_ExtString (const CT_ExtString& estr_)
	: wstring (static_cast <wstring> (estr_)), 
	str_Break_(estr_.str_Break_), 
	str_Tab_(estr_.str_Tab_),
	str_Escape_(estr_.str_Escape_), 
	str_Punctuation_(estr_.str_Punctuation_), 
	vo_Tokens_(estr_.vo_Tokens_),
	vo_LastTokenizedContents_(estr_.vo_LastTokenizedContents_)
{
//	str_Break_.erase();
//	str_Tab_.erase();
//	str_Escape_.erase();
//	str_Punctuation_.erase();

//	vo_LastTokenizedContents_.clear();
//	vo_LastTokenizedContents_.push_back (string(""));
//	vo_LastTokenizedContents_.push_back (string(""));
//	vo_Tokens_.clear();
	
//	str_Break_ = estr_.str_Break_;
//	str_Tab_ = estr_.str_Tab_;
//	str_Escape_ = estr_.str_Escape_;
//	str_Punctuation_ = estr_.str_Punctuation_;

//	vo_LastTokenizedContents_ = estr_.vo_LastTokenizedContents_;
//	vo_Tokens_ = estr_.vo_Tokens_;

}

//
// Dtor
//
CT_ExtString::~CT_ExtString()
{
	v_Null_();
}

void CT_ExtString::v_RegexMatch (const wstring& str_regex)
{
    str_Regex_ = str_regex;
}

//
// Private
//
void CT_ExtString::v_Null_()
{
	this->erase();
	str_Break_.erase();
	str_Tab_.erase();
	str_Escape_.erase();
	str_Punctuation_.erase();

	vo_LastTokenizedContents_.clear();
	vo_LastTokenizedContents_.push_back (wstring(L""));
	vo_LastTokenizedContents_.push_back (wstring(L""));
	vo_Tokens_.clear();
}

void CT_ExtString::v_CopyAttributes (const CT_ExtString& estr)
{
	str_Break_ = estr.str_Break_;
	str_Tab_ = estr.str_Tab_;
	str_Escape_ = estr.str_Escape_;
	str_Punctuation_ = estr.str_Punctuation_;
    str_Regex_ = estr.str_Regex_;
}

//
// Assignment
//
void CT_ExtString::v_DeepCopy_ (const CT_ExtString& estr)
{
	*this = estr;
}

CT_ExtString& CT_ExtString::operator = (const CT_ExtString& estr)
{
	v_Null_();

	assign (estr);
	str_Break_ = estr.str_Break_;
	str_Tab_ = estr.str_Tab_;
	str_Escape_ = estr.str_Escape_;
	str_Punctuation_ = estr.str_Punctuation_;
    str_Regex_ = estr.str_Regex_;

	vo_LastTokenizedContents_ = estr.vo_LastTokenizedContents_;
	vo_Tokens_ = estr.vo_Tokens_;

	return *this;
}

CT_ExtString& CT_ExtString::operator = (const wstring& str)
{
	assign (str);
	return *this;
}

CT_ExtString& CT_ExtString::operator = (const wchar_t * pchr_)
{
	assign (pchr_);
	return *this;
}

//
// --- Private method(s) ---
//
void CT_ExtString::v_Synchronize_()
{
	wstring str_separators = str_Break_ + str_Tab_ + str_Escape_ + str_Punctuation_ + str_Regex_;

	if ((vo_LastTokenizedContents_[0] != static_cast <wstring> (*this)) ||
		(vo_LastTokenizedContents_[1] != str_separators))
	{
		vo_LastTokenizedContents_.clear();
		vo_LastTokenizedContents_.push_back (wstring(L""));
		vo_LastTokenizedContents_.push_back (wstring(L""));

		vo_Tokens_.clear();

		for (int i_at = static_cast <int> (ec_TokenTypeFront);
			 i_at+1 < static_cast <int> (ec_TokenTypeBack);
			 ++i_at)
		{
			b_Tokenize_ (static_cast <et_TokenType>(i_at+1), vo_Tokens_);
		}

        sort (vo_Tokens_.begin(), vo_Tokens_.end());

		vo_LastTokenizedContents_[0] = static_cast <wstring> (*this);
		vo_LastTokenizedContents_[1] = str_separators;

	}

}	//  v_Synchronize_

bool CT_ExtString::b_Tokenize_ (const et_TokenType eo_type, 
							    vector<ST_Token>& vo_tokens)
{
	if (length() == 0)
	{	
        ERROR_LOG (L"Warning: Empty string");
		return true;
	}

    bool b_result = true;
    switch (eo_type)
    {
        case ec_TokenText:
        {
		    b_result =  b_ExtractTextTokens_ (vo_tokens);
            break;
        }
        case ec_TokenSpace:
        {
			b_result = b_ExtractSeparators_ (eo_type, str_Break_, vo_tokens);
            break;
        }
        case ec_TokenTab:
        {
			b_result = b_ExtractSeparators_ (eo_type, str_Tab_, vo_tokens);
            break;
        }
        case ec_TokenPunctuation:
        {
			b_result = b_ExtractSeparators_ (eo_type, str_Punctuation_, vo_tokens);
            break;
        }
        case ec_TokenRegexMatch:
        {
            break;
        }
	    default:
		{
		    ERROR_LOG (L"Bad token type");
			b_result = false;
	    }
    }

	return true;

}  // b_Tokenize_ (...)

bool CT_ExtString::b_ExtractTextTokens_ (vector<ST_Token>& vo_tokens)
{
	wstring::size_type se_start = 0;
	wstring::size_type se_length = 0;

	//
	// Extract text items
	//
	wstring str_separators = str_Break_ + 
							 str_Tab_ + 
							 str_Escape_ + 
							 str_Punctuation_;

	se_start = find_first_not_of (str_separators, 0);
	while (se_start != wstring::npos)
	{
		se_length = find_first_of (str_separators, se_start);
		if (se_length == 0)
		{
			ERROR_LOG (L"Token has a zero length");
            return false;
		}
		ST_Token st_token;
		st_token.eo_TokenType = ec_TokenText;
		st_token.i_Offset = static_cast <int> (se_start);
		st_token.i_Length = (se_length == wstring::npos) 
							 ? static_cast <int> (length()-se_start)
							 : static_cast <int> (se_length-se_start);
		vo_tokens.push_back (st_token);
		if (se_length != wstring::npos)
		{
			se_start = find_first_not_of (str_separators, se_length);
		}
		else
		{
			se_start = se_length;
		}
	}

	return true;

} // b_ExtractTextTokens_ (..)

bool CT_ExtString::b_ExtractSeparators_ (const et_TokenType eo_type,
                                         const wstring& str_matchOn, 
                                         vector<ST_Token>&vo_tokens)
{
	wstring::size_type se_start = find_first_of (str_matchOn);
	wstring::size_type se_length = 0;
	while (se_start != wstring::npos)
	{
		se_length = find_first_not_of (str_matchOn, se_start);
		if (se_length == 0)
		{
			ERROR_LOG (L"Token has a zero length");
            return false;
		}
		ST_Token st_token;
		st_token.eo_TokenType = eo_type;
		st_token.i_Offset = static_cast <int> (se_start);
		st_token.i_Length = (se_length == wstring::npos) 
							 ? static_cast <int> (length()- se_start)
							 : static_cast <int> (se_length - se_start);
		vo_tokens.push_back (st_token);
		if (se_length != wstring::npos)
		{
			se_start = find_first_of (str_matchOn, se_length);
		}
		else
		{
			se_start = se_length;
		}
	}	

	return true;

} // b_ExtractSeparators_ (..)

bool CT_ExtString::b_Regex_()
{
    if (str_Regex_.empty())
    {
        ERROR_LOG (L"Warning: Empty regex");
        return true;
    }

    CAtlRegExp<> co_atlRegex;
    BOOL ui_caseSensitive = TRUE;
    REParseError eo_status = co_atlRegex.Parse (str_Regex_.c_str(), ui_caseSensitive);
    if (REPARSE_ERROR_OK != eo_status)
    {
        return false;
    }

    CAtlREMatchContext<> co_atlContext;
    BOOL ui_ret = co_atlRegex.Match (this->c_str(), &co_atlContext);
    if (!ui_ret) 
    {
        return false;
    }

    for (UINT ui_groupIndex = 0; ui_groupIndex < co_atlContext.m_uNumGroups; ++ui_groupIndex)
    {
        const CAtlREMatchContext<>::RECHAR * sz_start = NULL;
        const CAtlREMatchContext<>::RECHAR * sz_end = NULL;
        co_atlContext.GetMatch (ui_groupIndex, &sz_start, &sz_end);
        vo_Tokens_[ui_groupIndex].eo_TokenType = ec_TokenRegexMatch;
        vo_Tokens_[ui_groupIndex].i_Length = (ptrdiff_t)(sz_end - sz_start);
        vo_Tokens_[ui_groupIndex].i_Offset = (ptrdiff_t) (sz_end - sz_start)/sizeof (wchar_t);
    }

    return true;

}   // b_Regex_()

//
//  --- Public methods ---
//
void CT_ExtString::v_ToLower()
{
	if (empty())
	{
		return;
	}
	v_Synchronize_();
	wchar_t * qcz_data = new wchar_t[length() + 1];
//	copy (qcz_data, length());
    wcscpy_s (qcz_data, this->length(), this->c_str());
	qcz_data[sizeof(wchar_t)*length()] = L'\0';
	for (unsigned int ui_token = 0; ui_token < vo_Tokens_.size(); ++ui_token)
	{
		ST_Token st_token = vo_Tokens_[ui_token];
		if (st_token.eo_TokenType != ec_TokenText)
		{
			continue;
		}
		if (st_token.i_Length < 1)
		{
			continue;
		}
		for (int i_at = 0; i_at < st_token.i_Length; ++i_at)
		{
			int i_pos = sizeof(char) * (st_token.i_Offset+i_at);
			qcz_data[i_pos] = tolower (qcz_data[i_pos]);
		}
		assign (qcz_data);
	}
	delete[] qcz_data;
}

void CT_ExtString::v_ToUpper()
{
	if (empty())
	{
		return;
	}
	v_Synchronize_();
	wchar_t * qcz_data = new wchar_t[length() + 1];
//	copy (qcz_data, length());
    wcscpy_s (qcz_data, this->length(), this->c_str());
	qcz_data[sizeof(wchar_t)*length()] = L'\0';
	for (unsigned int ui_token = 0; ui_token < vo_Tokens_.size(); ++ui_token)
	{
		ST_Token st_token = vo_Tokens_[ui_token];
		if (st_token.eo_TokenType != ec_TokenText)
		{
			continue;
		}
		if (st_token.i_Length < 1)
		{
			continue;
		}
		for (int i_at = 0; i_at < st_token.i_Length; ++i_at)
		{
			int i_pos = sizeof(wchar_t) * (st_token.i_Offset+i_at);
			qcz_data[i_pos] = toupper (qcz_data[i_pos]);
		}
		assign (qcz_data);
	}
	delete[] qcz_data;
}

//
// Capitalize first char of each token
//
void CT_ExtString::v_ToTitle()
{
	if (empty())
	{
		return;
	}

	v_Synchronize_();

	wchar_t * qcz_data = new wchar_t[length() + 1];
//	copy (qcz_data, length());
    wcscpy_s (qcz_data, this->length(), this->c_str());
	qcz_data[length()] = '\0';
	for (unsigned int ui_token = 0; ui_token < vo_Tokens_.size(); ++ui_token)
	{
		ST_Token st_token = vo_Tokens_[ui_token];
		if (st_token.eo_TokenType != ec_TokenText)
		{
			continue;
		}
		if (st_token.i_Length < 1)
		{
			continue;
		}
		qcz_data[st_token.i_Offset] = toupper (qcz_data[st_token.i_Offset]);
		for (int i_at = 1; i_at < st_token.i_Length; ++i_at)
		{
			int i_pos = st_token.i_Offset + i_at;
			qcz_data[i_pos] = tolower (qcz_data[i_pos]);
		}
		assign (qcz_data);
	}
	delete[] qcz_data;
}

//
// By default, we trim whitespace including tabs
//
void CT_ExtString::v_TrimLeft()
{
	if (empty())
	{
		return;
	}

	wstring str_charsToTrim = str_Break_ + str_Tab_;	
	wstring::size_type se_offset = find_first_not_of (str_charsToTrim);	
	erase (0, se_offset);	

}

void CT_ExtString::v_TrimLeft (const wstring& str_charsToTrim)
{
	if (empty())
	{
		return;
	}

	wstring::size_type se_offset = find_first_not_of (str_charsToTrim);	
	erase (0, se_offset);	
}

void CT_ExtString::v_TrimRight()
{
	if (empty())
	{
		return;
	}

	wstring str_charsToTrim = str_Break_ + str_Tab_;
	if (str_charsToTrim.empty())
	{
		return;
	}
	wstring::size_type se_offset = find_last_not_of (str_charsToTrim);
	erase (++se_offset);

}

void CT_ExtString::v_TrimRight (const wstring& str_charsToTrim)
{
	if (empty())
	{
		return;
	}
	
	wstring::size_type se_offset = find_last_not_of (str_charsToTrim);
	erase (++se_offset);
}

void CT_ExtString::v_Trim()
{
	if (empty())
	{
		return;
	}
	v_TrimLeft();
	v_TrimRight();
}

void CT_ExtString::v_Trim (const wstring& str_charsToTrim)
{
	if (empty())
	{
		return;
	}

	v_TrimLeft (str_charsToTrim);
	v_TrimRight (str_charsToTrim);
}

int CT_ExtString::i_CompareNoCase (const wstring& str) const
{
	int result = _wcsicmp (c_str(), str.c_str());

	return result;
}

bool CT_ExtString::b_GetToken (const int i_at, ST_Token& st_token)
{
    v_Synchronize_(); 
    if (i_at >= (int)vo_Tokens_.size())
    {
        ERROR_LOG (L"vo_Tokens_ member index out of range");
        return false;
    }
    return true; 
}

wstring CT_ExtString::str_GetField (const int i_at, 
								    const et_TokenType eo_type)								   
{
	v_Synchronize_();

	if (i_at >= static_cast<int>(vo_Tokens_.size()))
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
        return wstring (L"");
	}

	int i_token = 0;
	unsigned int ui_;
	for (ui_ = 0; (ui_ < vo_Tokens_.size()); ++ui_)
	{
		if (vo_Tokens_[ui_].eo_TokenType == eo_type)
		{
			if (i_token < i_at)
			{
				i_token++;
			}
			else
			{
				break;
			}
		}
	}
	if (ui_ == vo_Tokens_.size())
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
        return wstring (L"");
	}

	ST_Token st_token = vo_Tokens_[ui_];
	wstring str_token = substr (st_token.i_Offset, st_token.i_Length);

	return str_token;
}

wstring CT_ExtString::str_GetField (const int i_offset,
                                    const int i_at, 
                                    const et_TokenType eo_type)
{
	v_Synchronize_();

	if (i_at >= static_cast<int>(vo_Tokens_.size()))
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
        return wstring (L"");
	}

	int i_token = 0;
	unsigned int ui_;
	for (ui_ = 0; (ui_ < vo_Tokens_.size()); ++ui_)
	{
		if ((vo_Tokens_[ui_].eo_TokenType == eo_type) &&
			(vo_Tokens_[ui_].i_Offset >= i_offset))
		{
			if (i_token < i_at)
			{
				i_token++;
			}
			else
			{
				break;
			}
		}
	}
	if (ui_ == vo_Tokens_.size())
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
        return wstring (L"");
	}

	ST_Token st_token = vo_Tokens_[ui_];
	wstring str_token = substr (st_token.i_Offset, st_token.i_Length);

	return str_token;

}   //  str_GetField (...)

bool CT_ExtString::b_GetField (int i_at, 
                               const et_TokenType eo_type, 
                               CT_ExtString::ST_Token& st_token)
{
	v_Synchronize_();

	if (i_at >= static_cast<int>(vo_Tokens_.size()))
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
        return false;
	}

	int i_token = 0;
	unsigned int ui_;
	for (ui_ = 0; (ui_ < vo_Tokens_.size()); ++ui_)
	{
		if (vo_Tokens_[ui_].eo_TokenType == eo_type)
		{
			if (i_token < i_at)
			{
				i_token++;
			}
			else
			{
				break;
			}
		}
	}
	if (ui_ == vo_Tokens_.size())
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
	}

    st_token = vo_Tokens_[ui_];
	return true;

}   //  b_GetField (...)

bool CT_ExtString::b_GetField (const int i_offset,
                               int i_at,
							   const et_TokenType eo_type,
                               CT_ExtString::ST_Token& st_token)
{
	v_Synchronize_();

	if (i_at >= static_cast<int>(vo_Tokens_.size()))
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
        return false;
	}

	int i_token = 0;
	unsigned int ui_;
	for (ui_ = 0; (ui_ < vo_Tokens_.size()); ++ui_)
	{
		if ((vo_Tokens_[ui_].eo_TokenType == eo_type) &&
			(vo_Tokens_[ui_].i_Offset >= i_offset))
		{
			if (i_token < i_at)
			{
				i_token++;
			}
			else
			{
				break;
			}
		}
	}
	if (ui_ == vo_Tokens_.size())
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
        return false;
	}

    st_token = vo_Tokens_[ui_];

	return true;

}   //  b_GetField (...)

CT_ExtString::et_TokenType CT_ExtString::eo_GetTokenType (int i_at)
{
	v_Synchronize_();

	if (i_at >= static_cast<int>(vo_Tokens_.size()))
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
	}

	ST_Token st_token = vo_Tokens_[i_at];

	return st_token.eo_TokenType;
}

CT_ExtString::et_TokenType CT_ExtString::eo_GetTokenType (int i_offset, int i_at)
{
	v_Synchronize_();

	if (i_at >= static_cast<int>(vo_Tokens_.size()))
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
	}

	unsigned int ui_;
	for (ui_ = 0; ui_ < vo_Tokens_.size(); ++ui_)
	{
		if (vo_Tokens_[ui_].i_Offset >= i_offset)
		{
			break;
		}
	}

	if (i_at + ui_ >= vo_Tokens_.size())
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
	}

	ST_Token st_token = vo_Tokens_[i_at+ui_];

	return st_token.eo_TokenType;
}

wstring CT_ExtString::str_GetToken (const int i_at)
{
    v_Synchronize_(); 
    
    ST_Token st_token = vo_Tokens_[i_at]; 
    wstring str_token = substr (st_token.i_Offset, st_token.i_Length);

	return str_token;
}

wstring CT_ExtString::str_GetRegexMatch (const int i_at)
{
    if (str_Regex_.empty())
    {
        ERROR_LOG (L"Warning: Empty regex string");
        return wstring (L"");
    }

	v_Synchronize_();

	if (i_at >= static_cast<int>(vo_Tokens_.size()))
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
        return wstring (L"");
	}

	int i_token = 0;
	unsigned int ui_;
	for (ui_ = 0; (ui_ < vo_Tokens_.size()); ++ui_)
	{
        if (ec_TokenRegexMatch == vo_Tokens_[ui_].eo_TokenType)
		{
			if (i_token < i_at)
			{
				i_token++;
			}
			else
			{
				break;
			}
		}
	}
	if (ui_ == vo_Tokens_.size())
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
        return wstring (L"");
	}

	ST_Token st_token = vo_Tokens_[ui_];
	wstring str_token = substr (st_token.i_Offset, st_token.i_Length);

	return str_token;

}   // str_GetRegexMatch (...)

int CT_ExtString::i_GetNumOfFields (const et_TokenType eo_type)
{
	v_Synchronize_();

	int i_tokens = 0;
	for (unsigned int ui_ = 0; ui_ < vo_Tokens_.size(); ++ ui_)
	{
		if (vo_Tokens_[ui_].eo_TokenType == eo_type)
		{
			++i_tokens;
		}
	}
	
	return i_tokens;
}

int CT_ExtString::i_GetNumOfFields (const int i_offset, 
								    const int i_length,
								    const et_TokenType eo_type)
{
	v_Synchronize_();

	unsigned int ui_;
	for (ui_ = 0; ui_ < vo_Tokens_.size(); ++ui_)
	{
		if (vo_Tokens_[ui_].i_Offset >= i_offset)
		{
			break;
		}
	}
	int i_tokens = 0;
	unsigned int uj_;
	for (uj_ = ui_; 
		 (uj_ < vo_Tokens_.size()) && 
		 (vo_Tokens_[uj_].i_Offset < i_offset + i_length); 
		 ++ uj_)
	{
		if (vo_Tokens_[uj_].eo_TokenType == eo_type)
		{
			++i_tokens;
		}
	}
	
	return i_tokens;
}

int const CT_ExtString::i_GetFieldLength (const int i_at,
									      const et_TokenType eo_type)
{
	v_Synchronize_();

	if (i_at >= static_cast<int>(vo_Tokens_.size()))
	{
//		wcout << endl
//    	 	  << L"*** bool ExtString::i_GetFieldLength: "
//		 	  << L"vo_Tokens_ member index out of range. " 
//		 	  << endl;
		ERROR_LOG (L"vo_Tokens_ member index out of range");
	}

	int i_token = 0;
	unsigned int ui_;
	for (ui_ = 0; (ui_ < vo_Tokens_.size()); ++ui_)
	{
		if (vo_Tokens_[ui_].eo_TokenType == eo_type)
		{
			if (i_token < i_at)
			{
				i_token++;
			}
			else
			{
				break;
			}
		}
	}
	if (ui_ == vo_Tokens_.size())
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
	}

	ST_Token st_token = vo_Tokens_[i_at];
	return st_token.i_Length;
}

int const CT_ExtString::i_GetFieldLength (const int i_offset,
									      const int i_at,
									      const et_TokenType eo_type)
{
	v_Synchronize_();

	if (i_at >= static_cast<int>(vo_Tokens_.size()))
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
	}

	unsigned int ui_;
	for (ui_ = 0; ui_ < vo_Tokens_.size(); ++ui_)
	{
		if (vo_Tokens_[ui_].i_Offset >= i_offset)
		{
			break;
		}
	}
	if (ui_ >= vo_Tokens_.size())
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
	}

	int i_token = 0;
	unsigned int uj_;
	for (uj_ = ui_; (uj_ < vo_Tokens_.size()); ++uj_)
	{
		if (vo_Tokens_[uj_].eo_TokenType == eo_type)
		{
			if (i_token < i_at)
			{
				i_token++;
			}
			else
			{
				break;
			}
		}
	}
	if (uj_ == vo_Tokens_.size())
	{
		ERROR_LOG (L"vo_Tokens_ member index out of range");
	}

	ST_Token st_token = vo_Tokens_[i_at];

	return st_token.i_Length;

}	//  i_GetFieldLength (...)
