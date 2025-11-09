#include "player.h"
#include "util.h"
#include <cmath>

void Player::update(float dt)
{
	float delta_time = dt / 1000.0f;

	movePlayer(dt);

	// update x offset for other game objects
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	
	GameObject::update(dt);
}

void Player::draw()
{
	m_brush_player.texture = m_state->getFullAssetPath("boy.png");
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_pos_y, 1.0f, 1.0f, m_brush_player);

	if (m_state->m_debugging)
		debugDraw();

	// score board
	graphics::Brush score_brush;
	score_brush.outline_opacity = 0.0f;
	SETCOLOR(score_brush.fill_color, 1.0f, 1.0f, 1.0f);
	score_brush.fill_opacity = 1.0f;
	std::string text = "Score: " + std::to_string(score);
	graphics::drawText(m_state->getCanvasWidth() * 0.5f - 0.3f, m_pos_y - 1.5f, 0.2f, text, score_brush);

	//hp bar
	graphics::Brush total_hp_brush;
	total_hp_brush.outline_opacity = 1.0f;
	SETCOLOR(total_hp_brush.fill_color, 1.0f, 0.0f, 0.0f);
	total_hp_brush.fill_opacity = 1.0f;
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_pos_y - 1.0f, total_hp / 100, 0.1f, total_hp_brush);

	graphics::Brush hp_brush;
	hp_brush.outline_opacity = 0.0f;
	SETCOLOR(hp_brush.fill_color, 0.0f, 1.0f, 0.0f);
	hp_brush.fill_opacity = 1.0f;
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_pos_y - 1.0f, 1.0 * m_state->getPlayer()->getCurrentHP() / 100, 0.1f, hp_brush);
}

void Player::init()
{
	m_pos_x = 0.0f;
	m_pos_y = 5.0f;

	// initialize x offset for other game objects
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;

	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.outline_opacity = 0.0f;
	
	m_brush_player.texture = m_state->getFullAssetPath("boy.png");
	
	// Adjust width for finer collision detection
	m_width = 0.5f;

	//reset hp
	current_hp = 100;
}

void Player::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	//graphics::drawRect(m_state->getCanvasWidth()*0.5f, m_state->getCanvasHeight() * 0.5f, m_width, m_height, debug_brush);
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_pos_y, m_width, m_height, debug_brush);
	
	char s[20];
	sprintf_s(s,"(%5.2f, %5.2f)", m_pos_x, m_pos_y);
	SETCOLOR(debug_brush.fill_color, 1, 0, 0);
	debug_brush.fill_opacity = 1.0f;
	//graphics::drawText(m_state->getCanvasWidth() * 0.5f - 0.4f, m_state->getCanvasHeight() * 0.5f - 0.6f, 0.15f, s, debug_brush);
	graphics::drawText(m_state->getCanvasWidth() * 0.5f - 0.36f, m_pos_y - 0.6f, 0.15f, s, debug_brush);

}

void Player::movePlayer(float dt)
{
	float delta_time = dt / 1000.0f;

	float move = 0.0f;
	if (graphics::getKeyState(graphics::SCANCODE_A))
		move -= 1.0f;
	if (graphics::getKeyState(graphics::SCANCODE_D))
		move = 1.0f;

	m_vx = std::min<float>(m_max_velocity, m_vx + delta_time * move * m_accel_horizontal);
	m_vx = std::max<float>(-m_max_velocity, m_vx);

	// friction
	m_vx -= 0.1f * m_vx / (0.1f + fabs(m_vx));

	// apply static friction threshold
	if (fabs(m_vx) < 0.001f)
		m_vx = 0.0f;

	// adjust horizontal position
	m_pos_x += m_vx * delta_time;

	// jump only when not in flight:
	if (m_vy == 0.0f)
		m_vy -= ((graphics::getKeyState(graphics::SCANCODE_W) or graphics::getKeyState(graphics::SCANCODE_UP)) ? m_accel_vertical : 0.0f) * 0.02f;// not delta_time!! Burst 
	
	// add gravity
	m_vy += delta_time * m_gravity;

	// adjust vertical position
	m_pos_y += m_vy * delta_time;

	
}


const int Player::getTotalHP() { return total_hp; }

int Player::getCurrentHP() {
	if (current_hp > total_hp)
		current_hp = total_hp;
	return current_hp;
}

void Player::heal() {
	current_hp += 20;
	score += 10;
	if (current_hp > total_hp)
		current_hp = total_hp;
}

void Player::damage() {
	current_hp -= 20;
	if (current_hp <= 0)
		current_hp = 0;
}

void Player::success() {
	score += 100;
}

void Player::death() {
	graphics::playSound(m_state->getFullAssetPath("death.mp3"), 1.0f);
	score = 0;
}