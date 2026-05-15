#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "include/Bomb.hpp"
#include "include/Target.hpp"
#include <algorithm>

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // =========================
    // ОКНО
    // =========================
    int window_x = 800;
    int window_y = 600;

    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(window_x, window_y)),
        "Shooter Game"
    );
    window.setFramerateLimit(60);

    // =========================
    // ШРИФТ
    // =========================
    sf::Font font;

    if (!font.openFromFile("resources/arial.ttf"))
    {
        std::cout << "Ошибка загрузки шрифта!" << std::endl;
    }

    // =========================
    // ФОН
    // =========================
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("resources/background.png"))
    {
        std::cout << "⚠️ Фон не загружен" << std::endl;
    }

    sf::Sprite bgSprite(bgTexture);

    sf::Vector2u bgSize = bgTexture.getSize();
    float scaleX = window_x / (float)bgSize.x;
    float scaleY = window_y / (float)bgSize.y;

    bgSprite.setScale(sf::Vector2f(scaleX, scaleY));
    bgSprite.setPosition(sf::Vector2f(0, 0));

    // =========================
    // GAME OVER TEXT
    // =========================
    sf::Text gameOverText(font);

    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(100);
    gameOverText.setFillColor(sf::Color::Red);

    sf::FloatRect textBounds = gameOverText.getLocalBounds();

    gameOverText.setOrigin(
        sf::Vector2f(
            textBounds.position.x + textBounds.size.x / 2.0f,
            textBounds.position.y + textBounds.size.y / 2.0f
        )
    );

    gameOverText.setPosition(
        sf::Vector2f(window_x / 2.0f, window_y / 2.0f - 80)
    );

    // =========================
    // RESTART BUTTON
    // =========================
    sf::Text restartText(font);

    restartText.setString("Start again");
    restartText.setCharacterSize(50);
    restartText.setFillColor(sf::Color::White);

    sf::FloatRect restartBounds = restartText.getLocalBounds();

    restartText.setOrigin(
        sf::Vector2f(
            restartBounds.position.x + restartBounds.size.x / 2.0f,
            restartBounds.position.y + restartBounds.size.y / 2.0f
        )
    );

    restartText.setPosition(
        sf::Vector2f(window_x / 2.0f, window_y / 2.0f + 40)
    );

    // =========================
    // SCORE TEXT
    // =========================
    sf::Text scoreText(font);

    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);


    scoreText.setPosition(
        sf::Vector2f(window_x - 250, 20)
    );

    // =========================
    // ТЕКСТУРА ИГРОКА
    // =========================
    sf::Texture texture;

    bool textureLoaded = texture.loadFromFile("resources/cat.png");

    if (!textureLoaded)
    {
        std::cout << "Ошибка загрузки текстуры!" << std::endl;
    }

    sf::Sprite sprite(texture);

    sf::Vector2u textureSize = texture.getSize();

    float image_scale = 0.125f;

    sprite.setScale(sf::Vector2f(image_scale, image_scale));

    float image_x = image_scale * textureSize.x;
    float image_y = image_scale * textureSize.y;

    float posX = window_x / 2.0f - image_x / 2.0f;
    float posY = window_y - image_y;

    sprite.setPosition(sf::Vector2f(posX, posY));

    sf::Texture bombTexture;
    if (!bombTexture.loadFromFile("resources/bullka.png")) {}

    // =========================
    // GAME VARIABLES
    // =========================
    float angle = 0.0f;
    float scale = 2.0f;

    std::vector<Bomb> bombs;
    bombs.reserve(100);

    std::vector<Target> targets;

    bool gameOver = false;
    int score = 0;  

    sf::Clock targetSpawnClock;

    // =========================
    // MAIN LOOP
    // =========================
    while (window.isOpen())
    {
        // =========================
        // EVENTS
        // =========================
        while (auto event = window.pollEvent())
        {
            // CLOSE WINDOW
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // =========================
            // KEYBOARD
            // =========================
            if (event->is<sf::Event::KeyPressed>())
            {
                if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
                {
                    sf::Vector2f position = sprite.getPosition();

                    // Движение только если игра НЕ окончена
                    if (!gameOver)
                    {
                        if (keyEvent->code == sf::Keyboard::Key::Left)
                        {
                            sprite.setPosition(
                                sf::Vector2f(position.x - 20, position.y)
                            );
                        }

                        if (keyEvent->code == sf::Keyboard::Key::Right)
                        {
                            sprite.setPosition(
                                sf::Vector2f(position.x + 20, position.y)
                            );
                        }

                        // Выстрел
                        if (keyEvent->code == sf::Keyboard::Key::Space)
                        {
                            sf::Vector2f playerPos = sprite.getPosition();

                            bombs.emplace_back(
                                sf::Vector2f(
                                    playerPos.x + image_x / 2,
                                    playerPos.y + image_y / 2
                                ),
                                bombTexture
                            );
                        }
                    }

                    // Поворот
                    if (keyEvent->code == sf::Keyboard::Key::R)
                    {
                        angle += 15.0f;
                        sprite.setRotation(sf::degrees(angle));
                    }

                    // Масштаб
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

                    // ESC
                    if (keyEvent->code == sf::Keyboard::Key::Escape)
                    {
                        window.close();
                    }
                }
            }

            // =========================
            // MOUSE CLICK
            // =========================
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (gameOver)
                {
                    if (auto* mouseEvent =
                            event->getIf<sf::Event::MouseButtonPressed>())
                    {
                        sf::Vector2f mousePos(
                            static_cast<float>(mouseEvent->position.x),
                            static_cast<float>(mouseEvent->position.y)
                        );
                        //работа кнопки рестарт
                        if (restartText.getGlobalBounds().contains(mousePos))
                        {
                            gameOver = false;
                            score = 0; 
                            bombs.clear();
                            targets.clear();

                            // Возвращаем игрока
                            sprite.setPosition(
                                sf::Vector2f(
                                    window_x / 2.0f - image_x / 2.0f,
                                    window_y - image_y
                                )
                            );

                            targetSpawnClock.restart();
                        }
                    }
                }
            }
        }

        // =========================
        // SPAWN TARGETS
        // =========================
        if (!gameOver &&
            targetSpawnClock.getElapsedTime().asSeconds() > 1.0f)
        {
            float x = static_cast<float>(rand() % (window_x - 40));

            int type = rand() % 4;
            targets.emplace_back(x, -40, type);

            targetSpawnClock.restart();
        }

        // =========================
        // UPDATE BOMBS
        // =========================
        for (auto& bomb : bombs)
        {
            if (bomb.shooted)
            {
                if (!gameOver)
                {
                    bomb.update();
                }
            }
        }

        // =========================
        // UPDATE TARGETS
        // =========================
        for (auto& target : targets)
        {
            if (target.alive)
            {
                if (!gameOver)
                {
                    target.update();
                }
            }
        }

        // =========================
        // COLLISION PLAYER/TARGET
        // =========================
        for (auto& target : targets)
        {
            if (!target.alive)
                continue;

            if (sprite.getGlobalBounds()
                    .findIntersection(target.getBounds())
                    .has_value())
            {
                gameOver = true;
            }
        }

        // =========================
        // COLLISION BOMB/TARGET
        // =========================
        for (auto& bomb : bombs)
        {
            if (!bomb.shooted)
                continue;

            for (auto& target : targets)
            {
                if (!target.alive)
                    continue;

                if (bomb.getSprite()
                        .getGlobalBounds()
                        .findIntersection(target.getBounds())
                        .has_value())
                {
                    bool wasAlive = target.alive;

                    target.takeDamage();

                    if (wasAlive && !target.alive)
                    {
                        if (target.getType() == 1) score += 20;   
                        else if (target.getType() == 2) score += 60;  
                        else if (target.getType() == 3) score += 30;  
                    }

                    bomb.shooted = false;
                }
            }
        }

        // =========================
        // DELETE DEAD TARGETS
        // =========================
        targets.erase(
            std::remove_if(
                targets.begin(),
                targets.end(),
                [](Target& t)
                {
                    return !t.alive || t.getY() > 600;
                }),
            targets.end()
        );

        // =========================
        // DELETE USED BOMBS
        // =========================
        bombs.erase(
            std::remove_if(
                bombs.begin(),
                bombs.end(),
                [](Bomb& b)
                {
                    return !b.shooted;
                }),
            bombs.end()
        );

        // =========================
        // DRAW
        // =========================

        // Фон
        window.draw(bgSprite);

        // Игрок
        window.draw(sprite);

        // Бомбы
        for (auto& bomb : bombs)
        {
            if (bomb.shooted)
            {
                window.draw(bomb.getSprite());
            }
        }

        // Цели
        for (auto& target : targets)
        {
            if (target.alive)
            {
                target.draw(window);
            }
        }

        // Счёт
        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);

        // GAME OVER SCREEN
        if (gameOver)
        {
            window.draw(gameOverText);
            window.draw(restartText);
        }

        window.display();
    }

    return 0;
}