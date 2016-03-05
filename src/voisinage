#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;
int n,m;
typedef vector<vector<int>> zone;
void display(zone solution){
	for(int i=0;i<n;i++)
	{
		cout<<"[ ";
		for(int j =0;j<m;j++)
		{
			cout<<solution[i][j]<<" ";
		}
		cout<<"]"<<endl;
	}
	cout<<endl;
}
bool frontiere(zone solution,int a,int b){
	if(a==0||a==n)
	{
		if(b==0||b==m)
			return true;
	}
	else{
	int valeur=solution[a][b];
	//test si la case est sur un bord ie si un de ses voisins haut/bas/droite/gauche est à une valeur différente
	bool bord=false;
	if(b>0){
		if(solution[a][b-1]!=valeur)
			bord=true;}
	if(b<m-1){
		if(solution[b][b+1]!=valeur)
			bord=true;}

	if(bord==false&&a!=0){
		if(solution[a-1][b]!=valeur)
			bord=true;}
	if(bord==false&&a<n-1){
		if(solution[a+1][b]!=valeur)
			bord=true;}
	return bord;
	}
}
zone arbre(zone graphe, int x, int y)
{
	if(graphe[x][y]==1)
	{
		graphe[x][y]=2;
		//cout<<"examen du noeud "<<x<<" "<<y<<endl;
		//display(graphe);
		if(x!=0)
		{
			if(graphe[x-1][y]==1)
				graphe=arbre(graphe,x-1,y);
		}
		if(x!=n-1)
		{
			if(graphe[x+1][y]==1)
				graphe=arbre(graphe,x+1,y);
		}
		if(y!=0)
		{
			if(graphe[x][y-1]==1)
				graphe=arbre(graphe,x,y-1);
		}
		if(y!=m-1)
		{
			//cout<<x<<" "<<y<<endl<<"graphe[x][y+1]="<<graphe[x][y+1]<<endl;
			if(graphe[x][y+1]==1)
				graphe=arbre(graphe,x,y+1);
		}
	}
	return graphe;
}
bool connexite(zone solution,int a,int b)
{
	//on cree le graphe correspondant à la solution et on teste si ce graphe est connexe ou non
	int xDeb,yDeb;
	bool deb=true;
	zone graphe;
	vector<int> colonne;
	bool connexe=true;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			colonne.push_back(solution[i][j]);
			if(solution[i][j]==1&&deb){
				xDeb=i;
				yDeb=j;
				deb=false;
			}
		}
		graphe.push_back(colonne);
		colonne.clear();
	}
	//display(graphe);
	graphe=arbre(graphe,xDeb,yDeb);
	//display(graphe);
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			if(graphe[i][j]==1)
			{
				connexe=false;
				break;
			}
		}
	}
	return connexe;
}
zone voisinage1(zone solution,int a,int b,bool* chgt)
{
	bool bord=frontiere(solution,a,b);
	cout<<"la case est sur la frontiere? "<<bord<<endl;
	if(bord)
	{
		solution[a][b]=(solution[a][b]+1)%2;
		*chgt=true;
		bool connexe=false;
		if(solution[a][b]==0)
			//on teste si la solution reste connexe ou non
			connexe=connexite(solution,a,b);
		else
			connexe=true;
		if (connexe==false)
		{
			cout<<"la solution n'est pas connexe"<<endl;
			solution[a][b]=(solution[a][b]+1)%2;
			*chgt=false;
		}
		else
			cout<<"la solution est connexe"<<endl;
	}
	return solution;
}

int main (int argc, char* argv[]) {
	n=8;
	m=5;
	zone solution;
	for(int i=0;i<n;i++)
	{
		vector<int> colonne;
		for(int j=0;j<m;j++)
		{
			if(j<2&&i<2)
				colonne.push_back(1);
			else if(i==2)
				colonne.push_back(1);
			else if(j>2&&i>2)
				colonne.push_back(1);
			else
				colonne.push_back(0);
		}
		solution.push_back(colonne);
		//cout<<"colonne "<<i<<" "<<colonne.size()<<endl;
		colonne.clear();
	}
	display(solution);
	bool chgt=false;
	solution=voisinage1(solution,1,1,&chgt);
	display(solution);
	cout<<"chgt? "<<chgt<<endl;
	//return 0;
}
