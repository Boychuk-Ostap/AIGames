#include "player.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <numbers>

Player::Player() : playerTexture("../Assets/kenney_simple-space/PNG/Default/ship_sidesA.png"),
playerSprite(playerTexture)
{
	const auto textureSizeP = playerTexture.getSize();
	float scaleP = 75.0f / static_cast<float>(textureSizeP.x);
	playerSprite.setScale({ scaleP, scaleP });

	playerSprite.setOrigin(sf::Vector2f(textureSizeP) / 2.f); // origin in the center of the sprite for rotation

	position_P = { 200.f, 200.f };
	heading_P = 0.f; // Start facing to the right 
	speed_P = 150.f; // Start moving speed

	playerSprite.setPosition(position_P);
	playerSprite.setRotation(sf::degrees(heading_P + spriteRotationOffset));
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(playerSprite);
}

void Player::Update(float dt, sf::Vector2f window_Size)
{
	KeyBoardHandle(dt);
	Movement(dt);
	WrapPlayerAroundScreen(window_Size);
}

void Player::Movement(float dt)
{
	const float radians = heading_P * std::numbers::pi_v<float> / 180.f;

	velocity_P = { std::cos(radians) * speed_P,std::sin(radians) * speed_P };
	position_P += velocity_P * dt;

	playerSprite.setPosition(position_P);
	playerSprite.setRotation(sf::degrees(heading_P + spriteRotationOffset));
}

void Player::KeyBoardHandle(float dt)
{
	using Key = sf::Keyboard::Key;

	if (sf::Keyboard::isKeyPressed(Key::Up) || sf::Keyboard::isKeyPressed(Key::W)){
		speed_P += acceleration_P * dt;
	}
	if (sf::Keyboard::isKeyPressed(Key::Down) || sf::Keyboard::isKeyPressed(Key::S)) {
		speed_P -= acceleration_P * dt;
	}

	if (sf::Keyboard::isKeyPressed(Key::Left) || sf::Keyboard::isKeyPressed(Key::A)) {
		heading_P -= turnRate_P * dt;
	}
	if (sf::Keyboard::isKeyPressed(Key::Right) || sf::Keyboard::isKeyPressed(Key::D)) {
		heading_P += turnRate_P * dt;
	}

	speed_P = std::clamp(speed_P, 0.f, maxSpeed_P);
	heading_P = std::fmod(heading_P + 360.f, 360.f);
}

void Player::WrapPlayerAroundScreen(sf::Vector2f window_Size)
{
	sf::Vector2f playerSize = playerSprite.getGlobalBounds().size / 2.f;
	
	if (position_P.x + playerSize.x < 0.f) {
		position_P.x = window_Size.x; // Left -> right
	}
	else if (position_P.x > window_Size.x + 5) {
		position_P.x = -playerSize.x; // Right -> left
	}
	
	if (position_P.y + playerSize.y < 0.f) {
		position_P.y = window_Size.y; // top -> down
	}
	else if (position_P.y > window_Size.y) {
		position_P.y = -playerSize.y; // down -> top
	}
	
	playerSprite.setPosition(position_P);
}
