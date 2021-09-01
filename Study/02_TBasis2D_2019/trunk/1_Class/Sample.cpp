// 7장 클래스와 객체에 능숙해지기
#include "TPoint.h"

int main(void)
{
	TPoint p1, p2;		// 점의 좌표를 저장할 변수 선언

	p1.SetPosition(10, 20);	// p1의 좌표 설정
	p2.SetPosition(50, 60);	// p2의 좌표 설정

	p1.Move(5, 0);		// p1의 좌표 이동
	p2.Move(0, 5);		// p2의 좌표 이동

	p1.Show();		// p1의 좌표를 출력
	p2.Show();		// p2의 좌표를 출력

	return 0;
}
