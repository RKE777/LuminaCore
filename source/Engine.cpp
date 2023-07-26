#include "Engine.h"

#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

#include <implot/implot.h>


luminaCore::Engine::Engine() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE) {

	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(60);
	
	IMGUI_CHECKVERSION();
	ImGui::SFML::Init(window);

	ImPlot::CreateContext();

	std::cout << "luminaCore::Engine initialized." << std::endl;
}

luminaCore::Engine::~Engine() {

	ImPlot::DestroyContext();

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
	circle1.setFillColor(sf::Color(255, 255, 255, 255 ));
	circle1.setPointCount(64);

	texture1.loadFromFile("assets/sprite.png");
	sprite1.setTexture(texture1);
	sprite1.setTextureRect(sf::IntRect(0, 0, 32, 32));
	sprite1.setPosition(500.0, 100);
	sprite1.setScale(4.0, 4.0);

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
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Begin("Debug");
	ImGui::Text("Frametime: %.3f ms", 1000.0 / io.Framerate);
	ImGui::Text("FPS: %.1f", io.Framerate);
	ImPlot::BeginPlot("test");
	ImPlot::PlotDummy("test");
	ImPlot::EndPlot();
	ImGui::End();


	ImGui::Begin("Debug 2");
	ImGui::Text("Frametime: %.3f ms", frameTime * 1000.0);
	ImGui::Text("FPS: %.1f", 1.0 / frameTime);
	ImGui::End();

	ImGui::Begin("Circle");
	ImGui::DragFloat("Size", &circle1size, 1.0, 100.0, 400.0);
	ImGui::ColorEdit4("Color", circle1color, NULL);
	ImGui::End();

}

void luminaCore::Engine::updateFrame() {

	circle1.setRadius(circle1size);
	circle1.setFillColor(sf::Color((sf::Uint8)255 * circle1color[0], (sf::Uint8)255 * circle1color[1], (sf::Uint8)255 * circle1color[2], (sf::Uint8)255 * circle1color[3]));
}

void luminaCore::Engine::renderFrame() {

	window.clear(sf::Color::Black);

	//render
	window.draw(circle1);
	window.draw(sprite1);

	ImGui::SFML::Render(window);
	window.display();
}