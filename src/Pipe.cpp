#include "Pipe.hpp"
#include "Player.hpp"

Pipe::Pipe(float windowWidth, float windowHeight, float gap)
    : gap(gap), width(50.f), height(200.f) {
    // Randomize pipe position (top and bottom)
    float topPipeHeight = static_cast<float>(rand() % static_cast<int>(windowHeight - gap));
    float bottomPipeHeight = windowHeight - topPipeHeight - gap;

    // Set the position and size of the pipes
    topPipe.setSize(sf::Vector2f(width, topPipeHeight));
    topPipe.setPosition(windowWidth, 0.f);
    topPipe.setFillColor(sf::Color::Green);

    bottomPipe.setSize(sf::Vector2f(width, bottomPipeHeight));
    bottomPipe.setPosition(windowWidth, topPipeHeight + gap);
    bottomPipe.setFillColor(sf::Color::Green);

    x = windowWidth;  // The starting x position of the pipe (off-screen to the right)
}

void Pipe::update(sf::Time dt, float pipeSpeed) {
    // Move the pipes left
    x -= pipeSpeed * dt.asSeconds();
    topPipe.setPosition(x, topPipe.getPosition().y);
    bottomPipe.setPosition(x, bottomPipe.getPosition().y);
}

void Pipe::draw(sf::RenderWindow& window) {
    window.draw(topPipe);
    window.draw(bottomPipe);
}

bool Pipe::collidesWith(const Player& player) const {
    // Collision detection with the player (implement it based on your player class)
    // This is a placeholder example for collision checking
    if (player.getBounds().intersects(topPipe.getGlobalBounds())) {
        return true;
    }
    if (player.getBounds().intersects(bottomPipe.getGlobalBounds())) {
        return true;
    }
    return false;
}

float Pipe::getX() const {
    return x;
}

float Pipe::getWidth() const {
    return width;
}
