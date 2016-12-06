#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP

#include <Constants.hpp>

#include <tuple>
#include <algorithm>

#include <iostream>

float my_sigmoid(float t);

struct Neuron
{
    struct OutputsIDWeight 
    { 
        OutputsIDWeight(int id, float weight) : id(id), weight(weight) {}
        int id; 
        float weight; 
    };

    enum class Type {INPUT, HIDDEN, OUTPUT};

    Neuron (int id, Type type) :id(id), type(type) {}

    // INPUT NEURONS ARE NOT FIRED - the inputs of the simulation are already
    // the outputs of the input neurons! Otherwise the inputs would get
    // truncated without a chance of being weighted!
    void fire() 
    { 
        if (type != Type::INPUT) 
            output = my_sigmoid(total_weighted_input);
    }

    int id;
    Type type;
    float total_weighted_input = 0,
          output = 0;
    std::vector<OutputsIDWeight> outputs_id_weight;
};

template <int InputCount, int OutputCount>
struct NeuralNetwork
{
    typedef std::vector<Neuron> Neurons;
    typedef std::vector<Neuron *> NeuronsPtr;
    typedef std::array<float, InputCount> Inputs;
    typedef std::array<float, OutputCount> Outputs;

    // constructs network based on the given info
    NeuralNetwork(const Neurons &neurons);

    void supply_inputs(Inputs inputs);
    void flush();

    // after supplying the inputs and calling this function, the outputs will
    // be written to outputs
    void run_network();

    void run_network_r(NeuronsPtr &fire_row);

    Neurons    neurons;
    NeuronsPtr input_nodes;
    NeuronsPtr output_nodes;
    Outputs outputs;
};

template <int InputCount, int OutputCount>
NeuralNetwork<InputCount, OutputCount>::NeuralNetwork(const Neurons &neurons)
    : neurons(neurons)
{
    for (auto &n: this -> neurons)
    {   
        switch (n.type)
        {
            case Neuron::Type::INPUT:
                input_nodes.push_back(&n);
                break;
            case Neuron::Type::OUTPUT:
                output_nodes.push_back(&n);
                break;
            default:
                //hidden node, do nothin
                break;
        }
    }
}

template <int InputCount, int OutputCount>
void NeuralNetwork<InputCount, OutputCount>::supply_inputs(Inputs inputs)
{
    // IMPORTANT We do not fire input nodes: inputs don't go directly to the
    // sigmoid function, because in that case the inputs would be basically be
    // truncated without weighting them.
    for (unsigned int i = 0; i < inputs.size(); i++)
        input_nodes[i] -> output = inputs[i];
}

template <int InputCount, int OutputCount>
void NeuralNetwork<InputCount, OutputCount>::run_network()
{
    run_network_r(input_nodes);

    for (unsigned int i = 0; i < output_nodes.size(); i++)
        outputs[i] = output_nodes[i] -> output;
}

template <int InputCount, int OutputCount>
void NeuralNetwork<InputCount, OutputCount>::flush()
{
    for (auto &neuron: neurons)
        neuron.total_weighted_input = 0;
}

template <int InputCount, int OutputCount>
void NeuralNetwork<InputCount, OutputCount>::run_network_r(NeuronsPtr &fire_row)
{
    //std::cout << "the firing row has total inputs: " << std::endl;
    //for (const auto &el: fire_row)
        //std::cout << "(" << el -> id << ")->" << el -> total_weighted_input << " ";
    //std::cout << std::endl;

    NeuronsPtr next_fire_row;

    for (auto &fire_neuron: fire_row)
    {
        fire_neuron -> fire();
        for (auto &out_id_weight: fire_neuron -> outputs_id_weight)
        {
            for (auto &neuron: neurons)
                if (neuron.id == out_id_weight.id)
                {
                    neuron.total_weighted_input += 
                        fire_neuron -> output * out_id_weight.weight;
                    next_fire_row.push_back(&neuron);
                }
        }
    }

    std::sort(next_fire_row.begin(), next_fire_row.end());
    auto it = std::unique(next_fire_row.begin(), next_fire_row.end());
    next_fire_row.resize(std::distance(next_fire_row.begin(), it));

    if (next_fire_row.empty())
        return;

    run_network_r(next_fire_row);
}

#endif
