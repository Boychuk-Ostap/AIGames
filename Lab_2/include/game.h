#pragma once
#include <SFML/Graphics.hpp>
#include "../include/player.h"
#include "../include/npc.h"

class Game {
public:
	Game();
	void Run();

private:
	void ProcessEvents();
	void Update(sf::Time dt);
	void Render();

	sf::RenderWindow main_window;
	sf::Vector2f window_Size;

	sf::Texture alienTexture;
	sf::Font font;
	Player player;
	std::vector<Npc> npcs;

	float deltaTime = 0.f;
};