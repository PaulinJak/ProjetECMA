#include <ilcplex/ilocplex.h>
//#include "modele_Cplex.hpp"
//#include "options.hpp"
#include "instance_Cplex.hpp"
#include "modele_Cplex.hpp"

ILOSTLBEGIN


typedef IloArray<IloNumArray> DataMatrix;

typedef IloArray<IloBoolVarArray> BoolVarMatrix;
typedef IloArray<IloNumVarArray> NumVarMatrix;

//matrice à 3 dim pour les hauteurs:
typedef IloArray<BoolVarMatrix> BoolVarMatrix3;
 
void getData(const char* nom_fichier,IloEnv env,instance_Cplex& instance) {

  int& n(instance.n);
  int& m(instance.m);
  DataMatrix& Ha(instance.Ha);
  DataMatrix& Ca(instance.Ca);
  DataMatrix& Hp(instance.Hp); 
  DataMatrix& Cp(instance.Cp);

  char char1;
	double tmp;
	
	ifstream fichier(nom_fichier);
	if(fichier) {
		//pour n
		fichier >> char1 >> char1 >> n >> char1;
		cout <<"n="<< n<<"\n";
		//pour m
		fichier >> char1 >> char1 >> m >>char1;
		cout <<"m="<< m<<"\n";
		 
		//pour Ba, si existe!
		double Ba=0;
		fichier >> char1 ;
		if(char1=='B'){//then Ba est définie, et Bp aussi
		  fichier >> char1 >> char1>>Ba >>char1;
		  cout <<"Ba="<< Ba<<"\n";
		  
		  double Bp=0;
		  //pour Bp
		  fichier >> char1 >> char1 >>char1 >> Bp >>char1;
		
		  
		// Ha
		fichier >> char1 >> char1 >> char1 >> char1; // lit H a = [       
		}
		else if(char1=='H'){	fichier >> char1 >> char1 >> char1; // le 'H' a été lu, on lit " a = ["
		}
		else cout << "Problème de lecture de l'instance, données non conforme au modèle...\n";
		Ha=DataMatrix(env,m); // M est une matrice carrée à n lignes et n colonnes


		for(int i=0;i<m;i++) {
			fichier >> char1 ; // lit le [ de la ligne
			Ha[i]=IloNumArray(env); // On initialise la ième ligne de M
			for(int j=0;j<n;j++) {
				fichier >> tmp >> char1;
				// Chaque ligne du fichier contient n fois le motif suivant :
				// un entier suivi d’un caractère ("," ou "]" a la fin)
				Ha[i].add(tmp); // On ajoute l’entier lu à la ième ligne de M
			}
			fichier >> char1;//lit ",", Fin de la lecture de la ième ligne de M
		}
		fichier >> char1; //lit ; a la fin de E
		cout<< "Ha=" << Ha << "\n";

		// Ca
		Ca=DataMatrix(env,m); // M est une matrice carrée à n lignes et n colonnes
		fichier >> char1 >> char1 >> char1 >> char1; // lit C a = [

		for(int i=0;i<m;i++) {
			fichier >> char1 ; // lit le [ de la ligne
			Ca[i]=IloNumArray(env); // On initialise la ième ligne de M
			for(int j=0;j<n;j++) {
				fichier >> tmp >> char1;
				// Chaque ligne du fichier contient n fois le motif suivant :
				// un entier suivi d’un caractère ("," ou "]" a la fin)
				Ca[i].add(tmp); // On ajoute l’entier lu à la ième ligne de M
			}
			fichier >> char1;//lit ",", Fin de la lecture de la ième ligne de M
		}
		fichier >> char1; //lit ; a la fin du tableau
		cout<< "Ca=" << Ca << "\n";

		// Hp
		Hp=DataMatrix(env,m); // M est une matrice carrée à n lignes et n colonnes
		fichier >> char1 >> char1 >> char1 >> char1; // lit C a = [

		for(int i=0;i<m;i++) {
			fichier >> char1 ; // lit le [ de la ligne
			Hp[i]=IloNumArray(env); // On initialise la ième ligne de M
			for(int j=0;j<n;j++) {
				fichier >> tmp >> char1;
				// Chaque ligne du fichier contient n fois le motif suivant :
				// un entier suivi d’un caractère ("," ou "]" a la fin)
				Hp[i].add(tmp); // On ajoute l’entier lu à la ième ligne de M
			}
			fichier >> char1;//lit ",", Fin de la lecture de la ième ligne de M
		}
		fichier >> char1; //lit ; a la fin du tableau
		cout<< "Hp=" << Hp << "\n";

		// Cp
		Cp=DataMatrix(env,m); // M est une matrice carrée à n lignes et n colonnes
		fichier >> char1 >> char1 >> char1 >> char1; // lit C p = [

		for(int i=0;i<m;i++) {
			fichier >> char1 ; // lit le [ de la ligne
			Cp[i]=IloNumArray(env); // On initialise la ième ligne de M
			for(int j=0;j<n;j++) {
				fichier >> tmp >> char1;
				// Chaque ligne du fichier contient n fois le motif suivant :
				// un entier suivi d’un caractère ("," ou "]" a la fin)
				Cp[i].add(tmp); // On ajoute l’entier lu à la ième ligne de M
			}
			fichier >> char1;//lit ",", Fin de la lecture de la ième ligne de M
		}
		fichier >> char1; //lit ; a la fin du tableau
		cout<< "Cp=" << Cp << "\n";

		fichier.close();
	}
}


