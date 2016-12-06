#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <State.hpp>
#include <Solver.hpp> //Has typedef of Solution
#include <Renderer.hpp>

class Application
{
    enum class ScrollAction {NONE, FORWARD, BACKWARD};
    enum class ScrollType {STEP_BY_STEP, CONTINUOUS, REAL_TIME};
public:
    Application(sf::RenderWindow &window);
    
    void run();
    void run2();
    void random_input();
    void show_solution(int frames_per_input, Solution input_vector);
    
    void render(const State &state);
    
private:
    void check_for_exit() const;
    void update_statistics(sf::Time dt);

    ScrollAction get_scroll_action() const;
    const ScrollType scroll_type = ScrollType::REAL_TIME;
    
private:
    sf::RenderWindow &window_;
    Renderer renderer_;
    
    std::vector<State> states_;
};
