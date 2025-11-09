#pragma once

#include "gameobject.h"
#include <sgg/graphics.h>
#include "box.h"
#include "timer.cpp"

class Zombie : public Box, public GameObject
{
	std::vector<std::string> m_sprites;

	graphics::Brush m_brush_zombie;

	const float m_accel_horizontal = 12.0f;
	const float m_accel_vertical = 300.1f;
	const float m_max_velocity = 5.0f;
	const float m_gravity = 10.0f;

	Timer timer;
	int timeToTurn = 0;//it determines how many seconds has to pass until it's time for the zombie to turn
	float move = 1;//it determines the direction of the zombie

public:
	float m_vx = 0.0f;
	float m_vy = 0.0f;

public:
	void update(float dt) override;
	void draw() override;
	void init() override;

	bool notTired();//it returns if a second has passed or not (it is used for the collisions with the player)

	Zombie(std::string name, float x);

protected:
	void debugDraw();
	void moveZombie(float dt);

private:
	float getRandom();//returns a random float number between 3 to 12

};
