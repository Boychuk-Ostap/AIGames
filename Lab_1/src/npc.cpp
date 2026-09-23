#include "npc.h"
#include <random>

Npc::Npc()
{
	npcRect.setSize({ 25.0f, 25.0f });
	npcRect.setFillColor(sf::Color::Red);
	npcRect.setPosition({ 500.f, 500.f });
	speed_Npc = 200;
	random_Direction_Number = 0;

}

void Npc::Draw(sf::RenderWindow & window)
{
	window.draw(npcRect);
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
	switch (move_dir) {
	case Movement_direction_NPC::Up: npcRect.move({ 0.f, -speed_Npc * dt }); break;
	case Movement_direction_NPC::Down: npcRect.move({ 0.f, speed_Npc * dt }); break;
	case Movement_direction_NPC::Left: npcRect.move({ -speed_Npc * dt, 0.f }); break;
	case Movement_direction_NPC::Rigth: npcRect.move({ speed_Npc * dt, 0.f }); break;

	case Movement_direction_NPC::UpRight: npcRect.move({ speed_Npc * dt, -speed_Npc * dt }); break;
	case Movement_direction_NPC::UpLeft: npcRect.move({ -speed_Npc * dt, -speed_Npc * dt }); break;
	case Movement_direction_NPC::DownLeft: npcRect.move({ -speed_Npc * dt, speed_Npc * dt }); break;
	case Movement_direction_NPC::DownRight: npcRect.move({ speed_Npc * dt, speed_Npc * dt }); break;
	}
}

void Npc::KeyBoardHandle()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)) {
		random_Direction_Number = GetRandDir();
	}
}

int Npc::GetRandDir()
{
	static std::mt19937 rng{ std::random_device{}() };
	static std::uniform_int_distribution<int> dist(1, 8);
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
	sf::Vector2f npcPos = npcRect.getPosition();
	sf::Vector2f npcSize = npcRect.getSize();

	if (npcPos.x + npcSize.x < 0.f) {
		npcPos.x = window_Size.x; // Left -> right
	}
	else if (npcPos.x > window_Size.x + 5) {
		npcPos.x = -npcSize.x; // Right -> left
	}

	if (npcPos.y + npcSize.y < 0.f) {
		npcPos.y = window_Size.y; // top -> down
	}
	else if (npcPos.y > window_Size.y) {
		npcPos.y = -npcSize.y; // down -> top
	}

#
	npcRect.setPosition(npcPos);
}
