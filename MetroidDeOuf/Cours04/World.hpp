#pragma once
#include "Entity.hpp"
#include "DeathZone.hpp"
#include "Checkpoint.hpp"

class Enemy;

class World
{
private:

	sf::Texture*				wallTexture = nullptr;
	sf::Texture*				deathzoneTexture = nullptr;

	int							mapLength = 50;
	int							stride = 32;

	const char*					mapFilePath = "Assets/Data/map.txt";

public:

	bool						worldInitialized = false;

	float						gravity = 0.7f;

	std::vector<Entity*>		entities;
	std::vector<Entity*>		entitiesToDelete;
	std::vector<DeathZone*>		deathZones;
	std::vector<Checkpoint*>	checkpoints;
	Checkpoint*					lastActivatedCheckpoint = nullptr;

	World(int _stride = 32);
	~World();

	bool colidesWithWall(Entity target);

	void placeDeathZone(int _cx, int _cy);
	void placeWall(int _cx, int _cy);
	void placeCheckPoint(int _cx, int _cy);
	void eraseMap();

	void deleteEntities();

	void render(sf::RenderTarget& target);

	// files
	void saveMapInFile();
	void loadMap(bool eraseCurrentMap = true);

};

