#include <ilcplex/ilocplex.h>
#include <string>
#include <time.h>
#include <fstream>
#include <iostream>
#include "modele_Cplex.hpp"
#include "instance_Cplex.hpp"
#include "recuit.hpp"
#include "options.hpp"


int main (int argc, char* argv[]) {


  //get instance fileName:
 std::string  instanceFolder= "instances/instances_eleves/";
 std::string  outputFolder= "instances/output/";
 std::string  instanceExt =".dat";
std::string taille_instance;
cout<< "Donner taille des instances a traiter (e.g. 5_8_):\n";
cin >>taille_instance;
 std::string  instancePreName=  "projet_"+taille_instance;
  //int instanceNum=1;

 int Method =0;
 cout<<" Choisir méthode de résolution :\n 0. directe CPlex (connexity tree) \n 1.Cplex+ Callback (connexity tree)\n 2.Cplex+MinimizeEdges\n 3. Recuit simulé\n" ;

 cin >>Method ;//connexityTree" or MinimizeEdges;

cout<<"Methode choisie:"<<Method <<".\n";
RecuitParams recuitParams(0,0,0,0);
if(Method==3){ //recuit
recuitParams.set_params();}

 string inputfileName;
 string rep;
 srand (time(NULL));
 std::ostringstream ostr;
 ostr<<Method;
 string outputFilename =outputFolder+instancePreName+"Method"+ ostr.str()+".out";
  std:: fstream outputStream;
  outputStream.open(outputFilename,std::fstream::in | std::fstream::out | std::fstream::app);

  outputStream<< "Nom instance & Res non connexe & Res connexe & CPU time \\\n";
   
 const clock_t simulation_begin_time = clock();
 std::string fileName;


  for(int instanceNum=2; instanceNum<=2; instanceNum++){

  std::ostringstream ostr2;
 ostr2<<instanceNum;
    std::string instanceNumString =  ostr2.str();
    fileName=instanceFolder+instancePreName+instanceNumString+instanceExt;

      outputStream<<instancePreName << instanceNum <<" &" ;
cout<< "\nTraitement de l'instance " + instanceNumString <<":\n\n";
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
solve_Recuit(fileName,outputStream,recuitParams);
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
