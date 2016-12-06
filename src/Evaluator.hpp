#ifndef NN_EVALUATOR_HPP
#define NN_EVALUATOR_HPP

#include <State.hpp>
#include <NeuralNetwork.hpp>

typedef NeuralNetwork<9, 5> NN;

State::InputArray nn_output_to_state_input(NN::Outputs &outputs);
NN::Inputs get_inputs_for_neural_network(State &state);

float pln_evaluate_network(NN &nn);

#endif
