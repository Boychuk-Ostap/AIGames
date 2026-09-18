#include "player.h"
#include <iostream>

Player::Player()
{
	playerRect.setSize({ 50.0f, 50.0f });
	playerRect.setFillColor(sf::Color::White);
	playerRect.setPosition({ 100.f, 100.f });
	speed_Player = 500;
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(playerRect);
}



void Player::Update(float dt)
{
	KeyBoardHandle();
	Movement(dt);
}

void Player::Movement(float dt)
{
	speed_Player = 500;

	switch (move_dir) {
		case Movement_direction::Up: playerRect.move({ 0.f, -speed_Player * dt }); break;
		case Movement_direction::Down: playerRect.move({ 0.f, speed_Player * dt }); break;
		case Movement_direction::Left: playerRect.move({ -speed_Player * dt, 0.f }); break;
		case Movement_direction::Rigth: playerRect.move({ speed_Player * dt, 0.f }); break;
					
		case Movement_direction::UpRight: playerRect.move({ speed_Player * dt, -speed_Player * dt }); break;
		case Movement_direction::UpLeft: playerRect.move({ -speed_Player * dt, -speed_Player * dt }); break;
		case Movement_direction::DownLeft: playerRect.move({ -speed_Player * dt, speed_Player * dt }); break;
		case Movement_direction::DownRight: playerRect.move({ speed_Player * dt, speed_Player * dt }); break;
	}

}

void Player::KeyBoardHandle()
{
	bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
	bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
	bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
	bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);

	if (right && up) move_dir = Movement_direction::UpRight;
	else if (left && up) move_dir = Movement_direction::UpLeft;
	else if (right && down) move_dir = Movement_direction::DownRight;
	else if (left && down) move_dir = Movement_direction::DownLeft;
	else if (up) move_dir = Movement_direction::Up;
	else if (down) move_dir = Movement_direction::Down;
	else if (left) move_dir = Movement_direction::Left;
	else if (right) move_dir = Movement_direction::Rigth;


	else
		move_dir = Movement_direction::None;
}
