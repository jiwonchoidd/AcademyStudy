#pragma once

#include "EncodingBase.h"

namespace Core
{
	namespace Text
	{
		class CodePageBaseEncoding : public EncodingBase
		{
		public:
			CodePageBaseEncoding( unsigned int codePage, const CPINFOEXW& cpInfoEx, bool isSingleByte );
			virtual ~CodePageBaseEncoding();

		protected:
			CPINFOEXW m_cpInfoEx;
		};
	}
}