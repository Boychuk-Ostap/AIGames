#include "../include/game.h"
#include <random>

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

	const int flockCount = 100;

	flockTexture = sf::Texture("../Assets/kenney_simple-space/PNG/Default/enemy_A.png");
	npcFlock.reserve(flockCount);

	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> xDist(700.f, 1200.f);
	std::uniform_real_distribution<float> yDist(350.f, 750.f);

	for (int i = 0; i < flockCount; ++i) {
		npcFlock.emplace_back(
			flockTexture, font, Behaviour::Wander,
			sf::Vector2f{ xDist(rng), yDist(rng) },
			180.f, 32.f);
	}
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
			case K::F:
				for (auto& npc : npcFlock)
					npc.SetBehaviour(Behaviour::Flock);
				break;
			case K::S:
				for (auto& npc : npcFlock)
					npc.SetBehaviour(Behaviour::Swarm);
				break;
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
	for (auto& npc : npcFlock)
		npc.Update(deltaTime, player, npcFlock, window_Size);
}

void Game::Render()
{
	main_window.clear();
	player.Draw(main_window);
	for(auto& npc:npcs)
		npc.Draw(main_window);
	for (auto& npc : npcFlock)
		npc.Draw(main_window);
	main_window.display();
}
