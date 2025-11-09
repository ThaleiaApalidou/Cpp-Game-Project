#pragma once

#include "gameobject.h"
#include <sgg/graphics.h>
#include "box.h"

class Structure : public Box, public GameObject//base class for the classes Dumpster, Flat, Portal and Heal
{											   //provides the methods draw(), init() and debugDraw()
	graphics::Brush m_brush_structure;
	graphics::Brush m_brush_structure_debug;
	
public:
	void draw() override;
	void init() override;

protected:
	void debugDraw();

protected:
	std::string photo;//asset's name
	float ty;//measures how far will be the photo from the box on y
	float tx;//measures how far will be the photo from the box on x
	float h;//measures the height of the photo compared to the box
	float w;//measures the width of the photo compared to the box

};
