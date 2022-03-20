#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include<iostream>
#include<vector>
#include<sstream>
#include<fstream>
class Game
{
	bool gameOver;

	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event even;

	//tracking mouse position
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//sprites
	sf::Sprite bird;
	sf::Sprite halfTube1; //tube consists of two halftubes
	sf::Sprite halfTube2;
	std::vector<sf::Sprite> tubes; //every halftube stores in vector of sprites

	sf::Image icon; //icon for window
	sf::Texture textureGround;
	sf::Texture textureBackground;
	sf::Texture textureBird;
	sf::Texture textureTube;
	sf::Texture textureFinScore; 
	sf::Texture textureRestart;

	sf::RectangleShape ground;
	sf::Sprite background;
	sf::Sprite finalScore;
	sf::Sprite restart;

	sf::IntRect birdSource; //to activate bird animation

	sf::SoundBuffer flyUpSoundBuf;
	sf::Sound flyUpSound;
	sf::SoundBuffer fallingBuf;
	sf::Sound fallingAfterLose;
	sf::SoundBuffer collidingBuf;
	sf::Sound colliding;
	sf::SoundBuffer scoringPointBuf;
	sf::Sound scoringPoint;
	sf::SoundBuffer whistleBuf;
	sf::Sound whistleAfterLose;

	float gravity;  //gravity that velocity constantly increases with
	float velocity; //velocity bird is falling with
	float emptySpace; //space between two halves of tubes
	float pointsNumber; //number of points
	float highscoreNumber; //number of highscore
	bool mouseHeld; //check if mouse is held
	bool pauseState; //state game is in when started and waiting for input

	sf::Clock birdFloat; //during pause state bird floats
	sf::Clock tubeSpawnTimer; //tubes are spawned after certain periods of time
	sf::Clock flyUp; //animation of flying up
	sf::Clock birdAnimation; //moving wings animation
	sf::Clock deathScreenAnimation; //animation of final score screen becoming less transparent and going down

	sf::Text highscore; //highscore text during final score screen
	sf::Text points; //amount of points constantly being displayed
	sf::Font FB; //just font
	std::stringstream ss; //string that changes points and highscore

public:
	Game();
	virtual ~Game();

	const bool running()const; //if window is open

	void pollEvents(); //checking for mouse or keyboard being pressed
	void initVars(); //initializing variables
	void initWindow();
	void initBird();
	void initTubes();
	void spawnTubes();
	void initPoints();
	void initSprites();
	void initSounds();

	void updateTubes();
	void drawTubes(sf::RenderTarget& target);
	void updateBird();
	void drawBird(sf::RenderTarget& target);
	void updatePoints();
	void drawPoints(sf::RenderTarget& target);
	void drawBackground(sf::RenderTarget& target);
	void updateGround();
	void drawGround(sf::RenderTarget& target);
	void updateDeathScreen();
	void drawDeathScreen(sf::RenderTarget& target);
	void updateMousePosition();

	void update();
	void draw();
};

