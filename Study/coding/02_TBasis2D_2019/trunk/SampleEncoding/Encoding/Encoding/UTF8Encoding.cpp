#include "UTF8Encoding.h"

namespace Core
{
	namespace Text
	{
		UTF8Encoding::UTF8Encoding()
			: EncodingBase( eCodePage::UTF8, L"Unicode (UTF-8)", false )
		{
			m_preamble.resize( 3 );
			m_preamble[0] = 0xef;
			m_preamble[1] = 0xbb;
			m_preamble[2] = 0xbf;

			m_spaceToken.push_back( ' ' );
			m_tabToken.push_back( '\t' );
			m_carriageReturnToken.push_back( '\r' );
			m_nextLineToken.push_back( '\n' );
		}

		UTF8Encoding::~UTF8Encoding()
		{

		}
	
		unsigned int UTF8Encoding::GetMaxCharCount( unsigned int inputCount ) const
		{
			return inputCount * 3 + 3;
		}

		unsigned int UTF8Encoding::GetMaxWideCharCount( unsigned int inputCount ) const
		{
			return inputCount + 1;
		}

		unsigned int UTF8Encoding::GetCharUnitSize( const char& input ) const
		{
			return ( input & 0xFC ) == 0xFC ? 6 : ( input & 0xF8 ) == 0xF8 ? 5 : ( input & 0xF0 ) == 0xF0 ? 4 : ( input & 0xE0 ) == 0xE0 ? 3 : ( input & 0xC0 ) == 0xC0 ? 2 : 1;
		}
	}
}