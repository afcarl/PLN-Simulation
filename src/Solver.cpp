#include <Solver.hpp>

#include <SFML/Graphics.hpp>

#include <Constants.hpp>

#include <Debug.hpp>
#include <iostream>

/*
void render(const sf::RenderWindow &window, const State &state)
{
    window.clear(sf::Color::White);
    renderer.render(state);
    window.display();
}
*/

void expand_next(Frontier &frontier, ExploredSet &es)
{
    es.push_back(frontier.top());
    auto &top = es.back();
    
    frontier.pop();
    
    /*
    if (max_size < top.ball.position.x)
    {
        max_size = top.ball.position.x;
        TRACE(max_size);
    }
    */
    
    static const State::InputArray branch[] = 
    {
        {1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 1},
        
        {-1, 0, 0, 0, 0},
        {0, -1, 0, 0, 0},
        {0, 0, -1, 0, 0},
        {0, 0, 0, -1, 0},
        {0, 0, 0, 0, -1},
    };
    
    for (auto &input: branch)
    {
        State child(top);
        
        child.set_parent(&top);
        
        for (int i = 0; i < Constants::input_step; i++) 
        {
            child.process_input(input);
            child.update(Constants::TimePerFrame);
            
            /*
            render(child);
            
            TRACE(child.cost);
        
            bool f = true;
            while (f)
            {
                sf::Event event;
                while (window_.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        exit(true);
                    if (event.type == sf::Event::KeyPressed &&
                        event.key.code == sf::Keyboard::Right)
                        f = false;
                }
            }
            */
        }
        
        
        child.update_cost(top.f_cost + Constants::input_step);
        
        frontier.push(child);
    }
}

Solution find_solution()
{
    Frontier frontier;
    frontier.push(State());
    //frontier.top().update_cost(0);
    
    ExploredSet es;
    
    while(!frontier.empty())
    {
        if (frontier.top().is_goal())
        {
            Solution solution;
            const State *state = &frontier.top();
           
            TRACE(es.size());
            
            do
            {
                solution.push_front(state -> input);
                state = state -> parent;
            } while (state != nullptr);
            
            return solution;
        }

        expand_next(frontier, es);
    }
    
    return {}; //solution not found
}

