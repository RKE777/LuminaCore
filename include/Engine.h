#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>


namespace luminaCore {

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
		sf::Clock deltaClock;

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
	};
}