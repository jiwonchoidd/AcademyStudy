#pragma once
#include <iostream>
#include <vector>
using namespace std;

enum Direction { North, East, South, West };
class MapSite {
public:
	virtual void Enter() = 0;
};
class Room : public MapSite
{
public:
	Room() {}
	Room(int RoomNo) { _roomNumber = RoomNo; };
	MapSite* GetSide(Direction dir)const { return _sides[dir]; }
	void SetSide(Direction dir, MapSite* pMapSize) {
		_sides[dir] = pMapSize;
	};
	virtual void Enter() {};
private:
	MapSite * _sides[4];
	int _roomNumber;
};
class Wall : public MapSite
{
public:
	Wall() {}
	virtual void Enter() {}
};
class Door : public MapSite
{
public:
	Door() {}
	Door(Room* a, Room* b) {
		_room1 = a; _room2 = b;
	}
	virtual void Enter() {}
	Room* OtherSideFrom(Room*) {}
private:
	Room * _room1;
	Room * _room2;
	bool _isOpen;
};
class Maze {
public:
	Maze() {}
	void AddRoom(Room* room) { m_RoomList.push_back(room); }
	Room* RoomNo(int no)const { return m_RoomList[no]; }
	vector<Room*> m_RoomList;
};

