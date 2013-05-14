
#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <map>
#include <math.h>
#include <time.h>
#include <iostream>
#include "globalConst.h"
#include "tHMM.h"
#include "tAgent.h"
#include "tGame.h"
#include <string.h>

#define randDouble ((double)rand()/(double)RAND_MAX)

using namespace std;

//double replacementRate=0.1;
double perSiteMutationRate=0.005;
int update=0;
int repeats=1000;
int maxAgent=100;
int totalGenerations=1000;
char trialName[1000];


int main(int argc, char *argv[])
{
	vector<tAgent*>agent;
	vector<tAgent*>nextGen;
	tAgent *masterAgent;
	int i,j,who=0;
	tGame *game;
	double maxFitness;
	FILE *resFile;
	FILE *LOD;
	FILE *genomeFile;
	LOD=fopen(argv[1],"w+t");
	genomeFile=fopen(argv[2],"w+t");	
	srand(time(NULL));
	agent.resize(maxAgent);
	game=new tGame;
	masterAgent=new tAgent;
    //you can also loadAgent to load a non random ancestor to start with
	masterAgent->setupRandomAgent(5000);
	masterAgent->setupPhenotype();
	for(i=0;i<agent.size();i++){
		agent[i]=new tAgent;
		agent[i]->inherit(masterAgent,0.01,0);
		// using agent[i]->inherit(masterAgent,0.0,0);
        // would create a population of clones
	}
	nextGen.resize(agent.size());
	masterAgent->nrPointingAtMe--;
	cout<<"setup complete"<<endl;
    //the main loop
	while(update<totalGenerations){
        // set all fitnesses to 0.0
		for(i=0;i<agent.size();i++){
			agent[i]->fitness=0.0;
            //and also their buffer
			agent[i]->fitnesses.clear();
		}
        //iterate over all agents
		for(i=0;i<agent.size();i++){
			for(j=0;j<repeats;j++){
				game->executeGame(agent[i]);
                // must return the fitness of the agent in agent->fitness
				agent[i]->fitnesses.push_back((float)agent[i]->fitness);
			}
		}
        // find maximum fitness
		maxFitness=0.0;
		
		for(i=0;i<agent.size();i++){
            //summ up all fintesses in the finessess buffer
            agent[i]->fitness=0.0;
			for(j=0;j<repeats;j++)
                agent[i]->fitness+=agent[i]->fitnesses[j];
            // normalize by fitness
            agent[i]->fitness/=(double)repeats;
            
			if(agent[i]->fitness>maxFitness){
                who=i;
				maxFitness=agent[i]->fitness;
            }
		}
		cout<<update<<" "<<(double)maxFitness<<" "<<(double)agent[who]->fitness<<endl;
        //roulette wheel selection
		for(i=0;i<agent.size();i++){
			tAgent *d;
			d=new tAgent;
			do{
                j=rand()%(int)agent.size();
            } while(randDouble>(agent[j]->fitness/maxFitness));
            
			d->inherit(agent[j],perSiteMutationRate,update);
			nextGen[i]=d;
		}
        // moves "nextGen" to current population
		for(i=0;i<agent.size();i++){
			agent[i]->retire();
			agent[i]->nrPointingAtMe--;
			if(agent[i]->nrPointingAtMe==0)
				delete agent[i];
			agent[i]=nextGen[i];
		}
		agent=nextGen;
		update++;
	}
	agent[0]->ancestor->ancestor->saveLOD(LOD,genomeFile);
    agent[0]->ancestor->ancestor->setupPhenotype();
    agent[0]->ancestor->ancestor->showPhenotype();
	//agent[0]->ancestor->ancestor->saveGenome(genomeFile);
    return 0;
}

