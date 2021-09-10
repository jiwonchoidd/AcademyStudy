#include "Maze.h"
class BombedWall : public Wall
{
public:
	BombedWall() {};
	BombedWall(const BombedWall& other) : Wall(other)
	{
		_bomb = other._bomb;
	}
	virtual Wall* Clone() const
	{
		return new BombedWall(*this);
	}
private:
	bool _bomb;
};
class MazeFactory {
public:

	
	virtual Maze* MakeMaze() const {
		return new Maze;
	}
	virtual Wall* MakeWall() const {
		return new Wall;
	}
	virtual Room* MakeRoom(int n) const {
		return new Room(n);
	}
	virtual Door* MakeDoor(Room* r1, Room* r2) const {
		return new Door(r1, r2);
	}
	MazeFactory() {}
};


class MazePrototypeFactory : public MazeFactory {
public:
	MazePrototypeFactory(Maze* maze, Wall* wall, Room* room, Door* door) {
		_prototypeMaze = maze;
		_prototypeRoom = room;
		_prototypeWall = wall;
		_prototypeDoor = door;
	}
	virtual Wall* MakeWall() {
		return _prototypeWall->Clone();
	};
	virtual Door* MakeDoor(Room* r1, Room * r2) {
		Door* door = _prototypeDoor->Clone();
		door->Initialize(r1, r2);
		return door;
	};
private:
	Maze * _prototypeMaze;
	Room * _prototypeRoom;
	Wall * _prototypeWall;
	Door * _prototypeDoor;
};
class MazeGame
{
public:
	static MazeGame* Instance();
	// AbstractFactory
	Maze * CreateMaze(MazePrototypeFactory* factory)
	{
		Maze* aMaze = factory->MakeMaze();
		Room* r1 = factory->MakeRoom(1);
		Room* r2 = factory->MakeRoom(2);
		Door* theDoor = factory->MakeDoor(r1, r2);
		aMaze->AddRoom(r1);
		aMaze->AddRoom(r2);

		r1->SetSide(North, factory->MakeWall());
		r1->SetSide(East, theDoor);
		r1->SetSide(South, factory->MakeWall());
		r1->SetSide(West, factory->MakeWall());

		r2->SetSide(North, factory->MakeWall());
		r2->SetSide(East, factory->MakeWall());
		r2->SetSide(South, factory->MakeWall());
		r2->SetSide(West, theDoor);
		return aMaze;
	}
protected:
	MazeGame() {}
private:
	static MazeGame* _instance;
};
MazeGame* MazeGame::_instance = 0;
MazeGame* MazeGame::Instance()
{
	if (_instance == 0)
	{
		_instance = new MazeGame;
	}
	return _instance;
}
void main()
{
	MazeGame* game = MazeGame::Instance();
	MazePrototypeFactory simpleMazeFactory(new Maze, new Wall, new Room, new Door);
	Maze* maze = game->CreateMaze(&simpleMazeFactory);
}