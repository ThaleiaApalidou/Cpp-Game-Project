#pragma once

#include "structure.h"

class Dumpster : public Structure, public GameObject//is a derived class and it's base is the class Structure
{											   //is used as static object for the player to get on and avoid the zombies
public:
	Dumpster(std::string name, float x);
	~Dumpster() {}

};