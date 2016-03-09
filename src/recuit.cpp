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

#include "recuit.hpp"
using namespace std;


void RecuitParams::set_params(){
	int r=70; //nombre d'itérations par palier de température
	float Tinit=1000; //température
	float phi=0.8;//facteur de décroissance
	int Kmax=50;//nombre de paliers de température
	set_Tinit(Tinit);
	set_Kmax(Kmax);
	set_phi(phi);
	set_r(r);
	bool changeParams =false;
	cout << "Parametres par défaut : r= " <<r <<", Tinit="<< Tinit <<", phi="<<phi<<", Kmax=" <<Kmax << ".\nChanger les parametres ? (0/1)?\n";
	cin >>changeParams;
	if(changeParams){
		cout<< "Choisissez une temperature initiale Tinit:"<<endl;
		cin>>Tinit;
		set_Tinit(Tinit);
		cout<< "Choisissez un nombre de paliers de temperature Kmax:"<<endl;
		cin>>Kmax;
		set_Kmax(Kmax);
		cout<< "Choisissez le nombre d'iterations sur un meme palier r:"<<endl;
		cin>>r;
		set_r(r);
		cout<< "Choisissez un facteur de decroissance de la température phi:"<<endl;
		cin>>phi;
		set_phi(phi);}

}
void getData2(ifstream& fichier,Instance& instance) {
	carte Ha,Hp,Ca,Cp;
	int n,m;
	char char1;
	double tmp;
	if(fichier) {
		//pour n
		fichier >> char1 >> char1 >> n >> char1;
		//pour m
		fichier >> char1 >> char1 >> m >>char1;
		int Ba,Bp;
		//pour Ba, si existe!
		fichier >> char1 ;
		if(char1=='B'){//then Ba est définie, et Bp aussi
			fichier >> char1 >> char1>>Ba >>char1;
			//pour Bp
			fichier >> char1 >> char1 >>char1 >> Bp >>char1;


			// Ha
			fichier >> char1 >> char1 >> char1 >> char1; // lit H a = [       
		}
		else if(char1=='H'){	fichier >> char1 >> char1 >> char1; // le 'H' a été lu, on lit " a = ["
		}
		else cout << "Problème de lecture de l'instance, données non conforme au modèle.../n";



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
		instance.n=n;
		instance.m=m;
		instance.Ca=Ca;
		instance.Cp=Cp;
		instance.Ha=Ha;
		instance.Hp=Hp;
		fichier.close();
	}
}

solution voisinage(solution s,Instance& instance,int K)

