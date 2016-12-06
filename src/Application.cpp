#include <Application.hpp>

#include <Constants.hpp>

#include <Debug.hpp>
#include <cassert>

Application::Application(sf::RenderWindow &window)
    : window_(window)
    , renderer_(window)
{
}

void Application::run()
{
    sf::Clock clock;
    sf::Time time_since_last_update = sf::Time::Zero;
    
    State state;

    while (window_.isOpen())
    {
        sf::Time dt = clock.restart();
        time_since_last_update += dt;
        while (time_since_last_update > Constants::TimePerFrame)
        {
            time_since_last_update -= Constants::TimePerFrame;

            check_for_exit();
            state.update(Constants::TimePerFrame);
        }
        
        update_statistics(dt);
        render(state);
    }
}

Application::ScrollAction Application::get_scroll_action() const
{
    if (scroll_type == ScrollType::STEP_BY_STEP)
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
    }
    else
        check_for_exit();

    if (scroll_type == ScrollType::REAL_TIME)
    {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            return ScrollAction::BACKWARD;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            return ScrollAction::FORWARD;
    }

    if (scroll_type == ScrollType::CONTINUOUS)
        return ScrollAction::FORWARD;

    return ScrollAction::NONE;
}

void Application::run2()
{
    State new_state = State();
    
    //TRACE(new_state.function.derivative_at(0));
    
    //for (int i = 0; i < 60 * 60; i++)
    //{
    //    states_.push_back(new_state);
    //    new_state.update(TimePerFrame);
    //}
    
    states_.reserve(60 * 60);
    states_.push_back(State());
    
    unsigned int current_frame = 0;
    
    State::InputArray input = {};
    auto check_input = [&input] (sf::Keyboard::Key key, int k, int value)
    {
        assert(value == -1 || value == 1);
        assert(k < 5);
        
        if (sf::Keyboard::isKeyPressed(key))
            input[k] = value;
    };
    
    while (window_.isOpen())
    {
        render(states_[current_frame]);

        auto scroll_action = get_scroll_action();
        
        if (scroll_action == ScrollAction::BACKWARD)
            current_frame--;

        if (scroll_action == ScrollAction::FORWARD)
        {
            current_frame++;
            
            if (current_frame >= states_.size())
            {
                auto new_state(states_.back());
                new_state.process_input(input);
                new_state.update(Constants::TimePerFrame);
                states_.push_back(new_state);
                
                input = {0, 0, 0, 0, 0};
            } 
        }
        
        check_input(sf::Keyboard::Q, 0, 1);
        check_input(sf::Keyboard::W, 1, 1);
        check_input(sf::Keyboard::F, 2, 1);
        check_input(sf::Keyboard::P, 3, 1);
        check_input(sf::Keyboard::G, 4, 1);
        
        check_input(sf::Keyboard::A, 0, -1);
        check_input(sf::Keyboard::R, 1, -1);
        check_input(sf::Keyboard::S, 2, -1);
        check_input(sf::Keyboard::T, 3, -1);
        check_input(sf::Keyboard::D, 4, -1);
    }
}

void Application::render(const State &state)
{
    window_.clear(sf::Color::White);
    renderer_.render(state);
    window_.display();
}

void Application::check_for_exit() const
{
    sf::Event event;
    
    while (window_.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window_.close();
            exit(true);
        }
    }
}

void Application::update_statistics(sf::Time dt)
{
    static sf::Time round = sf::Time::Zero;
    static int fps = 0;
    fps += 1;
    round += dt;
    if (round.asSeconds() >= 1)
    {
        TRACE(fps);
        fps = 0;
        round = sf::Time::Zero;
    }        
}


void Application::show_solution(int frames_per_input, Solution input_vector)
{
    State state;
    //state.update(TimePerFrame);

    for (auto &input: input_vector)
    {
        //if (state.is_goal())
        for (int i = 0; i < frames_per_input; i++)
        {
            state.process_input(input);
            state.update(Constants::TimePerFrame);
            render(state);
            for (int j = 0; j < 5; j++)
                std::cout << input[j] << " ";
            std::cout << std::endl;
            
            while (true)
            {
                check_for_exit(); //check for exit by user
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                    break;
            }
            
        }
    }
}

void Application::random_input()
{
    Solution input_vector;
    State::InputArray input;
    
    srand(time(NULL));
    
    int action[] = {-1, 1};
    
    for (int i = 0; i < 300; i++)
    {
        for (int j = 0; j < 2; j++)
            input[rand() % 5] = action[rand() % 2];
        input_vector.push_back(input);
    }
    
    show_solution(Constants::input_step, input_vector);
}


size_t max_size = 0;
