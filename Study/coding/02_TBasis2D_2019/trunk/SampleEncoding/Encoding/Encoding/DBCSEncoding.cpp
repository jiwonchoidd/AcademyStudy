#include "DBCSEncoding.h"

namespace Core
{
	namespace Text
	{
		DBCSEncoding::DBCSEncoding( unsigned int codePage, const CPINFOEXW& cpInfoEx )
			: CodePageBaseEncoding( codePage, cpInfoEx, false )
		{

		}

		DBCSEncoding::~DBCSEncoding()
		{

		}

		unsigned int DBCSEncoding::GetMaxCharCount( unsigned int inputCount ) const
		{
			return inputCount * 2 + 2;
		}

		unsigned int DBCSEncoding::GetMaxWideCharCount( unsigned int inputCount ) const
		{
			return inputCount + 1;
		}

		unsigned int DBCSEncoding::GetCharUnitSize( const char& input ) const
		{
			return ::IsDBCSLeadByteEx( m_codePage, static_cast<unsigned char>( input ) ) ? 2 : 1;
		}
	}
}