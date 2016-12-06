#include <Function.hpp>

#include <Debug.hpp>

Function::Function(float range)
    : interp_points(n)
{
    for (int i = 0; i < n; i++)
    {
        interp_points[i].x = i * range / (n - 1);
        interp_points[i].y = offset;
    }
    
    //interp_points[1].y = -200 + offset;
    //interp_points[2].y = -200 + offset;
    //interp_points[3].y = 200 + offset;
}

float Function::at(float x) const
{
    /*
    P = 0;
    
    for j = 1:n
        cf = yp(j);
        for k = 1:n
            if (j ~= k)
                cf = cf * ((x - xp(k)) / (xp(j) - xp(k)));  
            end
        end
        P = P + cf;
    end
    */
   
    float result = 0;
    
    for (int j = 0; j < n; j++)
    {
        auto term = interp_points[j].y;
        for (int k = 0; k < n; k++)
            if(j != k)
                term *= ((x - interp_points[k].x) /
                         (interp_points[j].x - interp_points[k].x));
        
        result += term;
    }
    
    //result += offset;
    
    return result;
}

//return an approximation
float Function::derivative_at(float x) const
{
    const float delta = 0.01;
    
    //taking the avearge of derivatives at (x, x - delta) and (x, x + delta)
    return (at(x + delta) - at(x - delta)) / (2.f * delta);
    //return (at(x + delta) - at(x)) / delta;
}
