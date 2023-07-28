#include "Entity.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <string>
#include <array>

Entity::Entity(std::string file) {

	texture.loadFromFile(file);
	vertices.setPrimitiveType(sf::Triangles);
	vertices.resize(6);

	float x = static_cast<float>(texture.getSize().x);
	float y = static_cast<float>(texture.getSize().y);

	vertices[0].texCoords = sf::Vector2f(0.0, 0.0);
	vertices[1].texCoords = sf::Vector2f(x, 0.0);
	vertices[2].texCoords = sf::Vector2f(0.0, y);
	vertices[3].texCoords = sf::Vector2f(0.0, y);
	vertices[4].texCoords = sf::Vector2f(x, 0.0);
	vertices[5].texCoords = sf::Vector2f(x, y);
		
	vertices[0].position = sf::Vector2f(0.0, 0.0);
	vertices[1].position = sf::Vector2f(x, 0.0);
	vertices[2].position = sf::Vector2f(0.0, y);
	vertices[3].position = sf::Vector2f(0.0, y);
	vertices[4].position = sf::Vector2f(x, 0.0);
	vertices[5].position = sf::Vector2f(x, y);
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	states.transform *= getTransform();
	states.texture = &texture;

	target.draw(vertices, states);
}

void Entity::drawDebug(sf::RenderTarget& target, sf::RenderStates states) const {

	states.transform *= getTransform();

	sf::Vertex outline[] = { vertices[2], vertices[0], vertices[1], vertices[5], vertices[2], vertices[1] };
	for (size_t i = 0; i < vertices.getVertexCount(); i++) {
		outline[i].color = sf::Color::Red;
	}

	target.draw(outline, vertices.getVertexCount(), sf::LineStrip, states);
}