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
			printf("\n ���� �� �Ǽ��� �Է��Ͻÿ� =>");
			scanf("%d %f", &iValue, &fValue);

			printf("\n ���ڿ��� �Է��Ͻÿ� =>"); 
			scanf("%s", strValue);
			//scanf_s("%s", strValue, sizeof(strValue));
			
			printf("\n iValue  = %d", iValue );
			printf("\n fValue  = %f", fValue );
			printf("\n strValue  = %s", strValue ); // ����Ű������ ������ ä��.
			//fflush(stdin);
			//getchar();
			char c;
			//EOF ���ۿ� �ƹ��͵�(����¿��� ���ϳ�) ���ų� ���Ͱ��� �ƴϸ� �ݺ��Ͽ� �����.
			while ((c = getchar()) != '\n' && c != EOF);
			
			//fflush(stdin);		
			printf("\n ����1�� �Է��Ͻÿ� =>"); 
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
			// �߸��� �Է��� �޾��� ���
			if (cin.fail())
			{
				cout << "Wrong Number. Retry!" << endl; // ���� �޽��� ���
				cin.clear(); // ������Ʈ���� �ʱ�ȭ
				cin.ignore(256, '\n'); // �Է¹��۸� ���
				return getNumber(); // �Լ��� ��ȣ���Ѵ�
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

			cout << "������ �Ǽ� �Է��Ͻÿ� =>" << endl;
			cin >> iValue >> fValue;//������ �ݵ�� ������ �Է��ؾ���.
			char ch;
			while ((ch = cin.get()) != '\n' && ch != EOF);
			cout << "���ڿ��� �Է��Ͻÿ� =>"; cin >> strValue;
			cout << "iValue= " << iValue << endl;
			cout << "fValue= " << fValue << endl;
			cout << "strValue= " << strValue << endl;
			//fflush(stdin);		
			while ((ch = cin.get()) != '\n' && ch != EOF);
			cout << "����1�� �Է��Ͻÿ� => " << cValue1 << endl;
		
			//fflush(stdin);		
			while ((ch = cin.get()) != '\n' && ch != EOF);
			cout << "����2�� �Է��Ͻÿ� => " << cValue2 << endl;

			cout << cValue1 << cValue2 << endl;		


		}
	#endif

#else
	#ifndef  CPLUSPLUS
		#include <stdio.h>
		void main()
		{
			printf("KGCA ���Ӿ�ī����\n");
			// CODE 000
			printf("======= CODE 000 =======\n");
			int		i= 65;
			float	f= 123.456f;
			printf("���� i�� �� : %d\n", i); // ����
			printf("���� g�� �� : %g\n", f); // %f / %e �߿� ������ ������� ���( �ڸ��� ���� )
			printf("���� f�� �� : %f\n", f); // �Ҽ��� 6�ڸ� ���
			printf("���� e�� �� : %e\n", f); // ������������ ��� // ��ҹ��� ���� ���
			printf("���� E�� �� : %E\n", f); // ������������ ��� // ��ҹ��� ���� ���
			// CODE 001	
			printf("======= CODE 001 =======\n");
			printf("������ ���� ������ ���� ũ�� ���� ���õǰ� ��ü ���\n");
			printf("�Ҽ��� ���� �ڸ����� �����ϸ� �ش� �ڸ����� �ݿø��Ѵ�.\n");
			printf("���� 5d �� ��		: |%5d|\n", i); // ����	
			printf("���� 05d�� ��		: |%05d| %f\n", i,f); // ����
			printf("���� -5d�� ��		: |%-5d| %f\n", i,f); // ����	
			printf("���� 8.4g�� ��		: |%8.4g|\n", f); // %f / %e �߿� ������ ������� ���( �ڸ��� ���� )
			printf("���� -8.4g�� ��		: |%-8.4g|\n", f); // %f / %e �߿� ������ ������� ���( �ڸ��� ���� )
			printf("���� 1.4f�� ��		: |%020.4f|\n", f); // �Ҽ��� 6�ڸ� ���
			printf("���� -20.4f�� ��	: |%-20.4f|\n", f); // �Ҽ��� 6�ڸ� ���
			printf("���� 20.4E�� ��		: |%020.4E|\n", f); // ������������ ��� // ��ҹ��� ���� ���
			printf("���� -20.4E�� ��	: |%-20.4E|\n", f); 
		
		}
	#else
		#include <iostream>
		#include <iomanip>
		void main()
		{
			std::cout << "KGCA ���Ӿ�ī����" << std::endl;
			// CODE 000
			printf("======= CODE 000 =======\n");
			int		i = 65;
			float	f = 123.456f;
			std::cout << "���� i�� �� :" << std::dec << i << std::endl; // 10����
			std::cout << "���� i�� �� :" << std::oct << i << std::endl; // 8����
			std::cout << "���� i�� �� :" << std::hex << i << std::endl; // 16����
			std::cout << "���� i�� �� :" << std::fixed << f << std::endl; // �����Ҽ���
			std::cout << "���� i�� �� :" << std::scientific << f << std::endl; // ����		
			// CODE 001	
			std::cout << "======= CODE 001 =======" << std::endl;
			std::cout << "������ ����: |" << std::setw(5) << std::right << i << "|" << std::endl; // ����	
			std::cout << "���� ����  : |" << std::setw(5) << std::left << i << "|" << std::endl; // ����	
			std::cout << "����������,����ä�� : |" << std::setw(5) << std::right << std::setfill('0') << i << "|" << std::endl; // ����	
			std::cout << "���� ����,����ä��  : |" << std::setw(5) << std::left << std::setfill('0') << i << "|" << std::endl; // ����	
	
			std::cout << "�����Ҽ���,�Ҽ��� 4�ڸ� : |" << std::setw(20) << std::right << std::fixed << std::setprecision(4) << f << "|" << std::endl; // ����	
			std::cout << "�ε��Ҽ���,�Ҽ��� 4�ڸ� : |" << std::setw(20) << std::left << std::setprecision(4) << f << "|" << std::endl; // ����	

		}
	#endif
#endif