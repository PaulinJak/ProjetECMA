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
public:
int n;
int m;
zone valeurs;
float quotient;
float taille;
SimpleSolution(Instance& instance);
void computeQuotient(Instance& instance);
};

void printSolutions( vector< SimpleSolution> const& sols);

vector< SimpleSolution> findTrivialSolutions(Instance& instance);
void solve_Glouton(string instancePath, fstream& outputStream);

#endif
