#ifndef BALL_HPP
#define BALL_HPP

#include <SFML/System.hpp>

struct Ball
{
    Ball(sf::Vector2f position, sf::Vector2f velocity, float radius);
    
    sf::Vector2f position, velocity;
    float radius;
    
    static const float max_x_speed;
};


#endif
