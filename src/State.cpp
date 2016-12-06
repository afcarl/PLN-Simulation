#include <State.hpp>

#include <Constants.hpp>
#include <LinearAlgebraHelpers.hpp>

#include <SFML/Graphics.hpp>

#include <cmath>
#include <cfloat> //FLT_MAX

#include <cassert>
#include <Debug.hpp>

State::State()
    : ball({Constants::workspace.x / 10.f, Constants::workspace.y * 0.7f}, 
           {0, -20}, //velocity
           50) //radius
    , function(Constants::workspace.x)
    , old_function(function)
    , closest_point({0, 0})
    , f_cost(0)
    , cost(0)
    , parent(nullptr)
    , input({0})
{
    update_cost(0);
}

void State::set_parent(const State *p)
{
    parent = p;
}

bool State::is_goal() const
{
    if (ball.position.x >= Constants::workspace.x)
        return true;
    
    return false;
}

void State::process_input(const InputArray &input) //should probably add dt
{
    for (unsigned int i = 0; i < input.size(); i++)
        assert(input[i] == 0 || input[i] == -1 || input[i] == 1 );

    old_function = function;
    
    for (unsigned int i = 0; i < input.size(); i++)
    {
        auto &y = function.interp_points[i].y;
        
        y += input[i] * 15.f;
        
        const float bound = 200;
        
        if (y > function.offset + bound)
            y = function.offset + bound;
        if (y < function.offset - bound)
            y = function.offset - bound;
    }
    
    this -> input = input;
}


//should be called after the update function
void State::update_cost(float new_f_cost)
{
    f_cost = new_f_cost;
    cost = f_cost + (Constants::workspace.x - ball.position.x) /
           Ball::max_x_speed;
    
    if (ball.position.x + ball.radius < 0)
        cost = FLT_MAX;
}

void State::update(const sf::Time dt)
{
    ball.position.x += ball.velocity.x * dt.asSeconds();
    ball.position.y += ball.velocity.y * dt.asSeconds();

    
    //find the closest point using bisection method
    int
        iterations = 0,
        limit = 60;
    
    float
        a = ball.position.x - ball.radius, //0
        b = ball.position.x + ball.radius, //Constants::workspace.x
        x,
        y,
        dgdx,
        eps = 0.1;
    
    do
    {
        x = (a + b) / 2.f;
        y = function.at(x);
        
        // Not where the division is...
        // WARNING: Potential division by zero if the ball gets fast enough
        dgdx = 1.f / sqrt(pow(ball.position.x - x, 2) +
                          pow(ball.position.y - y, 2)) *
               ((x - ball.position.x) +
                (y - ball.position.y) * function.derivative_at(x));
        
        if (dgdx < 0)
            a = x;
        else
            b = x;
        
    } while (fabs(dgdx) > eps && iterations++ < limit);
    
    //if (iterations < limit)
    closest_point = {x, y};
    
    //Check if the ball intersects the function 
    if (distance(ball.position, closest_point) <= ball.radius)
    {
        //displacement is the vector we need to add to the ball's position in
        //order for it to not intersect with the function anymore
        auto displacement = 
            (closest_point - ball.position) -
                unit(closest_point - ball.position) * ball.radius * 1.01f;

        //Move the ball back 
        ball.position += displacement;

        //Reflect the ball
        
        sf::Vector2f
            tangent(1, function.derivative_at(closest_point.x)),
            //to the tangent of the impact point
            perpendicular(rotate(tangent, Constants::pi / 2.f));

        //We need to know whether we want the ball to reflect
        //in case of intersection.
        //We do not want to reflect in only one case when two
        //conditions are satisfied:
        //first, the ball is flying "away" from function
        //(angle between ball velocity and perpendicular_acc < 90);
        //second, the function is moving towards the ball, which is when
        //the function is moving upwards
        //(bounce_total_acceleration > 0)
        //In all other cases, the ball is reflected.
        //bool b checks for the case described above
        //Also, need to check that magnitude of ball velocity is not zero, 
        //because then the angle_between(ball.velocity, anything) is undefined.
        if ((magnitude(ball.velocity) != 0) &&
            !((fabs(angle_between(perpendicular, ball.velocity)) < 
                Constants::pi / 2.f)))
        {
            float theta = angle_between(ball.velocity, tangent);

            //TRACE(theta * 180 / Constants::pi);

            float angle_to_turn = 0;
            if (theta < Constants::pi / 2.f)
                angle_to_turn = 2 * theta;
            else
                angle_to_turn = 2 * (theta - Constants::pi);
            
            ball.velocity = rotate(ball.velocity, angle_to_turn);

            float damping = 0.75;
            ball.velocity *= damping;
        }
        
        //Effectively, by looking at how much the ball has intersected the
        //function, we know how much acceleration it will gain due to impact.
        // Avoiding division by zero.
        // Time wasted looking for this bug: ~7 hours (this program was run
        // as a compiled shared DLL and called from a Lisp process and the
        // bug happened at most occasionally. I was lucky to get a case when
        // the bug started to happen in the very beginning of each run, so
        // I took advantage of it and put the print-outs in. The program
        // crashed here.)
        if (magnitude(displacement) > 0.00001)
            ball.velocity += unit(displacement) * 
                (function.at(x) - old_function.at(x));
    }
    else
        ball.velocity -= {0, 971 * dt.asSeconds()};
}

bool State::operator > (const State &right) const
{
    return cost > right.cost;
}
