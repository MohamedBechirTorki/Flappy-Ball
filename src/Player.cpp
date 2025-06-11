#include "Player.hpp"

Player::Player()
: shape(20.f), velocity(0.f, 0.f)
{
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(100.f, 300.f);
}

void Player::jump() {
    velocity.y = JUMP_VELOCITY;
}

void Player::update(sf::Time dt) {
    // Disable jump gravity if using sound-based movement
    // velocity.y += GRAVITY * dt.asSeconds();
    // shape.move(0.f, velocity.y * dt.asSeconds());
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

float Player::getX() const { return shape.getPosition().x; }
float Player::getY() const { return shape.getPosition().y; }

void Player::setY(float y) {
    shape.setPosition(shape.getPosition().x, y);
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}
