#pragma once

#include "structure.h"

class Heal : public Structure, public GameObject//is a derived class and it's base is the class Structure
{											    //the player consume it to gain +20 life and +10 score
public:
	Heal(std::string name, float x);
	~Heal() {}

};