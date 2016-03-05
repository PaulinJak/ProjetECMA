
#ifndef HPP_MODELE_CPLEX
#define HPP_MODELE_CPLEX

#include <ilcplex/ilocplex.h>
#include "instance_Cplex.hpp"

//#include "options.hpp"

ILOSTLBEGIN

typedef IloArray<IloNumArray> DataMatrix;

typedef IloArray<IloBoolVarArray> BoolVarMatrix;
typedef IloArray<IloNumVarArray> NumVarMatrix;

//matrice à 3 dim pour les hauteurs:
typedef IloArray<BoolVarMatrix> BoolVarMatrix3;


void solve_Cplex(string fileName, fstream& outputStream,int Method);


void getIloData(string nom_fichier,IloEnv env,instance_Cplex& instance);



void
setdata (IloModel& model,IloEnv env, instance_Cplex& instance,BoolVarMatrix  x, NumVarMatrix u,  NumVarMatrix v,IloNumVar y,IloNumVar z, IloRangeArray constraintSet);


 //Connexity constraints

void
setConnexityConstraints(IloModel model,IloEnv env, instance_Cplex const& instance, BoolVarMatrix  x, BoolVarMatrix3 height, IloExprArray lhsCallback, IloNumArray rhsCallback,IloRangeArray connexityConstraintSet,IloInt solutionUnconnex);


void
solveTreeConnexityConstraints (IloCplex cplex,IloModel model,IloEnv env, instance_Cplex const& instance, BoolVarMatrix  x, NumVarMatrix u,  NumVarMatrix v,IloNumVar y,IloNumVar z,bool useCallBack,IloInt hMax,IloInt solutionUnconnex);


void
solveMinimizeEdges(IloCplex cplex,IloModel model ,IloEnv env, instance_Cplex const& instance, BoolVarMatrix  x, NumVarMatrix u,  NumVarMatrix v,IloNumVar y,IloNumVar z,IloInt solutionUnConnex);


#endif
