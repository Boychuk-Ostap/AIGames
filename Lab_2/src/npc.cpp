#include "npc.h"
#include <random>
#include <algorithm>
#include <cmath>
#include <numbers>

Npc::Npc() : npcTexture("../Assets/kenney_simple-space/PNG/Default/enemy_D.png"),
			 npcSprite(npcTexture)
{
	const auto texureSizeNPC = npcTexture.getSize();
	float scaleNPC = 100.f / static_cast<float>(texureSizeNPC.x);
	npcSprite.setScale({ scaleNPC, scaleNPC });
	npcSprite.setOrigin(sf::Vector2f(texureSizeNPC) / 2.f);

	position_N = { 800.f, 500.f };
	heading_N = GetRandHeading();

	npcSprite.setPosition(position_N);
	npcSprite.setRotation(sf::degrees(heading_N + spriteRotationOffset));
}

void Npc::Draw(sf::RenderWindow & window)
{
	window.draw(npcSprite);
}

void Npc::Update(float dt, sf::Vector2f window_Size)
{
	KeyBoardHandle();
	DirectionHandle();
	Movement(dt);
	WrapAroundScreen(window_Size);
}

void Npc::Movement(float dt)
{
	const float radians = heading_N * std::numbers::pi_v<float> / 180.f;

	velocity_N = { std::cos(radians) * speed_N, std::sin(radians) * speed_N };
	position_N += velocity_N * dt;

	npcSprite.setPosition(position_N);
	npcSprite.setRotation(sf::degrees(heading_N + spriteRotationOffset));

}

void Npc::KeyBoardHandle()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)) {
		heading_N = GetRandHeading();
	}
}

float Npc::GetRandHeading()
{
	static std::mt19937 rng{ std::random_device{}() };
	static std::uniform_real_distribution<float> dist(0.f, 360.f);
	return dist(rng);
}

void Npc::DirectionHandle()
{
	switch (random_Direction_Number) {
		case 1: move_dir = Movement_direction_NPC::Up; break;
		case 2: move_dir = Movement_direction_NPC::Down; break;
		case 3: move_dir = Movement_direction_NPC::Left; break;
		case 4: move_dir = Movement_direction_NPC::Rigth; break;
		case 5: move_dir = Movement_direction_NPC::UpRight; break;
		case 6: move_dir = Movement_direction_NPC::UpLeft; break;
		case 7: move_dir = Movement_direction_NPC::DownRight; break;
		case 8: move_dir = Movement_direction_NPC::DownLeft; break;
		default: move_dir = Movement_direction_NPC::None; break; // 0 = not started
	}

}

void Npc::WrapAroundScreen(sf::Vector2f window_Size)
{
	sf::Vector2f npcSize = npcSprite.getGlobalBounds().size / 2.0f;

	if (position_N.x + npcSize.x < 0.f) {
		position_N.x = window_Size.x; // Left -> right
	}
	else if (position_N.x > window_Size.x + 5) {
		position_N.x = -npcSize.x; // Right -> left
	}

	if (position_N.y + npcSize.y < 0.f) {
		position_N.y = window_Size.y; // top -> down
	}
	else if (position_N.y > window_Size.y) {
		position_N.y = -npcSize.y; // down -> top
	}

	npcSprite.setPosition(position_N);
}
