#include <NeuralNetwork.hpp>

float my_sigmoid(float t)
{
    // Outputs do not have to be very precise, but avoiding under/overflows
    // is important. 
    constexpr float thresh = 5.0; 
    //TODO There gotta be a flag for this
    if (t > thresh) t = thresh; else
        if (t < -thresh) t = -thresh;
    return 1.0 / (1.0 + pow(Constants::e, -t));
}
