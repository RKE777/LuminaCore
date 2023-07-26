#include "Engine.h"

#include <iostream>

#include <imGUI/imgui.h>
#include <imGUI/imgui-SFML.h>


luminaCore::Engine::Engine() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE) {

	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(60);

	ImGui::SFML::Init(window);
	std::cout << "luminaCore::Engine initialized." << std::endl;
}

luminaCore::Engine::~Engine() {

	ImGui::SFML::Shutdown();
	std::cout << "luminaCore::Engine shutdown." << std::endl;
}

void luminaCore::Engine::run() {

	initalizeAssets();

	sf::Clock frameTimer;

	while (window.isOpen()) {


		handleEvents();
		handleInputs();
		layoutGUI();
		updateFrame();
		renderFrame();

		frameTime = frameTimer.getElapsedTime().asSeconds();
		frameTimer.restart();
	}
}

void luminaCore::Engine::initalizeAssets() {

	circle1.setRadius(200.0);
	circle1.setFillColor(sf::Color(255 * 1.0, 255 * 1.0, 255 * 1.0, 255 * 1.0));
	circle1.setPointCount(64);

}

void luminaCore::Engine::handleEvents() {

	sf::Event event;

	while (window.pollEvent(event)) {

		ImGui::SFML::ProcessEvent(event);

		switch (event.type) {

		case sf::Event::Closed:
			window.close();
			break;
		}

	}
}

void luminaCore::Engine::handleInputs() {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window.close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {

		std::cout << "test" << std::endl;
	}
}

void luminaCore::Engine::layoutGUI() {

	ImGui::SFML::Update(window, deltaClock.restart());

	ImGui::Begin("Debug");
	ImGui::Text("Frametime: %.2f ms", frameTime * 1000.0);
	ImGui::Text("FPS: %.0f", 1.0 / frameTime);
	ImGui::End();

	ImGui::Begin("Circle");
	ImGui::DragFloat("Size", &circle1size, 1.0, 100.0, 400.0);
	ImGui::ColorEdit4("Color", circle1color, NULL);
	ImGui::End();

}

void luminaCore::Engine::updateFrame() {

	circle1.setRadius(circle1size);
	circle1.setFillColor(sf::Color(255 * circle1color[0], 255 * circle1color[1], 255 * circle1color[2], 255 * circle1color[3]));
}

void luminaCore::Engine::renderFrame() {

	window.clear(sf::Color::Black);

	//render
	window.draw(circle1);

	ImGui::SFML::Render(window);
	window.display();
}