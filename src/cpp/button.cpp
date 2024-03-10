// Button.cpp
#include "../hpp/button.hpp"

Button::Button(sf::Vector2f position, sf::Vector2f size, sf::Font& font, std::string buttonText) {
    shape.setPosition(position);
    shape.setSize(size);

    text.setFont(font);
    text.setString(buttonText);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);

    text.setPosition(position);

    // Center the text within the button
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);

    // Set colors for button states
    idleColor = sf::Color::White;
    hoverColor = sf::Color(192, 192, 192);
    activeColor = sf::Color(255, 255, 255);
}

Button::~Button() {
    // Destructor if needed
}

void Button::Update(const sf::Vector2f& mousePosition) {
    // Check if the mouse is over the button
    if (shape.getGlobalBounds().contains(mousePosition)) {
        shape.setFillColor(hoverColor);

        // Check if the button is clicked
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            shape.setFillColor(activeColor);
        }

        mouseOverButton = true;
    }
    else {
        shape.setFillColor(idleColor);
        mouseOverButton = false;
    }
}

void Button::Render(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Button::IsPressed() const {
    return (shape.getFillColor() == activeColor);
}

bool Button::IsMouseOverButton() {
    return mouseOverButton;
}