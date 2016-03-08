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
  // vector<pair<int,int > > ext();
  // exterieur=ext;
  vector<vector<int> > temp(instance.m, vector<int>(instance.n,0));
  valeurs=temp;
   numa=0;
   nump=0;
   denoma=0;
   denomp=0;
  quotient=0;
  taille=0;
}

void SimpleSolution::addCase(int i, int j,Instance& instance){
  taille=taille+1;
  updateQuotient(i,j,instance);
  valeurs[i][j]=1;
  updateExterieur(i,j);
}


void SimpleSolution::updateQuotient(int i, int j,Instance& instance ){

  numa=numa+instance.Ha[i][j]*instance.Ca[i][j]*valeurs[i][j];
  nump=nump+instance.Hp[i][j]*instance.Cp[i][j]*valeurs[i][j];
  denoma=denoma+instance.Ca[i][j]*valeurs[i][j];
  denomp=denomp+instance.Cp[i][j]*valeurs[i][j];
        
  quotient= (numa/denoma)+(nump/denomp);
}

void SimpleSolution::computeQuotient(Instance& instance){


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

void SimpleSolution::print(){
  for( int i=0;i<m;i++)
    {
      cout<<"[ ";
      for( int j =0;j<n;j++)
	{
	  cout<<valeurs[i][j]<<" ";
	}
      cout<<"]"<<endl;
    }
  cout<<endl;
}

void printSolutions(vector< SimpleSolution>& sols){
  for(unsigned int s=0;s<sols.size();s++)
    {
      cout << "Solution " << s+1 << ": \n";
      sols[s].print();
      cout<<endl;
    }
}

void updateExtVector(vector<pair<int,int> >& extVector,zone& valeurs,int m, int n, int i, int j){
  pair<int,int> p_ij_bas(i-1,j);
  pair<int,int> p_ij_haut(i+1,j);
  pair<int,int> p_ij_gau(i,j-1);
  pair<int,int> p_ij_droi(i,j+1);

  for(unsigned int k=0;k<extVector.size();k++)
	{
	  if(extVector[k].first==i && extVector[k].second==j)
	    extVector.erase(extVector.begin()+j);
	  break;
	}
  if(i+1<m && valeurs[i+1][j]==0 ) extVector.push_back(p_ij_haut);
  if(i>0 && valeurs[i-1][j]==0) extVector.push_back(p_ij_bas);
  if(j+1<n && valeurs[i][j+1]==0) extVector.push_back(p_ij_droi);
  if(j>0 && valeurs[i][j-1]==0) extVector.push_back(p_ij_gau);
}

void SimpleSolution::updateExterieur(int i, int j){
  pair<int,int> p_ij_bas(i-1,j);
  pair<int,int> p_ij_haut(i+1,j);
  pair<int,int> p_ij_gau(i,j-1);
  pair<int,int> p_ij_droi(i,j+1);

  for(unsigned int k=0;k<exterieur.size();k++)
	{
	  if(exterieur[k].first==i && exterieur[k].second==j)
	    exterieur.erase(exterieur.begin()+j);
	  break;
	}
  if(i+1<m && valeurs[i+1][j]==0 ) exterieur.push_back(p_ij_haut);
  if(i>0 && valeurs[i-1][j]==0) exterieur.push_back(p_ij_bas);
  if(j+1<n && valeurs[i][j+1]==0) exterieur.push_back(p_ij_droi);
  if(j>0 && valeurs[i][j-1]==0) exterieur.push_back(p_ij_gau);
}


void extendInitialSol(SimpleSolution& newsol, zone& caseChecked,Instance& instance){
  vector<pair<int,int> > extToCheck;
  for(unsigned int k=0; k<newsol.exterieur.size(); k++){
    pair<int,int> cop= newsol.exterieur.at(k);
    extToCheck.push_back(cop);
  }
  while(!extToCheck.empty()){
    pair<int,int> cop= extToCheck.back();
    extToCheck.pop_back();
    if(instance.Ha[cop.first][cop.second]+instance.Hp[cop.first][cop.second]>=2){
      newsol.addCase(cop.first,cop.second,instance);
      caseChecked[cop.first][cop.second]=1;
      updateExtVector(extToCheck, newsol.valeurs, instance.m, instance.n, cop.first,cop.second);
    }
  }
}

vector< SimpleSolution> findTrivialSolutions(Instance& instance){

  vector<SimpleSolution> sols;
  zone caseChecked(instance.m, vector<int>(instance.n));
   //cases déja prises dans les sols

  for (int i=0; i< instance.m; i++)
	   {
	     for(int j=0; j< instance.n; j++)
	       {
		 float quot=instance.Ha[i][j]+instance.Hp[i][j];
		 if( quot>=2 && caseChecked[i][j]==0)
		   {SimpleSolution newSol(instance);
		     newSol.addCase(i,j,instance);
		     caseChecked[i][j]=1;
		     extendInitialSol(newSol,caseChecked,instance);
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
