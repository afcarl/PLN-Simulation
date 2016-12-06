#include <LispAPI.h>

#include <Evaluator.hpp>
#include <NeuralNetwork.hpp>

#include <cassert>

#include <SFML/Graphics.hpp>
#include <Evaluator.hpp>

#include <EncodedNetworkHelpers.hpp>

/*
 * You could initialize a global render window and watch the progress - say
 * show every beaten record on the screen. However, it is not really needed,
 * because winner nets will be saved as files and can be read back later.
void c_inititialize_experiment()
{
    view.reset(sf::FloatRect(0, 0, 
                             Constants::workspace.x, Constants::workspace.y));
    view.rotate(180);
    view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    window.setView(view);
    window.setFramerateLimit(60);
}
*/

float xor_evaluate_network(NeuralNetwork<3, 1> nn)
{
    float errorsum = 0;

    nn.supply_inputs({1, 0, 0});
    nn.run_network();
    errorsum += fabs(nn.outputs[0]);

    nn.supply_inputs({1, 1, 0});
    nn.run_network();
    errorsum += fabs(1.0 - nn.outputs[0]);

    nn.supply_inputs({1, 0, 1});
    nn.run_network();
    errorsum += fabs(1.0 - nn.outputs[0]);

    nn.supply_inputs({1, 1, 1});
    nn.run_network();
    errorsum += fabs(nn.outputs[0]);

    return pow((4.0 - errorsum), 2);
}

#if __cplusplus
extern "C" 
#endif
float c_evalme(EncodedNeuron n[MAX_NEURONS_PER_NETWORK], int neuron_count)
{
    assert(neuron_count < MAX_NEURONS_PER_NETWORK);

    //std::cout << "---- Received neurons: " << neuron_count << std::endl;

    //for(int i = 0; i < neuron_count; i++)
        //print_encoded_neuron(n[i]);
        //
    auto nn = reconstruct_neural_network<9, 5>(n, neuron_count);

    auto fitness = pln_evaluate_network(nn);
    //std::cout << std::endl << fitness << std::endl;
    return fitness;
}
