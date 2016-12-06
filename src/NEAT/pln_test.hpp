#include <population.h>

#include <FitnessEvaluator.hpp>

void pln_test(int gens, FitnessEvaluator &fitness_evaluator);

int pln_epoch(NEAT::Population *pop,
                      int generation,
                      char *filename, 
                      int &winnernum, 
                      int &winnergenes,
                      int &winnernodes,
                      FitnessEvaluator &fitness_evaluator);

bool pln_evaluate(NEAT::Organism *org, FitnessEvaluator &fitness_evaluator);
