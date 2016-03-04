#include <stdio.h>
#include <stdlib.h>
#include <time.h>  

#include "voisinage.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <sstream>
using namespace std;


carte Ha,Hp,Ca,Cp;
int n,m;
void getData(ifstream& fichier) {
	char char1;
	double tmp;
	if(fichier) {
		//pour n
		fichier >> char1 >> char1 >> n >> char1;
		cout <<"n="<< n<<"\n";
		//pour m
		fichier >> char1 >> char1 >> m >>char1;
		cout <<"m="<< m<<"\n";
		int Ba,Bp;
		//pour Ba, si existe!
		fichier >> char1 ;
		if(char1=='B'){//then Ba est définie, et Bp aussi
		  fichier >> char1 >> char1>>Ba >>char1;
		  cout <<"Ba="<< Ba<<"\n";

		  //pour Bp
		  fichier >> char1 >> char1 >>char1 >> Bp >>char1;
		
		  
		// Ha
		fichier >> char1 >> char1 >> char1 >> char1; // lit H a = [       
		}
		else if(char1=='H'){	fichier >> char1 >> char1 >> char1; // le 'H' a été lu, on lit " a = ["
		}
		else cout << "Problème de lecture de l'instance, données non conforme au modèle...\n";
		

		
		for(int i=0;i<m;i++) {
			fichier >> char1 ; // lit le [ de la ligne
			vector<float> ligne; // On initialise la ième ligne de M
			for(int j=0;j<n;j++) {
				fichier >> tmp >> char1;
				// Chaque ligne du fichier contient n fois le motif suivant :
				// un entier suivi d’un caractère ("," ou "]" a la fin)
				ligne.push_back(tmp); // On ajoute l’entier lu à la ième ligne de M
			}
			Ha.push_back(ligne);
			ligne.clear();
			fichier >> char1;//lit ",", Fin de la lecture de la ième ligne de M
		}
		fichier >> char1; //lit ; a la fin de E
		displayData(Ha,n,m);
		fichier >> char1 >> char1 >> char1 >> char1; // lit C a = [

		for(int i=0;i<m;i++) {
			fichier >> char1 ; // lit le [ de la ligne
			vector<float> ligne; // On initialise la ième ligne de M
			for(int j=0;j<n;j++) {
				fichier >> tmp >> char1;
				// Chaque ligne du fichier contient n fois le motif suivant :
				// un entier suivi d’un caractère ("," ou "]" a la fin)
				ligne.push_back(tmp); // On ajoute l’entier lu à la ième ligne de M
			}
			Ca.push_back(ligne);
			ligne.clear();
			fichier >> char1;//lit ",", Fin de la lecture de la ième ligne de M
		}
		fichier >> char1; //lit ; a la fin du tableau
		displayData(Ca,n,m);

		// Hp
		
		fichier >> char1 >> char1 >> char1 >> char1; // lit C a = [

		for(int i=0;i<m;i++) {
			fichier >> char1 ; // lit le [ de la ligne
			vector<float> ligne;
			for(int j=0;j<n;j++) {
				fichier >> tmp >> char1;
				// Chaque ligne du fichier contient n fois le motif suivant :
				// un entier suivi d’un caractère ("," ou "]" a la fin)
				ligne.push_back(tmp); // On ajoute l’entier lu à la ième ligne de M
			}
			Hp.push_back(ligne);
			ligne.clear();
			fichier >> char1;//lit ",", Fin de la lecture de la ième ligne de M
		}
		fichier >> char1; //lit ; a la fin du tableau
		displayData(Hp,n,m);

		// Cp
		
		fichier >> char1 >> char1 >> char1 >> char1; // lit C p = [

		for(int i=0;i<m;i++) {
			fichier >> char1 ; // lit le [ de la ligne
			vector<float> ligne; // On initialise la ième ligne de M
			for(int j=0;j<n;j++) {
				fichier >> tmp >> char1;
				// Chaque ligne du fichier contient n fois le motif suivant :
				// un entier suivi d’un caractère ("," ou "]" a la fin)
				ligne.push_back(tmp); // On ajoute l’entier lu à la ième ligne de M
			}
			Cp.push_back(ligne);
			ligne.clear();
			fichier >> char1;//lit ",", Fin de la lecture de la ième ligne de M
		}
		fichier >> char1; //lit ; a la fin du tableau
		displayData(Ca,n,m);

		fichier.close();
	}
}
solution voisinage(solution s)
{
	bool chgt=false;
	solution svoisin=s;
	int compteur=0;
	while(chgt==false&&compteur<20){
	chgt=false;
	svoisin=voisinage1(s,&chgt,Ha,Hp,Ca,Cp,n,m);
	compteur++;
	}
	return svoisin;
}
solution recuit(solution& init,ofstream& sortie,int r,float Tinit,float phi,int Kmax)
{
	//paramètres
	time_t tic,tac;
	double tps;
	solution max=init;
	solution s=init;
	solution ss=init;
	float T=Tinit;
	int K=0;
	float delta;
	float q;
	float e;
	sortie<<"Paramètres"<<endl;
	sortie<<"Tinit = "<<Tinit<<endl<<"phi ="<<phi<<endl;
	sortie<<"Kmax = "<<Kmax<<endl<<"r = "<<r<<endl;
	while(K<Kmax)
	{
		tic=time(NULL);
		for(int nbIter=0;nbIter<r;nbIter++)
		{
			//on tire au sort une solution s'
			ss=voisinage(s);
			//cout<<"quotient de la solution "<<ss.quotient<<endl;
			//display(ss.z,n,m);
			delta=ss.taille-s.taille;
			if(delta>=0&&ss.quotient>=2)//la nouvelle solution a une énergie plus basse
			{
				s=ss;
				if(s.taille>max.taille){
					max=s;}
			}
			else
			{
				//On tire un nombre q pour décider si on garde la solution ss ou pas
				q=rand()%1000;
				q=q/1000;
				e=exp(delta/T);
				if(q<=e){
					s=ss;
				}
			}
		}
		tac=time(NULL);
		tps=difftime(tac,tic);
		sortie<<"Itération = "<<K<<endl;
		sortie<<"Temperature = "<<T<<endl;
		sortie<<"Temps de l'éxecution = "<<tps<<endl;
		sortie<<"Meileure solution trouvée jusqu'à présent = "<<max.taille<<endl;
		sortie<<"Quotient de la meilleure solution trouvee = "<<max.quotient<<endl;
		K++;
		//Changement de température
		T=T*phi;
	}
	return max;
}
solution initialisation(int n,int m)
{
	solution sol;
	bool find=false;
	for(int i=0;i<m;i++){
		vector<int> ligne;
		for(int j=0;j<n;j++) 
		{ 
			if((Ha[i][j]+Hp[i][j]>=2)&&find==false)
			{
				ligne.push_back(1);
				pair<int,int> p;
				p.first=i;
				p.second=j;
				sol.interieur.push_back(p);
				sol.exterieur=voisins(i,j,n,m);
				sol.taille=1;
				sol.quotient=Ha[i][j]+Hp[i][j];
				cout<<"quotient solution initiale: "<<sol.quotient<<endl;
				find=true;
			}
			else
				ligne.push_back(0);
		}
		sol.z.push_back(ligne);
	}
	if(find)
		cout<<"solution initiale admissible"<<endl;
	else
		cout<<"erreur"<<endl;
	display(sol.z,n,m);
	return sol;

}
int main ()
{
	time_t deb,fin;
	double tps;
  //get instance fileName:
 std::string  instancePreName=  "projet_";
 std::string  instanceFolder= "instances/instances_eleves/";
 std::string  instanceExt =".dat";
  //int instanceNum=1;

	string taille_instance;
	string inputfileName;
	string outputFilename;
	string rep;
	srand (time(NULL));
	outputFilename=instanceFolder+"instance.txt";

	cout<< "entrez taille de l'instance (eg 5_8):"<<endl;
	cin>>taille_instance;
	instancePreName=instancePreName+taille_instance;

	srand (time(NULL));
	outputFilename=instanceFolder+instancePreName+"_recuit.out";
	
	inputfileName=instanceFolder+instancePreName+"_";
	ofstream sortie;
	sortie.open(outputFilename, ios::out); 
	/*cout<<"entrez nom du fichier resultat:"<<endl;
	cin>>t;
	rep="instances/instances_eleves/projet_";
	t="a";*/
	 std:: fstream outputStream;
	 outputStream.open(outputFilename,std::fstream::in | std::fstream::out | std::fstream::app);

	 outputStream<< "Nom instance  & Res connexe & CPU time \\\n";


		int r=100; //nombre d'itérations par palier de température
		float Tinit=1000; //température
		float phi=0.8;//facteur de décroissance
		int Kmax=100;//nombre de paliers de température
		bool changeParams =false;
		cout << "Parametres par défaut : Tinit="<< Tinit <<", phi="<<phi<<", Kmax=" <<Kmax << ".\nChanger les parametres ? (0/1)?\n";
		cin >>changeParams;
		if(changeParams){
		cout<< "Choisissez une temperature initiale Tinit:"<<endl;
		cin>>Tinit;
		cout<< "Choisissez un nombre de paliers de temperature Kmax:"<<endl;
		cin>>Kmax;
		cout<< "Choisissez le nombre d'iterations sur un meme palier r:"<<endl;
		cin>>r;
		cout<< "Choisissez un facteur de decroissance de la température phi:"<<endl;
		cin>>phi;  }


 const clock_t simulation_begin_time = clock();


  for(int instanceNum=1; instanceNum<=10; instanceNum++){

    std::stringstream ss;
    ss << instanceNum;
    std::string instanceNumString =  ss.str();

  outputStream<<instancePreName << instanceNum <<" &" ;
  cout<< "Chemin de l'instance lue : \n" <<inputfileName+instanceNumString+instanceExt ;
	ifstream fichier(inputfileName+instanceNumString+instanceExt, ios::in); 
		//lecture du fichier de données
		getData(fichier);
		 const clock_t instance_begin_time = clock();

	       	//Calcul de la solution initiale
		solution Sinit=initialisation(n,m);
		
		//zone Sinit=initialisation(n,m);
		//Calcul de la solution par recuit simulé
		//tps=difftime(fin,deb);

		solution  Srecuit;
		deb=time(NULL);
		Srecuit=recuit(Sinit,sortie,r,Tinit,phi,Kmax);
		fin=time(NULL);
		tps=difftime(fin,deb);
		//sortie<<"Valeur finale de la solution par recuit simulé = "<<Srecuit.z<<endl;
		sortie<<"Temps d'éxécution total = "<<tps;
		display(Srecuit.z,n,m);
		cout<<"valeur "<<Srecuit.taille<<endl;
		cout<<"quotient "<<Srecuit.quotient<<endl;
		outputStream <<Srecuit.taille << " & ";
		double instance_cpu_duration = (clock() - instance_begin_time) / (double)CLOCKS_PER_SEC;
		outputStream <<  instance_cpu_duration <<"\\\n";
  } // end of instance loop

  double simul_cpu_duration = (clock() - simulation_begin_time) / (double)CLOCKS_PER_SEC;
  outputStream << "Total simulation time : " << simul_cpu_duration<< "s.\n";

  outputStream.close();

	}
			
