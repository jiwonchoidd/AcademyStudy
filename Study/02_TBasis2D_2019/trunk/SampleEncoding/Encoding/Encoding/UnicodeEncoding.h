#pragma once

#include "EncodingBase.h"

namespace Core
{
	namespace Text
	{
		class UnicodeEncoding : public EncodingBase
		{
		public:
			UnicodeEncoding( bool bBigEndian );
			virtual ~UnicodeEncoding();

			virtual unsigned int GetCharCount( const wchar_t* input, unsigned int count ) const;
			virtual void GetChars( const wchar_t* input, unsigned int inputCount, char* output, unsigned int outputCount ) const;
			virtual unsigned int GetWideCharCount( const char* input, unsigned int count ) const;
			virtual void GetWideChars( const char* input, unsigned int inputCount, wchar_t* output, unsigned outputCount ) const;

			virtual unsigned int GetMaxCharCount( unsigned int inputCount ) const;
			virtual unsigned int GetMaxWideCharCount( unsigned int inputCount ) const;

			virtual unsigned int GetCharUnitSize( const char& input ) const;

		private:
			bool m_bBigEndian;
		};
	}
}