#include "Engine.h"
#include "Buffer.h"

#include <iostream>
#include <Windows.h>


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>


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

	}
}

void luminaCore::Engine::layoutGUI() {



	ImGui::SFML::Update(window, dtGUI.restart());
	ImGuiIO& io = ImGui::GetIO();

	static float t = 0;
	static ScrollingBuffer frameTimeBuffer;
	t += ImGui::GetIO().DeltaTime;


	//ImGui::ShowDemoWindow();
	//ImPlot::ShowDemoWindow();

	frameTimeBuffer.AddPoint(t, frameTime * 1000.0);

	ImGui::Begin("Debug");
	ImGui::Text("Application running since %.0fs", t);
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
		ImPlot::PlotLine("Frametime", &frameTimeBuffer.Data[0].x, &frameTimeBuffer.Data[0].y, frameTimeBuffer.Data.size(), 0, frameTimeBuffer.Offset, 2 * sizeof(float));
		ImPlot::EndPlot();
	}
	ImGui::End();

	ImGui::Begin("Circle");
	ImGui::DragFloat("Size", &circle1size, 1.0, 100.0, 400.0);
	ImGui::ColorEdit4("Color", circle1color, ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
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