#ifndef FITNESS_EVALUATOR_HPP
#define FITNESS_EVALUATOR_HPP

#include <State.hpp>
#include <Renderer.hpp>

#include <NEAT/network.h>

#include <SFML/Graphics.hpp>


class FitnessEvaluator
{
public:
    FitnessEvaluator(sf::RenderWindow &window);

    double operator () (NEAT::Network *net);

    void render(const State &state);
    void demo(NEAT::Network *net, int simulation_frames);

private:
    sf::RenderWindow &window_;
    Renderer renderer_;
};

#endif //FITNESS_EVALUATOR_HPP
