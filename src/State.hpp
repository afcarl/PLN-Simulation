#ifndef STATE_HPP
#define STATE_HPP

#include <Ball.hpp>
#include <Function.hpp>

#include <SFML/System.hpp>

#include <array>

struct State
{
    typedef std::array<int, 5> InputArray;
    
    State();
    
    void update(const sf::Time dt);
    void update_cost(float new_f_cost);
    void process_input(const InputArray &input);
    
    void set_parent(const State *p);
    
    bool operator > (const State &right) const;
    bool is_goal() const;
    
    Ball ball;
    Function function, old_function;
    sf::Vector2f closest_point; //on the graph to the ball, for rendering only
    
    float f_cost, cost;
    
    //bookkeeping needed for the A* procedures
    const State * parent;
    InputArray input; //input applied to this state before calling update
};

#endif