void
setdata (IloModel& model,IloEnv env, instance_Cplex& instance,BoolVarMatrix  x, NumVarMatrix u,  NumVarMatrix v,IloNumVar y,IloNumVar z, IloRangeArray constraintSet)
{
   int& n(instance.n);
  int& m(instance.m);
  DataMatrix& Ha(instance.Ha);
  DataMatrix& Ca(instance.Ca);
  DataMatrix& Hp(instance.Hp); 
  DataMatrix& Cp(instance.Cp);

     //Calcul des bornes Ma et Mp
   double Ma=1;
   double Mp=1;
   cout << "Calcul des bornes..\n";
   for(int i=0; i<m; i++){
      for(int j=0; j<n; j++){
	if((Cp[i][j]>0) &&(1.0/Mp > Cp[i][j])) Mp=1.0/Cp[i][j];
       	if((Ca[i][j]>0) &&(1.0/Ma > Ca[i][j])) Ma=1.0/Cp[i][j];
      }
   }
   cout<< "Bornes Ma=" << Ma <<" et Mp=" << Mp <<".\n";
  
 //ATTENTION : certaines cases (i,j) ne correspondent à rien en réalité, et on a alors Ca(i,j)=Cp(i,j)=0. On fixe alors xij==0
   for(int i=0; i<m; i++){
     for(int j=0; j<n; j++){
       if((Ca[i][j]==0) && (Cp[i][j]==0)){
	 constraintSet.add(x[i][j]==0); //fixe les variables sans réalité à 0
       }
     }
   }
   
   cout << "Variables xij ne correspondant pas à des cases réelles fixées à 0.\n";

   //contraintes linéaires (5)
   IloExpr conLin5p(env);
   IloExpr conLin5a(env);
   for(int i=0; i<m; i++){
      for(int j=0; j<n; j++){ 
	if((Ca[i][j]>0) && (Cp[i][j]>0)){ //on verifie que la case existe ici
	  u[i][j]=IloNumVar(env, 0.0);
	  v[i][j]=IloNumVar(env, 0.0);
	  conLin5p += Cp[i][j] * u[i][j];
	  conLin5a += Ca[i][j] * v[i][j];
	}
      }
   }
   
   constraintSet.add(conLin5a ==1);
   constraintSet.add(conLin5p ==1);

   cout << "Contrainte 5 ajoutée.\n";

   //contraintes linéaires (6,7,8,9)
   y=IloNumVar(env,0.0);
   z=IloNumVar(env,0.0);

   for(int i=0; i<m; i++){
      for(int j=0; j<n; j++){ 
	if((Ca[i][j]>0) && (Cp[i][j]>0)){ //on verifie que la case existe ici
	    IloExpr conLin6p(env);  
	    IloExpr conLin6a(env);
	    IloExpr conLin7p(env);  
	    IloExpr conLin7a(env);
	    IloExpr conLin8p(env);  
	    IloExpr conLin8a(env);
	    IloExpr conLin9p(env);  
	    IloExpr conLin9a(env);
	    conLin6p = u[i][j]-x[i][j]*Mp;
	    constraintSet.add(conLin6p<=0);
	    conLin6a = v[i][j]-x[i][j]*Ma;
	    constraintSet.add(conLin6a<=0);
	    
	    conLin7p =u[i][j]-y ;
	    constraintSet.add(conLin7p<=0);
	    conLin7a =v[i][j]-z ;
	    constraintSet.add(conLin7a<=0);

	    conLin8p =u[i][j]-(x[i][j]-1)*Mp-y;
	    constraintSet.add(conLin8p>=0);
	    conLin8a =v[i][j]-(x[i][j]-1)*Ma-z;
	    constraintSet.add(conLin8a>=0);

	       conLin5a.end();
	       conLin5p.end();
	       conLin6a.end();
	       conLin6p.end();
	       conLin7a.end();
	       conLin7p.end();
	       conLin8a.end();
	       conLin8p.end();
	}  
      }
   }
   cout <<"Contraintes 6,7,8,9 ajoutées.\n";

   // contraine du critère linéarisée (10)
   IloExpr conLin10(env);
 for(int i=0; i<m; i++){
      for(int j=0; j<n; j++){ 
	if((Ca[i][j]>0) && (Cp[i][j]>0)){ //on verifie que la case existe ici
	  conLin10+=Hp[i][j]*Cp[i][j]*u[i][j]+Ha[i][j]*Ca[i][j]*v[i][j];
	}
      }
 }
 constraintSet.add(conLin10>=2);
 cout << "Contrainte 10 ajoutée."<<endl;


 // Objective Function
 IloExpr sumX(env);
 for(int i=0; i<m; i++){
      for(int j=0; j<n; j++){ 
	if((Ca[i][j]>0) && (Cp[i][j]>0)){ //on verifie que la case existe ici
	  sumX+= x[i][j];
	}}}

   model.add(IloMaximize(env, sumX));

   model.add(constraintSet);

   sumX.end();
   conLin10.end();


}

 //Connexity constraints

