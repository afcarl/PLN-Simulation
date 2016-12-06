#include <FitnessEvaluator.hpp>

#include <Constants.hpp>

#include <Debug.hpp>

#include <cassert>

State::InputArray get_network_output(NEAT::Network *net, const State &state);

typedef std::vector<float> NetworkInput;

FitnessEvaluator::FitnessEvaluator(sf::RenderWindow &window)
    : window_(window)
    , renderer_(window)
{
}

double FitnessEvaluator::operator () (NEAT::Network *net)
{
    static double max_fitness = -1;
    double fitness = 0;

    State state;
    const int simulation_frames = 3 * 60;

    for (int i = 0; i < simulation_frames; i++)
    {
        state.process_input(get_network_output(net, state));
        state.update(Constants::TimePerFrame);
    }

    //fitness calculation: +2000 so that it is not negative
    fitness = state.ball.position.x + 2000;
    if (fitness < 0)
        fitness = 0.0001;

    if (max_fitness < fitness)
    {
        max_fitness = fitness;
        std::cout << "max_fitness is beaten = " << max_fitness << std::endl;
        demo(net, simulation_frames);
    }

    return fitness;
}

void FitnessEvaluator::demo(NEAT::Network *net, int simulation_frames)
{
    State state;

    for (int i = 0; i < simulation_frames; i++)
    {
        state.process_input(get_network_output(net, state));
        state.update(Constants::TimePerFrame);
        render(state);
    }
}

void FitnessEvaluator::render(const State &state)
{
    window_.clear(sf::Color::White);
    renderer_.render(state);
    window_.display();
}

int classify_output(double x)
{
    //x is output of a sigmoid function
    if (x > 2.f / 3.f)
        return -1;

    if (x > 1.f / 3.f)
        return 0; 

    //if (x > 0)
    return 1;
}

int cap(double x)
{
    if (x > 1)
        return 1;
    if (x < -1)
        return -1;
    return x;
}

//Given a state, feed the state to the network inputs and see what
//the output of the network is - this output becomes the input for
//generating next state..
State::InputArray get_network_output(NEAT::Network *net, const State &state)
{
    //form network input
    std::vector<float> network_input;

    //5 inputs
    //x coordinates are constants
    for (auto &point: state.function.interp_points)
        network_input.push_back(point.y);

    //4 inputs
    network_input.push_back(state.ball.velocity.x);
    network_input.push_back(state.ball.velocity.y);
    network_input.push_back(state.ball.position.x);
    network_input.push_back(state.ball.position.y);

    net->load_sensors(network_input);

    auto net_depth = net->max_depth();

    //Relax net and get output
    auto success=net->activate();

    //network output becomes state's new input
    State::InputArray network_output;
    for (auto &output: network_output)
        output = 0;

    if (!success) 
    {
        std::cout << "get_network_output failed to activate net" << std::endl;
        return network_output;
    }

    assert(net_depth >= 0 && net_depth < 100);

    //use depth to ensure relaxation
    for (int relax = 0; relax <= net_depth; relax++) 
        success=net->activate();

    for (unsigned int i = 0; i < network_output.size(); i++)
        network_output[i] = classify_output(net->outputs.at(i)->activation);

    net->flush();

    return network_output;
}
