//msdn->https://msdn.microsoft.com/ko-kr/library/2e6a4at9.aspx
#include <iostream>
//#define _cplusplus
using namespace std;

#ifndef _cplusplus
	struct Point {
		int x, y;
	};
	void SetPosition(struct Point *pPoint, int _x, int _y) {
		pPoint->x = _x;
		pPoint->y = _y;
	}
	void Move(struct Point *pPoint, int _x, int _y) {
		pPoint->x += _x;
		pPoint->y += _y;
	}
	void Show(const struct Point *pPoint) {
		printf("(%d, %d)\n", pPoint->x, pPoint->y);
	}
	int main(void)
	{
		struct Point p1, p2;
		SetPosition(&p1, 10, 20);
		SetPosition(&p2, 50, 60);
		Move(&p1, 5, 0);
		Move(&p2, 0, 5);
		Show(&p1);
		Show(&p2);
		return 0;
	}
#else
	struct Point {
		int x, y;
		void SetPosition(int _x, int _y);
		void Move(int _x, int _y);
		void Show(void);
	};
	void Point::SetPosition(int _x, int _y) {
		x = _x;
		y = _y;
	}
	void Point::Move(int _x, int _y) {
		x += _x;
		y += _y;
	}
	void Point::Show(void) {
		cout << "(" << x << ", " << y << ")" << endl;
	}
	int main(void)
	{
		Point p1, p2;
		p1.SetPosition(10, 20);
		p2.SetPosition(50, 60);

		p1.Move(5, 0);
		p2.Move(0, 5);
		p1.Show();
		p2.Show();

		return 0;
	}

#endif


