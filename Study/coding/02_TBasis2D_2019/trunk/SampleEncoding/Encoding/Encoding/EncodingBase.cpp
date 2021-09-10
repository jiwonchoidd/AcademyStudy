#include "EncodingBase.h"

namespace Core
{
	namespace Text
	{
		EncodingBase::EncodingBase( unsigned int codePage, const std::wstring& encodingName, bool m_isSingleByte )
			: m_codePage( codePage )
			, m_encodingName( encodingName )
			, m_preamble()
			, m_spaceToken()
			, m_tabToken()
			, m_nextLineToken()
			, m_carriageReturnToken()
			, m_isSingleByte( m_isSingleByte )
			, m_isValid( true )
		{
		}

		EncodingBase::~EncodingBase()
		{

		}

		unsigned int EncodingBase::GetCharCount( const wchar_t* input, unsigned int count ) const
		{
			return ::WideCharToMultiByte( m_codePage, 0, &input[0], count, NULL, 0, NULL, NULL );
		}

		void EncodingBase::GetChars( const wchar_t* input, unsigned int inputCount, char* output, unsigned int outputCount ) const
		{
			::WideCharToMultiByte( m_codePage, 0, &input[0], inputCount, &output[0], outputCount, NULL, NULL );
		}

		unsigned int EncodingBase::GetWideCharCount( const char* input, unsigned int count ) const
		{
			return ::MultiByteToWideChar( m_codePage, 0, &input[0], count, NULL, 0 );
		}

		void EncodingBase::GetWideChars( const char* input, unsigned int inputCount, wchar_t* output, unsigned outputCount ) const
		{
			::MultiByteToWideChar( m_codePage, 0, &input[0], inputCount, &output[0], outputCount );
		}
	}
}