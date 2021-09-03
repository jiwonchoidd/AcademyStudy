#include "TChild.h"
int main(void)
{
	TChild aBoy;
	aBoy.Character();		// Parent에서 상속 받은 함수 호출
	aBoy.Appearance();	// Parent에서 상속 받은 함수 호출
	aBoy.Wealth();		// Parent에서 상속 받은 함수 호출
	aBoy.Humanity();		// Child에서 추가된 함수 호출
	return 0;
}
