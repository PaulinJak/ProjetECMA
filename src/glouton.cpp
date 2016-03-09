#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>  
#include <vector>
#include <math.h>
#include "voisinage.h"
#include "genetique.hpp"
#include "recuit.hpp"
#include "glouton.hpp"
#include <map>

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
  //cout << "\nadd case " <<i <<","<<j<<"\n";
  taille=taille+1;
  valeurs[i][j]=1;
  updateQuotient(i,j,instance);
  updateExterieur(i,j);
}


void SimpleSolution::updateQuotient(int i, int j,Instance& instance ){

  numa=numa+instance.Ha[i][j]*instance.Ca[i][j]*valeurs[i][j];
  nump=nump+instance.Hp[i][j]*instance.Cp[i][j]*valeurs[i][j];
  denoma=denoma+instance.Ca[i][j]*valeurs[i][j];
  denomp=denomp+instance.Cp[i][j]*valeurs[i][j];
    
  quotient= (numa/denoma)+(nump/denomp);
  //cout<< "DEBUG : updatequotient calcule :" << quotient << "\n";
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
      cout<< "Taille : " <<sols[s].taille <<", Quotient : " <<sols[s].quotient<< ".\n";
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
  valeurs[i][j]=0;
  if(i+1<m && valeurs[i+1][j]==0 && !isOnExterZone(valeurs,n,m,i+1,j) ) extVector.push_back(p_ij_haut);
  if(i>0 && valeurs[i-1][j]==0  && !isOnExterZone(valeurs,n,m,i-1,j)) extVector.push_back(p_ij_bas);
  if(j+1<n && valeurs[i][j+1]==0  && !isOnExterZone(valeurs,n,m,i,j+1)) extVector.push_back(p_ij_droi);
  if(j>0 && valeurs[i][j-1]==0  && !isOnExterZone(valeurs,n,m,i,j-1)) extVector.push_back(p_ij_gau);
  valeurs[i][j]=1;
}

bool SimpleSolution::isOnExter(int i, int j){
  return (valeurs[i][j]==0 && (
	  (i+1<m && valeurs[i+1][j]==1) || 
	    (j+1<n && valeurs[i][j+1]==1) || 
	    (i>0 && valeurs[i-1][j]==1) || 
	  (j>0 && valeurs[i][j-1]==1)));
}

bool isOnExterZone(zone& vals,int n, int m, int i, int j){
  return (vals[i][j]==0 && (
	  (i+1<m && vals[i+1][j]==1) || 
	    (j+1<n && vals[i][j+1]==1) || 
	    (i>0 && vals[i-1][j]==1) || 
	  (j>0 && vals[i][j-1]==1)));
}

