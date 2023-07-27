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

	std::shared_ptr<Entity> test = std::make_shared<Entity>("assets/sprite.png");

	test->setScale(10.0, 10.0);
	//test.setScale(5.0, 5.0);
	entities.push_back(test);
	


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

	circle1.setRadius(200.0);
	circle1.setFillColor(sf::Color(255, 255, 255, 255 ));
	circle1.setPointCount(64);

	std::vector<std::string> files = { "assets/test.png", "assets/sprite.png"};
	//std::vector<std::shared_ptr<sf::Texture>> textures;
	//std::vector<sf::Sprite> sprites;

	for (const auto& file : files) {
		auto texture = std::make_shared<sf::Texture>();
		if (!texture->loadFromFile(file)) {
			// Handle error if the texture fails to load
			std::cerr << "Error loading texture: " << file << std::endl;
		}
		else {
			textures.push_back(texture);
			sprites.emplace_back(*texture); // Construct sprite using the texture
		}
	}




	//config
	sprites.at(0).setTextureRect(sf::Rect(0, 0, 1920, 1080));
	sprites.at(0).setScale(0.5, 0.5);

	sprites.at(1).setTextureRect(sf::IntRect(0, 0, 32, 32));
	sprites.at(1).setPosition(500.0, 100);
	sprites.at(1).setScale(4.0, 4.0);


}

void Engine::handleEvents() {

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

void Engine::handleInputs() {


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window.close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {

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

	ImGui::Begin("Circle");
	ImGui::DragFloat("Size", &circle1size, 1.0, 100.0, 400.0);
	ImGui::ColorEdit4("Color", circle1color, ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
	ImGui::End();

}

void Engine::updateFrame() {

	circle1.setRadius(circle1size);
	circle1.setOrigin(circle1size, circle1size);
	sf::Vector2 mouse = sf::Mouse::getPosition(window);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		circle1.setPosition(mouse.x, mouse.y);
	}
	circle1.setFillColor(sf::Color((sf::Uint8)255 * circle1color[0], (sf::Uint8)255 * circle1color[1], (sf::Uint8)255 * circle1color[2], (sf::Uint8)255 * circle1color[3]));
}

void Engine::renderFrame() {

	window.clear(sf::Color::Black);

	//render

	for (auto sprite : sprites)
	{
		window.draw(sprite);
	}

	for (const auto &entity : entities) {

		window.draw(*entity);
		if (drawDebug) {
			entity->drawDebug(window, sf::RenderStates::Default);
		}

	}


	//window.draw(circle1);

	ImGui::SFML::Render(window);
	window.display();
}