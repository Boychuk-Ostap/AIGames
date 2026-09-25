#include "npc.h"
#include <random>
#include <algorithm>
#include <cmath>
#include <numbers>

#include "../include/player.h"

Npc::Npc(const sf::Texture& texture, const sf::Font& font, 
	Behaviour behaviour, sf::Vector2f spawn, float maxSpeed)
	: npcSprite(texture),
	label(font, BehaviourName(behaviour), 45),
	behaviour_N(behaviour),
	position_N(spawn),
	maxSpeed_N(maxSpeed)
{
	const auto texureSizeNPC = texture.getSize();
	float scaleNPC = 75.f / static_cast<float>(texureSizeNPC.x);
	npcSprite.setScale({ scaleNPC, scaleNPC });
	npcSprite.setOrigin(sf::Vector2f(texureSizeNPC) / 2.f);

	heading_N = GetRandHeading();
	wanderOrientation_N = GetRandHeading();
	const float rad = heading_N * std::numbers::pi_v<float> / 180.f;
	velocity_N = { std::cos(rad) * maxSpeed_N * 0.5f, std::sin(rad) * maxSpeed_N * 0.5f };

	label.setFillColor(sf::Color::White);

	visionCone.setPointCount(3);
	visionCone.setFillColor(sf::Color(0, 255, 0, 40));

	npcSprite.setPosition(position_N);
	npcSprite.setRotation(sf::degrees(heading_N + spriteRotationOffset));
	UpdateVisionCone();
	UpdateLabel();
}

void Npc::Draw(sf::RenderWindow & window)
{
	if (!active) return;
	window.draw(visionCone);
	window.draw(npcSprite);
	window.draw(label);

	if (behaviour_N == Behaviour::Wander) window.draw(wanderDebug);
}

void Npc::Update(float dt, const Player& player, const std::vector<Npc>& others, sf::Vector2f window_Size)
{
	if (!active) return;

	sf::Vector2f steering_N = ComputeStreering(player, others, dt);

	const sf::Vector2f avoidance = Avoid(player, others);
	if (avoidance != sf::Vector2f{ 0.f, 0.f })
		steering_N = steering_N * 0.4f + avoidance * avoidWeight_N;

	ApplySteering(steering_N, dt);
	WrapAroundScreen(window_Size);
	UpdateVisionCone();
	UpdateLabel();
}

float Npc::GetRandHeading()
{
	static std::mt19937 rng{ std::random_device{}() };
	static std::uniform_real_distribution<float> dist(0.f, 360.f);
	return dist(rng);
}

sf::Vector2f Npc::Seek(sf::Vector2f target) const
{
	sf::Vector2f toTarget = target - position_N;
	const float dist = std::hypot(toTarget.x, toTarget.y);
	if (dist < 0.001f) return { 0.0f, 0.0f };

	const sf::Vector2f desiredvelocity = (toTarget / dist) * maxSpeed_N;
	return desiredvelocity - velocity_N;
}

sf::Vector2f Npc::Arrive(sf::Vector2f target) const
{
	sf::Vector2f toTarget = target - position_N;
	const float dist = std::hypot(toTarget.x, toTarget.y);

	if (dist < targetRadius_N) {
		return -velocity_N / timeToTarget_N;
	}

	float targetSpeed = maxSpeed_N;
	if (dist < slowRadius_N) {
		targetSpeed = maxSpeed_N * (dist / slowRadius_N);
	}

	const sf::Vector2f targetVelocity = (toTarget / dist) * targetSpeed;
	return (targetVelocity - velocity_N) / timeToTarget_N;
}

sf::Vector2f Npc::Pursue(const Player& player) const
{
	const sf::Vector2f toPlayer = player.GetPosition() - position_N;
	const float dist = std::hypot(toPlayer.x, toPlayer.y);
	const float speed = std::hypot(velocity_N.x, velocity_N.y);

	float prediction = maxPrediction_N;
	if (speed > 0.001f && dist / speed < maxPrediction_N) {
		prediction = dist / speed;
	}

	const sf::Vector2f predictedPos = player.GetPosition() + player.GetVelocity() * prediction;
	return Seek(predictedPos);
}

sf::Vector2f Npc::Wander(float dt)
{
	wanderOrientation_N += RandomBinomial() * wanderRate_N * dt;

	const float headingRad = heading_N * std::numbers::pi_v<float> / 180.f;
	const sf::Vector2f headingVec{ std::cos(headingRad), std::sin(headingRad) };
	const sf::Vector2f circleCentre = position_N + headingVec * wanderOffset_N;

	const float targetRad = (heading_N + wanderOrientation_N) * std::numbers::pi_v<float> / 180.f;
	const sf::Vector2f target = circleCentre + sf::Vector2f{ std::cos(targetRad), std::sin(targetRad) } * wanderRadius_N;

	wanderDebug.setPosition(target - sf::Vector2f{ 5.0f, 5.0f });
	wanderDebug.setFillColor(sf::Color::Yellow);

	return Seek(target);
}

float Npc::RandomBinomial()
{
	static std::mt19937 rng{ std::random_device{}() };
	static std::uniform_real_distribution<float> dist(0.f, 1.f);
	return dist(rng) - dist(rng);
}

