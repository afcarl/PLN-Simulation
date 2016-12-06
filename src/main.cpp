#include <SFML/Graphics.hpp>

#include <Application.hpp>
#include <Constants.hpp>
#include <Solver.hpp>
#include <FitnessEvaluator.hpp>

#include <NEAT/neatmain.h>
#include <NEAT/pln_test.hpp>
#include <NeuralNetwork.hpp>

#include <EvolutionDemo.hpp>

void show_evolution(sf::RenderWindow &window)
{
    EvolutionDemo demo(window);
    demo.run(180);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1067, 600),"SFML works!");
    sf::View view;
    view.reset(sf::FloatRect(0, 0, 
                             Constants::workspace.x, Constants::workspace.y));
    view.rotate(180);
    view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    window.setView(view);
    window.setFramerateLimit(60);

    //Uncomment to control ball yourself
    //Application application(window);
    //application.run2();

    show_evolution(window);
}
