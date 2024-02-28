#include "Powerup.h"
#include <string.h>

Powerup::Powerup(const char _name[30], float _health, float _attack, float _defence)
	: m_name{ _name[30] }, m_healthMultiplier{ _health }, m_attackMultiplier{ _attack }, m_defenceMultiplier{_defence}
{
	strcpy_s(m_name, 30, _name);
}

Powerup::~Powerup()
{
}

char* Powerup::getName()
{
	return m_name;
}

float Powerup::getHealthMultiplier()
{
	return m_healthMultiplier;
}

float Powerup::getAtackMultiplier()
{
	return m_attackMultiplier;
}

float Powerup::getDefenceMultiplier()
{
	return m_defenceMultiplier;
}

bool Powerup::compare(const Powerup& p1, const Powerup& p2)
{
	return (strcmp(p1.m_name, p2.m_name) < 0) ? true : false;
}
