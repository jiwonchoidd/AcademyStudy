#include "Maze.h"
// �� ������ ������ ���� �������� ���� �ٸ� ǥ�� ����� ���� �� �ִ�.
// ��ü�� �����ϴ� �κ��� ���� �����ϰ�, �̸� ���������ν�
// ��ü ��ü�� �����ϸ�, ������ ��ü�� ������ �ս��� �߰�, Ȯ���� 
// �����ϰ� ��ȵ� �����̴�.
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
			Room* room = new Room(n); // ���� ����
			_curentMaze->AddRoom(room);// ���� �̷ο� �߰�
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