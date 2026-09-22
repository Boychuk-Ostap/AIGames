#pragma once
#include <SFML/Graphics.hpp>


enum class Movement_direction_NPC {
	None,
	Up,
	Down,
	Left,
	Rigth,
	UpRight,
	UpLeft,
	DownLeft,
	DownRight
};

class Npc {
public:
	Npc();
	void Draw(sf::RenderWindow& window);
	void Update(float dt);

private:
	void Movement(float dt);
	void KeyBoardHandle();
	int GetRandDir();
	void DirectionHandle();

	Movement_direction_NPC move_dir = Movement_direction_NPC::None;
	sf::RectangleShape npcRect;

	// ___ Variables ___
	float speed_Npc;
	int random_Direction_Number;


	
};