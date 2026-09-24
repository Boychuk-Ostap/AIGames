#pragma once
#include <SFML/Graphics.hpp>

class Player;

enum class Behaviour {
	Wander,
	Seek,
	Arrive,
	Pursue
};

class Npc {
public:
	Npc(const sf::Texture& texture, const sf::Font& font, 
		Behaviour behaviour, sf::Vector2f spawn, float maxSpeed);

	void Draw(sf::RenderWindow& window);
	void Update(float dt, const Player& player, 
		const std::vector<Npc>& others, sf::Vector2f window_Size);

	// ___ Toggle keys 1-5 ___
	void ToggleActive() { active = !active; }
	bool IsActive() const { return active; }

	// ___ Read only acces ___
	sf::Vector2f GetPosition() const { return position_N; }
	sf::Vector2f GetVelocity() const { return velocity_N; }
	Behaviour GetBehaviour() const { return behaviour_N; }

	static const char* BehaviourName(Behaviour bhvr_N);

private:
	sf::Vector2f ComputeStreering(const Player& player, const std::vector<Npc>& others, float dt);

	void ApplySteering(sf::Vector2f acceleration, float dt);
	void WrapAroundScreen(sf::Vector2f window_Size);
	void UpdateLabel();
	float GetRandHeading();

	sf::Vector2f Seek(sf::Vector2f target) const;
	sf::Vector2f Arrive(sf::Vector2f target) const;
	sf::Vector2f Pursue(const Player& player) const;
	sf::Vector2f Wander(float dt);

	// wander specs
	float wanderOrientation_N = 0.0f;
	float wanderOffset_N = 100.0f;
	float wanderRadius_N = 90.0f;
	float wanderRate_N = 400.0f;
	float RandomBinomial();
	sf::CircleShape wanderDebug{ 5.f };

	float slowRadius_N = 250.f; // slows down in the radius 
	float targetRadius_N = 15.f;// stop in this radius, as arrived
	float timeToTarget_N = 0.15f;

	float maxPrediction_N = 1.0f; // seconds

	sf::Sprite npcSprite;
	sf::Text label;

	Behaviour behaviour_N;
	bool active = true;
	
	// --- Kinematic ---
	sf::Vector2f position_N;
	sf::Vector2f velocity_N;
	float heading_N = 0.f;
	float speed_N = 200.f;
	float maxSpeed_N = 400.f;
	float maxAccel_N = 250.f;

	static constexpr float spriteRotationOffset = 90.f;
	
};