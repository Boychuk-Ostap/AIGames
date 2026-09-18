#include "player.h"
#include <iostream>

Player::Player()
{
	playerRect.setSize({ 50.0f, 50.0f });
	playerRect.setFillColor(sf::Color::White);
	playerRect.setPosition({ 100.f, 100.f });
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(playerRect);
}



void Player::Update()
{
}
