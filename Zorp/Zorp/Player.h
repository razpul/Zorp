#pragma once
#include "Point2D.h"
#include "Powerup.h"
#include <vector>
class Room;

class Player
{
public:
	Player();
	Player(int _x, int _y);
	~Player();

	void setPosition(Point2D _position);
	
	Point2D getPosition();

	void draw();

	bool executeCommand(int _command, Room* _pRoom);
private:
	bool consume(Room* _room);
	bool pickup(Room* _room);

private:
	Point2D m_mapPosition;

	std::vector<Powerup> m_powerups;

	int m_healthPoints;
	int m_attackPoints;
	int m_defencePoints;

};

