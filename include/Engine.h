#pragma once

#include <SFML/Graphics.hpp>

namespace luminaCore {

	class Engine {

	public:
		Engine();
		~Engine();

	private:

		const char* WINDOW_TITLE = "LuminaCore";
		const int WINDOW_WIDTH = 1280;
		const int WINDOW_HEIGHT = 720;

		sf::RenderWindow window; 
		sf::Clock deltaClock;

	};
}