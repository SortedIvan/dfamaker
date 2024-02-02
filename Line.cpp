#include "SFML/Graphics.hpp"
#include "line.hpp"

sfLine::sfLine(const sf::Vector2f& point1, const sf::Vector2f& point2) :
    color(sf::Color::Yellow), thickness(4.f)
{
    sf::Vector2f direction = point2 - point1;
    sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

    sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

    vertices[0].position = point1 + offset;
    vertices[1].position = point2 + offset;
    vertices[2].position = point2 - offset;
    vertices[3].position = point1 - offset;

    for (int i = 0; i < 4; ++i)
        vertices[i].color = color;
}

sfLine::sfLine(){

}

void sfLine::SetArrowColor(sf::Color color) {
    this->color = color;

    for (int i = 0; i < 4; ++i) {
        vertices[i].color = color;
    }

}


void sfLine::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(vertices, 4, sf::Quads);
}

void sfLine::setLinePoints(sf::Vector2f pointA, sf::Vector2f pointB)
{
    thickness = 4.f;
    color = sf::Color::Yellow;

    float collisionThickness = 16.f;

    sf::Vector2f direction = pointB - pointA;
    sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

    sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

    vertices[0].position = pointA + offset;
    vertices[1].position = pointB + offset;
    vertices[2].position = pointB - offset;
    vertices[3].position = pointA - offset;

    for (int i = 0; i < 4; ++i) {
        vertices[i].color = color;
    }

    boundingBox.left = std::min(vertices[0].position.x - collisionThickness, std::min(vertices[1].position.x - collisionThickness, std::min(vertices[2].position.x - collisionThickness, vertices[3].position.x - collisionThickness)));
    boundingBox.top = std::min(vertices[0].position.y - collisionThickness, std::min(vertices[1].position.y - collisionThickness, std::min(vertices[2].position.y - collisionThickness, vertices[3].position.y - collisionThickness)));
    boundingBox.width = std::max(vertices[0].position.x + collisionThickness, std::max(vertices[1].position.x + collisionThickness, std::max(vertices[2].position.x + collisionThickness, vertices[3].position.x + collisionThickness))) - boundingBox.left;
    boundingBox.height = std::max(vertices[0].position.y + collisionThickness, std::max(vertices[1].position.y + collisionThickness, std::max(vertices[2].position.y + collisionThickness, vertices[3].position.y + collisionThickness))) - boundingBox.top;

}

sf::FloatRect sfLine::getBoundingBox() const
{
    return boundingBox;
}

void sfLine::SetSelfLoopPoint(const sf::Vector2f& center, float radius, float curvature)
{
    thickness = 4.f;
    color = sf::Color::Yellow;

    sf::Vector2f offset(thickness / 2.f, thickness / 2.f);

    vertices[0].position = center + offset;
    vertices[1].position = center + offset;
    vertices[2].position = center - offset;
    vertices[3].position = center - offset;

    // Adjust positions for the curved self-loop
    vertices[0].position.x += radius * std::cos(curvature);
    vertices[0].position.y += radius * std::sin(curvature);

    vertices[1].position.x += radius * std::cos(curvature + 3.14f);
    vertices[1].position.y += radius * std::sin(curvature + 3.14f);

    vertices[2].position.x += radius * std::cos(curvature);
    vertices[2].position.y += radius * std::sin(curvature);

    vertices[3].position.x += radius * std::cos(curvature + 3.14f);
    vertices[3].position.y += radius * std::sin(curvature + 3.14f);

    // Update the bounding box
    boundingBox.left = center.x - radius - thickness / 2.f;
    boundingBox.top = center.y - radius - thickness / 2.f;
    boundingBox.width = 2 * (radius + thickness / 2.f);
    boundingBox.height = 2 * (radius + thickness / 2.f);
}