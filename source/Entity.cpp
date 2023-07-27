#include "Entity.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <string>
#include <iostream>

Entity::Entity(std::string file) {

	texture.loadFromFile(file);
	vertices.setPrimitiveType(sf::Triangles);
	vertices.resize(6);

	vertices[0].texCoords = sf::Vector2f(0.0, 0.0);
	vertices[1].texCoords = sf::Vector2f(static_cast<float>(texture.getSize().x), 0.0);
	vertices[2].texCoords = sf::Vector2f(0.0, static_cast<float>(texture.getSize().y));
	vertices[3].texCoords = sf::Vector2f(0.0, static_cast<float>(texture.getSize().y));
	vertices[4].texCoords = sf::Vector2f(static_cast<float>(texture.getSize().x), 0.0);
	vertices[5].texCoords = sf::Vector2f(static_cast<float>(texture.getSize().x), static_cast<float>(texture.getSize().y));
		

	float width = static_cast<float>(texture.getSize().x), height = static_cast<float>(texture.getSize().y);

	vertices[0].position = sf::Vector2f(0.0, 0.0);
	vertices[1].position = sf::Vector2f(width, 0.0);
	vertices[2].position = sf::Vector2f(0.0, height);
	vertices[3].position = sf::Vector2f(0.0, height);
	vertices[4].position = sf::Vector2f(width, 0.0);
	vertices[5].position = sf::Vector2f(width, height);


	//gen transparent hack stuff
	sf::Vector2u textureSize = texture.getSize();

	sf::Image image;
	image.create(textureSize.x, textureSize.y);

	for (unsigned int x = 0; x < textureSize.x; ++x) {
		for (unsigned int y = 0; y < textureSize.y; ++y) {
			sf::Color pixelColor = image.getPixel(x, y);
			pixelColor.a = 0;
			image.setPixel(x, y, pixelColor);
		}
	}

	transparent.loadFromImage(image);
	//i hope image gets destroyed after this, i dont know anythign aout lifetime haha

}

Entity::~Entity() {
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	states.transform *= sf::Transformable::getTransform();

	states.texture = &texture;

	target.draw(vertices, states);

}

void Entity::drawDebug(sf::RenderTarget& target, sf::RenderStates states) const {

     states.transform *= getTransform();

    for (std::size_t i = 0; i < vertices.getVertexCount(); i += 3) {
        sf::Vertex triangleOutline[] = {
            vertices[i + 0],
            vertices[i + 1],
            vertices[i + 2],
            vertices[i + 0]
        };

        for (std::size_t j = 0; j < 4; ++j) {
            triangleOutline[j].color = sf::Color::Red;
        }

        target.draw(triangleOutline, 4, sf::LineStrip, states);
    }

	//hacky stuff to get the vertex shapes to draw, without drawing a white missing texture box
    sf::RenderStates textureStates = states;
    textureStates.blendMode = sf::BlendAlpha;
    textureStates.texture = &transparent;
    target.draw(vertices, textureStates);
}