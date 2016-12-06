#include <EvolutionDemo.hpp>

#include <State.hpp>

#include <sstream>
#include <string>
#include <fstream>

#include <cassert>

EvolutionDemo::EvolutionDemo(sf::RenderWindow &window)
    : window_(window)
    , renderer_(window)
{
    font_.loadFromFile("Consolas.ttf");
    read_winner_records();
    text_.setFont(font_);
    text_.setFillColor(sf::Color::Black);
}

void print_neuron(Neuron &n)
{
    auto count = n.outputs_id_weight.size();
    std::cout << "id = " <<  n.id
              << " type = " << (n.type == Neuron::Type::INPUT ? "in " : "ot ");
    for (unsigned int i = 0; i < count; i++)
        std::cout << "(" << n.outputs_id_weight[i].id << ", " 
                  << n.outputs_id_weight[i].weight << ") ";
    std::cout << std::endl;
}

NN EvolutionDemo::read_network(std::string &filename)
{
    std::ifstream infile(filename + ".org");

    std::istringstream iss;

    // reading how many neurons there is - the number is in the first line
    int neurons_total = 0;
    std::string line;
    std::getline(infile, line);
    iss.str(line);
    iss >> neurons_total;

    //std::cout << "neurons total = " << neurons_total << std::endl;

    NN::Neurons neurons;

    for (int i = 0; i < neurons_total; i++)
    {
        std::getline(infile, line);
        std::istringstream iss(line); //not sure if this is necessary

        int id, type;
        iss >> id >> type;

        Neuron::Type nt = Neuron::Type::HIDDEN; // nt: neuron type
        switch (type)
        {
            case 0: nt = Neuron::Type::INPUT; break;
            case 1: nt = Neuron::Type::HIDDEN; break;
            case 2: nt = Neuron::Type::OUTPUT; break;
            default: 
                throw 
                    std::runtime_error("Incorrect type when reading network");
                break;
        }

        Neuron neuron(id, nt);

        //now need to read (out-node-id, weight) pairs
        
        int out_id;
        float out_weight;
        while (iss >> out_id >> out_weight)
            neuron.outputs_id_weight.push_back({out_id, out_weight});

        neurons.push_back(neuron);

        //print_neuron(neuron);
    }

    float fitness;
    std::getline(infile, line);
    iss = std::istringstream(line); //not sure if this is necessary
    iss >> fitness;

    nets_names_.push_back(filename);
    nets_fitness_.push_back(fitness);

    return {neurons};
}


// Expects the list of winner nets to be found in all.orgs in the current
// folder
void EvolutionDemo::read_winner_records()
{
    std::ifstream infile("all.orgs");
    std::string line;
    while (std::getline(infile, line))
        if (line != "")
            nets_.push_back(read_network(line));
}

// Copied from Application.hpp
EvolutionDemo::ScrollAction EvolutionDemo::process_user_input()
{
    sf::Event event;
    while (window_.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window_.close();
            exit(true);
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Left)
                return ScrollAction::BACKWARD;

            if (event.key.code == sf::Keyboard::Right)
                return ScrollAction::FORWARD;
        }
    }

    return ScrollAction::NONE;
}

void EvolutionDemo::run(int frames_per_network)
{

    auto total_nets = nets_.size();

    if (total_nets == 0)
    {
        std::cerr << "No networks were read!" << std::endl;
        return;
    }

    int current_net = 0;

    while (true)
    {
        State state;

        auto &nn = nets_[current_net];

        renderer_.reset_camera();

        //When user presses left or right, next network is shown
        bool user_switch = false;
        for (int i = 0; (i < frames_per_network) && !user_switch; i++)
        {
            window_.clear(sf::Color::White);

            nn.flush();
            nn.supply_inputs(get_inputs_for_neural_network(state));
            nn.run_network();
            auto inputs_to_state = nn_output_to_state_input(nn.outputs);
            state.process_input(inputs_to_state);
            state.update(Constants::TimePerFrame);

            //Rendering things...
            renderer_.render(state);

            const int x_pos = 10;

            text_.setString("Name: " + nets_names_[current_net]);
            text_.setPosition(x_pos, 10);
            renderer_.render_text(text_);

            std::ostringstream oss;
            oss << "Fitness: " << nets_fitness_[current_net];
            text_.setString(oss.str());
            text_.setPosition(x_pos, 50);
            renderer_.render_text(text_);

            /*
            oss = std::ostringstream();
            oss << "Current fitness: " << state.ball.position.x + 2000;
            text_.setString(oss.str());
            text_.setPosition(x_pos, 50);
            renderer_.render_text(text_);
            */
            //rendering over

            switch (process_user_input())
            {
                case ScrollAction::NONE: 
                    break; 
                case ScrollAction::BACKWARD: 
                    current_net--; 
                    if (current_net < 0)
                        current_net = total_nets - 1;
                    user_switch = true;
                    break;
                case ScrollAction::FORWARD: 
                    current_net++; 
                    if (current_net >= static_cast<int>(total_nets))
                        current_net = 0;
                    user_switch = true;
                    break; 
                default: assert(false); break;
            }

            window_.display();
        }

        if (!user_switch)
        {
            current_net++;
            if (current_net > static_cast<int>(total_nets - 1))
                current_net = 0;
        }
    }
}
