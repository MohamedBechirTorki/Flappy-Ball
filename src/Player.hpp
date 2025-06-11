#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void jump();
    void update(sf::Time dt);
    void draw(sf::RenderWindow& window) const;

    float getX() const;
    float getY() const;
    void setY(float y);              // <-- NEW METHOD
    sf::FloatRect getBounds() const;

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    static constexpr float GRAVITY = 900.f;
    static constexpr float JUMP_VELOCITY = -350.f;
};

#endif // PLAYER_HPP
