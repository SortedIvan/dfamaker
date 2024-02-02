#pragma once
#include "SFML/Graphics.hpp"
class sfLine : public sf::Drawable
{
private:
    sf::Vertex vertices[4];
    float thickness;
    sf::Color color;
public:
    sfLine(const sf::Vector2f& point1, const sf::Vector2f& point2);
    sfLine();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void setLinePoints(sf::Vector2f pointA, sf::Vector2f pointB);
};