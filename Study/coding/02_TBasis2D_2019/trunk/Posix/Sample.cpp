//POSIX(������)�� �̽� ���� � ü�� �������̽�(portable operating system interface)�� ���ڷ�, 
//���� �ٸ� UNIX OS�� ���� API�� �����Ͽ� �̽ļ���
//���� ���н� ���� ���α׷��� �����ϱ� ���� �������� IEEE�� å���� ���ø����̼� �������̽� �԰��̴�.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <fcntl.h>
#include <io.h> //<fcntl.h>, <sys/types.h>, <sys/stat.h>
#include <wchar.h > // _wopen  io.h or wchar.h

int main(int argc, char *argv[])
{
	int fd1, fd2;
	char buf[BUFSIZ];
	int n;
	if (argc != 3) 
	{
		fprintf(stderr, "USAGE: %s sourcefile objectfile\n", *argv);
		return 1;
	}
	if ((fd1 = _open(*(argv + 1), O_RDONLY | O_BINARY)) < 0)
	{
		fprintf(stderr, "Error: Cannot open %s\n", *(argv + 1));
		return 2;
	}
	if ((fd2 = _open(*(argv + 2), O_WRONLY | O_TRUNC | O_CREAT | O_BINARY, 0700)) < 0)
	{
		fprintf(stderr, "Error: Cannot create %s\n", *(argv + 2));
		return 3;
	}
	printf("FileCopy %s to %s\n\n", *(argv + 1), *(argv + 2));
	while ((n = _read(fd1, buf, BUFSIZ)) > 0)
	{
		if (_write(fd2, buf, n) != n)
		{ 
			/* read�� ���������� �������� ȭ�� ����¿��� ����Լ��� write �ۿ� ����. 
			//���� ���� �����͸� ����ϱ⿡�� �������� ������ �Ǽ����� ����ϰ��� �� ������ 
			//�̸� ASCII �ڵ� ���·�, �� ���� ���·� ��ȯ�� ���� ��� �Ͽ��� �ϱ� ������ 
			//�׷��� ��쿡�� ������� ����� �Լ��� ����ϴ� ���� �� ���ϴ� */
			fprintf(stderr, "Error in writing %s\n", *(argv + 2));
			return 4;
		}
	}
	if (n < 0)
	{
		fprintf(stderr, "Error in reading %s\n", *(argv + 1));
	}
	_close(fd1);
	_close(fd2);
}



