#include "Engine.h"
#include "Buffer.h"
#include "EngineConfig.h"

#include <iostream>
#include <Windows.h>


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

#include <implot/implot.h>


Engine::Engine() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE) {

	window.setVerticalSyncEnabled(true);

	//window.setFramerateLimit(60);
	
	IMGUI_CHECKVERSION();
	ImGui::SFML::Init(window);
	ImPlot::CreateContext();
	std::cout << "luminaCore::Engine initialized." << std::endl;
}

Engine::~Engine() {

	ImPlot::DestroyContext();
	ImGui::SFML::Shutdown();
	std::cout << "luminaCore::Engine shutdown." << std::endl;
}

void Engine::run() {

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

void Engine::initalizeAssets() {

	std::vector<std::string> files = { "assets/bg.png", "assets/dude.png" , "assets/cat.png"};


	for (const auto& file : files) {

		std::shared_ptr<Entity> entityTemp = std::make_shared<Entity>(file);
		entities.push_back(entityTemp);
	}

	entities.at(0)->scale(2.0/3.0, 2.0 / 3.0);
	entities.at(1)->scale(2.0, 2.0);
	entities.at(2)->scale(5.0, 5.0);
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

	const float movementSpeed = 150.0;
	const int playerNumber = 2;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window.close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { //up

		entities.at(playerNumber)->setPosition(entities.at(playerNumber)->getPosition().x, entities.at(playerNumber)->getPosition().y - (movementSpeed * frameTime));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) { //down
		entities.at(playerNumber)->setPosition(entities.at(playerNumber)->getPosition().x, entities.at(playerNumber)->getPosition().y + (movementSpeed * frameTime));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { //left
		entities.at(playerNumber)->setPosition(entities.at(playerNumber)->getPosition().x - (movementSpeed * frameTime), entities.at(playerNumber)->getPosition().y);
			entities.at(playerNumber)->setScale(5.0, 5.0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { //right
		entities.at(playerNumber)->setPosition(entities.at(playerNumber)->getPosition().x + (movementSpeed * frameTime), entities.at(playerNumber)->getPosition().y);
		entities.at(playerNumber)->setScale(-5.0, 5.0);

	}
	//dont judge my colision stuff, its only that i dont loose my man in a tragic accident
	if (entities.at(playerNumber)->getPosition().x < 0.0) {
		entities.at(playerNumber)->setPosition(0.0, entities.at(playerNumber)->getPosition().y);
	}
	if (entities.at(playerNumber)->getPosition().x > static_cast<float>(WINDOW_WIDTH)) {
		entities.at(playerNumber)->setPosition(static_cast<float>(WINDOW_WIDTH), entities.at(playerNumber)->getPosition().y);
	}

	if (entities.at(playerNumber)->getPosition().y < 0.0) {
		entities.at(playerNumber)->setPosition(entities.at(playerNumber)->getPosition().x, 0.0);
	}
	if (entities.at(playerNumber)->getPosition().y > static_cast<float>(WINDOW_HEIGHT)) {
		entities.at(playerNumber)->setPosition(entities.at(playerNumber)->getPosition().x, static_cast<float>(WINDOW_HEIGHT));
		// accidently typed window_width here and lost one to the void, im sorry may the lord have mercy on him, i am so terribly sorry
	}
}

void Engine::layoutGUI() {

	ImGui::SFML::Update(window, dtGUI.restart());
	ImGuiIO& io = ImGui::GetIO();

	static float t = 0;
	static ScrollingBuffer frameTimeBuffer;
	t += ImGui::GetIO().DeltaTime;


	//ImGui::ShowDemoWindow();
	//ImPlot::ShowDemoWindow();

	frameTimeBuffer.AddPoint(t, frameTime * 1000.0);

	ImGui::Begin("Debug");
	ImGui::Text("Application running since %.1fs", t);
	ImGui::Text("Frametime: %.3f ms", 1000.0 / io.Framerate);
	ImGui::Text("FPS: %.1f", io.Framerate);

	if (ImPlot::BeginPlot("Frametime Graph", ImVec2(-1, 100), ImPlotFlags_NoFrame | ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs)) {
		ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks, ImPlotAxisFlags_Lock | ImPlotAxisFlags_NoGridLines);
		ImPlot::SetupAxisLimits(ImAxis_X1, t - 10, t, ImGuiCond_Always);

		//stuff to get the refresh rate of the monitor the window is currently beeing rendered to with vsync framelock enabled. wacky stuff, i know
		HWND hWnd = window.getSystemHandle();
		HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFOEX monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(hMonitor, &monitorInfo);

		DEVMODE dm;
		dm.dmSize = sizeof(dm);
		EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &dm);

		unsigned int refreshRate = dm.dmDisplayFrequency;
		//end

		ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 2 * 1000.0 / refreshRate, ImGuiCond_Always);
		ImPlot::PlotLine("Frametime", &frameTimeBuffer.data[0].x, &frameTimeBuffer.data[0].y, frameTimeBuffer.data.size(), 0, frameTimeBuffer.offset, 2 * sizeof(float));
		ImPlot::EndPlot();
	}

	ImGui::NewLine();
	ImGui::Text("Debug Options");
	ImGui::Checkbox("Show Vertecies", &drawDebug);
	ImGui::End();

}

void Engine::updateFrame() {
	
}

void Engine::renderFrame() {

	window.clear(sf::Color::Black);

	for (const auto &entity : entities) {

		window.draw(*entity);

		if (drawDebug) {
			entity->drawDebug(window, sf::RenderStates::Default);
		}
	}

	ImGui::SFML::Render(window);
	window.display();
}