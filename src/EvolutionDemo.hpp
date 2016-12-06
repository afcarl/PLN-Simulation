#ifndef EVOLUTION_DEMO_HPP
#define EVOLUTION_DEMO_HPP

#include <NeuralNetwork.hpp>
#include <Constants.hpp>
#include <Renderer.hpp>
#include <Evaluator.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <string>

class EvolutionDemo
{
    enum class ScrollAction {NONE, FORWARD, BACKWARD};

public:
    EvolutionDemo(sf::RenderWindow &window);

    void run(int frames_per_network);

private:
    void read_winner_records();

    NN read_network(std::string &filename);

    ScrollAction process_user_input();

private:
    sf::RenderWindow &window_;

    Renderer renderer_;

    sf::Font font_;

    std::vector<NN> nets_;
    std::vector<std::string> nets_names_;
    std::vector<int> nets_fitness_;

    sf::Text text_;
};

#endif
