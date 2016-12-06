#ifndef ENCODED_NETWORK_HELPERS
#define ENCODED_NETWORK_HELPERS

#include <NeuralNetwork.hpp>

template<int inputs, int outputs>
NeuralNetwork<inputs, outputs>
reconstruct_neural_network(EncodedNeuron *encoded_neurons, int count)
{
    typename NeuralNetwork<inputs, outputs>::Neurons neurons;

    for (int i = 0; i < count; i++)
    {
        auto &en = encoded_neurons[i];
        Neuron::Type type = Neuron::Type::HIDDEN;
        switch (en.type)
        {
            case 0: type = Neuron::Type::INPUT;  break;
            case 1: type = Neuron::Type::HIDDEN; break;
            case 2: type = Neuron::Type::OUTPUT; break;
            default: assert(false); break;
        }

        Neuron neuron(en.id, type); 

        for (int j = 0; j < en.out_count; j++)
            neuron.outputs_id_weight.push_back({ en.out_nodes[j], 
                                                 en.out_weights[j] });

        neurons.push_back(neuron);
    }

    return NeuralNetwork<inputs, outputs>(neurons);
}

void print_encoded_neuron(const EncodedNeuron &n)
{
    assert(n.out_count < MAX_NEURONS_PER_NETWORK);

    std::cout << "id = " <<  n.id
              << " type = " << n.type
              << " count = " <<  n.out_count << std::endl
              << "l/w pairs: ";
    for (int i = 0; i < n.out_count; i++)
        std::cout << "("<< n.out_nodes[i] << ", " << n.out_weights[i] << ") ";
    std::cout << std::endl << std::endl;
}

#endif