void
setConnexityConstraints(IloModel model,IloEnv env,  instance_Cplex const& instance, BoolVarMatrix  x, BoolVarMatrix3 height, IloExprArray lhsCallback, IloNumArray rhsCallback,IloRangeArray connexityConstraintSet){

   int n(instance.n);
  int m(instance.m);

 //Contrainte (11)- une et une seule racine
 IloExpr sumH(env);
 for(int i=0; i<m; i++){
      for(int j=0; j<n; j++){ 
	if((Ca[i][j]>0) && (Cp[i][j]>0)){ //on verifie que la case existe ici
	  sumH+= height[i][j][0];
	}}}
 connexityConstraintSet.add(sumH==1);
 sumH.end();

 cout <<"Contrainte 11 ajoutée."<<endl;

 //Contrainte (12) (n*m contraintes) seules les cases selectionées ont une hauteur, celles vides n'ont pas de hauteur
 for(int i=0; i<m; i++){
      for(int j=0; j<n; j++){ 
	  IloExpr sumHij(env);
	  for(int h=0; h<height[i][j].getSize();h++){
	    sumHij+=height[i][j][h];
	  }
	  sumHij+=-x[i][j];
	  connexityConstraintSet.add(sumHij==0);
	  sumHij.end();
	      }
 }
 cout <<"Contrainte 12 ajoutée." <<endl;

 //Contrainte (13) Connexity constraints for Callback -> (O(n²m²)contraintes)

 for(int i=0; i<m; i++){
      for(int j=0; j<n; j++){ 
	if((Ca[i][j]>0) && (Cp[i][j]>0)){
	  for(int h=0;h<height[i][j].getSize()-1;h++){
	    IloExpr lhsExprLijh(env);
	    lhsExprLijh+=height[i][j][h+1];
	    //verifie que l'on n'est pas sur les cotés du carré à chaque ajout de variable:
	    if(i>0)lhsExprLijh+=-height[i-1][j][h];
	    if(i<m-1)lhsExprLijh+=-height[i+1][j][h];
	    if(j>0)lhsExprLijh+=-height[i][j-1][h];
	    if(j<n-1)lhsExprLijh+=-height[i][j+1][h];
	    lhsCallback.add(lhsExprLijh);
	    rhsCallback.add(0.0);
	  }
	}
      }
 }
 // model.add(connexityConstraintSet); //constraints added in main

}

