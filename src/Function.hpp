#ifndef FUNCTIONHPP
#define FUNCTIONHPP

#include <SFML/System.hpp>

#include <vector>

struct Function
{
    Function(float range);
    
    float at(float x) const;
    float derivative_at(float x) const;
    
    //float a = -1, b = 1333.3333, c = 100;
    
    std::vector<sf::Vector2f> interp_points;
    
    static const int n = 5; //number of interpolation points
    static const int offset = 500;
};

#endif