{
	bool chgt=false;
	solution svoisin=s;
	int compteur=0;
	while(chgt==false&&compteur<20){
		chgt=false;
		if(compteur==0&&K==0)
			svoisin=voisinagesup2(s,&chgt,instance);
		else
		{
			if(K>10)
			{
				svoisin=maxlocal1(s,&chgt,instance);
				if(svoisin.taille==s.taille)
					svoisin=maxlocal2(s,&chgt,instance);
			}
			else
			{
			svoisin=voisinage1(s,&chgt,instance);
			if(chgt&&K>2)
			{
				svoisin=voisinage1(svoisin,&chgt,instance);
				if(chgt&&K>4)
				{
					svoisin=voisinage1(svoisin,&chgt,instance);
				}
			}
			}
		}
		compteur++;
	}
	return svoisin;
}
solution recuit(solution& init,ofstream& sortie,RecuitParams recuitParams, Instance& instance)
{

	carte Ha=instance.Ha;
	carte Hp=instance.Hp;
	carte Ca=instance.Ca;
	carte Cp=instance.Cp;
	/*cout<<"CA"<<endl;
	displayData(Ca,instance.n,instance.m);
	cout<<"CP"<<endl;
	displayData(Cp,instance.n,instance.m);
	cout<<"HA"<<endl;
	displayData(Ha,instance.n,instance.m);
	cout<<"HP"<<endl;
	displayData(Hp,instance.n,instance.m);*/
	//paramètres
	clock_t tic,tac;
	double tps;
	solution max=init;
	solution s=init;
	solution ss=init;
	float T=recuitParams.Tinit_p;
	int K=0;
	int Kmax=recuitParams.Kmax_p;
	int r=recuitParams.r_p;
	float phi=recuitParams.phi_p;
	float delta;
	float q;
	float e;
	//sortie<<"Paramètres"<<endl;
	//sortie<<"Tinit = "<<T<<endl<<"phi ="<<recuitParams.phi_p<<endl;
	//sortie<<"Kmax = "<<recuitParams.Kmax_p<<endl<<"r = "<<recuitParams.r_p<<endl;
	int chgt=1;
	int distance=0;
	while(K<Kmax&&distance<10)
	{
 		if(chgt==0)
			distance++;
		chgt=0;
		tic=time(NULL);
		s=max;
		for(int nbIter=0;nbIter<r;nbIter++)
		{
			//on tire au sort une solution s'

			if(distance<7)
				ss=voisinage(s,instance,distance);
			else
			{
				if(distance==8){
				ss=voisinage(max,instance,distance);
				s=max;
				}
			}
			//sortie<<"quotient de la solution "<<ss.quotient<<endl;
			//display(ss.z,instance.n,instance.m);
			delta=ss.taille-s.taille;
			if(delta>=0&&ss.quotient>=2)//la nouvelle solution a une énergie plus basse
			{
				s=ss;
				if(s.taille>max.taille){
					max=s;
					chgt++;
					distance=0;
				}
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
		tac=clock();
		tps=(tac-tic)/(double)CLOCKS_PER_SEC;
		/*sortie<<"Itération = "<<K<<endl;
		sortie<<"Temperature = "<<T<<endl;
		sortie<<"Temps de l'éxecution = "<<tps<<endl;
		sortie<<"Meileure solution trouvée jusqu'à présent = "<<max.taille<<endl;
		sortie<<"Quotient de la meilleure solution trouvee = "<<max.quotient<<endl;*/
		K++;
		//Changement de température
		T=T*phi;
	}

	return max;
}
solution initialisation(Instance& instance)
{
	int n= instance.n;
	int m=instance.m;
	carte Ha=instance.Ha;
	carte Hp=instance.Hp;
	carte Ca=instance.Ca;
	carte Cp=instance.Cp;
	solution sol;
	bool find=false;
	float max=0; 
	int x,y;
	for(int i=0;i<m;i++){
		vector<int> ligne;
		for(int j=0;j<n;j++) 
		{ 
			if((Ha[i][j]+Hp[i][j]>=2)&&(Ca[i][j]+Cp[i][j]+Hp[i][j]+Ha[i][j])!=0)
			{
				find=true;
				if(Ha[i][j]+Hp[i][j]>max)
				{
					max=Ha[i][j]+Hp[i][j];
					x=i;
					y=j;
				}
			}
			ligne.push_back(0);
		}
		sol.z.push_back(ligne);
		ligne.clear();
	}
	if(find)
	{
		sol.z[x][y]=1;
		pair<int,int> p;
		p.first=x;
		p.second=y;
		sol.interieur.push_back(p);
		sol.exterieur=voisins(x,y,n,m);
		sol.taille=1;
		sol.quotient=Ha[x][y]+Hp[x][y];
		//sortie<<"quotient solution initiale: "<<sol.quotient<<endl;
		find=true;
		//sortie<<"solution initiale admissible"<<endl;
	}
	else
		cout<<"erreur"<<endl;
	//display(sol.z,n,m,sortie);
	return sol;

}


void solve_Recuit(string instancePath, fstream& outputStream,RecuitParams params)
{                      

	time_t deb,fin;
	double tps;

	ofstream sortie("C:/Users/J.pages/Documents/ProjetECMA/instances/output/sortie.txt", ios::app);
	//lecture du fichier de données
	Instance instance;
	ifstream inputInstance(instancePath,ios::in);
	getData2(inputInstance,instance);

	const clock_t instance_begin_time = clock();
	deb=time(NULL);
	//Calcul de la solution initiale
	solution Sinit=initialisation(instance);

	//zone Sinit=initialisation(n,m);
	//Calcul de la solution par recuit simulé
	//tps=difftime(fin,deb);

	solution  Srecuit;
	Srecuit=recuit(Sinit,sortie,params,instance);
	fin=time(NULL);
	tps=difftime(fin,deb);
	//sortie<<"Valeur finale de la solution par recuit simulé = "<<Srecuit.z<<endl;
	sortie<<"Temps d'éxécution total = "<<tps;
	display(Srecuit.z,instance.n,instance.m);
	cout<<"valeur "<<Srecuit.taille<<endl;
	cout<<"quotient "<<Srecuit.quotient<<endl;
	outputStream <<Srecuit.taille << " & ";
	double instance_cpu_duration = (clock() - instance_begin_time) / (double)CLOCKS_PER_SEC;
	outputStream <<  instance_cpu_duration <<"\\"<<"\\"<<"\n";

}

