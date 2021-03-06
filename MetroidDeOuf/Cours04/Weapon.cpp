#include "Weapon.hpp"
#include "Game.hpp"

Weapon::Weapon()
{
	this->spr = new sf::Sprite();
	this->spr->setPosition(13, 13);
	this->spr->setOrigin(spr->getOrigin().x / 2, spr->getOrigin().y / 2);

	texture = new sf::Texture();
	if (!texture->loadFromFile("Assets/Graphs/canon.png"))
		printf("Could not load canon texture from Assets/Graphs/canon.png");
	this->spr->setTexture(*texture);

	for (int i = 0; i < poolCount; i++)
	{
		Bullet* b = new Bullet(cx, cy, stride);
		bulletsPool.push_back(b);
	}

	soundsOrigin = new sf::Sound();
	fireSound = new sf::SoundBuffer();
	if (!fireSound->loadFromFile("Assets/Sounds/lasershoot.wav"))
		printf("Could not load laser shoot sound from Assets/Sounds/lasershoot.wav");

	fireParticles = new Particles(sf::Color::Yellow, 1);
}

Weapon::~Weapon()
{
	for (int i = 0; i < poolCount; i++)
		delete bulletsPool[i];

	delete(soundsOrigin);
	delete(fireSound);

	delete(this->spr);
	delete(this->texture);
}

void Weapon::setPosition(int _cx, float _rx, int _cy, float _ry)
{
	cx = _cx;
	rx = _rx;
	cy = _cy;
	ry = _ry;
	xx = (cx + rx) * stride;
	yy = (cy + ry) * stride;
}

void Weapon::setAudiomanager(AudioManager* _audioManagerRef)
{
	this->audioManagerRef = _audioManagerRef;
	for (int i = 0; i < poolCount; i++)
	{
		bulletsPool[i]->audioManagerRef = _audioManagerRef;
	}
}

void Weapon::setOffset(sf::Vector2f _offset)
{
	this->offset = _offset;
}

void Weapon::lookAtMouse()
{
	if (sf::Mouse::getPosition() != pastMousePos)
	{
		angle = atan2(-offset.y, -offset.x) * (360 / (3.14 * 2));
		if (angle > -90.0f && angle < 90.0f)
		{
			this->spr->setScale(1, 1);
		}
		else
		{
			this->spr->setScale(1, -1);
		}
		this->spr->setRotation(angle);

		pastMousePos = sf::Mouse::getPosition();
	}

	float axisPositionX = sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::U);
	float axisPositionY = sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::V);
	if ((axisPositionX > gameRef->controllerDeadZone || axisPositionX < -gameRef->controllerDeadZone) ||
		(axisPositionY > gameRef->controllerDeadZone || axisPositionY < -gameRef->controllerDeadZone))
	{
		sf::Mouse::setPosition(sf::Vector2i(sf::Mouse::getPosition().x + axisPositionX / 100 * joystickSpeed, 
											sf::Mouse::getPosition().y + axisPositionY / 100 * joystickSpeed));
	}
	
}

void Weapon::update(float dt)
{
	lookAtMouse();

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		fire();

	if (sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::Z) > gameRef->controllerDeadZone ||
		sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::Z) < -gameRef->controllerDeadZone)
		fire();

	CDtimer -= dt;

	for (auto b : bulletsPool)
		b->update(dt);
	checkBulletCollision();

	fireParticles->update(dt);
}

void Weapon::render(sf::RenderTarget& target, sf::RenderStates states)
{
	for (auto b : bulletsPool)
		b->render(target);
	target.draw(*this->spr, states);
	fireParticles->render(target);
}

void Weapon::fire()
{
	if (CDtimer > 0 || !canFire)
		return;
	std::srand(time(NULL));

	fireParticles->create(xx, yy, 200, 200, 0.3f, 3);
	CDtimer = fireCD;
	sf::Listener::setPosition(xx, yy, 0.f);
	audioManagerRef->playSound(fireSound, soundsOrigin);
	for (auto b : bulletsPool)
	{
		if (!b->isActive())
		{
			b->setPosition(this->cx, this->rx, this->cy, this->ry);
			b->setActive(true);
			setBullet(b);
			return;
		}
	}
	Bullet* b = new Bullet(cx, cy, stride);
	b->audioManagerRef = this->audioManagerRef;
	bulletsPool.push_back(b);
	setBullet(b);

}

void Weapon::setBullet(Bullet* bullet)
{
	bullet->setPosition(this->cx, this->rx, this->cy, this->ry);
	sf::Vector2f direction = { mousePosition.x - ((cx + rx) * stride), mousePosition.y - ((cy + ry) * stride) };

	bullet->setDirection(sf::Vector2f(NormalizeVector(direction)));
	bullet->setActive(true);
}

void Weapon::checkBulletCollision()
{
	for (auto b : bulletsPool)
	{
		if (b->isActive())
		{
			if (worldRef->colidesWithWall(*b))
			{
				b->playWallHitSound();
				b->setActive(false);
				b->createParticles();
			}
			if (gameRef->checkIfBulletHitsEnemy(b->cx, b->cy, damages, knockbackForce))
			{
				b->setActive(false);
				b->createParticles();
			}
		}
	}
}
