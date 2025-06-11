/* Game.hpp */
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <fstream>
#include "Player.hpp"
#include "Pipe.hpp"
#include "SoundInput.hpp"

enum class GameState { Menu, Settings, Playing, GameOver };

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time dt);
    void render();
    void resetGame();
    void loadHighScore();
    void saveHighScore();
    void updateMusic();

    sf::RenderWindow window;
    GameState state;

    Player player;
    std::vector<Pipe> pipes;
    sf::Time pipeSpawnTimer;
    SoundInput soundInput;
    bool isGameOver;
    int score;
    int highScore;

    // Settings
    float volumeDivisor;
    bool musicEnabled;
    bool sfxEnabled;

    // Audio
    sf::Music menuMusic;
    sf::Music gameOverMusic;
    sf::SoundBuffer loseBuffer;
    sf::Sound loseSound;

    // Backgrounds
    sf::Texture menuBgTexture;
    sf::Texture gameBgTexture;
    sf::Texture gameOverBgTexture;
    sf::Sprite menuBgSprite;
    sf::Sprite gameBgSprite;
    sf::Sprite gameOverBgSprite;

    // UI Panel & Texts
    sf::RectangleShape panel;
    sf::Font font;
    sf::Text scoreText;
    sf::Text highScoreText;
    sf::Text menuText;
    sf::Text gameOverText;
    sf::Text settingsText;
};

#endif // GAME_HPP
