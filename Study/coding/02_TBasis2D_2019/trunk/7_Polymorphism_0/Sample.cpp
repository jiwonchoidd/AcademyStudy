// 9장 클래스 상속활용 테크닉
#include "TEllipse.h"
#include "TTriangle.h"
#include "TRectangle.h"

int main(void)
{
	//TEllipse ellipse(30, 20, 50, 20);
	//TTriangle triangle(10, 10, 20, 30);
	//TRectangle rectangle(20, 30, 10, 20);

	//ellipse.Draw();		// 타원 그리기
	//triangle.Draw();		// 삼각형 그리기
	//rectangle.Draw();		// 사각형 그리기

	const int FIGURES=3;

	TFigure *figures[FIGURES] = {
		new TTriangle(10, 10, 20, 30),
		new TRectangle(20, 30, 10, 20),
		new TEllipse(30, 20, 50, 20),				
	};

	for (int i = 0; i< FIGURES; i++)
		figures[i]->Draw();		// 저장된 모든 도형 그리기

	for (int i = 0; i< FIGURES; i++)
		delete figures[i];

	return 0;
}
