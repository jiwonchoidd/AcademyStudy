//flyweight는 '(권투·레슬링 등의) 플라이급 선수(보통 체중 48~51kg 사이)'를 말한다. 즉 가벼운 것이다
//flyweight 패턴은 메모리 사용량을 줄이기 위한 방법으로, 
//인스턴스를 필요한 대로 다 만들어 쓰지 말고, 
//동일한 것은 가능하면 공유해서 객체 생성을 줄이자는 것이다.

#include <iostream>
using namespace std;

class EnemyImage 
{
public:
	virtual void Display(int x, int y) = 0;
protected:
	EnemyImage() {}
	EnemyImage(const EnemyImage& rhs);
};

class EnemyNoActionImage : public EnemyImage 
{
public:
	static EnemyImage * CreateInstance() 
	{
		if (pInstance_ == 0) {
			pInstance_ = new EnemyNoActionImage;
			cout << "EnemyNoActionImage" << endl;
		}
		return pInstance_;
	}

	void Display(int x, int y) 
	{
		// -- x,y 위치에 비트맵 이미지 표시
	}
protected:
	EnemyNoActionImage() { }
	EnemyNoActionImage(const EnemyNoActionImage& rhs);
	static EnemyImage * pInstance_;
};

EnemyImage* EnemyNoActionImage::pInstance_ = 0;

class EnemyMoveImage : public EnemyImage 
{
public:
	static EnemyImage * CreateInstance() 
	{
		if (pInstance_ == 0) {
			pInstance_ = new EnemyMoveImage;
			cout << "EnemyMoveActionImage" << endl;
		}
		return pInstance_;
	}

	void Display(int x, int y) 
	{
		// -- x,y 위치에 비트맵 이미지 표시
	}
protected:
	EnemyMoveImage() { }
	EnemyMoveImage(const EnemyMoveImage& rhs);
	static EnemyImage * pInstance_;
};

EnemyImage* EnemyMoveImage::pInstance_ = 0;

class EnemyAttackImage : public EnemyImage
{
public:
	static EnemyImage * CreateInstance() 
	{
		if (pInstance_ == 0) {
			pInstance_ = new EnemyAttackImage;
			cout << "EnemyAttackActionImage" << endl;
		}
		return pInstance_;
	}

	void Display(int x, int y)
	{
		// -- x,y 위치에 비트맵 이미지 표시
	}
protected:
	EnemyAttackImage() { }
	EnemyAttackImage(const EnemyAttackImage& rhs);
	static EnemyImage * pInstance_;
};

EnemyImage* EnemyAttackImage::pInstance_ = 0;

class EnemyDieImage : public EnemyImage 
{
public:
	static EnemyImage * CreateInstance() 
	{
		if (pInstance_ == 0) {
			pInstance_ = new EnemyDieImage;
			cout << "EnemyDieActionImage" << endl;
		}
		return pInstance_;
	}

	void Display(int x, int y) 
	{
		// -- x,y 위치에 비트맵 이미지 표시
	}
protected:
	EnemyDieImage() { }
	EnemyDieImage(const EnemyDieImage& rhs);
	static EnemyImage * pInstance_;
};

EnemyImage* EnemyDieImage::pInstance_ = 0;

class Enemy 
{
public:
	Enemy(int x, int y) 
	{
		curX_ = x; curY_ = y;
		pCurImage_ = EnemyNoActionImage::CreateInstance();
	}

	void Move(int x, int y) 
	{
		curX_ = x; curY_ = y;
		pCurImage_ = EnemyMoveImage::CreateInstance();
	}

	void Attack() 
	{
		pCurImage_ = EnemyAttackImage::CreateInstance();
	}

	void Display() 
	{
		pCurImage_->Display(curX_, curY_);
	}

private:
	int curX_, curY_;
	EnemyImage * pCurImage_;
};

void main()
{
	Enemy e1(10, 10), e2(20, 20);

	e1.Move(30, 30);
	e2.Attack();
	e2.Move(40, 40);
}