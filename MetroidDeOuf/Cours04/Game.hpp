#pragma once
#include "stdafx.hpp"
#include "AudioManager.hpp"
#include "CharactersManager.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Character.hpp"
#include "World.hpp"
#include "Menu.hpp"
#include "Item.hpp"
#include "BallItem.hpp"

class BallItem;

class Game
{
private:

	// vars

	sf::RenderWindow			window;
	sf::View*					mainView;
	sf::Event					gameEvent;

	const int					WIDTH = 1600;
	const int					HEIGHT = 900;

	sf::Font					titleFont;
	sf::Font					baseFont;
	sf::Text					stateText;

	sf::RectangleShape			gridRct;
	sf::Vector2f				gridSize = { 25,25 };
	bool						renderGrid = false;

	bool						debugMouse = false;
	bool						isMouseOverButton = false;

	sf::CircleShape				mouseShape;

	sf::Clock					clock;
	sf::Time					elapsedTime;
	float						dt = 0;

	AudioManager				audioManager;
	sf::SoundBuffer*			saveSound = nullptr;
	CharactersManager*			charactersManager = nullptr;

	const char*					entities[6] = {"none", "wall", "deathZone", "checkpoint", "winzone", "ball item"};
	const char*					selectedEntity = "";
	std::vector<Enemy*>			enemiesList;

	Menu*						currentMenu = nullptr;
	World*						world;

	int							imIdx = 0;

	float						joystickMenuSelection_CD = 0.15f;
	float						joystickMenuSelection_TIMER = 0.15f;

	sf::Vector2i				pastMousePos;

	const char*					playerDataPath = "Assets/Data/player.txt";
	const char*					playerSavedDataPath = "Assets/Data/Save/player.txt";
	const char*					optionsDataPath = "Assets/Data/options.txt";
	bool						loadSave = false;

	// funcs

	void initWindow();
	void initFonts();
	void closeWindow();
	void initMusic();
	void initPlayer();
	void initWorld();
	void initGrid();

	void initMainMenu();
	void initPauseMenu();
	void initGameOverMenu();
	void initWinMenu();
	void initOptionsMenu();
	void initTutoMenu();

	void loadMainMenu();
	void unloadMainMenu();

	void loadGame();
	void loadGameFromSave();
	void unloadGame();

	void saveOptions();
	void loadOptions();

	void pressSelectedButton();
	bool pressSelectedButtonOptions(bool positiveAmount);

public:

	enum ControllerButtons
	{
		south = 0,
		east = 1,
		west = 2,
		north = 3,
		start = 7,
		left = 8,
		right = 9,
	};

	// vars
	float controllerDeadZone = 20;

	Player*						player;

	sf::Vector2f				windowSize;
	sf::Vector2f				windowCenter;

	int							stride = 32;

	float						gravity = 2.976f;

	enum class						GameState
	{
		MainMenu,
		InGame,
		Pause,
		GameOver,
		Win,
		ItemPickup,
	};
	GameState					GS = GameState::MainMenu;

	// funcs

	Game();
	~Game();


	void update();

	bool checkIfBulletHitsEnemy(int _cx, int _cy, float damages, int knockbackForce = 0);
	bool checkIfPlayerTouchCheckpoint();
	bool checkIfPlayerEntersInWinZone();
	bool checkIfPlayerTouchesItem();

	void savePlayer(const char* filePath);
	void loadPlayer(const char* filePath);

	void savePlayerDataInFile();
	void loadPlayerDataInFile();

	void savePlayerDataInSave();
	void loadPlayerDataFromSave();

	// keys managers
	void checkPressedKey(sf::Keyboard::Key key);
	void checkReleasedKey(sf::Keyboard::Key key);
	void checkPressedMouse(sf::Keyboard::Key key);
	void checkPressedJoystic(sf::Event::JoystickButtonEvent buttonEvent);
	void checkReleasedJoystic(sf::Event::JoystickButtonEvent buttonEvent);
	void checkJoysticAxis(sf::Joystick::Axis axis);

	// imgui
	void processImGui();
	void charactersImGui(Character* chara, int idx, bool isPlayer = false);

	// renderers
	void moveCamera(float x, float y);
	void drawGrid();
	void render();
	void renderWorldAndCharacters();

	// dt
	float deltaTime();

	// gets
	World* getWorld();
	sf::RenderWindow& getWindow();
	sf::Vector2f getMousePosition();

	// sets
	void setGameState(GameState _GS);
	void activateStateText(std::string text = "");
	void activateStateText(sf::Vector2f pos, std::string text = "");
	void deactivateStateText();

};

