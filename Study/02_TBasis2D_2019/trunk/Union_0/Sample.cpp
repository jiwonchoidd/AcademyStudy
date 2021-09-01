#include <cstdio>
#include <memory.h>
union TData
{
	char a;
	int b;
	double c;
};

struct TData2
{
	int   f; // 4���δ�	
	
	short c; // 2+ 2
	
	int   f2;// 4
	
	short d;	
	char a;
	char u;
	
	short s; // 2+2
};
// ��Ʈ�ʵ屸��ü
struct TBool{	
	unsigned char a : 1;
	unsigned char b : 1;
	unsigned char c : 1;
	unsigned char d : 1;
	unsigned char e : 1;
	unsigned char f : 1;
	unsigned char g : 1;
	unsigned char h : 1;
};
//��Ʈ�ʵ屸��ü�� ����ü
typedef struct {
	union {
		struct {
			unsigned long Zone : 28;
			unsigned long Level : 4;
		};
		unsigned long Value;
	};
} DATA, *PDATA;

typedef struct in_addr {
	union {
		struct { unsigned char s_b1, s_b2, s_b3, s_b4; } S_un_b;
		struct { unsigned short s_w1, s_w2; } S_un_w;
		unsigned long S_addr;
	} S_un;
#define s_addr  S_un.S_addr			
#define s_host  S_un.S_un_b.s_b2    
#define s_net   S_un.S_un_b.s_b1    
#define s_imp   S_un.S_un_w.s_w2    
#define s_impno S_un.S_un_b.s_b4    
#define s_lh    S_un.S_un_b.s_b3    
} IN_ADDR, *PIN_ADDR;


void main()
{	
	// �޸𸮸� ���� ���鼭 �� ���
	TData data;
	int iSize = sizeof(data);
	data.a = 65;
	printf("\n%c", data.a);
	data.b = 100;
	printf("\n%d", data.b);
	data.c = 3.14;
	printf("\n%f", data.c);

	TBool BData;
	iSize = sizeof(BData);
	memset(&BData, 0, sizeof(BData));
	
	BData.a = 0;
	BData.b = 1; // 0001
	BData.c = 2; // 0010
	BData.d = 3; // 0011
	printf("%d %d %d %d",
		BData.a, BData.b,
		BData.c, BData.d);

	TData2 data1;
	iSize = sizeof(data1);

	// ��Ʈ �ʵ� ����ü�� ����ϴ� ����ü
	DATA dt;
	memset(&dt, 0, sizeof(DATA));
	dt.Value = 0x55667788;
	printf("\n %d", dt.Zone);
	printf(" %d", dt.Level);

	IN_ADDR addr;
	// ����, AMD cpu�� Little-Endian���� ����ȴ�. ������ ���� �������� ����Ǵ� ���.
	// �ֻ��� �����Ͱ� ���� ���� �ּҿ� ����Ǵ� ���.
	// ������̶� ����Ʈ�� ����Ǵ� ������ ���Ѵ�.

	// �ݴ�� Big-Endian�̶���.(unix) ��Ʈ��ũ���� ���ȴ�.
	// ���� ���� ���������� �����ϴ� ������ ����.
	// �ֻ��� �����Ͱ� ���� ���� �ּҿ� ����Ǵ� ���.
	// ���� ���� : �ƽ�Ű �ڵ�� ���� ���� ����Ʈ ���� �����ʹ� ����� ���¿� ���� ������ ����.

	addr.S_un.S_addr = 0x6601a8c0;// "192(c0).168(a8).1(01).102(66)";
	printf("\n%d", addr.S_un.S_un_b.s_b1);
	printf(" %d", addr.S_un.S_un_b.s_b2);
	printf(" %d", addr.S_un.S_un_b.s_b3);
	printf(" %d", addr.S_un.S_un_b.s_b4);

	// ����Ʈ �����ؼ� ����
	addr.S_un.S_un_b.s_b2 = 0xa9;
	printf("\n%x", addr.S_un.S_addr);
	printf("\n%d", addr.S_un.S_un_b.s_b1);
	printf(" %d", addr.S_un.S_un_b.s_b2);
	printf(" %d", addr.S_un.S_un_b.s_b3);
	printf(" %d", addr.S_un.S_un_b.s_b4);
		
}