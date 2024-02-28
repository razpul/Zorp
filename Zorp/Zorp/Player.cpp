#include "Player.h"
#include "GameDefines.h"
#include <iostream>
#include <algorithm>
#include "Room.h"


Player::Player()
	: m_mapPosition{ 0, 0 }, m_healthPoints{ 100 }, m_attackPoints{ 20 }, m_defencePoints{20}
{
}

Player::Player(int _x, int _y)
	: m_mapPosition{_x, _y}, m_healthPoints{ 100 }, m_attackPoints{ 20 }, m_defencePoints{ 20 }
{
}

Player::~Player()
{
}

void Player::setPosition(Point2D _position)
{
	m_mapPosition = _position;
}

Point2D Player::getPosition()
{
	return m_mapPosition;
}

void Player::draw()
{
	Point2D outPos = {
		INDENT_X + (6 * m_mapPosition.x) + 3,
		MAP_Y + m_mapPosition.y };

	// Draw the playes current position on teh map
	// Move cursor to map pos ad delete character at current position
	std::cout << CSI << outPos.y << ";" << outPos.x << "H";
	std::cout << MAGENTA << "\x81" << RESET_COLOR;

	std::cout << INVENTORY_OUTPUT_POS;
	for (int i = 0; i < m_powerups.size(); i++)
	{
		std::cout << m_powerups[i].getName() << '\t';
	}

}

bool Player::consume(Room* _room)
{
	static const char itemNames[15][30] = {
		"Apple", "Mango", "Peach", "Pear", "Banana", "Chocolate", "Cookie", "Steak", "Roast Lamb", "Chicken", "Golden Paste",
		"Enchanted Lolipop", "Sweet Stapler", "Raw Egg", "Burned Toast"
	};
	int item = rand() % 15;
	char name[30] = "";
	strncat_s(name, itemNames[item], 30);
	std::cout << EXTRA_OUTPUT_POS << "You hungrily devour the " << name;
	if (m_healthPoints < 200) 
	{
		int restoredHealth = std::min(20, (200-m_healthPoints));
		std::cout << " and gain " << restoredHealth << " Health Points" << std::endl;
		m_healthPoints += restoredHealth;
	}
	else
	{
		std::cout << " It was Delicious, but your health is already full." << std::endl;
	}
	// Set the room type to EMPTY
	_room->setType(EMPTY);
	std::cout << INDENT << "Press 'Enter' to continue.";
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();
	return true;
}


bool Player::pickup(Room* _room) 
{
	static const char itemNames[15][30] = {
	"indifference", "invisibility", "invulnerability", "incontinence",
	"improbability", "infertility", "impatience", "indecision", "inspiration",
	"independence", "incurability", "integration", "invocation",
	"inferno", "indigestion"
	};
	int item = rand() % 15;
	char name[30] = "";
	float hpMod = 1.0f;
	float atMod = 1.0f;
	float dfMod = 1.0f;

	switch (_room->getType()) 
	{
	case TREASURE_HP:
		strcpy_s(name, "Potion of ");
		hpMod = 2.0f;
		break;
	case TREASURE_AT:
		strcpy_s(name, "Sword of ");
		atMod = 2.0f;
		break;
	case TREASURE_DF:
		strcpy_s(name, "Shield of ");
		dfMod = 2.0f;
		break;
	default:
		return false;
	}

	// append the item name to the string
	strncat_s(name, itemNames[item], 30);
	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You pick up the " << name << std::endl;
	m_powerups.emplace_back(Powerup(name, hpMod, atMod, dfMod));

	std::sort(m_powerups.begin(), m_powerups.end(), Powerup::compare);
	// Set the room type to EMPTY
	_room->setType(EMPTY);

	std::cout << INDENT << "Press 'Enter' to continue.";
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();
	return true;


}



bool Player::executeCommand(int _command, Room* _pRoom)
{
	switch (_command)
	{
	case EAST:
		if (m_mapPosition.x < MAZE_WIDTH - 1)
			m_mapPosition.x++;
		return true;
	case WEST:
		if (m_mapPosition.x > 0)
			m_mapPosition.x--;
		return true;
	case NORTH:
		if (m_mapPosition.y > 0)
			m_mapPosition.y--;
		return true;
	case SOUTH:
		if (m_mapPosition.y < MAZE_HEIGHT - 1)
			m_mapPosition.y++;
		return true;
	case PICKUP:
		return pickup(_pRoom);
	case CONSUME:
		return consume(_pRoom);
	case SUICIDE:
		m_healthPoints = 0;
		return true;
	}
	return false;
}
