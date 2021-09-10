#pragma once

#include "EncodingBase.h"

namespace Core
{
	namespace Text
	{
		class ErrorEncoding : public EncodingBase
		{
		public:
			ErrorEncoding();
			virtual ~ErrorEncoding();

			virtual unsigned int GetCharCount( const wchar_t* input, unsigned int count ) const;
			virtual unsigned int GetCharCount( const std::vector<wchar_t>& input ) const;
			virtual unsigned int GetCharCount( const std::wstring& input ) const;

			virtual void GetChars( const wchar_t* input, unsigned int inputCount, char* output, unsigned int outputCount ) const;
			virtual void GetChars( const std::vector<wchar_t>& input, std::vector<char>& output ) const;
			virtual void GetChars( const std::vector<wchar_t>& input, std::string& output ) const;
			virtual void GetChars( const std::wstring& input, std::vector<char>& output ) const;
			virtual void GetChars( const std::wstring& input, std::string& output ) const;

			virtual unsigned int GetWideCharCount( const char* input, unsigned int count ) const;
			virtual unsigned int GetWideCharCount( const std::vector<char>& input ) const;
			virtual unsigned int GetWideCharCount( const std::string& input ) const;

			virtual void GetWideChars( const char* input, unsigned int inputCount, wchar_t* output, unsigned outputCount ) const;
			virtual void GetWideChars( const std::vector<char>& input, std::vector<wchar_t>& output ) const;
			virtual void GetWideChars( const std::vector<char>& input, std::wstring& output ) const;
			virtual void GetWideChars( const std::string& input, std::vector<wchar_t>& output ) const;
			virtual void GetWideChars( const std::string& input, std::wstring& output ) const;

			virtual unsigned int GetMaxCharCount( unsigned int inputCount ) const;
			virtual unsigned int GetMaxWideCharCount( unsigned int inputCount ) const;

			virtual unsigned int GetCharUnitSize( const char& input ) const;
		};
	}
}