bool Npc::InCone(sf::Vector2f point) const
{
	const sf::Vector2f toPoint = point - position_N;
	const float dist = std::hypot(toPoint.x, toPoint.y);
	if (dist < 0.001f || dist > visionLength_N) return false;

	const float headingRad = heading_N * std::numbers::pi_v<float> / 180.f;
	const sf::Vector2f headingVec{ std::cos(headingRad), std::sin(headingRad) };

	const float cosAngle = (headingVec.x * toPoint.x + headingVec.y * toPoint.y) / dist;
	const float angleDeg = std::acos(std::clamp(cosAngle, -1.f, 1.f)) * 180.f / std::numbers::pi_v<float>;
	return angleDeg <= visionHalfAngle_N;
}

sf::Vector2f Npc::Avoid(const Player& player, const std::vector<Npc>& others)
{
	seesPlayer = InCone(player.GetPosition());
	seesNpc = false;

	const Npc* closest = nullptr;
	float closestDist = visionLength_N;

	for (const Npc& other : others) {
		if (&other == this || !other.IsActive()) continue;
		if (!InCone(other.GetPosition())) continue;

		const sf::Vector2f d = other.GetPosition() - position_N;
		const float dist = std::hypot(d.x, d.y);
		if (dist < closestDist) {
			closestDist = dist;
			closest = &other;
		}
	}

	if (!closest) return { 0.f, 0.f };
	seesNpc = true;

	const float headingRad = heading_N * std::numbers::pi_v<float> / 180.f;
	const sf::Vector2f headingVec{ std::cos(headingRad), std::sin(headingRad) };
	const sf::Vector2f lateral{ -headingVec.y, headingVec.x };

	const sf::Vector2f toObs = closest->GetPosition() - position_N;
	const float side = (headingVec.x * toObs.y - headingVec.y * toObs.x) > 0.f ? -1.f : 1.f;

	const float urgency = 1.f - (closestDist / visionLength_N);

	return (lateral * side + headingVec * -0.3f) * maxAccel_N * (0.5f + urgency);
}

void Npc::UpdateVisionCone()
{
	const float toRad = std::numbers::pi_v<float> / 180.f;
	const float leftRad = (heading_N - visionHalfAngle_N) * toRad;
	const float rightRad = (heading_N + visionHalfAngle_N) * toRad;

	visionCone.setPoint(0, position_N);
	visionCone.setPoint(1, position_N + sf::Vector2f{ std::cos(leftRad), std::sin(leftRad) } * visionLength_N);
	visionCone.setPoint(2, position_N + sf::Vector2f{ std::cos(rightRad), std::sin(rightRad) } * visionLength_N);

	if (seesNpc) visionCone.setFillColor(sf::Color(255, 0, 0, 70));
	else if (seesPlayer) visionCone.setFillColor(sf::Color(255, 255, 0, 50));
	else visionCone.setFillColor(sf::Color(0, 255, 0, 40));
}

const char* Npc::BehaviourName(Behaviour bhvr_N)
{
	switch (bhvr_N) {
		case Behaviour::Wander: return "Wander";
		case Behaviour::Seek: return "Seek";
		case Behaviour::Arrive: return "Arrive";
		case Behaviour::Pursue: return "Pursue";
	}
	return "?";
}

sf::Vector2f Npc::ComputeStreering(const Player& player, const std::vector<Npc>& others, float dt)
{
	switch (behaviour_N) {
		case Behaviour::Wander: return Wander(dt);
		case Behaviour::Seek: return Seek(player.GetPosition());
		case Behaviour::Arrive: return Arrive(player.GetPosition());
		case Behaviour::Pursue: return Pursue(player);
	}
	return { 0.f, 0.f };
}

void Npc::ApplySteering(sf::Vector2f acceleration, float dt)
{
	// clamp acceleration if too big
	const float accLen = std::hypot(acceleration.x, acceleration.y);
	if (accLen > maxAccel_N)
		acceleration *= maxAccel_N / accLen;

	velocity_N += acceleration * dt;

	// clamp speed if too big
	const float speed = std::hypot(velocity_N.x, velocity_N.y);
	if (speed > maxSpeed_N)
		velocity_N *= maxSpeed_N / speed;

	position_N += velocity_N * dt;

	if (speed > 1.f)
		heading_N = std::atan2(velocity_N.y, velocity_N.x) * 180.f / std::numbers::pi_v<float>;

	npcSprite.setPosition(position_N);
	npcSprite.setRotation(sf::degrees(heading_N + spriteRotationOffset));
}

void Npc::WrapAroundScreen(sf::Vector2f window_Size)
{
	sf::Vector2f npcSize = npcSprite.getGlobalBounds().size / 2.0f;

	if (position_N.x + npcSize.x < 0.f) {
		position_N.x = window_Size.x; // Left -> right
	}
	else if (position_N.x > window_Size.x + 5) {
		position_N.x = -npcSize.x; // Right -> left
	}

	if (position_N.y + npcSize.y < 0.f) {
		position_N.y = window_Size.y; // top -> down
	}
	else if (position_N.y > window_Size.y) {
		position_N.y = -npcSize.y; // down -> top
	}

	npcSprite.setPosition(position_N);
}

void Npc::UpdateLabel()
{
	label.setPosition(position_N + sf::Vector2f{ -45.f, -85.f });
}
