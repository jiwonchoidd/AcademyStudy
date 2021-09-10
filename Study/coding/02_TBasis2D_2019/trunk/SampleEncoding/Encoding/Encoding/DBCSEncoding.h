#pragma once

#include "CodePageBaseEncoding.h"

namespace Core
{
	namespace Text
	{
		class DBCSEncoding : public CodePageBaseEncoding
		{
		public:
			DBCSEncoding( unsigned int codePage, const CPINFOEXW& cpInfoEx );
			virtual ~DBCSEncoding();

			virtual unsigned int GetMaxCharCount( unsigned int inputCount ) const;
			virtual unsigned int GetMaxWideCharCount( unsigned int inputCount ) const;

			virtual unsigned int GetCharUnitSize( const char& input ) const;
		};
	}
}