#pragma once

#include <string>
#include <vector>
#include <windows.h>

#include "../CodePage/CodePage.h"

namespace Core
{
	namespace Text
	{
		class Encoding;
		class EncodingBase
		{
			friend Encoding;

		protected:
			EncodingBase( unsigned int codePage, const std::wstring& encodingName,  bool m_isSingleByte );
			virtual ~EncodingBase();

		public:
			virtual unsigned int GetCharCount( const wchar_t* input, unsigned int count ) const;
			virtual void GetChars( const wchar_t* input, unsigned int inputCount, char* output, unsigned int outputCount ) const;
			virtual unsigned int GetWideCharCount( const char* input, unsigned int count ) const;
			virtual void GetWideChars( const char* input, unsigned int inputCount, wchar_t* output, unsigned outputCount ) const;

			virtual unsigned int GetMaxCharCount( unsigned int inputCount ) const = 0;
			virtual unsigned int GetMaxWideCharCount( unsigned int inputCount ) const = 0;

			virtual unsigned int GetCharUnitSize( const char& input ) const = 0;
		
			unsigned int GetCodePage() const { return m_codePage; }
			const std::wstring& GetEncodingName() const { return m_encodingName; }
			const std::vector<unsigned char>& GetPreamble() const { return m_preamble; }

			const std::string& GetSpaceToken() const { return m_spaceToken; }
			const std::string& GetTabToken() const { return m_tabToken; }
			const std::string& GetCarriageReturnToken() const { return m_carriageReturnToken; }
			const std::string& GetNextLineToken() const { return m_nextLineToken; }

			bool IsSingleByte() const { return m_isSingleByte; } 
			bool IsValid() const { return m_isValid; }

		protected:
			unsigned int m_codePage;
			std::wstring m_encodingName;
			std::vector<unsigned char> m_preamble;
			std::string m_spaceToken;
			std::string m_tabToken;
			std::string m_carriageReturnToken;
			std::string m_nextLineToken;
			bool m_isSingleByte;
			bool m_isValid;
		};
	}
}