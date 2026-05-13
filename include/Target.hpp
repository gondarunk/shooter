#pragma once

#include <SFML/Graphics.hpp>

class Target {
private:
    sf::CircleShape shape;
    float speed;

public:
    bool alive;

    Target(float x, float y) {
        shape.setRadius(20.f);
        shape.setFillColor(sf::Color::Red);

        shape.setPosition(sf::Vector2f(x, y));

        speed = 4.0f;
        alive = true;
    }

    void update() {
        shape.move(sf::Vector2f(0, speed));
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }

    float getY() const {
        return shape.getPosition().y;
    }
};