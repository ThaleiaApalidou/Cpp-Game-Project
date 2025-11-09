#include "heal.h"


Heal::Heal(std::string name, float x) : GameObject(name)
{
	m_pos_x = x;

	m_pos_y = 3.8f;
	m_width = 0.3f;
	m_height = 0.5f;
	
	Structure::photo = "sprite";
	Structure::h = 1.0;
	Structure::ty = 1.0f;
	Structure::w = 1.4;
	Structure::tx = 1.0f;
}
