#include "Room.h"
#include "GameDefines.h"
#include <iostream>

Room::Room()
	: m_type{ EMPTY }, m_mapPosition{ 0,0 }
{
}

Room::~Room()
{
}

void Room::setPosition(Point2D _position)
{
	m_mapPosition = _position;
}

void Room::setType(int _type)
{
	m_type = _type;
}

int Room::getType()
{
	return m_type;
}

void Room::draw()
{
	int outX = INDENT_X + (6 * m_mapPosition.x) + 1;
	int outY = MAP_Y + m_mapPosition.y;

	// jump to correct location
	std::cout << CSI << outY << ";" << outX << "H";
	// draw the room
	switch (m_type)
	{
	case EMPTY:
		std::cout << "[ " << GREEN << "\xb0" << RESET_COLOR << " ]";
		break;
	case ENEMY:
		std::cout << "[ " << RED << "\x94" << RESET_COLOR << " ]";
		break;
	case TREASURE_HP:
	case TREASURE_AT:
	case TREASURE_DF:
		std::cout << "[ " << YELLOW << "$" << RESET_COLOR << " ]";
		break;
	case FOOD:
		std::cout << "[ " << WHITE << "\xcf" << RESET_COLOR << " ]";
		break;
	case ENTRANCE:
		std::cout << "[ " << WHITE << "\x9d" << RESET_COLOR << " ]";
		break;
	case EXIT:
		std::cout << "[ " << WHITE << "\xFE" << RESET_COLOR << " ]";
		break;
	}
}

void Room::drawDescription()
{
	std::cout << RESET_COLOR;								// reset draw colours
	std::cout << CSI << ROOM_DESC_Y << ";" << 0 << "H";		// Jump to correct pos
	// delete 4 lines and insert 4 empty lines
	std::cout << CSI << "4M" << CSI << "4L" << std::endl;
	// Write room Description
	switch (m_type)
	{
	case EMPTY:
		std::cout << INDENT << "You are in an empty meadow. There is nothing of note here." << std::endl;
		break;
	case ENEMY:
		std::cout << INDENT << RED << "BEWARE." << RESET_COLOR << " An Enemy is approaching." << std::endl;
		break;
	case TREASURE_HP:
	case TREASURE_AT:
	case TREASURE_DF:
		std::cout << INDENT << "There appears to be some " << YELLOW << "Treasure" << RESET_COLOR << " here. Perhaps you should investigate further." << std::endl;
		break;
	case FOOD:
		std::cout << INDENT << "At last! you collect some food to sustain you on your journey." << std::endl;
		break;
	case ENTRANCE:
		std::cout << INDENT << "The entrance you used to enter this maze is " << RED << "Blocked" << RESET_COLOR << ". There is no going back now." << std::endl;
		break;
	case EXIT:
		std::cout << INDENT << "Despite all odds, you made it to the exit. " << YELLOW << "Congratulations!" << RESET_COLOR << std::endl;
		break;
	}
}

bool Room::executeCommand(int _command)
{
	switch (_command)
	{
	case LOOK:
		if (m_type == TREASURE_HP || m_type == TREASURE_AT || m_type == TREASURE_DF) 
		{
			std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "There is " << YELLOW << "Treasure" << RESET_COLOR 
				<< " here. It looks small enough to pick up." << std::endl;
		}
		else
		{
			std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You look around, but see nothing worth mentioning." << std::endl;
		}
		std::cout << INDENT << "Press 'Enter' to continue.";
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cin.get();
		return true;
	case FIGHT:
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You could try to fight, but you do not have a weapon." << std::endl;
		std::cout << INDENT << "Press 'Enter' to continue.";
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cin.get();
		return true;
	default:
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You try, but you just can't do it." << std::endl;
		std::cout << INDENT << "Press 'Enter' to continue.";
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cin.get();
		break;
	}
	return false;
}
