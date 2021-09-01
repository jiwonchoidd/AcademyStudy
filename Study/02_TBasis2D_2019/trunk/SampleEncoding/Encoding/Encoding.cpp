#include "Encoding.h"
#include "Encoding/ErrorEncoding.h"
#include "Encoding/ASCIIEncoding.h"
#include "Encoding/UTF8Encoding.h"
#include "Encoding/UnicodeEncoding.h"
#include "Encoding/SBCSEncoding.h"
#include "Encoding/DBCSEncoding.h"

namespace Core
{
	namespace Text
	{
		Encoding::Helper Encoding::s_helper;

		Encoding::Helper::Helper()
			: m_encodings()
		{

		}

		Encoding::Helper::~Helper()
		{
			if( !m_encodings.empty() )
			{
				EncodingMap::iterator iter = m_encodings.begin();
				EncodingMap::iterator enditer = m_encodings.end();
				while( iter != enditer )
				{
					delete iter->second;
					++iter;
				}

				m_encodings.clear();
			}
		}

		const Encoding& Encoding::Helper::GetEncoding( unsigned int codePage )
		{
			if( codePage == eCodePage::Default )
			{
				codePage = ::GetACP();
			}

			EncodingMap::const_iterator iter = m_encodings.find( codePage );
			if( iter!= m_encodings.end() )
			{
				return *iter->second;
			}

			Encoding* pEncoding = NULL;
			switch( codePage )
			{
			case eCodePage::ASCII:
				{
					pEncoding = new Encoding( new ASCIIEncoding() );
				}
				break;

			case eCodePage::UTF8:
				{
					pEncoding = new Encoding( new UTF8Encoding() );
				}
				break;

			case eCodePage::Unicode:
			case eCodePage::UnicodeBigEndian:
				{
					pEncoding = new Encoding( new UnicodeEncoding( codePage == eCodePage::UnicodeBigEndian ) );
				}
				break;

			default:
				{
					CPINFOEXW cpInfoEx;
					if( ::GetCPInfoExW( codePage, 0, &cpInfoEx ) == TRUE )
					{
						// UTF-7, ISO-2022 시리즈, ISCII 시리즈, hz_gb_2312, GB18030등 2바이트 이상을 사용하는 코드페이지는 지원하지 않음
						if( cpInfoEx.MaxCharSize == 1 )
						{
							pEncoding = new Encoding( new SBCSEncoding( codePage, cpInfoEx ) );
						}
						else if( cpInfoEx.MaxCharSize == 2 )
						{
							pEncoding = new Encoding( new DBCSEncoding( codePage, cpInfoEx ) );
						}
					}
				}
				break;
			}

			if( pEncoding == NULL )
			{
				pEncoding = new Encoding( new ErrorEncoding() );
			}

			m_encodings.insert( std::make_pair( codePage, pEncoding ) );
			return *pEncoding;
		}

		Encoding::Encoding()
			: m_pEncodingData( NULL )
		{
		}

		Encoding::Encoding( EncodingBase* pEncodingBase )
			: m_pEncodingData( pEncodingBase )
		{
		}

		Encoding::~Encoding()
		{
			if( m_pEncodingData != NULL )
			{
				delete m_pEncodingData;
			}
		}

		const Encoding& Encoding::Default()
		{
			return GetEncoding( eCodePage::Default );
		}

		const Encoding& Encoding::ASCII()
		{
			return GetEncoding( eCodePage::ASCII );
		}

		const Encoding& Encoding::UTF8()
		{
			return GetEncoding( eCodePage::UTF8 );
		}

		const Encoding& Encoding::Unicode()
		{
			return GetEncoding( eCodePage::Unicode );
		}

		const Encoding& Encoding::UnicodeBigEndian()
		{
			return GetEncoding( eCodePage::UnicodeBigEndian );
		}

		const Encoding& Encoding::GetEncoding( unsigned int codePage )
		{
			return s_helper.GetEncoding( codePage );
		}

		bool Encoding::Convert( const Encoding& srcEncoding, const Encoding& dstEncoding, const std::vector<char>& input, std::vector<char>& output )
		{
			if( !srcEncoding.IsValid() && !dstEncoding.IsValid() )
			{
				return false;
			}

			std::vector<wchar_t> temp;
			srcEncoding.GetWideChars( input, temp );
			dstEncoding.GetChars( temp, output );
			return true;
		}

