/**
 * @file line.cpp
 * @brief Description of the contents of the file.
 *
 * GNU GENERAL PUBLIC LICENSE
 * Version 3, 29 June 2007
 *
 * Copyright (C) [year] Ivan Ovcharov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * ---
 *
 * If you have questions or need further information, you can contact me through my GitHub:
 *
 * Ivan Ovcharov
 * GitHub: https://github.com/SortedIvan
 */


#include "SFML/Graphics.hpp"
#include "../hpp/line.hpp"
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
    //drawBoundingBox(target); //<------ FOR DEBUGGING
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

    boundingBoxes.clear();

    thickness = 4.f;
    color = sf::Color::White;

    float collisionThickness = 8.f;

    sf::Vector2f direction = pointB - pointA;
    float lineLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Determine the number of bounding boxes based on the line length
    int numBoundingBoxes = static_cast<int>(lineLength / 5); // boundingBoxSpacing is a predefined constant

    // Calculate the unit direction and offset
    sf::Vector2f unitDirection = direction / lineLength;
    sf::Vector2f offset = (thickness / 2.f) * sf::Vector2f(-unitDirection.y, unitDirection.x);

    // Clear existing bounding boxes
    //boundingBoxes.clear();

    // Create evenly spaced bounding boxes
    for (int i = 0; i <= numBoundingBoxes; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(numBoundingBoxes);
        sf::Vector2f position = pointA + t * direction;
        sf::Vector2f boxOffset = (collisionThickness / 2.f) * sf::Vector2f(-unitDirection.y, unitDirection.x);

        sf::FloatRect box;
        box.left = position.x - collisionThickness + boxOffset.x;
        box.top = position.y - collisionThickness + boxOffset.y;
        box.width = collisionThickness + 2 * collisionThickness;
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

