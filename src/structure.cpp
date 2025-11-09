#include "structure.h"
#include "util.h"

void Structure::draw()
{
	float x = m_pos_x + m_state->m_global_offset_x;
	float y = m_pos_y + m_state->m_global_offset_y;

	m_brush_structure.texture = m_state->getFullAssetPath(photo + ".png");

	graphics::drawRect(tx * x, ty * y, w * m_width, h * m_height, m_brush_structure); //float ty measures how far will be the photo from the box on y
																					 //float tx measures how far will be the photo from the box on x
																					 //float h measures the height of the photo compared to the box
																				  	 //float w measures the width of the photo compared to the box
	if (m_state->m_debugging)
		debugDraw();
}

void Structure::init()
{
	m_brush_structure.fill_opacity = 1.0f;
	m_brush_structure.outline_opacity = 0.0f;

	m_brush_structure.texture = m_state->getFullAssetPath(photo + ".png");
}

void Structure::debugDraw()
{
	float x = m_pos_x + m_state->m_global_offset_x;
	float y = m_pos_y + m_state->m_global_offset_y;

	graphics::Brush m_brush_structure_debug;
	SETCOLOR(m_brush_structure_debug.fill_color, 0.1f, 1.0f, 0.1f);
	SETCOLOR(m_brush_structure_debug.outline_color, 0.3f, 1.0f, 0.2f);
	m_brush_structure_debug.fill_opacity = 0.1f;
	m_brush_structure_debug.outline_opacity = 1.0f;
	graphics::drawRect(x, y, m_width, m_height, m_brush_structure_debug);
}
