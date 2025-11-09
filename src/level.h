#pragma once

#include "gameobject.h"
#include <vector>
#include <list>
#include <string>
#include <sgg/graphics.h>
#include "player.h"
#include "zombie.h"
#include "structure.h"
#include "flat.h"
#include "dumpster.h"
#include "portal.h"
#include "heal.h"

class Level : public GameObject
{
	graphics::Brush m_brush_background;

    std::vector<Structure*> m_static_list;
    std::list<Structure*> m_heal_list;
    std::vector<Structure*> m_portal_list;
    std::list<Zombie*> m_zombie_list;

	// add some collidable blocks
	std::vector<Box> m_blocks;
	std::vector<std::string> m_block_names;
	const float m_block_size = 0.6f;
	graphics::Brush m_block_brush;
	graphics::Brush m_block_brush_debug;

	float m_center_x = 5.0f;
	float m_center_y = 5.0f; 

	int timeToTurn = 0;

	// dedicated method to draw a block
	void drawBlock(int i);

	// detect collisions
	void checkCollisions();

public:
	void update(float dt) override;
	void draw() override;
	void init() override;

	Level(const std::string & name = "Level0");
	~Level() override;

protected:
    void generate_map();//initializes all the structures and zombies in the map
	void respond_heal();

};