void SimpleSolution::updateExterieur(int i, int j){
  pair<int,int> p_ij_bas(i-1,j);
  pair<int,int> p_ij_haut(i+1,j);
  pair<int,int> p_ij_gau(i,j-1);
  pair<int,int> p_ij_droi(i,j+1);
  
  /*cout <<"DEBUG updateExterieur, vals in ext before erasing:\n";
  for(unsigned int k=0;k<exterieur.size();k++){
    cout<<exterieur[k].first <<"," <<exterieur[k].second <<" ; ";
    }*/
  for(unsigned int k=0;k<exterieur.size();k++)
	{
	  if(exterieur[k].first==i && exterieur[k].second==j){
	    exterieur.erase(exterieur.begin()+k);
	    //cout <<"\nDEBUG updateExterieur, erase: " <<i << ","<<j<< "\n";
	    break;}
	}
  valeurs[i][j]=0; //fix tempr to avoid cases and use simply isOnExter
  //check if case is not in solution or already in ext
  if(i+1<m && valeurs[i+1][j]==0 && !isOnExter(i+1,j)) exterieur.push_back(p_ij_haut);
  if(i>0 && valeurs[i-1][j]==0  && !isOnExter(i-1,j)) exterieur.push_back(p_ij_bas);
  if(j+1<n && valeurs[i][j+1]==0  && !isOnExter(i,j+1)) exterieur.push_back(p_ij_droi);
  if(j>0 && valeurs[i][j-1]==0  && !isOnExter(i,j-1)) exterieur.push_back(p_ij_gau);
  valeurs[i][j]=1;
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



bool SimpleSolution::isExtendable(pair<int,int>& cop, Instance& instance){
  float comp_quotient= (numa+instance.Ca[cop.first][cop.second]* instance.Ha[cop.first][cop.second])/(denoma+ instance.Ca[cop.first][cop.second]) +(nump+instance.Cp[cop.first][cop.second]* instance.Hp[cop.first][cop.second])/(denomp+ instance.Cp[cop.first][cop.second]);
  //cout <<"DEBUG isExtendable: quotient calculé "<<comp_quotient <<".\n";
  return (comp_quotient>=2);
  }

void extendToExt(SimpleSolution& sol,Instance& instance){
  vector<pair<int,int> > extToCheck;
  for(unsigned int k=0; k<sol.exterieur.size(); k++){
    pair<int,int> cop= sol.exterieur.at(k);
    extToCheck.push_back(cop);
  }
  while(!extToCheck.empty()){
    pair<int,int> cop= extToCheck.back();
    extToCheck.pop_back();
    if(sol.isExtendable(cop,instance)){ //check that quotient still >=2
      sol.addCase(cop.first,cop.second,instance);
      //caseChecked[cop.first][cop.second]=1;
      updateExtVector(extToCheck, sol.valeurs, instance.m, instance.n, cop.first,cop.second);
    }
  }
}

bool SimpleSolution::isEqual(SimpleSolution& sol2){
  if(taille!=sol2.taille) return false;
  else{ 
    bool res = true;
    int i=0,j=0;
    while(i<m && res){
      j=0;
      while(j<n && res){
	res= (valeurs[i][j]==sol2.valeurs[i][j]);
	j++;
      }
      i++;
    }
    return res;}
}


//the next function tries to get, when possible, the different solutions when we can add a case
vector<SimpleSolution> getSolutionkPlus1(vector<SimpleSolution>& solk,Instance& instance){
  vector<SimpleSolution> solkPlus1;

  for (std::vector<SimpleSolution>::iterator solPointer = solk.begin() ; solPointer != solk.end(); ++solPointer){
    vector<pair<int,int> > extToCheck;
    
    for(unsigned int k=0; k<(*solPointer).exterieur.size(); k++)
      {
	pair<int,int> cop= (*solPointer).exterieur.at(k);
	extToCheck.push_back(cop);
      }
    bool hasFoundBiggerSol=false; //if we find a bigger sol(in the sens of inclusion) then we can forget the other one
    while(!extToCheck.empty()){
      pair<int,int> cop= extToCheck.back();
      extToCheck.pop_back();
      //cout <<"DEBUG quotient du pointer : " << (*solPointer).quotient <<".\n";
      if( (*solPointer).isExtendable(cop,instance)){ //check that quotient still >=2
	hasFoundBiggerSol=true;
	SimpleSolution newSol=*solPointer;

	newSol.addCase(cop.first,cop.second,instance);

	if(not(newSol.isIn(solkPlus1))) solkPlus1.push_back(newSol);
      }
    }
    //cout<<"DEBUG bigger sol found, gonna erase solpointer..";
    if(hasFoundBiggerSol) solk.erase(solPointer);
    cout << "erased\n";
    --solPointer;
  }
  cout <<"get out of getsolutionkplus1\n";
  return solkPlus1;
}

map<int,vector<SimpleSolution> > getMapOfBiggestSols(SimpleSolution& solInit,  Instance& instance){
  int k=solInit.taille;
  map<int,vector<SimpleSolution> > mapOfSols;
  vector<SimpleSolution> firstLevel;
  firstLevel.push_back(solInit);
  //cout <<"DEBUG getMapofbiggestsols, arrive before init\n" ;
  mapOfSols[k]=firstLevel;
  bool findBigger=true;
  while(findBigger){
    cout <<"begin computing sols of taille : "<< k+1 <<"continue :\n";
    int ahah;
    cin >>ahah;
    vector<SimpleSolution> nextLevel;
    nextLevel= getSolutionkPlus1(mapOfSols[k],instance);
    printSolutions(nextLevel);
    k++;
    findBigger=(nextLevel.size()>0);
    mapOfSols[k]=nextLevel;
  }
  return mapOfSols;
}

bool  SimpleSolution::nextGreedyMove(Instance& instance){
  if(exterieur.empty()) return false;
  else{
    pair<int,int> best=exterieur[0];
    float best_coeff=instance.Ha[best.first][best.second]+instance.Hp[best.first][best.second];
    for(unsigned int k=0; k< exterieur.size();k++)
      {
	float k_coeff = instance.Ha[(exterieur[k]).first][(exterieur[k]).second]+instance.Hp[(exterieur[k]).first][(exterieur[k]).second];
	if(k_coeff>best_coeff)
	  {
	    best=exterieur[k];
	    best_coeff=k_coeff;
	  }
      }
    if(isExtendable(best,instance)){
      addCase(best.first,best.second,instance);
      return true;}
    else return false;
  }
}

bool  SimpleSolution::nextGreedyMove_quotient(Instance& instance){
  if(exterieur.empty()) return false;
  else{
    pair<int,int> best=exterieur[0];
   
    int i=best.first, j=best.second;
     //check si cette case est vide
    bool isExtendWith0 =!(instance.Ca[i][j]==0 && instance.Cp[i][j]==0 && instance.Ha[i][j]==0 && instance.Hp[i][j]==0);
    float best_quotient=(numa+instance.Ca[i][j]*instance.Ha[i][j])/(denoma+instance.Ca[i][j])+(nump+instance.Cp[i][j]*instance.Hp[i][j])/(denomp+instance.Cp[i][j]);
    
    for(unsigned int k=0; k< exterieur.size();k++)
      {
	int i=exterieur[k].first, j=exterieur[k].second;
	//check si la case est vide
	if(instance.Ca[i][j]==0 && instance.Cp[i][j]==0 && instance.Ha[i][j]==0 && instance.Hp[i][j]==0 ) break;
	float k_quotient = (numa+instance.Ca[i][j]*instance.Ha[i][j])/(denoma+instance.Ca[i][j])+(nump+instance.Cp[i][j]*instance.Hp[i][j])/(denomp+instance.Cp[i][j]);

	if(k_quotient>best_quotient)
	  {
	    best=exterieur[k];
	    best_quotient=k_quotient;
	    isExtendWith0=true; //on a trouvé une case meilleure, donc best n'est plus = à exterieur[0]
	  }
      }
    if(isExtendable(best,instance) &&isExtendWith0){
      addCase(best.first,best.second,instance);
      return true;}
    else return false;
  }
}


bool SimpleSolution::isIn(vector<SimpleSolution>& vectToCheck){
  bool res= false;
  for (std::vector<SimpleSolution>::iterator solPointer = vectToCheck.begin() ; solPointer != vectToCheck.end(); ++solPointer){
    res=isEqual(*solPointer);
    if(res) break;
  }
  return res;
}


bool SimpleSolution::canBeConnected(SimpleSolution& sol2){
  for(unsigned int k=0;k<exterieur.size();k++){
    if(sol2.valeurs[exterieur[k].first][exterieur[k].second]==1){return true;}
  }
  return false;
}

void SimpleSolution::superpose(SimpleSolution& sol2,Instance& instance){
  
  vector<pair<int,int> > newExterieur;
  for(unsigned int k=0; k<exterieur.size();k++){
    pair<int,int> pair_copy=exterieur[k];
    if(sol2.valeurs[exterieur[k].first][exterieur[k].second]==0) newExterieur.push_back(pair_copy);
  }
   for(unsigned int k=0; k<sol2.exterieur.size();k++){
    pair<int,int> pair_copy=sol2.exterieur[k];
    if(valeurs[pair_copy.first][pair_copy.second]==0) newExterieur.push_back(pair_copy);
   }
   exterieur=newExterieur;
   
  
  for (int i=0; i<m; i++)
    {
      for(int j=0; j<n; j++)
	{
	  if(valeurs[i][j]==0 && sol2.valeurs[i][j]==1){
	    taille+=1;
	    valeurs[i][j]=1;
	    
	  }
	}
    }
  computeQuotient(instance);
  
   
}

pair<int,int> SimpleSolution::find_case_interieur(int i, int j){

  if (i>0 && valeurs[i-1][j]==1) return pair<int,int>(i-1,j);
  else if (j>0 && valeurs[i][j-1]==1) return pair<int,int>(i,j-1);
  else if (i<m && valeurs[i+1][j]==1) return pair<int,int>(i+1,j);
  else if (i<n && valeurs[i][j+1]==1) return pair<int,int>(i,j+1);
  else{ cout <<" WARNING CASE PAS SUR EXTERIEUR \n\n\n\n" ;
    return pair<int,int>(0,0) ;
  }
}


bool SimpleSolution::bestPath(SimpleSolution& sol2, SimpleSolution& aBestPath, Instance& instance){
  vector< vector<int>> vus(m, vector<int>(n,0));
  vector< pair<int,int>> aVoir;
  
  vector<vector<caseForPath> > casesPath(m, vector<caseForPath>(n,caseForPath()));
  

  for(unsigned int k=0; k<sol2.exterieur.size(); k++){
    //les cases de l'exterieur de sol2 sont particulieres car ce sont les dernieres cases d'un chemin
    int i= sol2.exterieur[k].first;
    int j= sol2.exterieur[k].second;
    
    //casesPath[i][
    aVoir.push_back(pair<int,int>(i,j));
  }
  
  while(!aVoir.empty()){
    int i=aVoir.back().first;
    int j=aVoir.back().second;
  }
  
  return false;

}


void tryToConnect(vector<SimpleSolution>& sols,Instance& instance){
  bool canConnect =(sols.size()>1);
  while(canConnect){
    bool connectFound=false;
    unsigned int sol1=0;
    while(sol1<sols.size() && !connectFound){
      unsigned int sol2=sol1+1;
      while(sol2<sols.size() && !connectFound){
	if(sols[sol1].canBeConnected(sols[sol2])){
	  cout << "\nConnection found!!\n\n ";
	  connectFound=true;
	  sols[sol1].superpose(sols[sol2],instance);
	  sols.erase(sols.begin()+sol2);
	}
	sol2++;
      }
      sol1++;
    }
    canConnect=connectFound;
  }
}

void solve_Glouton(string instancePath, fstream& outputStream){

  ofstream sortie(instancePath+"\r\r\rout", ios::app);
  //lecture du fichier de données
  Instance instance;
  ifstream inputInstance(instancePath,ios::in);
  getData2(inputInstance,instance);
		
  const clock_t instance_begin_time = clock();
		
  vector<SimpleSolution> initialSols =findTrivialSolutions(instance);


 for(unsigned int initSol=0; initSol<initialSols.size();initSol++){
    while (initialSols[initSol].nextGreedyMove(instance)){}
 }

 printSolutions(initialSols);

 tryToConnect(initialSols,instance);

 //we need to check if some moves are now possible with the connexions
 for(unsigned int initSol=0; initSol<initialSols.size();initSol++){
    while (initialSols[initSol].nextGreedyMove(instance)){}
 }

 // now we look for the best soluton :
  int best_Res=0;
  int ind_best=-1;
  
  for(unsigned int initSol=0; initSol<initialSols.size();initSol++){
    if(initialSols[initSol].taille>best_Res){
      best_Res=initialSols[initSol].taille;
      ind_best=initSol;
    }
  }

 
  outputStream <<best_Res << " & ";
  double instance_cpu_duration = (clock() - instance_begin_time) / (double)CLOCKS_PER_SEC;
  outputStream <<  instance_cpu_duration <<"\\ \n";
  
  printSolutions(initialSols);
 
		
}
