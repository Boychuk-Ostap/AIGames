#pragma once
#include <SFML/Graphics.hpp>
#include "../include/player.h"

class Game {
public:
	Game();
	void Run();

	Player player;
private:
	void ProcessEvents();
	void Update(sf::Time dt);
	void Render();

	sf::RenderWindow main_window;

	float deltaTime;
};