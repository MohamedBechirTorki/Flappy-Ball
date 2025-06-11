#include "Game.hpp"
#include <sstream>
#include <algorithm>
#include <iostream>

static const float PIPE_SPAWN_INTERVAL = 1.5f; //
static const float PIPE_SPEED = 200.f;
static const float PIPE_GAP = 200.f;

Game::Game()
: window(sf::VideoMode(400, 600), "Flappy Sound"),
  state(GameState::Menu),
  player(),
  pipeSpawnTimer(sf::Time::Zero),
  soundInput(),
  isGameOver(false),
  score(0),
  highScore(0),
  volumeDivisor(0.2f),
  musicEnabled(true),
  sfxEnabled(true)
{
    window.setFramerateLimit(60);

    // Load backgrounds
    if (!menuBgTexture.loadFromFile("assets/images/menu.jpeg"))
        std::cerr << "Error: Cannot load assets/images/menu.jpeg\n";
    menuBgSprite.setTexture(menuBgTexture);
    menuBgSprite.setScale(
        window.getSize().x / float(menuBgTexture.getSize().x),
        window.getSize().y / float(menuBgTexture.getSize().y));

    if (!gameBgTexture.loadFromFile("assets/images/gm.png"))
        std::cerr << "Error: Cannot load assets/images/gm.png\n";
    gameBgSprite.setTexture(gameBgTexture);
    gameBgSprite.setScale(
        window.getSize().x / float(gameBgTexture.getSize().x),
        window.getSize().y / float(gameBgTexture.getSize().y));

    if (!gameOverBgTexture.loadFromFile("assets/images/go.jpeg"))
        std::cerr << "Error: Cannot load assets/images/go.jpeg\n";
    gameOverBgSprite.setTexture(gameOverBgTexture);
    gameOverBgSprite.setScale(
        window.getSize().x / float(gameOverBgTexture.getSize().x),
        window.getSize().y / float(gameOverBgTexture.getSize().y));

    // Panel
    panel.setSize({ window.getSize().x * 0.8f, window.getSize().y * 0.6f });
    panel.setFillColor(sf::Color(20, 20, 20, 200));
    panel.setPosition(window.getSize().x * 0.1f, window.getSize().y * 0.2f);

    // Load font
    if (!font.loadFromFile("assets/fonts/OpenSans-Bold.ttf"))
        std::cerr << "Error: Cannot load assets/fonts/OpenSans-Bold.ttf\n";

    // Score texts
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20.f, 20.f);

    highScoreText.setFont(font);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor(sf::Color::White);
    highScoreText.setPosition(20.f, 50.f);

    // Menu text
    menuText.setFont(font);
    menuText.setCharacterSize(36);
    menuText.setFillColor(sf::Color(230, 109, 35));
    menuText.setString("Press SPACE to Start\nS: Settings");
    {
        auto bounds = menuText.getLocalBounds();
        menuText.setOrigin(bounds.width/2, bounds.height/2);
        menuText.setPosition(window.getSize().x/2, window.getSize().y*0.35f);
    }

    // Game Over text
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(32);
    gameOverText.setFillColor(sf::Color(255, 69, 0));
    gameOverText.setLineSpacing(1.5f);  // Increase line height

    // Initial origin/position
    {
        auto bounds = gameOverText.getLocalBounds();
        gameOverText.setOrigin(bounds.width/2, bounds.height/2);
        gameOverText.setPosition(window.getSize().x/2, window.getSize().y*0.35f);
    }

    // Settings text
    settingsText.setFont(font);
    settingsText.setCharacterSize(24);
    settingsText.setFillColor(sf::Color(40, 40, 40));
    settingsText.setPosition(panel.getPosition() + sf::Vector2f(20.f, 20.f));

    loadHighScore();

    // Load audio
    if (!menuMusic.openFromFile("assets/audio/menu.ogg"))
        std::cerr << "Error: Cannot load assets/audio/menu.ogg\n";
    menuMusic.setLoop(true);
    if (!gameOverMusic.openFromFile("assets/audio/gameover.ogg"))
        std::cerr << "Error: Cannot load assets/audio/gameover.ogg\n";
    gameOverMusic.setLoop(false);
    if (!loseBuffer.loadFromFile("assets/audio/lose.wav"))
        std::cerr << "Error: Cannot load assets/audio/lose.wav\n";
    loseSound.setBuffer(loseBuffer);

    updateMusic();
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        processEvents();
        sf::Time dt = clock.restart();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
        else if (event.type == sf::Event::KeyPressed) {
            GameState oldState = state;
            switch (state) {
                case GameState::Menu:
                    if (event.key.code == sf::Keyboard::Space) { state = GameState::Playing; resetGame(); }
                    else if (event.key.code == sf::Keyboard::S) state = GameState::Settings;
                    break;
                case GameState::Settings:
                    if (event.key.code == sf::Keyboard::Up)      volumeDivisor = std::max(0.01f, volumeDivisor - 0.01f);
                    else if (event.key.code == sf::Keyboard::Down) volumeDivisor += 0.01f;
                    else if (event.key.code == sf::Keyboard::M)    { musicEnabled = !musicEnabled; updateMusic(); }
                    else if (event.key.code == sf::Keyboard::N)    { sfxEnabled = !sfxEnabled;
                        if (!sfxEnabled && loseSound.getStatus()==sf::Sound::Playing) loseSound.stop(); }
                    else if (event.key.code == sf::Keyboard::Escape) state = GameState::Menu;
                    break;
                case GameState::Playing:
                    break;
                case GameState::GameOver:
                    if (event.key.code == sf::Keyboard::R) state = GameState::Menu;
                    break;
            }
            if (state != oldState) updateMusic();
        }
    }
}

