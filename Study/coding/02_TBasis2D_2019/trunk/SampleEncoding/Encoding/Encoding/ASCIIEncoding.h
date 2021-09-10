#pragma once

#include "EncodingBase.h"

namespace Core
{
	namespace Text
	{
		class ASCIIEncoding : public EncodingBase
		{
		public:
			ASCIIEncoding();
			virtual ~ASCIIEncoding();

			virtual void GetWideChars( const char* input, unsigned int inputCount, wchar_t* output, unsigned outputCount ) const;

			virtual unsigned int GetMaxCharCount( unsigned int inputCount ) const;
			virtual unsigned int GetMaxWideCharCount( unsigned int inputCount ) const;

			virtual unsigned int GetCharUnitSize( const char& input ) const;
		};
	}
}