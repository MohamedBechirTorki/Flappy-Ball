#ifndef PIPE_HPP
#define PIPE_HPP

#include <SFML/Graphics.hpp>
#include "Player.hpp"  // Include Player.hpp here

class Pipe {
public:
    Pipe(float windowWidth, float windowHeight, float gap);
    void update(sf::Time dt, float pipeSpeed);
    void draw(sf::RenderWindow& window);
    bool collidesWith(const Player& player) const;  // Use Player in collision
    float getX() const;
    float getWidth() const;

    bool passed = false;

private:
    sf::RectangleShape topPipe;
    sf::RectangleShape bottomPipe;
    float gap;
    float x;
    float width;
    float height;
};

#endif // PIPE_HPP
