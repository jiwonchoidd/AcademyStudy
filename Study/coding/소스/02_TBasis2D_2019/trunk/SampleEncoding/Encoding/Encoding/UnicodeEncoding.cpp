#include "UnicodeEncoding.h"

namespace Core
{
	namespace Text
	{
		UnicodeEncoding::UnicodeEncoding( bool bBigEndian )
			: EncodingBase( bBigEndian ? eCodePage::UnicodeBigEndian : eCodePage::Unicode, bBigEndian ? L"UnicodeBigEndian" : L"Unicode", false )
			, m_bBigEndian( bBigEndian )
		{
			m_preamble.resize( 2 );
			m_spaceToken.resize( 2 );
			m_tabToken.resize( 2 );
			m_carriageReturnToken.resize( 2 );
			m_nextLineToken.resize( 2 );
			if( bBigEndian )
			{
				m_preamble[0] = 0xfe;
				m_preamble[1] = 0xff;

				m_spaceToken[0] = 0;
				m_spaceToken[1] = ' ';

				m_tabToken[0] = 0;
				m_tabToken[1] = '\t';

				m_carriageReturnToken[0] = 0;
				m_carriageReturnToken[1] = '\r';

				m_nextLineToken[0] = 0;
				m_nextLineToken[1] = '\n';
			}
			else
			{
				m_preamble[0] = 0xff;
				m_preamble[1] = 0xfe;

				m_spaceToken[0] = ' ';
				m_spaceToken[1] = 0;

				m_tabToken[0] = '\t';
				m_tabToken[1] = 0;

				m_carriageReturnToken[0] = '\r';
				m_carriageReturnToken[1] = 0;

				m_nextLineToken[0] = '\n';
				m_nextLineToken[1] = 0;
			}
		}

		UnicodeEncoding::~UnicodeEncoding()
		{

		}

		unsigned int UnicodeEncoding::GetCharCount( const wchar_t*, unsigned int count ) const
		{
			return count * sizeof( wchar_t );
		}

		void UnicodeEncoding::GetChars( const wchar_t* input, unsigned int inputCount, char* output, unsigned int outputCount ) const
		{
			if( m_bBigEndian )
			{
				for( unsigned int i = 0; i < inputCount; ++i )
				{
					const char* pTemp = reinterpret_cast<const char*>( &input[ i ] );
					output[ i * sizeof( wchar_t ) ] = pTemp[ 1 ];
					output[ i * sizeof( wchar_t ) + 1 ] = pTemp[ 0 ];
				}
			}
			else
			{
				::memcpy_s( &output[0], outputCount, &input[0], inputCount * sizeof( wchar_t ) );
			}
		}

		unsigned int UnicodeEncoding::GetWideCharCount( const char*, unsigned int count ) const
		{
			return count / sizeof( wchar_t );
		}

		void UnicodeEncoding::GetWideChars( const char* input, unsigned int inputCount, wchar_t* output, unsigned outputCount ) const
		{
			if( m_bBigEndian )
			{
				for( unsigned int i = 0; i < outputCount; ++i )
				{
					char* pTemp = reinterpret_cast<char*>( &output[ i ] );
					pTemp[ 0 ] = input[ i * sizeof( wchar_t ) + 1 ];
					pTemp[ 1 ] = input[ i * sizeof( wchar_t ) ];
				}
			}
			else
			{
				::memcpy_s( &output[0], outputCount * sizeof( wchar_t ), &input[0], inputCount );
			}
		}

		unsigned int UnicodeEncoding::GetMaxCharCount( unsigned int inputCount ) const
		{
			return inputCount * 2 + 2;
		}

		unsigned int UnicodeEncoding::GetMaxWideCharCount( unsigned int inputCount ) const
		{
			return ( inputCount + 1 ) / 2 + 1;
		}

		unsigned int UnicodeEncoding::GetCharUnitSize( const char& ) const
		{
			return 2;
		}
	}
}