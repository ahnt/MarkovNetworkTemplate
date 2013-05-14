
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
int repeats=1;
int maxAgent=100;
int totalGenerations=5000;
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
	srand(getpid());
	agent.resize(maxAgent);
	game=new tGame;
	masterAgent=new tAgent;
	masterAgent->setupRandomAgent(5000);
	masterAgent->setupPhenotype();
	for(i=0;i<agent.size();i++){
		agent[i]=new tAgent;
		agent[i]->inherit(masterAgent,0.01,0);
	}
	nextGen.resize(agent.size());
	masterAgent->nrPointingAtMe--;
	cout<<"setup complete"<<endl;
	while(update<totalGenerations){
		for(i=0;i<agent.size();i++){
			agent[i]->fitness=0.0;
			agent[i]->fitnesses.clear();
		}
		for(i=0;i<agent.size();i++){
			for(j=0;j<repeats;j++){
				game->executeGame(agent[i]);
				agent[i]->fitnesses.push_back((float)agent[i]->correct);
			}
		}
		maxFitness=0.0;
		
		for(i=0;i<agent.size();i++){
            agent[i]->fitness=1.0;
			for(j=0;j<repeats;j++)
                agent[i]->fitness*=agent[i]->fitnesses[j];
            //agent[i]->fitness=sqrt(agent[i]->fitness);
            agent[i]->fitness=pow(1.02,agent[i]->fitness);
			if(agent[i]->fitness>maxFitness){
                who=i;
				maxFitness=agent[i]->fitness;
            }
		}
		cout<<update<<" "<<(double)maxFitness<<" "<<(double)agent[who]->fitness<<endl;
		for(i=0;i<agent.size();i++)
		{
			tAgent *d;
			d=new tAgent;
			do{ j=rand()%(int)agent.size(); } while(randDouble>(agent[j]->fitness/maxFitness));
			d->inherit(agent[j],perSiteMutationRate,update);
			nextGen[i]=d;
		}
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
	agent[0]->saveLOD(LOD,genomeFile);
	//agent[0]->ancestor->ancestor->saveGenome(genomeFile);
    return 0;
}

