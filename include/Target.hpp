#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>

// =========================
// TARGET
// =========================
class Target {
private:
    sf::Sprite* sprite;  
    int type;           
    float speed;
    float speedX;
    int hp;

    // =========================
    // ТЕКСТУРЫ (статические, грузятся один раз)
    // =========================
    static sf::Texture* tex[4];
    static bool loaded;

    static void loadTextures() {
        if (loaded) return;
        for (int i = 0; i < 4; i++) tex[i] = nullptr;
        
        tex[0] = new sf::Texture(); if (!tex[0]->loadFromFile("resources/enemy_4.png")) {}
        tex[1] = new sf::Texture(); if (!tex[1]->loadFromFile("resources/enemy_5.png")) {}
        tex[2] = new sf::Texture(); if (!tex[2]->loadFromFile("resources/enemyyy.png")) {}
        tex[3] = new sf::Texture(); if (!tex[3]->loadFromFile("resources/enemYy.png")) {}
        
        loaded = true;
    }

public:
    bool alive;
    int getType() const { return type; }

    // =========================
    // КОНСТРУКТОР
    // =========================
    Target(float x, float y, int t = 1) { 
        loadTextures();
        
        type = t;
        alive = true;
        speed = 3.5f;
        speedX = 0.f;
        hp = 1;

        // =========================
        // НАСТРОЙКА ПОВЕДЕНИЯ ПО ТИПУ
        // =========================
        if (type == 0) { speed = 4.0f; hp = 1; }           // Утка
        if (type == 1) { speed = 3.5f; hp = 1; }           // Черепаха
        if (type == 2) { speed = 0.f; speedX = 7.f; if (y < 0) y = 60.f; }  // Медведь
        if (type == 3) { speed = 2.5f; hp = 2; }           // Пришелец

        // =========================
        // СПРАЙТ (как у игрока)
        // =========================
        sprite = new sf::Sprite(*tex[type]);

        sf::Vector2u textureSize = tex[type]->getSize();

    // =========================
    // РАЗМЕР ДЛЯ КАЖДОГО ТИПА
    // =========================
        float image_scale = 0.15f; 

        if (type == 0) image_scale = 0.12f;   
        if (type == 1) image_scale = 0.35f;   
        if (type == 2) image_scale = 0.13f;   
        if (type == 3) image_scale = 0.15f;   

        sprite->setScale(sf::Vector2f(image_scale, image_scale));

        float image_x = image_scale * textureSize.x;
        float image_y = image_scale * textureSize.y;

        sprite->setPosition(sf::Vector2f(x, y));
    }

    // =========================
    // ДЕСТРУКТОР
    // =========================
    ~Target() {
        // delete sprite;  
    }

    // =========================
    // UPDATE
    // =========================
    void update() {
        if (!alive) return;

        if (type == 2) {
            // Медведь — летит горизонтально
            sprite->move(sf::Vector2f(speedX, 0));
            
            sf::FloatRect bounds = sprite->getGlobalBounds();
            if (bounds.position.x + bounds.size.x < 0 || bounds.position.x > 800) {
                alive = false; // Уходит за экран -> помечаем на удаление
            }
        } else {
            // Остальные — падают вниз
            sprite->move(sf::Vector2f(0, speed));
        }
    }

    // =========================
    // DRAW
    // =========================
    void draw(sf::RenderWindow& window) {
        if (alive) {
            window.draw(*sprite);
        }
    }

    // =========================
    // GET BOUNDS
    // =========================
    sf::FloatRect getBounds() const {
        return sprite->getGlobalBounds();
    }

    // =========================
    // GET Y
    // =========================
    float getY() const {
        return sprite->getPosition().y;
    }

    // =========================
    // TAKE DAMAGE
    // =========================
    void takeDamage() {
        // Утка не умирает от бомб
        if (type == 0) return;
        
        hp--;
        if (hp <= 0) {
            alive = false;
        }
    }
};

// =========================
// ИНИЦИАЛИЗАЦИЯ СТАТИКИ
// =========================
sf::Texture* Target::tex[4] = {nullptr, nullptr, nullptr, nullptr};
bool Target::loaded = false;