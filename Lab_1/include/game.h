#pragma once
#include <SFML/Graphics.hpp>
#include "../include/player.h"

class Game {
public:
	Game();
	void run();

	Player player;
private:
	void processEvents();
	void update(sf::Time dt);
	void render();

	sf::RenderWindow main_window;
};