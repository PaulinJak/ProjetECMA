#include <ilcplex/ilocplex.h>

#include <time.h>
#include <fstream>
#include <iostream>
#include "modele_Cplex.hpp"
#include "instance_Cplex.hpp"
#include "recuit.hpp"
#include "options.hpp"


int main (int argc, char* argv[]) {


  //get instance fileName:
 std::string  instancePreName=  "projet_5_8_";
 std::string  instanceFolder= "instances/instances_eleves/";
 std::string  instanceExt =".dat";
  //int instanceNum=1;

 int Method =0;
 cout<<" Choisir méthode de résolution :\n 0. directe CPlex (connexity tree) \n 1.Cplex+ Callback (connexity tree)\n 2.Cplex+MinimizeEdges\n 3. Recuit simulé\n" ;

 cin >>Method ;//connexityTree" or MinimizeEdges;

 string taille_instance;
 string inputfileName;
 string outputFilename;
 string rep;
 srand (time(NULL));
 outputFilename=instanceFolder+"instance.txt";

 std::string temp  ="output_"+instancePreName+ to_string(Method)+".out";
  std:: fstream outputStream;
  outputStream.open(outputFilename,std::fstream::in); // | std::fstream::out | std::fstream::app);

  outputStream<< "Nom instance & Res non connexe & Res connexe & CPU time \\\n";
   
 const clock_t simulation_begin_time = clock();
 std::string fileName;


  for(int instanceNum=1; instanceNum<=10; instanceNum++){

    
    std::string instanceNumString =  to_string(instanceNum);
    fileName=instanceFolder+instancePreName+instanceNumString+instanceExt;

      outputStream<<instancePreName << instanceNum <<" &" ;

    switch(Method)
      {
      case 0:
	solve_Cplex(fileName,outputStream,Method);
	  break;
      case 1:
	solve_Cplex(fileName,outputStream,Method);
	break;
      case 2:
	solve_Cplex(fileName,outputStream,Method);
	break;
      case 3:
	solve_Recuit(fileName,outputStream);
	break;
      default:
	cout <<"No or incorrect method number given!\n";
      }

  } //end of instance treatment

      double simul_cpu_duration = (clock() - simulation_begin_time) / (double)CLOCKS_PER_SEC;
      outputStream << "Total simulation time : " << simul_cpu_duration<< "s.\n";

      outputStream.close();

 
   return 0;

}  