		bool Encoding::Convert( const Encoding& srcEncoding, const Encoding& dstEncoding, const std::string& input, std::string& output )
		{
			if( !srcEncoding.IsValid() && !dstEncoding.IsValid() )
			{
				return false;
			}

			std::wstring temp;
			srcEncoding.GetStringW( input, temp );
			dstEncoding.GetStringA( temp, output );
			return true;
		}

		std::string Encoding::Convert( const Encoding& srcEncoding, const Encoding& dstEncoding, const std::string& input )
		{
			std::string output;
			Convert( srcEncoding, dstEncoding, input, output );
			return output;
		}

		unsigned int Encoding::GetCharCount( const wchar_t* input, unsigned int count ) const
		{
			return m_pEncodingData->GetCharCount( input, count );
		}

		unsigned int Encoding::GetCharCount( const wchar_t* input ) const
		{
			return GetCharCount( &input[0], ::wcslen( input ) );
		}

		unsigned int Encoding::GetCharCount( const std::vector<wchar_t>& input ) const
		{
			return GetCharCount( &input[0], input.size() );
		}

		unsigned int Encoding::GetCharCount( const std::wstring& input ) const
		{
			return GetCharCount( &input[0], input.length() );
		}

		void Encoding::GetChars( const wchar_t* input, unsigned int inputCount, char* output, unsigned int outputCount ) const
		{
			m_pEncodingData->GetChars( input, inputCount, output, outputCount );
		}

		void Encoding::GetChars( const std::vector<wchar_t>& input, std::vector<char>& output ) const
		{
			const wchar_t* pInput = &input[0];
			unsigned int inputCount = input.size();
			output.resize( GetCharCount( pInput, inputCount ) );
			GetChars( pInput, inputCount, &output[0], output.size() );
		}

		void Encoding::GetChars( const std::wstring& input, std::vector<char>& output ) const
		{
			const wchar_t* pInput = &input[0];
			unsigned int inputCount = input.length();
			output.resize( GetCharCount( pInput, inputCount ) );
			GetChars( pInput, inputCount, &output[0], output.size() );
		}

		std::vector<char> Encoding::GetChars( const wchar_t* input ) const
		{
			std::vector<char> output;
			GetChars( std::wstring( input ), output );
			return output;
		}

		std::vector<char> Encoding::GetChars( const std::vector<wchar_t>& input ) const
		{
			std::vector<char> output;
			GetChars( input, output );
			return output;
		}

		std::vector<char> Encoding::GetChars( const std::wstring& input ) const
		{
			std::vector<char> output;
			GetChars( input, output );
			return output;
		}

		void Encoding::GetStringA( const std::vector<wchar_t>& input, std::string& output ) const
		{
			const wchar_t* pInput = &input[0];
			unsigned int inputCount = input.size();
			output.resize( GetCharCount( pInput, inputCount ) );
			GetChars( pInput, inputCount, &output[0], output.length() );
		}

		void Encoding::GetStringA( const std::wstring& input, std::string& output ) const
		{
			const wchar_t* pInput = &input[0];
			unsigned int inputCount = input.length();
			output.resize( GetCharCount( pInput, inputCount ) );
			GetChars( pInput, inputCount, &output[0], output.length() );
		}

		std::string Encoding::GetStringA( const std::vector<wchar_t>& input ) const
		{
			std::string output;
			GetStringA( input, output );
			return output;
		}

		std::string Encoding::GetStringA( const std::wstring& input ) const
		{
			std::string output;
			GetStringA( input, output );
			return output;
		}

		unsigned int Encoding::GetWideCharCount( const char* input, unsigned int count ) const
		{
			return m_pEncodingData->GetWideCharCount( input, count );
		}

		unsigned int Encoding::GetWideCharCount( const char* input ) const
		{
			return GetWideCharCount( &input[0], ::strlen( input ) );
		}

		unsigned int Encoding::GetWideCharCount( const std::vector<char>& input ) const
		{
			return GetWideCharCount( &input[0], input.size() );
		}

