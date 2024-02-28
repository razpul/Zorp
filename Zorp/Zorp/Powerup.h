#pragma once

class Powerup
{
public:
	Powerup(const char _name[30], float _health, float _attack, float _defence);
	~Powerup();

	char* getName();
	float getHealthMultiplier();
	float getAtackMultiplier();
	float getDefenceMultiplier();

	static bool compare(const Powerup& p1, const Powerup& p2);

private:
	char m_name[30];

	float m_healthMultiplier;
	float m_attackMultiplier;
	float m_defenceMultiplier;
};