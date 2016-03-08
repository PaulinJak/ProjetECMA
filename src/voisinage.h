
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#pragma once
using namespace std;
typedef vector<vector<int> > zone;
typedef vector<vector<float> > carte;
struct solution{
	zone z;
	vector<pair<int,int> > interieur;
	vector<pair<int,int> > exterieur;
	float quotient;
	float taille;
};


class Instance{
public:
int n,m;
carte Ha,Hp,Ca,Cp;
};

void display(zone sol, int n, int m);
void displayData(carte data, int n, int m);
bool frontiere(zone sol,int a,int b,int n,int m);
zone arbre(zone graphe, int x, int y,int n,int m);
bool connexite(zone sol,int n,int m);
vector<pair<int,int> > voisins(int x,int y,int n,int m);
float admissible(zone s, Instance& instance);
float score(zone s,int n,int m);
solution voisinage1(solution s,bool* chgt,Instance& instance);

