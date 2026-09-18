#pragma once
#include <SFML/Graphics.hpp>

enum class Movemen_direction {
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
	void Update(float dt);

private:
	void Movement(float dt);
	void KeyBoardHandle();

	Movemen_direction move_dir = Movemen_direction::None;
	sf::RectangleShape playerRect;

	// --- Variables --- 
	float speed_Player;

};