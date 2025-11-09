#include "flat.h"


Flat::Flat(std::string name, float x) : GameObject(name)
{
	m_pos_x = x;

	m_pos_y = 3.3f;
	m_width = 4.0f;
	m_height = 0.2f;
	
	Structure::photo = "flat";
	Structure::h = 20.0;
	Structure::ty = 1.2f;
	Structure::w = 1.0;
	Structure::tx = 1.0f;
}
