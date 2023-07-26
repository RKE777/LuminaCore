#include "Engine.h"

#include <SFML/Graphics.hpp>
#include <imGUI/imgui.h>
#include <imGUI/imgui-SFML.h>
#include <iostream>

luminaCore::Engine::Engine() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE) {

	ImGui::SFML::Init(window);
	std::cout << "luminaCore::Engine initialized." << std::endl;
}

luminaCore::Engine::~Engine() {

	ImGui::SFML::Shutdown();
	std::cout << "luminaCore::Engine shutdown." << std::endl;
}