		unsigned int Encoding::GetWideCharCount( const std::string& input ) const
		{
			return GetWideCharCount( &input[0], input.length() );
		}

		void Encoding::GetWideChars( const char* input, unsigned int inputCount, wchar_t* output, unsigned outputCount ) const
		{
			m_pEncodingData->GetWideChars( input, inputCount, output, outputCount );
		}

		void Encoding::GetWideChars( const std::vector<char>& input, std::vector<wchar_t>& output ) const
		{
			const char* pInput = &input[0];
			unsigned int inputCount = input.size();
			output.resize( GetWideCharCount( pInput, inputCount ) );
			GetWideChars( pInput, inputCount, &output[0], output.size() );
		}

		void Encoding::GetWideChars( const std::string& input, std::vector<wchar_t>& output ) const
		{
			const char* pInput = &input[0];
			unsigned int inputCount = input.length();
			output.resize( GetWideCharCount( pInput, inputCount ) );
			GetWideChars( pInput, inputCount, &output[0], output.size() );
		}

		std::vector<wchar_t> Encoding::GetWideChars( const char* input ) const
		{
			std::vector<wchar_t> output;
			GetWideChars( std::string( input ), output );
			return output;
		}

		std::vector<wchar_t> Encoding::GetWideChars( const std::vector<char>& input ) const
		{
			std::vector<wchar_t> output;
			GetWideChars( input, output );
			return output;
		}

		std::vector<wchar_t> Encoding::GetWideChars( const std::string& input ) const
		{
			std::vector<wchar_t> output;
			GetWideChars( input, output );
			return output;
		}

		void Encoding::GetStringW( const std::vector<char>& input, std::wstring& output ) const
		{
			unsigned int inputCount = input.size();

			if( inputCount <= 0 )
			{
				return;
			}

			const char* pInput = &input[0];
			output.resize( GetWideCharCount( pInput, inputCount ) );
			GetWideChars( pInput, inputCount, &output[0], output.length() );
		}

		void Encoding::GetStringW( const std::string& input, std::wstring& output ) const
		{
			unsigned int inputCount = input.length();

			if( inputCount <= 0 )
			{
				return;
			}

			const char* pInput = &input[0];
			output.resize( GetWideCharCount( pInput, inputCount ) );
			GetWideChars( pInput, inputCount, &output[0], output.length() );
		}

		std::wstring Encoding::GetStringW( const std::vector<char>& input ) const
		{
			std::wstring output;
			GetStringW( input, output );
			return output;
		}

		std::wstring Encoding::GetStringW( const std::string& input ) const
		{
			std::wstring output;
			GetStringW( input, output );
			return output;
		}

		unsigned int Encoding::GetMaxCharCount( unsigned int inputCount ) const
		{
			return m_pEncodingData->GetMaxCharCount( inputCount );
		}

		unsigned int Encoding::GetMaxWideCharCount( unsigned int inputCount ) const
		{
			return m_pEncodingData->GetMaxWideCharCount( inputCount );
		}

		unsigned int Encoding::GetCharUnitSize( const char& input ) const
		{
			return m_pEncodingData->GetCharUnitSize( input );
		}

		unsigned int Encoding::GetCodePage() const
		{
			return m_pEncodingData->GetCodePage();
		}

		const std::wstring& Encoding::GetEncodingName() const
		{
			return m_pEncodingData->GetEncodingName();
		}

		const std::vector<unsigned char>& Encoding::GetPreamble() const
		{
			return m_pEncodingData->GetPreamble();
		}

		const std::string& Encoding::GetSpaceToken() const
		{
			return m_pEncodingData->GetSpaceToken();
		}

		const std::string& Encoding::GetTabToken() const
		{
			return m_pEncodingData->GetTabToken();
		}

		const std::string& Encoding::GetCarriageReturnToken() const
		{
			return m_pEncodingData->GetCarriageReturnToken();
		}

		const std::string& Encoding::GetNextLineToken() const
		{
			return m_pEncodingData->GetNextLineToken();
		}

		bool Encoding::IsSingleByte() const
		{
			return m_pEncodingData->IsSingleByte();
		}

		bool Encoding::IsValid() const
		{
			return m_pEncodingData->IsValid();
		}
	}
}