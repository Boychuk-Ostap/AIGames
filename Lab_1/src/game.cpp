#include "../include/game.h"

Game::Game() : main_window(sf::VideoMode(sf::Vector2u(window_Size)), "Lab1")
{
}

void Game::Run()
{
	sf::Clock clock;
	while (main_window.isOpen()) {
		ProcessEvents();
		Update(clock.restart());
		Render();
	}
}

void Game::ProcessEvents()
{
	while (const std::optional event = main_window.pollEvent()) {
		if (event->is < sf::Event::Closed>()) {
			main_window.close();
		}
	}
}

void Game::Update(sf::Time dt)
{
	deltaTime = dt.asSeconds();
	player.Update(deltaTime);
	npc.Update(deltaTime, sf::Vector2f(window_Size));
}

void Game::Render()
{
	main_window.clear();
	player.Draw(main_window);
	npc.Draw(main_window);
	main_window.display();
}
