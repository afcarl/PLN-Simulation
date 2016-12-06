#include <Ball.hpp>

const float Ball::max_x_speed = 1000.f;

Ball::Ball(sf::Vector2f position, sf::Vector2f velocity, float radius)
    : position(position)
    , velocity(velocity)
    , radius(radius)
{
}