void Game::update(sf::Time dt) {
    if (state == GameState::Playing && !isGameOver) {
        float vol = soundInput.getVolume();
        float norm = std::min(vol / volumeDivisor, 1.f);
        float targetY = (1.f - norm) * window.getSize().y;
        float curY = player.getY();
        float smooth = 5.f;
        float newY = curY + (targetY - curY) * smooth * dt.asSeconds();
        player.setY(newY);

        pipeSpawnTimer += dt;
        if (pipeSpawnTimer.asSeconds() >= PIPE_SPAWN_INTERVAL) {
            pipes.emplace_back(window.getSize().x, window.getSize().y, PIPE_GAP);
            pipeSpawnTimer = sf::Time::Zero;
        }
        for (auto& p : pipes) {
            p.update(dt, PIPE_SPEED);
            if (!p.passed && p.getX() + p.getWidth() < player.getX()) { p.passed = true; score++; }
            if (p.collidesWith(player)) isGameOver = true;
        }
        pipes.erase(std::remove_if(pipes.begin(), pipes.end(), [](auto& p){ return p.getX() + p.getWidth() < 0; }), pipes.end());

        std::ostringstream ss; ss << "Score: " << score;
        scoreText.setString(ss.str());

        if (player.getY() < 0 || player.getY() > window.getSize().y) isGameOver = true;
        if (isGameOver) {
            if (score > highScore) { highScore = score; saveHighScore(); }
            std::ostringstream go;
            go << "Game Over\nScore: " << score << "\nHigh Score: " << highScore << "\nPress R to Replay";
            gameOverText.setString(go.str());
            if (sfxEnabled) loseSound.play();
            state = GameState::GameOver;
            updateMusic();
        }
    }
}

void Game::updateMusic() {
    if (!musicEnabled) { menuMusic.stop(); gameOverMusic.stop(); return; }
    switch (state) {
        case GameState::Menu:
        case GameState::Settings:
            gameOverMusic.stop(); if (menuMusic.getStatus() != sf::Music::Playing) menuMusic.play(); break;
        case GameState::Playing:
            menuMusic.stop(); gameOverMusic.stop(); break;
        case GameState::GameOver:
            menuMusic.stop(); if (gameOverMusic.getStatus() != sf::Music::Playing) gameOverMusic.play(); break;
    }
}

void Game::render() {
    window.clear();
    if (state == GameState::Menu) window.draw(menuBgSprite);
    else if (state == GameState::Playing) window.draw(gameBgSprite);
    else if (state == GameState::GameOver) window.draw(gameOverBgSprite);
    else if (state == GameState::Settings) window.draw(menuBgSprite);

    switch (state) {
        case GameState::Menu:
            //window.draw(panel);
            window.draw(menuText);
            highScoreText.setString("High Score: " + std::to_string(highScore));
            window.draw(highScoreText);
            break;
        case GameState::Settings:
            //window.draw(panel);
            {
                std::ostringstream st;
                st << "Settings:\nSensitivity: " << volumeDivisor << " (Up/Down)\n"
                   << "Music (M): " << (musicEnabled ? "On" : "Off") << "\n"
                   << "SFX (N): " << (sfxEnabled ? "On" : "Off") << "\n"
                   << "Esc: Back";
                settingsText.setString(st.str());
                window.draw(settingsText);
            }
            break;
        case GameState::Playing:
            for (auto& p : pipes) p.draw(window);
            player.draw(window);
            window.draw(scoreText);
            break;
        case GameState::GameOver:
            //window.draw(panel);
            {
                auto bounds = gameOverText.getLocalBounds();
                gameOverText.setOrigin(bounds.width/2, bounds.height/2);
                gameOverText.setPosition(window.getSize().x/2, window.getSize().y*0.35f);
            }
            window.draw(gameOverText);
            break;
    }

    window.display();
}

void Game::resetGame() { player = Player(); pipes.clear(); score = 0; pipeSpawnTimer = sf::Time::Zero; isGameOver = false; }

void Game::loadHighScore() { std::ifstream f("highscore.txt"); f >> highScore; }

void Game::saveHighScore() { std::ofstream f("highscore.txt"); f << highScore; }
