#include <ilcplex/ilocplex.h>

#include "modele_Cplex.hpp"
#include "instance_Cplex.hpp"
//#include "options.hpp"

ILOSTLBEGIN



int main (int argc, char* argv[]) {
   
 

  //get instance fileName:
  const char*  fileName;
  if(argc>1)//we passed the filename in arg
    fileName=argv[1];
  else fileName = "instances/instances_eleves/projet_5_8_1.dat";
  
  //DONNEES DE L INSTANCE

  typedef IloArray<IloNumArray> DataMatrix;
  IloEnv env;
  instance_Cplex instance ;

  
  getData(fileName,env,instance);
  cout << "Données récupérées!\n";
  
    int& n(instance.n);
  int& m(instance.m);
   try {
     
      IloModel model(env);

      BoolVarMatrix x(env,m);
      for(int i=0;i<m;i++) x[i]=IloBoolVarArray(env,n);

      NumVarMatrix u(env,m);
      for(int i=0;i<m;i++) u[i]=IloNumVarArray(env,n);

      NumVarMatrix v(env,m);
      for(int i=0;i<m;i++) v[i]=IloNumVarArray(env,n);

      IloRangeArray constraintSet(env);
      
      IloNumVar y(env);

      IloNumVar z(env);
      cout << "Variables créées!\n";
      
      setdata(model,env, instance, x,u,v,y,z, constraintSet);
      cout << " Modèle et contraintes définies!\n";

      
      IloCplex cplex(model);

      cplex.solve();
      
      IloInt solutionUnConnex;
      solutionUnConnex=cplex.getObjValue();


      DataMatrix xUnconnex(env,m) ; //Solution optimale non connexe
      for(int i=0;i<m;i++){
	xUnconnex[i]=IloNumArray(env,n);
	  cplex.getValues(xUnconnex[i], x[i]);
	  }
      
      std::string Method ="MinimizeEdges"; //connexityTree";
      if(Method=="connexityTree"){
      
	//We solve a first time the model without the connexity constraints, to get a good max bound of the height max : the value of the solution/2, then we add the connexity constraints :
	IloInt hMax;
	if(argc>2){ // we gave the hmax in argument/
	  istringstream (argv[2])>>hMax;
	  if(hMax==0)  hMax=solutionUnConnex/2+1;
	} //else we use our upper bound
	else hMax=solutionUnConnex/2+1;//(n*m/2+n/2);

	bool useCallBack=false;
	if(argc>3 && *argv[3]=='1')  useCallBack= true;
	solveTreeConnexityConstraints(cplex,model,env,instance,x,u,v,y,z,useCallBack,hMax);
      }
      
     else if(Method=="MinimizeEdges"){
	cout << "MinimizingEdges Method. \n";
	solveMinimizeEdges(cplex,model,env,instance,x,u,v,y,z,solutionUnConnex);
      }
           cout << "\nSolution non connexe optimale de valeur : "<<solutionUnConnex<<endl<<endl;
           env.out() << "Values  of x in the unconnex solution  = " << endl;
      for(int i=0;i<m;i++){
	  cout<<xUnconnex[i]<<endl;}
      
      env.out() << "Values  of x      = " << endl;
      for(int i=0;i<m;i++){
	IloNumArray vals(env,n);
	  cplex.getValues(vals, x[i]);
	  cout<<vals<<endl;
      }
   

      /* env.out()<< "Value of y = "<< cplex.getValue(y)<<endl;
      // env.out()<< "Value of z = "<< cplex.getValue(z)<<endl;

      env.out() << "Values  of u      = " << endl;
      for(int i=0;i<m;i++){
	IloNumArray vals(env,n);
	cout << u[0][0];
	  cplex.getValues(vals, u[i]);
	  cout<<vals<<endl;
      }
      env.out() << "Values  of v      = "  << endl;
      for(int i=0;i<m;i++){
	  IloNumArray vals(env);
	  cplex.getValues(vals, v[i]);
	  cout<<vals<<endl;
	  }*/
      
     
      // cplex.getSlacks(vals, constraintSet);
      //env.out() << "Slacks        = " << vals << endl;

      cplex.exportModel("pl.lp");
      }
   catch (IloException& e) {
      cerr << "Concert exception caught: " << e << endl;
      cout << "Aucune maille ne peut etre selectionnée pour obtenir une solution admissible...\n";
      if(argc>2){//then we used the hmax given in argument, so :
	cout<<" hMax was given, try using calculated upper bound on hMax (without passing it as an argument.\n\n";
      } 
   }
   catch (...) {
      cerr << "Unknown exception caught" << endl;
      }

   env.end();
   return 0;

}  
