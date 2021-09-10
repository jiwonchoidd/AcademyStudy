#include "Maze.h"
//상위 클래스에서는 인스턴스를 만드는 방법만 결정하고, 구체적인 클래스 이름은 뒤로 미룬다.
// 어떤 클래스의 인스턴스를 생성할 지에 대한 결정은 서브클래스가 결정한다.
//1)어떤 클래스가 자신이 생성해야 하는 객체의 클래스를 예측할 수 없을 때
//2)생성할 객체를 기술하는 책임을 자신의 서브클래스가 지정했으면 할 때
//3)객체 생성의 책임을 몇 개의 보조 서브클래스 가운데 하낭게 위임하고,
// 어떤 서브클래스가 위임자인지에 대한 정보를 국소화시키고 싶을 때
class Spell {
public:
	Spell() {};
};
class EnchantedRoom : public Room
{
public:
	EnchantedRoom(int n, Spell*) : Room(n)
	{
	}
};
class BombedWall: public Wall
{
public:
	BombedWall() {}
};
class BombedWithRoom : public Room
{
public:
	BombedWithRoom(int n): Room(n){}
};
class DoorNeedingSpell : public Door
{
public:
	DoorNeedingSpell(Room* r1, Room* r2) : Door(r1, r2)
	{
	}
};
class MazeGame
{
public:
	Maze * CreateMaze()
	{
		Maze* aMaze = MakeMaze();
		Room* r1 = MakeRoom(1);
		Room* r2 = MakeRoom(2);
		Door* theDoor = MakeDoor(r1, r2);
		aMaze->AddRoom(r1);
		aMaze->AddRoom(r2);

		r1->SetSide(North, MakeWall());
		r1->SetSide(East, theDoor);
		r1->SetSide(South, MakeWall());
		r1->SetSide(West, MakeWall());

		r2->SetSide(North, MakeWall());
		r2->SetSide(East, MakeWall());
		r2->SetSide(South, MakeWall());
		r2->SetSide(West, theDoor);
		return aMaze;
	}
	// Factory Method
	virtual Maze* MakeMaze() { return new Maze; }
	virtual Room* MakeRoom(int n) { return new Room(n); }
	virtual Wall* MakeWall() { return new Wall; }
	virtual Door* MakeDoor(Room* r1, Room* r2) { return new Door(r1,r2); }
};
class BombedMazeGame : public MazeGame
{
public:
	virtual Wall* MakeWall()
	{
		return new BombedWall;
	}
	virtual Room* MakeRoom(int n)
	{
		return new BombedWithRoom(n);
	}
};
class EnchantedMazeGame : public MazeGame
{
public:
	virtual Door* MakeDoor(Room* r1, Room * r2)
	{
		return new DoorNeedingSpell(r1, r2);
	}
	virtual Room* MakeRoom(int n)
	{
		return new EnchantedRoom(n, CastSpell());
	}
protected:
	Spell * CastSpell() { return 0; };
};
void main()
{
	MazeGame mg;
	Maze* pMaze1 = mg.CreateMaze();

	BombedMazeGame bombedGame;
	Maze* pMaze2 = bombedGame.CreateMaze();
	EnchantedMazeGame EnchantedGame;
	Maze* pMaze3 = EnchantedGame.CreateMaze();

}