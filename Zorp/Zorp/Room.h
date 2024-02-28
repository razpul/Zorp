#pragma once
#include "Point2D.h"

class Room
{
public:
	Room();
	~Room();

public:
	void setPosition(Point2D _position);
	void setType(int _type);

	int getType();

	void draw();
	void drawDescription();

	bool executeCommand(int _command);

private:
	int m_type;
	Point2D m_mapPosition;


};

