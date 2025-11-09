#include "level.h"
#include <sgg/graphics.h>
#include "player.h"
#include "util.h"
#include "zombie.h"
#include "structure.h"
#include <iostream>

using namespace std;


Level::Level(const std::string& name)
	: GameObject(name)
{
	generate_map();
}

void Level::update(float dt)
{
	//update player
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->update(dt);

	//update zombies
	for (auto p_gob : m_zombie_list)
		if (p_gob) p_gob->update(dt);

	checkCollisions();

	GameObject::update(dt);
}

void Level::draw()
{
	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();

	float offset_x = m_state->m_global_offset_x / 2.0f + w / 2.0f;
	float offset_y = h / 2.0f;

	//draw background
	graphics::drawRect(offset_x, offset_y, 4.0f * w, 1.0f * h, m_brush_background);

	//draw buildings
	for (auto p_gob : m_static_list)
		if (p_gob) p_gob->draw();

	//draw sprites
	for (auto p_gob : m_heal_list)
		if (p_gob) p_gob->draw();

	//draw safehouse
	for (auto p_gob : m_portal_list)
		if (p_gob) p_gob->draw();

	//draw zombies
	for (auto p_gob : m_zombie_list)
		if (p_gob) p_gob->draw();

	//draw player
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->draw();

	//draw blocks
	for (int i = 0; i < m_blocks.size(); i++)
	{
		drawBlock(i);
	}
}

void Level::init()
{
	//initialize zombies
	for (auto p_gob : m_zombie_list)
		if (p_gob) p_gob->init();

	//initialize cars and flats
	for (auto p_gob : m_static_list)
		if (p_gob) p_gob->init();

	//initialize heal
	for (auto p_gob : m_heal_list)
		if (p_gob) p_gob->init();

	//initialize safehouse
	for (auto p_gob : m_portal_list)
		if (p_gob) p_gob->init();
}

void Level::drawBlock(int i)
{
	Box& box = m_blocks[i];
	std::string& name = m_block_names[i];
	
	float x = box.m_pos_x +m_state->m_global_offset_x;
	float y = box.m_pos_y +m_state->m_global_offset_y;

	m_block_brush.texture = m_state->getFullAssetPath(name);

	graphics::drawRect(x, y, 1.8f * m_block_size, 1.8f * m_block_size, m_block_brush);

	if (m_state->m_debugging)
		graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush_debug);

}

