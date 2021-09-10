#include "ErrorEncoding.h"

namespace Core
{
	namespace Text
	{
		ErrorEncoding::ErrorEncoding()
			: EncodingBase( 0, L"", false )
		{
			m_isValid = false;
		}

		ErrorEncoding::~ErrorEncoding()
		{
		}

		unsigned int ErrorEncoding::GetCharCount( const wchar_t*, unsigned int ) const
		{
			return 0;
		}

		unsigned int ErrorEncoding::GetCharCount( const std::vector<wchar_t>& ) const
		{
			return 0;
		}

		unsigned int ErrorEncoding::GetCharCount( const std::wstring& ) const
		{
			return 0;
		}

		void ErrorEncoding::GetChars( const wchar_t*, unsigned int, char*, unsigned int ) const
		{
		}

		void ErrorEncoding::GetChars( const std::vector<wchar_t>&, std::vector<char>& ) const
		{
		}

		void ErrorEncoding::GetChars( const std::vector<wchar_t>&, std::string& ) const
		{
		}

		void ErrorEncoding::GetChars( const std::wstring&, std::vector<char>& ) const
		{
		}

		void ErrorEncoding::GetChars( const std::wstring&, std::string& ) const
		{
		}

		unsigned int ErrorEncoding::GetWideCharCount( const char*, unsigned int ) const
		{
			return 0;
		}

		unsigned int ErrorEncoding::GetWideCharCount( const std::vector<char>& ) const
		{
			return 0;
		}

		unsigned int ErrorEncoding::GetWideCharCount( const std::string& ) const
		{
			return 0;
		}

		void ErrorEncoding::GetWideChars( const char*, unsigned int, wchar_t*, unsigned ) const
		{
		}

		void ErrorEncoding::GetWideChars( const std::vector<char>&, std::vector<wchar_t>& ) const
		{
		}

		void ErrorEncoding::GetWideChars( const std::vector<char>&, std::wstring& ) const
		{
		}

		void ErrorEncoding::GetWideChars( const std::string&, std::vector<wchar_t>& ) const
		{
		}

		void ErrorEncoding::GetWideChars( const std::string&, std::wstring& ) const
		{
		}

		unsigned int ErrorEncoding::GetMaxCharCount( unsigned int ) const
		{
			return 0;
		}

		unsigned int ErrorEncoding::GetMaxWideCharCount( unsigned int ) const
		{
			return 0;
		}

		unsigned int ErrorEncoding::GetCharUnitSize( const char& ) const
		{
			return 0;
		}
	}
}