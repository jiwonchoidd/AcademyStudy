//POSIX(포직스)는 이식 가능 운영 체제 인터페이스(portable operating system interface)의 약자로, 
//서로 다른 UNIX OS의 공통 API를 정리하여 이식성이
//높은 유닉스 응용 프로그램을 개발하기 위한 목적으로 IEEE가 책정한 애플리케이션 인터페이스 규격이다.
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
			/* read와 마찬가지로 저수준의 화일 입출력에서 출력함수는 write 밖에 없다. 
			//따라서 문자 데이터를 출력하기에는 괜찮지만 정수나 실수값을 출력하고자 할 때에는 
			//이를 ASCII 코드 형태로, 즉 문자 형태로 변환한 다음 출력 하여야 하기 때문에 
			//그러한 경우에는 고수준의 입출력 함수를 사용하는 것이 더 편리하다 */
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



