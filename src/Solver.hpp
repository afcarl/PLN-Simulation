#include <State.hpp>

#include <list>
#include <queue>

//list because other states have pointers to their parents and
//if vector reallocs, then pointers are no longer valid
//with a vector
typedef std::list<State> 
            ExploredSet;

typedef std::list<State::InputArray> 
            Solution;

typedef std::priority_queue<State, std::deque<State>, std::greater<State>> 
            Frontier;

Solution    find_solution();
void        expand_next(Frontier &frontier, ExploredSet &es);
