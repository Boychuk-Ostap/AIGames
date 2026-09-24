#pragma once
#include <SFML/Graphics.hpp>

enum class Movement_direction {
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

class Player {
public:
	Player();
	void Draw(sf::RenderWindow& window);
	void Update(float dt, sf::Vector2f window_Size);

private:
	void Movement(float dt);
	void KeyBoardHandle(float dt);
	void WrapPlayerAroundScreen(sf::Vector2f window_Size);

	Movement_direction move_dir = Movement_direction::None;

	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	sf::RectangleShape playerRect;

	// --- Kinematic attributes ---
	sf::Vector2f position_P;
	sf::Vector2f velocity_P;
	float heading_P = 0.f;
	float speed_P = 0.f;
	float maxSpeed_P = 600.f;
	float acceleration_P = 300.f;
	float turnRate_P = 180.f;

	static constexpr float spriteRotationOffset = 90.f;

};