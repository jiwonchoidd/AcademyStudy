#include "CodePageBaseEncoding.h"

namespace Core
{
	namespace Text
	{
		CodePageBaseEncoding::CodePageBaseEncoding( unsigned int codePage, const CPINFOEXW& cpInfoEx, bool isSingleByte )
			: EncodingBase( codePage, cpInfoEx.CodePageName, isSingleByte )
			, m_cpInfoEx( cpInfoEx )
		{
			m_spaceToken.push_back( ' ' );
			m_tabToken.push_back( '\t' );
			m_carriageReturnToken.push_back( '\r' );
			m_nextLineToken.push_back( '\n' );
		}

		CodePageBaseEncoding::~CodePageBaseEncoding()
		{

		}
	}
}