ILOLAZYCONSTRAINTCALLBACK3(CtCallback, IloExprArray, lhsCallback, IloNumArray, rhsCallback, IloNum, eps) {
   IloInt lhsSize = lhsCallback.getSize();
   for (IloInt i = 0; i < lhsSize; i++) {
      IloNum xrhs = rhsCallback[i];
      if ( xrhs < IloInfinity  &&  getValue(lhsCallback[i]) > xrhs + eps ) {
         IloRange cut;
         try {
            cut = (lhsCallback[i] <= xrhs);
            add(cut).end();
            rhsCallback[i] = IloInfinity;
         }
         catch (...) {
            cut.end();
            throw;
         }
      }
   }
}


void
solveTreeConnexityConstraints (IloCplex cplex,IloModel model,IloEnv env, instance_Cplex const& instance, BoolVarMatrix  x, NumVarMatrix u,  NumVarMatrix v,IloNumVar y,IloNumVar z,bool useCallBack,IloInt hMax)
{      

    int  n(instance.n);
  int  m(instance.m);
  DataMatrix  Ha(instance.Ha);
  DataMatrix  Ca(instance.Ca);
  DataMatrix  Hp(instance.Hp); 
  DataMatrix  Cp(instance.Cp);

  IloRangeArray connexityConstraintSet(env);

      BoolVarMatrix3 height(env,m);
      
      cout << "Valeur maximale de la hauteur de l'arbre de connexité hMax= " << hMax <<endl;

      for(int i=0;i<m;i++) {
	height[i]=BoolVarMatrix(env,n);
	for(int j=0;j<n;j++){
	  height[i][j]=IloBoolVarArray(env,hMax);
	}
      }
      
      IloExprArray lhsCallback(env);
      IloNumArray rhsCallback(env);

      setConnexityConstraints(model,env,instance, x,height,lhsCallback,rhsCallback,connexityConstraintSet);
      cout<< "\nContraintes de connexité ajoutées!\n\n";
      

      if(useCallBack) {
	  cplex.use(CtCallback(env, lhsCallback, rhsCallback, 0.5));
	  cout << "eps parameter for cut callback is : "<<  cplex.getParam(IloCplex::EpRHS);
	  cplex.setParam(IloCplex::MIPInterval,1000);//display results
	  cplex.setParam(IloCplex::MIPSearch,IloCplex ::Traditional);
	   cplex.setParam(IloCplex::CutsFactor, 1.0);//no cuts generated (problem can t grow more than once with the cuts)
	  cplex.setParam(IloCplex::EachCutLim,0); //no cuts generated
	   cplex.setParam(IloCplex::PreInd,0);//allow cplex to presolve
      
	   cplex.extract(model);
      }

      else{ for(int i=0;i<lhsCallback.getSize();i++){
	      connexityConstraintSet.add(lhsCallback[i]<=0);
	      	    }
      }
      model.add(connexityConstraintSet);
      cplex.solve();

      env.out() << "Solution status = " << cplex.getStatus() << endl;
      env.out() << "Solution value  = " << cplex.getObjValue() << endl;


   env.out()<<"\nValues of height given to the solution :\n";
      for(int i=0; i<m; i++){
	cout<<"[";
      for(int j=0; j<n; j++){ 
	if(cplex.getValue(x[i][j])==1){
	  for(int h=0;h<hMax;h++){
	    if(cplex.getValue(height[i][j][h])==1) cout << h<<";" ;
	  }
	}
	else cout<< "#;";
      }
      cout << "]"<<endl; //end of line i
      }
      cout <<endl;
}

