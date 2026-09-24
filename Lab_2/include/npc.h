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
	void Update(float dt, sf::Vector2f window_Size);


private:
	void Movement(float dt);
	void KeyBoardHandle();
	float GetRandHeading();
	void DirectionHandle();
	void WrapAroundScreen(sf::Vector2f window_Size);

	Movement_direction_NPC move_dir = Movement_direction_NPC::None;
	sf::Texture npcTexture;
	sf::Sprite npcSprite;
	sf::RectangleShape npcRect;

	// ___ Variables ___
	int random_Direction_Number;

	// --- Kinematic ---
	sf::Vector2f position_N;
	sf::Vector2f velocity_N;
	float heading_N = 0.f;
	float speed_N = 200.f;
	float maxSpeed_N = 400.f;

	static constexpr float spriteRotationOffset = 90.f;
	
};