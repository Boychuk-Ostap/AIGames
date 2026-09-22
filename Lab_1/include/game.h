#pragma once
#include <SFML/Graphics.hpp>
#include "../include/player.h"
#include "../include/npc.h"

class Game {
public:
	Game();
	void Run();

	Player player;
	Npc npc;
private:
	void ProcessEvents();
	void Update(sf::Time dt);
	void Render();

	sf::RenderWindow main_window;

	float deltaTime;
};