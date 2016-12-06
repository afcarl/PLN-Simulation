#include <Evaluator.hpp>

NN::Inputs          get_inputs_for_neural_network(State &state);
State::InputArray   nn_output_to_state_input(NN::Outputs &outputs);

#include <Renderer.hpp>
#include <cassert>

float pln_evaluate_network(NN &nn)
{
    double fitness = 0;

    State state;
    const int simulation_frames = 3 * 60;

    for (int i = 0; i < simulation_frames; i++)
    {
        nn.flush();
        auto nn_inputs = get_inputs_for_neural_network(state);
        nn.supply_inputs(nn_inputs);
        nn.run_network();
        auto inputs_to_state = nn_output_to_state_input(nn.outputs);
        state.process_input(inputs_to_state);
        state.update(Constants::TimePerFrame);
    }

    //fitness calculation: +2000 so that it is not negative
    fitness = state.ball.position.x + 2000;
    if (fitness < 0)
        fitness = 0.0001;

    return fitness;
}

NN::Inputs get_inputs_for_neural_network(State &state)
{
    int k = 0;

    NN::Inputs inputs;

    //5 inputs
    //x coordinates are constants
    for (auto &point: state.function.interp_points)
        inputs[k++] = point.y;

    //4 inputs
    inputs[k++] = state.ball.velocity.x;
    inputs[k++] = state.ball.velocity.y;
    inputs[k++] = state.ball.position.x;
    inputs[k++] = state.ball.position.y;

    return inputs;
}


State::InputArray nn_output_to_state_input(NN::Outputs &outputs)
{
    State::InputArray input_to_state;
    assert(outputs.size() == input_to_state.size());
    for (unsigned int i = 0; i < outputs.size(); i++)
    {
        float &x = outputs[i];

        //x is output of a sigmoid function
        if (x > 2.f / 3.f)
            input_to_state[i] = -1;
        else
        {
            if (x > 1.f / 3.f)
                input_to_state[i] = 0;
            else
                input_to_state[i] = 1; //if (x > 0)
        }
    }

    return input_to_state;
}
