#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <State.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <Debug.hpp>

class Renderer
{
public:
    Renderer(sf::RenderWindow &window);
    
    // Optional string for info output
    void render(const State &state);

    void render_text(sf::Text text);

    void reset_camera(); // Demonstration resets camera
    
private:
    sf::RenderWindow &window_;

    sf::Transform tr_;
    float camera_ball_center, camera_middle_x, camera_scale;
    sf::Vector2f half_camera_space;

    sf::View original_view_;

    std::vector<sf::Vertex> graph_;
    
    bool show_more_info;
};

#endif
