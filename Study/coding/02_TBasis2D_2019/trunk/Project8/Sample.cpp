#include <stdio.h>
// �ƽ�Ű�ڵ尪 0 ~ 126���� ����ȭ �Ǿ��ִ�.
void main()
{
	for (int i = 65; i <= 90; i++)
	{
		printf("%c", i);
	}
	printf("\n");
	for (int i = 97; i <= 122; i++)
	{
		printf("%c", i);
	}

	printf("\n%s", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	//���ڿ� ������ NULL���ڰ� �־�� �Ѥ�.
	//���ĺ����� + 1(�ι���)= 27
	char alphabat[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	printf("\n%s", alphabat);
	printf("\n");

	// ���ڿ��� ��� ���� ������ ����Ѵ�.
	for (int i = 0; i < 27; i++)
	{
		printf("%c", alphabat[i]);
	}	
}