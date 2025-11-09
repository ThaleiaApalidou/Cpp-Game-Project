#pragma once

#include "structure.h"

class Portal : public Structure, public GameObject//is a derived class and it's base is the class Structure
{												  //when the player gets in the portal gains +100 score
public:
	Portal(std::string name, float x);
	~Portal() {}

};