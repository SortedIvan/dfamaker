#include "SFML/Graphics.hpp"
#include "line.hpp"
#include <algorithm>
# define M_PI  3.14159265358979323846  /* pi */


sfLine::sfLine(const sf::Vector2f& point1, const sf::Vector2f& point2) :
    color(sf::Color::White), thickness(4.f)
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
    drawBoundingBox(target);
}


void sfLine::SetSelfLoopPoint(const sf::Vector2f& center, float radius, float curvature)
{
    thickness = 4.f;
    color = sf::Color::White;

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

void sfLine::setLinePoints(sf::Vector2f pointA, sf::Vector2f pointB)
{
    thickness = 4.f;
    color = sf::Color::White;

    float collisionThickness = 4.f;

    sf::Vector2f direction = pointB - pointA;
    float lineLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Determine the number of bounding boxes based on the line length
    int numBoundingBoxes = static_cast<int>(lineLength / 5); // boundingBoxSpacing is a predefined constant

    // Calculate the unit direction and offset
    sf::Vector2f unitDirection = direction / lineLength;
    sf::Vector2f offset = (thickness / 2.f) * sf::Vector2f(-unitDirection.y, unitDirection.x);

    // Clear existing bounding boxes
    boundingBoxes.clear();

    // Create evenly spaced bounding boxes
    for (int i = 0; i <= numBoundingBoxes; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(numBoundingBoxes);
        sf::Vector2f position = pointA + t * direction;
        sf::Vector2f boxOffset = (thickness / 2.f) * sf::Vector2f(-unitDirection.y, unitDirection.x);

        sf::FloatRect box;
        box.left = position.x - collisionThickness;
        box.top = position.y - collisionThickness;
        box.width = thickness + 2 * collisionThickness;
        box.height = lineLength / numBoundingBoxes + 2 * collisionThickness;

        boundingBoxes.push_back(box);
    }

    // Update the vertices based on the first bounding box
    vertices[0].position = pointA + offset;
    vertices[1].position = pointB + offset;
    vertices[2].position = pointB - offset;
    vertices[3].position = pointA - offset;

    for (int i = 0; i < 4; ++i) {
        vertices[i].color = color;
    }
}

// Add a function to draw the bounding boxes for debugging
void sfLine::drawBoundingBox(sf::RenderTarget& target) const
{
    sf::RectangleShape rect;
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(1.0f);
    rect.setFillColor(sf::Color::Transparent);

    for (const sf::FloatRect& box : boundingBoxes) {
        rect.setPosition(box.left, box.top);
        rect.setSize(sf::Vector2f(box.width, box.height));
        target.draw(rect);
    }
}

bool sfLine::CheckCollision(sf::Vector2f point) {
    for (int i = 0; i < boundingBoxes.size(); i++) {
        if (boundingBoxes[i].contains(point)) {
            return true;
        }
    }
    return false;
}

