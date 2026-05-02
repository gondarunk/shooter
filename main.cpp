#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "include/Bomb.hpp"


int main()
{
    // Создаем окно
    int window_x = 800;
    int window_y = 600;
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(window_x, window_y)), 
        "SFML Tutorial - Example 2: Sprites and Textures"
    );
    window.setFramerateLimit(60);
    
    // Шаг 1: Создание текстуры
    sf::Texture texture;
    
    // Попытка загрузить изображение
    // ВАЖНО: Путь к файлу должен быть правильным!
    // Если изображение не найдено, можно использовать простой цветной прямоугольник
    bool textureLoaded = texture.loadFromFile("../resources/cat.png");
    
    // Шаг 2: Создание спрайта
    // SFML 3.0: Конструктор Sprite требует текстуру
    sf::Sprite sprite(texture);
    
    // Шаг 4: Масштабирование (опционально)
    // SFML 3.0: setScale принимает Vector2f
    sf::Vector2u textureSize = texture.getSize();
    
    float image_scale = 0.15;
    sprite.setScale(sf::Vector2f(image_scale, image_scale));
    
    float image_x = image_scale * textureSize.x;
    float image_y = image_scale * textureSize.y;
    
    float posX = window_x / 2 - image_x / 2;
    float posY = window_y - image_y;
    
    // Убеждаемся, что позиция не отрицательная (если спрайт больше экрана)
    // posX = std::max(0.0f, posX);
    // posY = std::max(0.0f, posY);
    
    sprite.setPosition(sf::Vector2f(posX, posY));
    
    // Переменные для анимации
    float angle = 0.0f;
    float scale = 2.0f;
    bool growing = true;
    
    std::cout << "Управление:" << std::endl;
    std::cout << "  Стрелки - перемещение спрайта" << std::endl;
    // std::cout << "  R - поворот" << std::endl;
    std::cout << "  +/- - масштабирование" << std::endl;
    std::cout << "  ESC - выход" << std::endl;
    
    std::vector<Bomb> bombs;
    bombs.reserve(100);

    // Главный цикл
    while (window.isOpen())
    {
        // Обработка событий
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            
            // Обработка нажатий клавиш
            if (event->is<sf::Event::KeyPressed>())
            {
                // SFML 3.0: используем getIf для получения события
                if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
                {
                    // Перемещение стрелками
                    // SFML 3.0: setPosition принимает Vector2f
                    sf::Vector2f position = sprite.getPosition();
                    
                    if (keyEvent->code == sf::Keyboard::Key::Left)
                        sprite.setPosition(sf::Vector2f(position.x - 10, position.y));
                    if (keyEvent->code == sf::Keyboard::Key::Right)
                        sprite.setPosition(sf::Vector2f(position.x + 10, position.y));
                    // if (keyEvent->code == sf::Keyboard::Key::Up)
                    //     sprite.setPosition(sf::Vector2f(position.x, position.y - 10));
                    // if (keyEvent->code == sf::Keyboard::Key::Down)
                    //     sprite.setPosition(sf::Vector2f(position.x, position.y + 10));
                    
                    // Поворот
                    // SFML 3.0: setRotation принимает Angle, используем функцию sf::degrees()
                    if (keyEvent->code == sf::Keyboard::Key::R)
                    {
                        angle += 15.0f;
                        sprite.setRotation(sf::degrees(angle));
                    }
                    
                    if (keyEvent->code == sf::Keyboard::Key::Space) {
                        sf::Vector2f position = sprite.getPosition();
                        bombs.emplace_back(sf::Vector2f(position.x + image_x / 2, position.y + image_y / 2), texture);
                    }
                    // Масштабирование
                    // SFML 3.0: setScale принимает Vector2f
                    if (keyEvent->code == sf::Keyboard::Key::Equal || 
                        keyEvent->code == sf::Keyboard::Key::Add)
                    {
                        scale += 0.1f;
                        sprite.setScale(sf::Vector2f(scale, scale));
                    }
                    if (keyEvent->code == sf::Keyboard::Key::Hyphen || 
                        keyEvent->code == sf::Keyboard::Key::Subtract)
                    {
                        scale = std::max(0.1f, scale - 0.1f);
                        sprite.setScale(sf::Vector2f(scale, scale));
                    }
                    
                    // Выход
                    if (keyEvent->code == sf::Keyboard::Key::Escape)
                        window.close();
                }
            }
        }
        
        // Автоматическая анимация (пульсация)
        // static float time = 0.0f;
        // time += 0.02f;
        // float pulseScale = 2.0f + 0.3f * std::sin(time);
        // Можно раскомментировать для автоматической пульсации:
        // SFML 3.0: setScale принимает Vector2f
        // sprite.setScale(sf::Vector2f(pulseScale, pulseScale));
        
        // Очистка экрана
        window.clear(sf::Color(30, 30, 50));
        
        // Отрисовка спрайта
        window.draw(sprite);

        for (auto& bomb : bombs) {
            if (bomb.shooted) {
                bomb.update();
                window.draw(bomb.getSprite());
            }
        }
        
        // Отображение
        window.display();
    }
    
    return 0;
}
