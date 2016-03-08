#ifndef HPP_GLOUTON
#define HPP_GLOUTON

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
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
};

void updateExtVector(vector<pair<int,int> >& extVector,zone& valeurs,int m, int n,int i, int j);

void extendInitialSol(SimpleSolution& newsol, zone& caseChecked);

void printSolutions( vector< SimpleSolution>& sols);

vector< SimpleSolution> findTrivialSolutions(Instance& instance);
void solve_Glouton(string instancePath, fstream& outputStream);

#endif
