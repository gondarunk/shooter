#include <SFML/Graphics.hpp>


class Bomb {
    sf::Sprite sprite;
    float scale = 0.1;
    float angle = 0.0f;

public:
    bool shooted = true;

    Bomb(sf::Vector2f position, const sf::Texture& texture) : sprite(texture) {
        sprite.setScale(sf::Vector2f(scale, scale));
        auto bounds = sprite.getLocalBounds();
        sprite.setOrigin(
            bounds.size / 2.f
        );
        sprite.setPosition(position);

    }

    sf::Sprite& getSprite() {
        return sprite;
    }

    void setPosition(float x, float y) {
        sprite.setPosition(sf::Vector2f(x, y));
    }

    void setRotation(float angle) {
        sprite.setRotation(sf::degrees(angle));
    }

    void update() {
        if (shooted) {
            sf::Vector2f current_pos = sprite.getPosition();
            sprite.setPosition(sf::Vector2f(current_pos.x, current_pos.y - 4));
            angle += 5.0f;
            sprite.setRotation(sf::degrees(angle));
            if (current_pos.y < 0)
                shooted = false;
        }
    }
};