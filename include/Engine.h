#pragma once

#include "Entity.h"
#include <SFML/Graphics.hpp>

class Engine {

public:
	Engine();
	~Engine();

	void run();

private:
	void initalizeAssets();

	void handleEvents();
	void handleInputs();

	void layoutGUI();

	void updateFrame();
	void renderFrame();


	sf::RenderWindow window;
	sf::Clock dtGUI;

	float frameTime = 0.0;
	bool drawDebug = false;

	std::vector<std::shared_ptr<Entity>> entities;
};