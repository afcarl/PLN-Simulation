#include <pln_test.hpp>

#include <iostream>
//#include <fstream>
#include <string>
//#include <stringstream>
#include <sstream>

//Perform evolution on XOR, for gens generations
void pln_test(int gens, FitnessEvaluator &fitness_evaluator)
{
    using namespace NEAT;
    using namespace std;

    //Population *pop=0;
    Genome *start_genome;
    char curword[20];
    int id;

    ostringstream *fnamebuf;
    int gen;

    int evals[NEAT::num_runs];  //Hold records for each run
    int genes[NEAT::num_runs];
    int nodes[NEAT::num_runs];
    int winnernum;
    int winnergenes;
    int winnernodes;
    //For averaging
    int totalevals=0;
    int totalgenes=0;
    int totalnodes=0;
    int expcount;
    int samples;  //For averaging

    memset (evals, 0, NEAT::num_runs * sizeof(int));
    memset (genes, 0, NEAT::num_runs * sizeof(int));
    memset (nodes, 0, NEAT::num_runs * sizeof(int));

    ifstream iFile("plnstartgenes",ios::in);

    cout<<"START PLN TEST"<<endl;

    cout<<"Reading in the start genome"<<endl;
    //Read in the start Genome
    iFile>>curword;
    iFile>>id;
    cout<<"Reading in Genome id "<<id<<endl;
    start_genome=new Genome(id,iFile);
    iFile.close();

    expcount = 0;

    //Spawn the Population
    cout<< "Spawning Population off Genome2" <<endl;
    cout << NEAT::pop_size << std::endl;

    Population pop(start_genome, NEAT::pop_size);

    cout<<"Verifying Spawned Pop"<<endl;

    pop.verify(); //for debugging purposes

    cout << "GENS = " << gens << std::endl;

    for (gen=1;gen<=gens;gen++) 
    {
        cout << "Epoch "<< gen << endl;	

        //This is how to make a custom filename
        fnamebuf = new ostringstream();
        (*fnamebuf) << "gen_" << gen << ends;  //needs end marker

#ifndef NO_SCREEN_OUT
        cout << "name of fname: " << fnamebuf->str() <<endl;
#endif
        char temp[50];
        sprintf (temp, "gen_%d", gen);

        //Check for success
        if (pln_epoch(&pop,
                       gen,
                       temp,
                       winnernum,
                       winnergenes,
                       winnernodes, 
                       fitness_evaluator)) 
        {
            //Collect Stats on end of experiment
            evals[expcount] = NEAT::pop_size*(gen-1) + winnernum;
            genes[expcount] = winnergenes;
            nodes[expcount] = winnernodes;
            gen = gens;
        }

        //Clear output filename
        fnamebuf->clear();
        delete fnamebuf;
    }

    //Average and print stats
    cout<<"Nodes: "<<endl;
    for(expcount=0;expcount<NEAT::num_runs;expcount++) 
    {
        cout<<nodes[expcount]<<endl;
        totalnodes+=nodes[expcount];
    }

    cout<<"Genes: "<<endl;
    for(expcount=0;expcount<NEAT::num_runs;expcount++) 
    {
        cout<<genes[expcount]<<endl;
        totalgenes+=genes[expcount];
    }

    cout<<"Evals "<<endl;
    samples=0;
    for(expcount=0;expcount<NEAT::num_runs;expcount++) 
    {
        cout<<evals[expcount]<<endl;

        if (evals[expcount]>0)
        {
            totalevals+=evals[expcount];
            samples++;
        }
    }

    cout<<"Failures: "<<(NEAT::num_runs-samples)<<" out of "<<NEAT::num_runs<<" runs"<<endl;
    cout<<"Average Nodes: "<<(samples>0 ? (double)totalnodes/samples : 0)<<endl;
    cout<<"Average Genes: "<<(samples>0 ? (double)totalgenes/samples : 0)<<endl;
    cout<<"Average Evals: "<<(samples>0 ? (double)totalevals/samples : 0)<<endl;
}

int pln_epoch(NEAT::Population *pop,
              int generation,
              char *filename,
              int &winnernum,
              int &winnergenes,
              int &winnernodes,
              FitnessEvaluator &fitness_evaluator) 
{
    using namespace NEAT;
    using namespace std;

    vector<Organism*>::iterator curorg;
    vector<Species*>::iterator curspecies;

    bool win=false;

    LOG(Beginning of epoch. Starting to evaluate population...);

    //Evaluate each organism on a test
    for(curorg = (pop->organisms).begin();
        curorg != (pop->organisms).end();
        curorg++) 
        pln_evaluate(*curorg, fitness_evaluator);

    LOG(Middle of epoch function. Average and max fitnesses...);

    //Average and max their fitnesses for dumping to file and snapshot
    for(curspecies=(pop->species).begin();
        curspecies!=(pop->species).end();
        curspecies++) 
    {
        //This experiment control routine issues commands to collect ave
        //and max fitness, as opposed to having the snapshot do it, 
        //because this allows flexibility in terms of what time
        //to observe fitnesses at

        (*curspecies)->compute_average_fitness();
        (*curspecies)->compute_max_fitness();
    }

    //Take a snapshot of the population, so that it can be
    //visualized later on
    //if ((generation%1)==0)
    //  pop->snapshot();

    //Only print to file every print_every generations
    if  (win || ((generation%(NEAT::print_every))==0))
        pop->print_to_file_by_species(filename);


    if (win) 
    {
        for(curorg=(pop->organisms).begin();
            curorg!=(pop->organisms).end();
            curorg++) 
        {
            if ((*curorg)->winner) 
            {
                cout<<"WINNER IS #"<<((*curorg)->gnome)->genome_id<<endl;
                //Prints the winner to file
                //IMPORTANT: This causes generational file output!
                print_Genome_tofile((*curorg)->gnome,"xor_winner");
            }
        }

    }

    LOG(End of epoch function. Calling epoch for population...);
    pop->epoch(generation);
    LOG(End of epoch function. Population epoch is complete...);

    if (win) 
        return 1;
    else 
        return 0;
}

bool pln_evaluate(NEAT::Organism *org, FitnessEvaluator &fitness_evaluator) 
{
    org->fitness = fitness_evaluator(org->net);

    //if (org -> fitness > 1000)
    //    return true;

    return false;
}

