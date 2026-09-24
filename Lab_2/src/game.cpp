#include "../include/game.h"

Game::Game() : main_window(sf::VideoMode(sf::Vector2u({1920u, 1080u})), "Lab2"),
window_Size(sf::Vector2f(main_window.getSize())),
alienTexture("../Assets/kenney_simple-space/PNG/Default/enemy_D.png"),
font("../Assets/Fonts/CHILLER.TTF")
{
	npcs.reserve(5);

	npcs.emplace_back(alienTexture, font, Behaviour::Wander, sf::Vector2f{ 300.f, 300.f }, 250.f);
	npcs.emplace_back(alienTexture, font, Behaviour::Seek, sf::Vector2f{ 1500.f, 300.f }, 300.f);
	npcs.emplace_back(alienTexture, font, Behaviour::Arrive, sf::Vector2f{ 300.f, 800.f }, 250.f);
	npcs.emplace_back(alienTexture, font, Behaviour::Arrive, sf::Vector2f{ 1500.f, 800.f }, 450.f);
	npcs.emplace_back(alienTexture, font, Behaviour::Pursue, sf::Vector2f{ 900.f, 900.f }, 320.f);
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
		else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
			using K = sf::Keyboard::Key;
			switch (key->code) {
			case K::Num1: npcs[0].ToggleActive(); break;
			case K::Num2: npcs[1].ToggleActive(); break;
			case K::Num3: npcs[2].ToggleActive(); break;
			case K::Num4: npcs[3].ToggleActive(); break;
			case K::Num5: npcs[4].ToggleActive(); break;
			case K::Escape: main_window.close(); break;
			default: break;
			
			}
		}
	}
}

void Game::Update(sf::Time dt)
{
	deltaTime = dt.asSeconds();
	player.Update(deltaTime, sf::Vector2f(window_Size));
	for(auto& npc : npcs)
		npc.Update(deltaTime, player, npcs, window_Size);
}

void Game::Render()
{
	main_window.clear();
	player.Draw(main_window);
	for(auto& npc:npcs)
		npc.Draw(main_window);
	main_window.display();
}
