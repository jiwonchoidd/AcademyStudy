// TextEncoding.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include <stdio.h>
#include <tchar.h>
#include "Encoding/Encoding.h"
#include "Encoding/CodePage/CodePage.h"

using namespace Core::Text;

int _tmain(int argc, _TCHAR* argv[])
{
	if( Encoding::Default() == Encoding::GetEncoding( eCodePage::Korean ) )
	{
		printf( "Default는 Korean\n" );
	}

	printf( "Default [%s] -> ASCII [%s]\n", "asdㅁㄴㅇ123", Encoding::Convert( Encoding::Default(), Encoding::ASCII(), "asdㅁㄴㅇ123" ).c_str() );

	printf( "Unicode [hello] -> ASCII 로 변환시 char count : %d\n", Encoding::ASCII().GetCharCount( L"hello" ) );
	printf( "Unicode [안녕하세요] -> ASCII 로 변환시 char count : %d\n", Encoding::ASCII().GetCharCount( L"안녕하세요" ) );
	printf( "Unicode [안녕하세요] -> Default 로 변환시 char count : %d\n", Encoding::Default().GetCharCount( L"안녕하세요" ) );
	printf( "Unicode [hello] -> UTF8 로 변환시 char count : %d\n", Encoding::UTF8().GetCharCount( L"hello" ) );
	printf( "Unicode [안녕하세요] -> UTF8 로 변환시 char count : %d\n", Encoding::UTF8().GetCharCount( L"안녕하세요" ) );

	printf( "ASCII [hello] -> Unicode 로 변환시 wide char count : %d\n", Encoding::ASCII().GetWideCharCount( "hello" ) );
	printf( "ASCII [안녕하세요] -> Unicode 로 변환시 wide char count : %d\n", Encoding::ASCII().GetWideCharCount( "안녕하세요" ) );
	printf( "Default [안녕하세요] -> Unicode 로 변환시 wide char count : %d\n", Encoding::Default().GetWideCharCount( "안녕하세요" ) );
	printf( "UTF-8 [hello] -> Unicode 로 변환시 wide char count : %d\n", Encoding::UTF8().GetWideCharCount( "hello" ) );
	std::string strUTF8Text = Encoding::Convert( Encoding::Default(), Encoding::UTF8(), "안녕하세요" );
	printf( "UTF-8 [안녕하세요] -> Unicode 로 변환시 wide char count : %d\n", Encoding::UTF8().GetWideCharCount( strUTF8Text.c_str() ) );
	return 0;
}

