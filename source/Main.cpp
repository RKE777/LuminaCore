#include <SFML/Graphics.hpp>

#include <imGUI/imgui.h>
#include <imGUI/imgui-SFML.h>

#include "Engine.h"

int main() {

    //luminaCore::Engine engine;

    sf::RenderWindow window(sf::VideoMode(1280, 720), "LuminaCore");
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {

            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Test");
        ImGui::Text("cool text right?");
        ImGui::End();


        window.clear(sf::Color::Black);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
