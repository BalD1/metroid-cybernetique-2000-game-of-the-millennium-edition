#pragma once
#include "Entity.hpp"
#include "World.hpp"
#include "AudioManager.hpp"

class Character : public Entity, public sf::Transformable
{
private:


protected:

	sf::SoundBuffer*	groundHitSound = nullptr;
	sf::Sound*			characterSoundPlayer = nullptr;

public:

	sf::Transform		transform;
	std::string			name = "NONAME";
	float				maxHealth = 100;
	float				currentHealth = 100;
	float				speed = 7.5f;
	float				invincibility_CD = 1;
	float				invincibility_Timer = 0;
	float				mass = 1;
	float				knockback_DURATION = 1;
	float				knockback_TIMER = 1;
	float				frct_x = 0.86f;
	float				frct_y = 0.96f;
	float				fallingSpeed = 0;
	float				maxFallingSpeed = 4.0f;
	float				fallingSpeedFactor = 8.256f;

	bool				moved = false;
	bool				isGrounded = false;
	bool				ignoreGravity = false;

	enum class			State { Idle, Walking, Jumping, Falling };
	State				characterState = State::Idle;

	World*				worldRef = nullptr;
	AudioManager*		audioManagerRef = nullptr;

	Character(std::string _name, int _cx, int _cy, int _stride);
	Character(std::string _name, float _speed, float _invicibilityCD, float _maxHealth, int _cx, int _cy, int _stride);
	~Character();

	void initSounds();

	void setGravity(float _gravity, bool _ignoreGravity = false);
	void setWorld(World* _worldRef);

	bool isCollidingWithWorld(int _cx, int _cy);
	bool isCollidingSelf(int _cx, int _cy);
	void manageMovements(float dt);
	void applyGravity(float dt);
	void manageState();
	void syncTransform();

	bool alive() { return this->currentHealth > 0; };
	void takeDamages(float rawDamages);
	void knockBack(int otherXX, int otherYY, int _knockbackForce);
	void heal(float rawHeal);

	void update(float dt);
	void render(sf::RenderTarget& target);

};

