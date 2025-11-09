#include "zombie.h"
#include "util.h"
#include <cmath>
#include <thread>
#include <chrono> 
#include <cstdlib>  
#include <ctime>

void Zombie::update(float dt)
{
	float delta_time = dt / 1000.0f;

	moveZombie(dt);
}

void Zombie::draw()
{
	float x = m_pos_x + m_state->m_global_offset_x;
	float y = m_pos_y + m_state->m_global_offset_y;

	m_brush_zombie.texture = m_state->getFullAssetPath("zombie.png");
	graphics::drawRect(x, y, 1.0f, 1.0f, m_brush_zombie);

	if (m_state->m_debugging)
		debugDraw();
}

void Zombie::init()
{
	m_pos_y = 5.0f;

	m_brush_zombie.fill_opacity = 1.0f;
	m_brush_zombie.outline_opacity = 0.0f;

	m_brush_zombie.texture = m_state->getFullAssetPath("zombie.png");

	// Adjust width for finer collision detection
	m_width = 0.5f;

	move = (rand() > RAND_MAX / 2) ? -1 : 1;//it initialazes randomly the direction of the zombie
}

void Zombie::debugDraw()
{
	float x = m_pos_x + m_state->m_global_offset_x;
	float y = m_pos_y + m_state->m_global_offset_y;

	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 0.1f, 1.0f, 0.1f);
	SETCOLOR(debug_brush.outline_color, 0.3f, 1.0f, 0.2f);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(x, y, m_width, m_height, debug_brush);

	char s[20];
	sprintf_s(s, "(%5.2f, %5.2f)", x, y);
	SETCOLOR(debug_brush.fill_color, 0.1f, 1.0f, 0.1f);
	debug_brush.fill_opacity = 1.0f;
	graphics::drawText(x - 0.36f, y - 0.6f, 0.15f, s, debug_brush);
}


void Zombie::moveZombie(float dt)
{
	float delta_time = dt / 1000.0f;

	//The zombie walks for a random number of seconds(3 to 12) in one direction and when the timeToTurn has passed it changes direction
	//a timer is used to count the seconds and reset it when it is time to turn
	double timeThatHasPassed = timer.getElapsedSeconds();
	if (timeThatHasPassed >= timeToTurn) {
		timeToTurn = getRandom();
		move = move * (-1);//it changes its direction from left to right and from right to left
		timer.reset();
	}

	m_vx = std::min<float>(m_max_velocity, m_vx + delta_time * move * m_accel_horizontal);
	m_vx = std::max<float>(-m_max_velocity, m_vx);

	// friction
	m_vx -= 0.2f * m_vx / (0.1f + fabs(m_vx));

	// apply static friction threshold
	if (fabs(m_vx) < 0.01f)
		m_vx = 0.0f;

	// adjust horizontal position
	m_pos_x += m_vx * delta_time;
	
	//gravity
	m_vy += delta_time * m_gravity;
	m_pos_y += m_vy * delta_time;
}


float Zombie::getRandom()//returns a random float number between 3 to 12
{
	return (float)((rand() % 10) + 3);
}

bool Zombie::notTired()//it returns if a second has passed or not (it is used for the collisions with the player)
{
	double deltaTime = timer.getElapsedSeconds();
	if (deltaTime >= 1.0) {
		timer.reset();
		return true;
	}
	else {
		return false;
	}
}


Zombie::Zombie(std::string name, float x) : GameObject(name)
{
	m_pos_x = x;
}