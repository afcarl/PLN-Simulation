#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/System.hpp>

struct Constants
{
    //Workspace is the the boundary of the world in world coordinates.
    static const sf::Vector2f workspace;
    static const float pi, e;
    //Input step is for how many frames input will be supplied per step
    //of search. Used for uniform-cost search.
    static const int input_step;
    static const sf::Time TimePerFrame;
};

#endif
