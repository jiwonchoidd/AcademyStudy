#pragma once

#include <string>
#include <vector>
#include <map>

namespace Core
{
	namespace Text
	{
		class EncodingBase;
		class Encoding
		{
		private:
			class Helper
			{
			public:
				Helper();
				~Helper();

				const Encoding& GetEncoding( unsigned int codePage );

			private:
				typedef std::map<unsigned int, Encoding*> EncodingMap;
				EncodingMap m_encodings;
			};

		public:
			static const Encoding& Default();
			static const Encoding& ASCII();
			static const Encoding& UTF8();
			static const Encoding& Unicode();
			static const Encoding& UnicodeBigEndian();

			static const Encoding& GetEncoding( unsigned int codePage );
			static bool Convert( const Encoding& srcEncoding, const Encoding& dstEncoding, const std::vector<char>& input, std::vector<char>& output );
			static bool Convert( const Encoding& srcEncoding, const Encoding& dstEncoding, const std::string& input, std::string& output );
			static std::string Convert( const Encoding& srcEncoding, const Encoding& dstEncoding, const std::string& input );
		
		public:
			bool operator == ( const Encoding& other ) const
			{
				return m_pEncodingData == other.m_pEncodingData;
			}

			bool operator != ( const Encoding& other ) const
			{
				return m_pEncodingData != other.m_pEncodingData;
			}

		private:
			Encoding();
			Encoding( EncodingBase* pEncodingBase );
			~Encoding();

		public:
			unsigned int GetCharCount( const wchar_t* input, unsigned int count ) const;
			unsigned int GetCharCount( const wchar_t* input ) const;
			unsigned int GetCharCount( const std::vector<wchar_t>& input ) const;
			unsigned int GetCharCount( const std::wstring& input ) const;

			void GetChars( const wchar_t* input, unsigned int inputCount, char* output, unsigned int outputCount ) const;
			void GetChars( const std::vector<wchar_t>& input, std::vector<char>& output ) const;
			void GetChars( const std::wstring& input, std::vector<char>& output ) const;
			std::vector<char> GetChars( const wchar_t* input ) const;
			std::vector<char> GetChars( const std::vector<wchar_t>& input ) const;
			std::vector<char> GetChars( const std::wstring& input ) const;

			void GetStringA( const std::vector<wchar_t>& input, std::string& output ) const;
			void GetStringA( const std::wstring& input, std::string& output ) const;
			std::string GetStringA( const std::vector<wchar_t>& input ) const;
			std::string GetStringA( const std::wstring& input ) const;

			unsigned int GetWideCharCount( const char* input, unsigned int count ) const;
			unsigned int GetWideCharCount( const char* input ) const;
			unsigned int GetWideCharCount( const std::vector<char>& input ) const;
			unsigned int GetWideCharCount( const std::string& input ) const;

			void GetWideChars( const char* input, unsigned int inputCount, wchar_t* output, unsigned outputCount ) const;
			void GetWideChars( const std::vector<char>& input, std::vector<wchar_t>& output ) const;
			void GetWideChars( const std::string& input, std::vector<wchar_t>& output ) const;
			std::vector<wchar_t> GetWideChars( const char* input ) const;
			std::vector<wchar_t> GetWideChars( const std::vector<char>& input ) const;
			std::vector<wchar_t> GetWideChars( const std::string& input ) const;

			void GetStringW( const std::vector<char>& input, std::wstring& output ) const;
			void GetStringW( const std::string& input, std::wstring& output ) const;
			std::wstring GetStringW( const std::vector<char>& input ) const;
			std::wstring GetStringW( const std::string& input ) const;

			unsigned int GetMaxCharCount( unsigned int inputCount ) const;
			unsigned int GetMaxWideCharCount( unsigned int inputCount ) const;

			unsigned int GetCharUnitSize( const char& input ) const;

			unsigned int GetCodePage() const;
			const std::wstring& GetEncodingName() const;
			const std::vector<unsigned char>& GetPreamble() const;

			const std::string& GetSpaceToken() const;
			const std::string& GetTabToken() const;
			const std::string& GetCarriageReturnToken() const;
			const std::string& GetNextLineToken() const;

			bool IsSingleByte() const;
			bool IsValid() const;
		
		private:
			static Helper s_helper;

			EncodingBase* m_pEncodingData;
		};
	}
}
