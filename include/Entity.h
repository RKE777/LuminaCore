#pragma once
#include <SFML/Graphics.hpp>

class Entity : public sf::Drawable, public sf::Transformable {

public:

	Entity(std::string file);
	~Entity() = default;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void drawDebug(sf::RenderTarget& target, sf::RenderStates states) const;

private:

	sf::Texture texture;
	sf::VertexArray vertices;
};