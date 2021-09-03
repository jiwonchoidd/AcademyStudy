#include "Maze.h"
// 이 패턴은 동일한 생성 절차에서 서로 다른 표현 결과를 만들 수 있다.
// 객체를 구성하는 부분을 먼저 생성하고, 이를 조합함으로써
// 전체 객체를 생성하며, 생성할 객체의 종류가 손쉽게 추가, 확장이 
// 가능하게 고안된 패턴이다.
class MazeBuilder
{
public:
	virtual void BuildMaze() {}
	virtual void BuildRoom(int room) {}
	virtual void BuildDoor(int from, int to) {}
	virtual Maze* GetMaze() {return 0;}
public:
	MazeBuilder() {}
};
class StandardMazebuiler : public MazeBuilder
{
public:
	virtual void BuildMaze() { _curentMaze = new Maze; }
	virtual void BuildRoom(int n) 
	{
		if (!_curentMaze->RoomNo(n))
		{
			Room* room = new Room(n); // 방을 생성
			_curentMaze->AddRoom(room);// 방을 미로에 추가
			room->SetSide(North, new Wall);
			room->SetSide(South, new Wall);
			room->SetSide(East, new Wall);
			room->SetSide(West, new Wall);
		}
	}
	virtual void BuildDoor(int n1, int n2) 
	{
		Room* r1 = _curentMaze->RoomNo(n1);
		Room* r2 = _curentMaze->RoomNo(n2);
		Door* d = new Door(r1, r2);
		r1->SetSide(CommonWall(r1, r2), d);
		r1->SetSide(CommonWall(r2, r1), d);
	}
	virtual Maze* GetMaze() { return _curentMaze; }
private:
	Direction CommonWall(Room* r1, Room* r2) 
	{ 
		return (Direction)(rand () % 4);
	};
		
	Maze* _curentMaze;
public:
	StandardMazebuiler()
	{
		_curentMaze = 0;
	}
};
class MazeGame
{
public:
	Maze * CreateMaze(MazeBuilder& builder)
	{
		builder.BuildMaze();
		builder.BuildRoom(1);
		builder.BuildRoom(2);
		builder.BuildDoor(1,2);
		return builder.GetMaze();
	}
};
void main()
{
	MazeGame mg;
	StandardMazebuiler builder;
	mg.CreateMaze(builder);
	Maze* maze = builder.GetMaze();
}