#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <algorithm>

#include "voisinage.h"
#include "genetique.hpp"
#include "recuit.hpp"

solution superposition(solution const& solution1, solution const& solution2,Instance& instance){
  solution superp=solution1;
  superp.interieur.clear();
  superp.exterieur.clear();
  superp.taille=0;
  for (int i=0; i<instance.n; i++){
    for(int j=0; j<instance.m; j++){
	superp.z[i][j]=(solution1.z[i][j] or solution2.z[i][j]);
	if(superp.z[i][j]){
	  superp.taille+=1;
	  pair<int,int> p_ij(i,j);
	  if((std::find(solution1.exterieur.begin(), solution1.exterieur.end(), p_ij) != solution1.exterieur.end()) xor (std::find(solution2.exterieur.begin(), solution2.exterieur.end(), p_ij) != solution2.exterieur.end())){
	    superp.exterieur.push_back(p_ij);
	  }
	  else superp.interieur.push_back(p_ij);
      }
	  }
  }
  superp.quotient=admissible(superp.z,instance);  
  
  return(superp);
}

