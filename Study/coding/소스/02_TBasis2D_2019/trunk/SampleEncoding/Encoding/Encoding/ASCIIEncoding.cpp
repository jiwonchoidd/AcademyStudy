#include "ASCIIEncoding.h"

namespace Core
{
	namespace Text
	{
		ASCIIEncoding::ASCIIEncoding()
			: EncodingBase( eCodePage::ASCII, L"US-ASCII", true )
		{
			m_spaceToken.push_back( ' ' );
			m_tabToken.push_back( '\t' );
			m_carriageReturnToken.push_back( '\r' );
			m_nextLineToken.push_back( '\n' );
		}

		ASCIIEncoding::~ASCIIEncoding()
		{

		}

		void ASCIIEncoding::GetWideChars( const char* input, unsigned int inputCount, wchar_t* output, unsigned outputCount ) const
		{
			std::vector<char> temp;
			temp.resize( inputCount );
			for( unsigned int i = 0 ; i < inputCount ; ++i )
			{
				temp[ i ] = input[ i ] < 0 ? 0x3f : input[ i ];
			}

			EncodingBase::GetWideChars( &temp[ 0 ], inputCount, output, outputCount );
		}

		unsigned int ASCIIEncoding::GetMaxCharCount( unsigned int inputCount ) const
		{
			return inputCount + 1;
		}

		unsigned int ASCIIEncoding::GetMaxWideCharCount( unsigned int inputCount ) const
		{
			return inputCount;
		}

		unsigned int ASCIIEncoding::GetCharUnitSize( const char& ) const
		{
			return 1;
		}
	}
}