#include <LinearAlgebraHelpers.hpp>

#include <cmath>

float distance(const sf::Vector2f &v1, const sf::Vector2f &v2)
{
    return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}

float dot_product(const sf::Vector2f &v1, const sf::Vector2f &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

float magnitude(const sf::Vector2f &v)
{
    return sqrt(dot_product(v, v));
}

// Assumes the magnitude of v to be non-zero (to avoid divison by zero)
sf::Vector2f unit(const sf::Vector2f &v)
{
    return v / magnitude(v);
}

sf::Vector2f rotate(const sf::Vector2f &v, float theta)
{
    return { float(v.x * cos(theta) - v.y * sin(theta)),
             float(v.x * sin(theta) + v.y * cos(theta)) };
}

//does not check for magnitudes being zero
float angle_between(const sf::Vector2f &v1, const sf::Vector2f &v2)
{
    
    //Sometimes, due to float arithmetic, when the angle
    //is calculated, the argument to acos may be just slightly
    //above/below 1/-1, which is out of its domain.
    //So the following is used to prevent acos from receiving a wrong argument:
    auto level = [] (float x) -> float
    {
        if (x < -1.f)
            return -1.0f;
        if (x > 1.f)
            return 1.0f;
        return x;
    };
    
    float
        a = magnitude(v1),
        b = magnitude(v2),
        c = magnitude(v2 - v1);
    
    
    //TRACE(level((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b)));
    //TRACE((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b));
    //TRACE(pow(a, 2) + pow(b, 2) - pow(c, 2));
    //TRACE(2 * a * b);
    
    //find angle using cosine formula
    return acos(level((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b)));
}
