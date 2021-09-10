#pragma once

#include "CodePageBaseEncoding.h"

namespace Core
{
	namespace Text
	{
		class SBCSEncoding : public CodePageBaseEncoding
		{
		public:
			SBCSEncoding( unsigned int codePage, const CPINFOEXW& cpInfoEx );
			virtual ~SBCSEncoding();

			virtual unsigned int GetMaxCharCount( unsigned int inputCount ) const;
			virtual unsigned int GetMaxWideCharCount( unsigned int inputCount ) const;

			virtual unsigned int GetCharUnitSize( const char& input ) const;
		};
	}
}