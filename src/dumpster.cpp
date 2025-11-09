#include "dumpster.h"

Dumpster::Dumpster(std::string name, float x) : GameObject(name)
{
	m_pos_x = x;

	m_pos_y = 4.8f;
	m_width = 3.0f;
	m_height = 0.2f;

	Structure::photo = "dumpster";
	Structure::h = 13.0;
	Structure::ty = 1.0f;
	Structure::w = 1.0;
	Structure::tx = 1.0f;
}