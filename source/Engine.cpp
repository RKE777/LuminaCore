#include "EngineConfig.h"
#include "Engine.h"
#include "Buffer.h"

#include <iostream>
#include <Windows.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>
#include <implot/implot.h>


Engine::Engine() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE) {

	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(120);
	
	IMGUI_CHECKVERSION();
	ImGui::SFML::Init(window);
	ImPlot::CreateContext();
	std::cout << "luminaCore::Engine initialized." << std::endl;
}

Engine::~Engine() {

	for (const auto &entity : entities) {
		entity.~Entity();
	}

	ImPlot::DestroyContext();
	ImGui::SFML::Shutdown();
	std::cout << "luminaCore::Engine shutdown." << std::endl;
}

void Engine::run() {

	initalizeAssets();
	sf::Clock dtFrame;

	while (window.isOpen()) {

		handleEvents();
		if (window.hasFocus()) {
			handleInputs();
		}
		layoutGUI();
		updateFrame();
		renderFrame();

		frameTime = dtFrame.getElapsedTime().asSeconds();
		dtFrame.restart();
	}
}

void Engine::initalizeAssets() {

	std::vector<std::string> files = { "assets/bg.png", "assets/dude.png" , "assets/cat.png"};

	for (const auto &file : files) {

		entities.push_back(Entity(file));
	}

	//TODO: reliable tracking of entities
	//Settings
	entities.at(0).scale(4.0, 4.0);
	entities.at(1).scale(8.0, 8.0);
	entities.at(2).scale(5.0, 5.0);
}

void Engine::handleEvents() {

	sf::Event event;

	while (window.pollEvent(event)) {

		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed) {

			window.close();
		}
	}
}

void Engine::handleInputs() {

	//TODO: input + colission is temporary

	const float movementSpeed = 150.0;
	const int playerNumber = 2;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window.close();
	}

	auto updatePlayerPosition = [this](float x, float y) {
		entities.at(playerNumber).setPosition(x, y);
	};

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		updatePlayerPosition(entities.at(playerNumber).getPosition().x, entities.at(playerNumber).getPosition().y - movementSpeed * frameTime);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		updatePlayerPosition(entities.at(playerNumber).getPosition().x, entities.at(playerNumber).getPosition().y + movementSpeed * frameTime);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		updatePlayerPosition(entities.at(playerNumber).getPosition().x - movementSpeed * frameTime, entities.at(playerNumber).getPosition().y);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		updatePlayerPosition(entities.at(playerNumber).getPosition().x + movementSpeed * frameTime, entities.at(playerNumber).getPosition().y);
	}


	if (entities.at(playerNumber).getPosition().x < 0.0f) {
		updatePlayerPosition(0.0f, entities.at(playerNumber).getPosition().y);
	}

	if (entities.at(playerNumber).getPosition().x > static_cast<float>(WINDOW_WIDTH)) {
		updatePlayerPosition(static_cast<float>(WINDOW_WIDTH), entities.at(playerNumber).getPosition().y);
	}

	if (entities.at(playerNumber).getPosition().y < 0.0f) {
		updatePlayerPosition(entities.at(playerNumber).getPosition().x, 0.0f);
	}

	if (entities.at(playerNumber).getPosition().y > static_cast<float>(WINDOW_HEIGHT)) {
		updatePlayerPosition(entities.at(playerNumber).getPosition().x, static_cast<float>(WINDOW_HEIGHT));
	}
}

void Engine::layoutGUI() {

	static ImGuiIO& io = ImGui::GetIO();
	static ScrollingBuffer frameTimeBuffer;
	static float t = 0.0;

	ImGui::SFML::Update(window, dtGUI.restart());

	t += frameTime;
	frameTimeBuffer.AddPoint(t, frameTime * 1000.0);

	ImGui::Begin("Debug");
	ImGui::Text("Running: %.1fs", t);
	ImGui::Text("FPS: %.1f", io.Framerate);
	ImGui::Text("FrameTime: %.3f ms", 1000.0 / io.Framerate);

	if (ImPlot::BeginPlot("FrameTime Graph", ImVec2(-1, 75), ImPlotFlags_NoFrame | ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs)) {
		ImPlot::PushStyleColor(ImPlotCol_PlotBg, { sf::Color::Transparent }); //Transparent Background

		//stuff to get the refresh rate of the monitor the window is currently beeing rendered to with vsync, to scale the frametime graph
		static HWND hWnd = window.getSystemHandle();

		static MONITORINFOEX monitorInfo = { sizeof(MONITORINFOEX) };
		GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &monitorInfo);

		static DEVMODE dm = { sizeof(dm) };
		EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &dm);
		//end
		

		static ImPlotAxisFlags xyFlags = ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickLabels;
		ImPlot::SetupAxes(nullptr, nullptr, xyFlags, xyFlags);
		ImPlot::SetupAxisLimits(ImAxis_X1, t - 10, t, ImGuiCond_Always);
		ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 2 * 1000.0 / dm.dmDisplayFrequency, ImGuiCond_Always);

		ImPlot::PlotLine("FrameTime", &frameTimeBuffer.data[0].x, &frameTimeBuffer.data[0].y, frameTimeBuffer.data.size(), 0, frameTimeBuffer.offset, sizeof(ImVec2));
		ImPlot::EndPlot();
		ImPlot::PopStyleColor(1); //Reset Transparent Background
	}
	
	ImGui::NewLine();
	ImGui::Text("Options");
	ImGui::Checkbox("BoundingBox", &drawDebug);
	ImGui::End();
}

void Engine::updateFrame() {
	
}

void Engine::renderFrame() {

	window.clear(sf::Color::Black);

	for (const auto entity : entities) {
		window.draw(entity);
		if (drawDebug) {
			entity.drawDebug(window, sf::RenderStates::Default);
		}
	}

	ImGui::SFML::Render(window);
	window.display();
}