#pragma once
#include "SFML/Graphics.hpp"
class sfLine : public sf::Drawable
{
    private:
        sf::Vertex vertices[4];
        float thickness;
        sf::Color color;
        sf::FloatRect boundingBox;
        std::vector<sf::FloatRect> boundingBoxes;
    public:                                       // todo: fix the fucking names
        sfLine(const sf::Vector2f& point1, const sf::Vector2f& point2);
        sfLine();
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void setLinePoints(sf::Vector2f pointA, sf::Vector2f pointB);
        void SetArrowColor(sf::Color color);
        void SetSelfLoopPoint(const sf::Vector2f& center, float radius, float curvature);
        void drawBoundingBox(sf::RenderTarget& target) const;
        bool CheckCollision(sf::Vector2f point);
};