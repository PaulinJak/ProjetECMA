#ifndef HPP_INSTANCE_CPLEX
#define HPP_INSTANCE_CPLEX

#include <ilcplex/ilocplex.h>



typedef IloArray<IloNumArray> DataMatrix;

class instance_Cplex 
{
  
  int n;
  int m;
  DataMatrix Ha;
  DataMatrix Ca;
  DataMatrix Hp; 
  DataMatrix Cp;

};

#endif
