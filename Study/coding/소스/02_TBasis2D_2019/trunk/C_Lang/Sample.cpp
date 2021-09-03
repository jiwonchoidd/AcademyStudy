#define _CRT_SECURE_NO_WARNINGS
#define SAMPLE_01
//#undef SAMPLE_01
#define CPLUSPLUS
//#undef CPLUSPLUS

#ifdef SAMPLE_01

	#ifndef  CPLUSPLUS
		#include <stdio.h>	//printf, scanf
		void main()
		{
			printf("======= CODE 002 =======\n");
			int		iValue;
			float	fValue;
			char	strValue[20];
			char	cValue1, cValue2;	
			printf("\n 정수 및 실수를 입력하시오 =>");
			scanf("%d %f", &iValue, &fValue);

			printf("\n 문자열을 입력하시오 =>"); 
			scanf("%s", strValue);
			//scanf_s("%s", strValue, sizeof(strValue));
			
			printf("\n iValue  = %d", iValue );
			printf("\n fValue  = %f", fValue );
			printf("\n strValue  = %s", strValue ); // 엔터키값으로 공백을 채움.
			//fflush(stdin);
			//getchar();
			char c;
			//EOF 버퍼에 아무것도(입출력에선 파일끝) 없거나 엔터값이 아니면 반복하여 지운다.
			while ((c = getchar()) != '\n' && c != EOF);
			
			//fflush(stdin);		
			printf("\n 문자1을 입력하시오 =>"); 
			scanf("%c", &cValue1);
			printf("\n strValue  = %s, %c ",strValue, cValue1);
		}
	#else
		#include <cstdio>
		#include <iostream>
		using namespace std;
		int getNumber() 
		{
			int retNum;
			cout << "Enter Number: ";
			cin >> retNum;
			// 잘못된 입력을 받았을 경우
			if (cin.fail())
			{
				cout << "Wrong Number. Retry!" << endl; // 에러 메시지 출력
				cin.clear(); // 오류스트림을 초기화
				cin.ignore(256, '\n'); // 입력버퍼를 비움
				return getNumber(); // 함수를 재호출한다
			}
			return retNum;
		}

		void main()
		{
			printf("======= CODE 002 =======\n");
			

			int		iValue;
			float	fValue;
			char	strValue[20];
			char	cValue1, cValue2;
			
			cout << "fValue= " << getNumber() << endl;

			cout << "정수와 실수 입력하시오 =>" << endl;
			cin >> iValue >> fValue;//정수는 반드시 정수로 입력해야함.
			char ch;
			while ((ch = cin.get()) != '\n' && ch != EOF);
			cout << "문자열을 입력하시오 =>"; cin >> strValue;
			cout << "iValue= " << iValue << endl;
			cout << "fValue= " << fValue << endl;
			cout << "strValue= " << strValue << endl;
			//fflush(stdin);		
			while ((ch = cin.get()) != '\n' && ch != EOF);
			cout << "문자1을 입력하시오 => " << cValue1 << endl;
		
			//fflush(stdin);		
			while ((ch = cin.get()) != '\n' && ch != EOF);
			cout << "문자2을 입력하시오 => " << cValue2 << endl;

			cout << cValue1 << cValue2 << endl;		


		}
	#endif

#else
	#ifndef  CPLUSPLUS
		#include <stdio.h>
		void main()
		{
			printf("KGCA 게임아카데미\n");
			// CODE 000
			printf("======= CODE 000 =======\n");
			int		i= 65;
			float	f= 123.456f;
			printf("변수 i의 값 : %d\n", i); // 정수
			printf("변수 g의 값 : %g\n", f); // %f / %e 중에 적당한 방식으로 출력( 자리수 맞춤 )
			printf("변수 f의 값 : %f\n", f); // 소수점 6자리 출력
			printf("변수 e의 값 : %e\n", f); // 지수형식으로 출력 // 대소문자 구별 출력
			printf("변수 E의 값 : %E\n", f); // 지수형식으로 출력 // 대소문자 구별 출력
			// CODE 001	
			printf("======= CODE 001 =======\n");
			printf("숫자의 폭이 지정폭 보다 크면 폭은 무시되고 전체 출력\n");
			printf("소수점 이하 자리수를 지정하면 해당 자리수로 반올림한다.\n");
			printf("변수 5d 의 값		: |%5d|\n", i); // 정수	
			printf("변수 05d의 값		: |%05d| %f\n", i,f); // 정수
			printf("변수 -5d의 값		: |%-5d| %f\n", i,f); // 정수	
			printf("변수 8.4g의 값		: |%8.4g|\n", f); // %f / %e 중에 적당한 방식으로 출력( 자리수 맞춤 )
			printf("변수 -8.4g의 값		: |%-8.4g|\n", f); // %f / %e 중에 적당한 방식으로 출력( 자리수 맞춤 )
			printf("변수 1.4f의 값		: |%020.4f|\n", f); // 소수점 6자리 출력
			printf("변수 -20.4f의 값	: |%-20.4f|\n", f); // 소수점 6자리 출력
			printf("변수 20.4E의 값		: |%020.4E|\n", f); // 지수형식으로 출력 // 대소문자 구별 출력
			printf("변수 -20.4E의 값	: |%-20.4E|\n", f); 
		
		}
	#else
		#include <iostream>
		#include <iomanip>
		void main()
		{
			std::cout << "KGCA 게임아카데미" << std::endl;
			// CODE 000
			printf("======= CODE 000 =======\n");
			int		i = 65;
			float	f = 123.456f;
			std::cout << "변수 i의 값 :" << std::dec << i << std::endl; // 10진수
			std::cout << "변수 i의 값 :" << std::oct << i << std::endl; // 8진수
			std::cout << "변수 i의 값 :" << std::hex << i << std::endl; // 16진수
			std::cout << "변수 i의 값 :" << std::fixed << f << std::endl; // 고정소수점
			std::cout << "변수 i의 값 :" << std::scientific << f << std::endl; // 지수		
			// CODE 001	
			std::cout << "======= CODE 001 =======" << std::endl;
			std::cout << "오른쪽 정렬: |" << std::setw(5) << std::right << i << "|" << std::endl; // 정수	
			std::cout << "왼쪽 정렬  : |" << std::setw(5) << std::left << i << "|" << std::endl; // 정수	
			std::cout << "오른쪽정렬,공백채움 : |" << std::setw(5) << std::right << std::setfill('0') << i << "|" << std::endl; // 정수	
			std::cout << "왼쪽 정렬,공백채움  : |" << std::setw(5) << std::left << std::setfill('0') << i << "|" << std::endl; // 정수	
	
			std::cout << "고정소수점,소수점 4자리 : |" << std::setw(20) << std::right << std::fixed << std::setprecision(4) << f << "|" << std::endl; // 정수	
			std::cout << "부동소수점,소수점 4자리 : |" << std::setw(20) << std::left << std::setprecision(4) << f << "|" << std::endl; // 정수	

		}
	#endif
#endif