#include "portal.h"


Portal::Portal(std::string name, float x) : GameObject(name)
{
	m_pos_x = x;

	m_pos_y = 4.0f;
	m_width = 1.0f;
	m_height = 1.0f;
	
	Structure::photo = "portal";
	Structure::h = 2.0;
	Structure::ty = 1.0f;
	Structure::w = 2.0;
	Structure::tx = 1.0f;
}
