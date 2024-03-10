// Button.hpp
#pragma once

#include <SFML/Graphics.hpp>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color activeColor;
    bool mouseOverButton = false;

public:
    Button(sf::Vector2f position, sf::Vector2f size, sf::Font& font, std::string buttonText);
    ~Button();

    void Update(const sf::Vector2f& mousePosition);
    void Render(sf::RenderWindow& window);
    bool IsPressed() const;
    bool IsMouseOverButton();
};
