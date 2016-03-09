
#ifndef HPP_RECUIT
#define HPP_RECUIT

#include "voisinage.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class RecuitParams{
public:
  int r_p;
  float Tinit_p;
  float phi_p;
  int Kmax_p;
  void set_r(int r){r_p=r;};
  void set_Tinit(float T){Tinit_p=T;};
  void set_phi(float phi){phi_p=phi;};
  void set_Kmax(int Kmax){Kmax_p=Kmax;};
void set_params();
  RecuitParams( int r, float T, float phi, int Kmax): r_p(r), Tinit_p(T), phi_p(phi),Kmax_p(Kmax){}; //constructor

};

void getData2(ifstream& fichier,Instance& instance);

solution voisinage(solution s,Instance& instance,int K);

solution initialisation(Instance& instance);

solution recuit(solution& init,ofstream& sortie,RecuitParams recuitParams, Instance& instance);

void solve_Recuit(string instancePath, fstream& outputStream,RecuitParams recuitParams);



#endif
