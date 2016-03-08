#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include "voisinage.h"
#include "genetique.hpp"
#include "recuit.hpp"
#include "glouton.hpp"

SimpleSolution::SimpleSolution(Instance& instance){
  n=instance.n;
  m=instance.m;
  vector<vector<int>> temp(instance.m, vector<int>(instance.n));
  valeurs=temp;
  quotient=0;
  taille=0;
}

void SimpleSolution::computeQuotient(Instance& instance){

	float numa=0;
	float nump=0;
	float denoma=0;
	float denomp=0;
	for(int i=0;i<instance.m;i++)
	{
		for(int j=0;j<instance.n;j++)
		{
			numa=numa+instance.Ha[i][j]*instance.Ca[i][j]*valeurs[i][j];
			nump=nump+instance.Hp[i][j]*instance.Cp[i][j]*valeurs[i][j];
			denoma=denoma+instance.Ca[i][j]*valeurs[i][j];
			denomp=denomp+instance.Cp[i][j]*valeurs[i][j];
		}
	}
	quotient= (numa/denoma)+(nump/denomp);
}


void printSolutions(vector< SimpleSolution> const& sols){
  for(unsigned int s=0;s<sols.size();s++)
    {
      cout << "Solution " << s+1 << ": \n";
      for( int i=0;i<sols[s].m;i++)
	{
		cout<<"[ ";
		for( int j =0;j<sols[s].n;j++)
		{
			cout<<sols[s].valeurs[i][j]<<" ";
		}
		cout<<"]"<<endl;
	}
	cout<<endl;
    }
}



vector< SimpleSolution> findTrivialSolutions(Instance& instance){

  vector<SimpleSolution> sols;
  for (int i=0; i< instance.m; i++)
	   {
	     for(int j=0; j< instance.n; j++)
	       {
		 float quot=instance.Ha[i][j]+instance.Hp[i][j];
		 if( quot>=2)
		   {SimpleSolution newSol(instance);
		     newSol.valeurs[i][j]=1;
		     newSol.taille=1;
		     newSol.quotient=quot;
		     sols.push_back(newSol);
		   }
	       }
	   }

  return sols;
}


void solve_Glouton(string instancePath, fstream& outputStream){

  ofstream sortie(instancePath+"\r\r\rout", ios::app);
  //lecture du fichier de données
  Instance instance;
  ifstream inputInstance(instancePath,ios::in);
  getData2(inputInstance,instance);
		
  vector<SimpleSolution> sols =findTrivialSolutions(instance);
  printSolutions(sols);
		
}
