#include "../include/game.h"

Game::Game() : main_window(sf::VideoMode({ 1920, 1080 }), "Lab1")
{
	
}

void Game::run()
{
	sf::Clock clock;
	while (main_window.isOpen()) {
		processEvents();
		update(clock.restart());
		render();
	}
}

void Game::processEvents()
{
	while (const std::optional event = main_window.pollEvent()) {
		if (event->is < sf::Event::Closed>()) {
			main_window.close();
		}
	}
}

void Game::update(sf::Time dt)
{
	player.Update();
}

void Game::render()
{
	main_window.clear();
	player.Draw(main_window);
	main_window.display();
}
