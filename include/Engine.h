#pragma once

#include <vector>

#include "Entity.h"
#include <SFML/Graphics.hpp>

class Engine {

public:
	Engine();
	~Engine();

	void run();

private:

	const char* WINDOW_TITLE = "LuminaCore";
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;

	sf::RenderWindow window;
	sf::Clock dtGUI;

	float frameTime = 0.0;

	void initalizeAssets();

	void handleEvents();
	void handleInputs();

	void layoutGUI();

	void updateFrame();
	void renderFrame();


	//assets:
	sf::CircleShape circle1;
	float circle1color[4] = { 1.0, 1.0, 1.0, 1.0 };
	float circle1size = 200.0;

	std::vector<sf::Sprite> sprites;
	std::vector<std::shared_ptr<sf::Texture>> textures;

	std::vector<std::shared_ptr<Entity>> entities;

	bool drawDebug = false;

	//
	

	//sf::Texture texture1;
	//sf::Sprite sprite1;
};