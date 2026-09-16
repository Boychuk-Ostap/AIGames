#pragma once
#include <SFML/Graphics.hpp>

class Game {
public:
	Game();
	void run();

private:
	void processEvents();
	void update(sf::Time dt);
	void render();

	sf::RenderWindow main_window;
};