void Level::checkCollisions()//if you are on the car press shift to fall down
{
	//player-blocks intersectDown//
	for (auto& block : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectDown(block))
		{
			m_state->getPlayer()->m_pos_y += offset;

			// add sound event
			if (m_state->getPlayer()->m_vy > 1.0f)
				graphics::playSound(m_state->getFullAssetPath("Metal2.wav"), 1.0f);
			
			m_state->getPlayer()->m_vy = 0.0f;
	
			break;
		}
	}

	//player-blocks intersectSideways//
	for (auto& block : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectSideways(block))
		{
			m_state->getPlayer()->m_pos_x += offset;

			m_state->getPlayer()->m_vx = 0.0f;
			break;
		}

	}

	//player-static srtuctures intersectDown//
	for (auto p_gob : m_static_list)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectDown(*p_gob))
		{
			m_state->getPlayer()->m_pos_y += offset;

			// add sound event
			if (m_state->getPlayer()->m_vy > 1.0f)
				graphics::playSound(m_state->getFullAssetPath("Metal2.wav"), 1.0f);

			m_state->getPlayer()->m_vy = 0.0f;
		}
	}

	//player-heal//
	//if the player consumes the heal he gains +20 life and +10 score and the heal gets deleted
	//the heal that gets consumed is inserted in a delete list and gets deleted after the iteration of the m_heal_list
	std::list<Structure*> delete_list;
	for (auto p_gob : m_heal_list) {
			
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersect(*p_gob))
		{
			if (p_gob->isActive()) {
				graphics::playSound(m_state->getFullAssetPath("heal.mp3"), 1.0f);
				m_state->getPlayer()->heal();
				delete_list.push_back(p_gob);
			}
		}

	}
	for (auto p_gob : delete_list)
	{
		m_heal_list.remove(p_gob);
		delete p_gob;
	}
	
	//player-portal//
	//when the player gets in the portal gains +100 score and initialize
	//also a heal responds
	for (auto p_gob : m_portal_list) {

		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersect(*p_gob))
		{
			//Game Ends

			// add sound event
			m_state->getPlayer()->success();
			graphics::playSound(m_state->getFullAssetPath("end.mp3"), 1.0f);
			m_state->getPlayer()->init();

			respond_heal();
		}
	}

	//zombies-blocks intersectDown//
	for (auto p_gob : m_zombie_list) {

		for (auto& block : m_blocks) 
		{
			float offset = 0.0f;
			if (offset = p_gob->intersectDown(block))
			{
				p_gob->m_pos_y += offset;

				p_gob->m_vy = 0.0f;

				break;
			}
		}
	}

	//zombies-blocks intersectSideways//
	for (auto p_gob : m_zombie_list) {
			
		for (auto& block : m_blocks)
		{
			float offset = 0.0f;
			if (offset = p_gob->intersectSideways(block))
			{
				p_gob->m_pos_x += offset;

				p_gob->m_vx = 0.0f;

				break;
			}
		}
	}
		
	//player-zombies//
	//if the player intersects with a zombie, the player loses -20 life
	//if he has lost all his life he initializes
	for (auto p_gob : m_zombie_list) {

		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersect(*p_gob))
		{
			if (p_gob->notTired()) {//after a collision the zombie waits one second to attack again
				graphics::playSound(m_state->getFullAssetPath("oof.mp3"), 1.0f);
				m_state->getPlayer()->damage();
				if (m_state->getPlayer()->getCurrentHP() == 0)
				{
					m_state->getPlayer()->death();
					m_state->getPlayer()->init();
				}
			}
		}
	}

	//player falls//
	//if the player falls he dies and initialaze
	if (m_state->getPlayer()->m_pos_y>10.0f) 
	{
		m_state->getPlayer()->death();
		m_state->getPlayer()->init();
	}

}


void Level::generate_map()
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("city.png");

	int k = 10;
	for (int i = -k; i <= k; i++) {
		m_blocks.push_back(Box(i, 6.5, 1, 1));
	}

	for (int i = -k; i <= k; i++) {
		m_block_names.push_back("box.png");
	}

	m_block_brush.outline_opacity = 0.0f;
	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.1f, 1.0f, 0.1f);
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);



	class Zombie* m_zombie = new Zombie("Zombie", 5.0f);
	class Zombie* m_zombiee = new Zombie("Zombie", 3.0f);

	m_zombie_list.push_back(m_zombie);
	m_zombie_list.push_back(m_zombiee);

	class Flat* m_flat = new Flat("flat", 5.5);

	class Dumpster* m_dumpster = new Dumpster("dumpster", 0.0);

	class Heal* m_heal = new Heal("sprite", 9.0);

	class Portal* m_portal = new Portal("portal", 12.0);

	m_static_list.push_back(m_flat);
	m_static_list.push_back(m_dumpster);
	m_heal_list.push_back(m_heal);
	m_portal_list.push_back(m_portal);
}

void Level::respond_heal() 
{
	if (m_heal_list.size()==0) {
		
		class Heal* m_heal = new Heal("sprite", 9.0);
		m_heal_list.push_back(m_heal);
		for (auto p_gob : m_heal_list)
			if (p_gob) p_gob->init();
	}
}

Level::~Level()//TODO
{
	for (auto p_gob : m_static_list)
		delete p_gob;

	//draw sprites
	for (auto p_gob : m_heal_list)
		delete p_gob;

	//draw safehouse
	for (auto p_gob : m_portal_list)
		delete p_gob;

	//draw zombies
	for (auto p_gob : m_zombie_list)
		delete p_gob;
}
