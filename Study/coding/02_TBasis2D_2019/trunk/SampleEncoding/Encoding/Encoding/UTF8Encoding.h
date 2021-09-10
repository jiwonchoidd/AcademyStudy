#pragma once

#include "EncodingBase.h"

namespace Core
{
	namespace Text
	{
		class UTF8Encoding : public EncodingBase
		{
		public:
			UTF8Encoding();
			virtual ~UTF8Encoding();

			virtual unsigned int GetMaxCharCount( unsigned int inputCount ) const;
			virtual unsigned int GetMaxWideCharCount( unsigned int inputCount ) const;

			virtual unsigned int GetCharUnitSize( const char& input ) const;
		};
	}
}