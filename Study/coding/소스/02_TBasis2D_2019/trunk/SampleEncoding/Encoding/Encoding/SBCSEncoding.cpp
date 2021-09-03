#include "SBCSEncoding.h"

namespace Core
{
	namespace Text
	{
		SBCSEncoding::SBCSEncoding( unsigned int codePage, const CPINFOEXW& cpInfoEx )
			: CodePageBaseEncoding( codePage, cpInfoEx, true )
		{

		}

		SBCSEncoding::~SBCSEncoding()
		{

		}

		unsigned int SBCSEncoding::GetMaxCharCount( unsigned int inputCount ) const
		{
			return inputCount + 1;
		}

		unsigned int SBCSEncoding::GetMaxWideCharCount( unsigned int inputCount ) const
		{
			return inputCount;
		}

		unsigned int SBCSEncoding::GetCharUnitSize( const char& ) const
		{
			return 1;
		}
	}
}