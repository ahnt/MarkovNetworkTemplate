/*
 *  tGame.h
 *  HMMBrain
 *
 *  Created by Arend on 9/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
 
#ifndef _tGame_h_included_
#define _tGame_h_included_

#include "globalConst.h"
#include "tAgent.h"
#include <vector>
#include <map>
#include <set>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define xDim 256
#define yDim 16
#define startMazes 1
#define cPI 3.14159265

class tGame{
public:
    int scoreTable[83][9];
	vector<int> executeGame(tAgent* agent,FILE *f,int maxTicks);
	tGame();
	~tGame();
	double mutualInformation(vector<int> A,vector<int>B);
	double ei(vector<int> A,vector<int> B,int theMask);
	double computeAtomicPhi(vector<int>A,int states);
	double predictiveI(vector<int>A);
	double nonPredictiveI(vector<int>A);
	double predictNextInput(vector<int>A);
	double computeR(vector<vector<int> > table,int howFarBack);
	double computeOldR(vector<vector<int> > table);
	double entropy(vector<int> list);

    void computeAllMI(char *filename);
    void makeAllSets(char *filename,set<int> target,set<int> source);
    double doInformationCombination(set<int> combo);
};
#endif