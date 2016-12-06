#ifndef LISP_API_HPP
#define LISP_API_HPP

#if __cplusplus
extern "C" 
{
#endif

// I am using fixed size arrays because I could not make Lisp pass a dynamic
// array to C++. Also, I am quite sure that having fixed-size arrays will make
// data transfer faster (heap allocations are eliminated).
#define MAX_NEURONS_PER_NETWORK 100

struct EncodedNeuron
{
    int id;
    int type; //0 - input, 1 - hidden, 2 - output
    int out_nodes[MAX_NEURONS_PER_NETWORK];
    float out_weights[MAX_NEURONS_PER_NETWORK];
    int out_count;
};

float c_evalme(EncodedNeuron n[MAX_NEURONS_PER_NETWORK], int neuron_count);

#if __cplusplus
}
#endif

#endif
