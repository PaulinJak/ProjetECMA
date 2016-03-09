#ifndef HPP_GLOUTON
#define HPP_GLOUTON

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <map>
#include "voisinage.h"
#include "genetique.hpp"
#include "recuit.hpp"

class SimpleSolution{
private:
  int n;
  int m;
  float numa;
  float nump;
  float denoma;
  float denomp;
public:
  zone valeurs;
  vector<pair<int,int> > exterieur;
  float quotient;
  float taille;
  SimpleSolution(Instance& instance);
  void updateExterieur(int i, int j);
  void updateQuotient(int i, int j, Instance& instance);
  void computeQuotient(Instance& instance);
  void addCase(int i, int j,Instance& instance);
  void print();
  bool isExtendable(pair<int,int>& cop, Instance& instance);
  bool isOnExter(int i, int j);
  bool isIn(vector<SimpleSolution>& vectToCheck);
  bool isEqual(SimpleSolution& sol2);
  bool nextGreedyMove(Instance& instance);
  bool nextGreedyMove_quotient(Instance& instance);
  bool canBeConnected(SimpleSolution& sol2);
  void superpose(SimpleSolution& sol2,Instance& instance);
  bool bestPath(SimpleSolution& sol2, SimpleSolution& aBestPath,Instance& instance);
  pair<int,int> find_case_interieur(int i, int j);
  
};

class caseForPath{
public:
  float numa;
  float denoma;
  float nump;
  float denomp;
  float quotientIfPath;
  pair<int,int> successorInPath;
  caseForPath(){}
};



void tryToConnect(vector<SimpleSolution>& sols,Instance& instance);

map<int,vector<SimpleSolution> > getMapOfBiggestSols(SimpleSolution& solInit, Instance& instance);

bool isOnExterZone(zone& vals,int n, int m, int i, int j);

void updateExtVector(vector<pair<int,int> >& extVector,zone& valeurs,int m, int n,int i, int j);

void extendInitialSol(SimpleSolution& newsol, zone& caseChecked, Instance& instance);

void printSolutions( vector< SimpleSolution>& sols,Instance& instance);

vector< SimpleSolution> findTrivialSolutions(Instance& instance);


void extendToExt(SimpleSolution& sol,Instance& instance);

vector<SimpleSolution> getSolutionkPlus1(vector<SimpleSolution>& solk,Instance& instance);


void solve_Glouton(string instancePath, fstream& outputStream);

#endif