void
solveMinimizeEdges(IloCplex cplex,IloModel model,IloEnv env,  instance_Cplex const& instance, BoolVarMatrix  x, NumVarMatrix u,  NumVarMatrix v,IloNumVar y,IloNumVar z,IloInt solutionUnConnex){

   int  n(instance.n);
  int  m(instance.m);
  DataMatrix  Ha(instance.Ha);
  DataMatrix  Ca(instance.Ca);
  DataMatrix  Hp(instance.Hp); 
  DataMatrix  Cp(instance.Cp);
  BoolVarMatrix verticalEdges(env,m);

   IloExpr sumVerticalEdges(env);
  for (int i=0;i<m;i++){
    verticalEdges[i]=IloBoolVarArray(env,n+1);
    sumVerticalEdges+=IloSum(verticalEdges[i]);
  }
  

  BoolVarMatrix HorizontalEdges(env,m+1);//horizontalEdge[i][j] is between cases[u
  IloExpr sumHorizontalEdges(env);
  for (int i=0;i<m+1;i++){
   HorizontalEdges[i]=IloBoolVarArray(env,n);
   sumHorizontalEdges+=IloSum(HorizontalEdges[i]);
  }


    IloRangeArray verticalEdgesDefinition(env);
  for (int i=0;i<m;i++){
     for(int j=0;j<n+1; j++){
  
       if(j==0)
	 verticalEdgesDefinition.add(verticalEdges[i][j]-x[i][j]>=0);
       else if (j==n)
	 verticalEdgesDefinition.add(verticalEdges[i][j]-x[i][j-1]>=0);

       else{ 
	 verticalEdgesDefinition.add(verticalEdges[i][j]-x[i][j]+x[i][j-1]>=0);
	 verticalEdgesDefinition.add(verticalEdges[i][j]-x[i][j-1]+x[i][j]>=0);

       }

       
     }
  }
  

    IloRangeArray HorizontalEdgesDefinition(env);
   for (int i=0;i<m+1;i++){
     for(int j=0;j<n; j++){
       if(i==0)
	 HorizontalEdgesDefinition.add(HorizontalEdges[i][j]-x[i][j]>=0);
       else if (i==m)
	 HorizontalEdgesDefinition.add(HorizontalEdges[i][j]-x[i-1][j]>=0);
       else{ 
	 HorizontalEdgesDefinition.add(HorizontalEdges[i][j]-x[i][j]+x[i-1][j]>=0);
	 HorizontalEdgesDefinition.add(HorizontalEdges[i][j]-x[i-1][j]+x[i][j]>=0);
       }
       
     }
  }
  


 IloExpr sumX(env);
 for(int i=0; i<m; i++){
      for(int j=0; j<n; j++){ 
	if((Ca[i][j]>0) && (Cp[i][j]>0)){ //on verifie que la case existe ici
	  sumX+= x[i][j];
	}
      }
 }

 IloRange CutUnconnexSol;
 CutUnconnexSol=(sumX<=solutionUnConnex);
 model.add(CutUnconnexSol);

 IloNum EdgeMultiplier= 0.499;

 model.add(HorizontalEdgesDefinition);
 model.add(verticalEdgesDefinition);
IloExtractable objective;
for (IloModel::Iterator it(model); it.ok(); ++it) {
   if ((*it).isObjective()) {
      objective = (*it).asObjective();
      break;
   }
}
 model.remove(objective);
 model.add(IloMaximize(env,sumX-EdgeMultiplier*(sumHorizontalEdges+sumVerticalEdges)));

      cplex.solve();

      env.out() << "Solution status = " << cplex.getStatus() << endl;
      env.out() << "Solution value  = " << cplex.getObjValue() <<", surface select : " <<cplex.getValue(sumX)<< endl;
    
}

