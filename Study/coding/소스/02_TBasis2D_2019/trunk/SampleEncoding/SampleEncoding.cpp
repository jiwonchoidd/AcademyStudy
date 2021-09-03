// TextEncoding.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
		printf( "Default�� Korean\n" );
	}

	printf( "Default [%s] -> ASCII [%s]\n", "asd������123", Encoding::Convert( Encoding::Default(), Encoding::ASCII(), "asd������123" ).c_str() );

	printf( "Unicode [hello] -> ASCII �� ��ȯ�� char count : %d\n", Encoding::ASCII().GetCharCount( L"hello" ) );
	printf( "Unicode [�ȳ��ϼ���] -> ASCII �� ��ȯ�� char count : %d\n", Encoding::ASCII().GetCharCount( L"�ȳ��ϼ���" ) );
	printf( "Unicode [�ȳ��ϼ���] -> Default �� ��ȯ�� char count : %d\n", Encoding::Default().GetCharCount( L"�ȳ��ϼ���" ) );
	printf( "Unicode [hello] -> UTF8 �� ��ȯ�� char count : %d\n", Encoding::UTF8().GetCharCount( L"hello" ) );
	printf( "Unicode [�ȳ��ϼ���] -> UTF8 �� ��ȯ�� char count : %d\n", Encoding::UTF8().GetCharCount( L"�ȳ��ϼ���" ) );

	printf( "ASCII [hello] -> Unicode �� ��ȯ�� wide char count : %d\n", Encoding::ASCII().GetWideCharCount( "hello" ) );
	printf( "ASCII [�ȳ��ϼ���] -> Unicode �� ��ȯ�� wide char count : %d\n", Encoding::ASCII().GetWideCharCount( "�ȳ��ϼ���" ) );
	printf( "Default [�ȳ��ϼ���] -> Unicode �� ��ȯ�� wide char count : %d\n", Encoding::Default().GetWideCharCount( "�ȳ��ϼ���" ) );
	printf( "UTF-8 [hello] -> Unicode �� ��ȯ�� wide char count : %d\n", Encoding::UTF8().GetWideCharCount( "hello" ) );
	std::string strUTF8Text = Encoding::Convert( Encoding::Default(), Encoding::UTF8(), "�ȳ��ϼ���" );
	printf( "UTF-8 [�ȳ��ϼ���] -> Unicode �� ��ȯ�� wide char count : %d\n", Encoding::UTF8().GetWideCharCount( strUTF8Text.c_str() ) );
	return 0;
}

