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
	void Update(float dt);

private:
	void Movement(float dt);
	void KeyBoardHandle();

	Movement_direction move_dir = Movement_direction::None;
	sf::RectangleShape playerRect;

	// --- Variables --- 
	float speed_Player;

};