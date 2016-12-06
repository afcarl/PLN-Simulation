#include <Renderer.hpp>
#include <iostream>

#include <Constants.hpp>

#include <cmath>
#include <cassert>

#include <LinearAlgebraHelpers.hpp>

Renderer::Renderer(sf::RenderWindow &window)
    : window_(window)
    , camera_ball_center(1.1f)
    , camera_scale(1.3)
    , half_camera_space({Constants::workspace.x * camera_scale / 2.f,
                         Constants::workspace.y * camera_scale / 2.f})
    , original_view_(window_.getView())
    , show_more_info(false)
{
    assert(fabs(window.getSize().x / Constants::workspace.x -
                window.getSize().y / Constants::workspace.y) < 0.01);

    reset_camera();

    //the view is rotated 180 degrees and covers area in view.
    //This transformation reflects around the vertical middle of the workspace.
    tr_.scale(-1.f / camera_scale, 1.f / camera_scale, 
              Constants::workspace.x / 2.f, Constants::workspace.y / 2.f);
}

void Renderer::reset_camera()
{
    camera_middle_x = Constants::workspace.x * camera_ball_center;
    window_.setView(original_view_);
}

void Renderer::render_text(sf::Text text)
{
    sf::View v = window_.getView();
    window_.setView(window_.getDefaultView());
    window_.draw(text);
    window_.setView(v);
}

void Renderer::render(const State &state)
{
    //adjusting camera
    auto center_diff = state.ball.position.x - camera_middle_x;
    if (state.ball.position.x >= Constants::workspace.x * camera_ball_center)
    {
        auto new_view = window_.getView();
        new_view.move(-center_diff, 0);
        window_.setView(new_view);
        camera_middle_x += center_diff;
    }

    //rendering the ball
    sf::CircleShape circle;
    
    circle.setFillColor(sf::Color::Black);
    circle.setOrigin(state.ball.radius, state.ball.radius);
    circle.setRadius(state.ball.radius);
    circle.setPosition(state.ball.position);

    window_.draw(circle, tr_);
    
    //rendering the function
    
    graph_.clear();
    //for (unsigned int x = 0; x < graph_.size(); x++)
    float view_center = window_.getView().getCenter().x,
          x           = view_center - half_camera_space.x,
          x_end       = camera_middle_x + half_camera_space.x,
          y           = 0;
    while (x++ < x_end)// && fabs(y) < half_camera_space.y * 2.f)
    {
        y = state.function.at(x);
        if (fabs(y) < half_camera_space.y * 2.f)
            graph_.push_back(sf::Vertex({x, y}, sf::Color::Green));
    }
    
    window_.draw(&*graph_.begin(),
                 graph_.size(),
                 sf::PrimitiveType::LinesStrip,
                 tr_);
    
    //drawing interpolation points
    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::Black);
    rect.setSize({4, 4});
    rect.setOrigin(2, 2);
    for (int i = 0; i < state.function.n; i++) 
    {
        rect.setPosition(state.function.interp_points[i]);
        window_.draw(rect, tr_);
    }

    if (!show_more_info)
        return;
    
    //show closest point
    rect.setFillColor(sf::Color::Red);
    rect.setPosition(state.closest_point);
    window_.draw(rect, tr_);

    //show velocity direction
    sf::Vertex velocity_line[] =
    {
        state.ball.position,
        state.ball.position + state.ball.velocity
    };
    window_.draw(velocity_line, 2, sf::PrimitiveType::LinesStrip, tr_);
    
    //show tangent to closest point
    sf::Vector2f tangent
        (1, state.function.derivative_at(state.closest_point.x));

    sf::Vertex tangent_line[] =
    {
        {state.closest_point,                  sf::Color::Blue},
        {state.closest_point + tangent * 50.f, sf::Color::Blue},
    };

    window_.draw(tangent_line, 2, sf::PrimitiveType::LinesStrip, tr_);

    //show perpendicular to tangent line, in the direction of acceleration
    //due to impact
    auto perpendicular = 27.f * rotate(tangent, Constants::pi / 2.f);
        //27.f * unit({tangent.x, -1.f / tangent.y});

    sf::Vertex perpendicular_line[] =
    {
        {state.closest_point,                 sf::Color::Blue},
        {state.closest_point + perpendicular, sf::Color::Blue},
    };

    window_.draw(perpendicular_line, 2, sf::PrimitiveType::LinesStrip, tr_);
}
