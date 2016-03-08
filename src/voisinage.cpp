#include "voisinage.h"

using namespace std;

void display(zone sol,int n, int m){
	for(int i=0;i<m;i++)
	{
		cout<<"[ ";
		for(int j =0;j<n;j++)
		{
			cout<<sol[i][j]<<" ";
		}
		cout<<"]"<<endl;
	}
	cout<<endl;
}
void displayData(carte data,int n, int m){
	for(int i=0;i<m;i++)
	{
		cout<<"[ ";
		for(int j =0;j<n;j++)
		{
			cout<<data[i][j]<<" ";
		}
		cout<<"]"<<endl;
	}
	cout<<endl;
}
bool frontiere(zone sol,int a,int b,int n,int m){
	if(a==0||a==m)
	{
		if(b==0||b==n)
			return true;
		else return false;
	}
	else{
		int valeur=sol[a][b];
		//test si la case est sur un bord ie si un de ses voisins haut/bas/droite/gauche est à une valeur différente
		bool bord=false;
		if(b>0){
			if(sol[a][b-1]!=valeur)
				bord=true;}
		if(b<n-1){
			if(sol[b][b+1]!=valeur)
				bord=true;}

		if(bord==false&&a!=0){
			if(sol[a-1][b]!=valeur)
				bord=true;}
		if(bord==false&&a<m-1){
			if(sol[a+1][b]!=valeur)
				bord=true;}
		return bord;
	}
}
zone arbre(zone graphe, int x, int y,int n,int m)
{
	if(graphe[x][y]==1)
	{
		graphe[x][y]=2;
		//cout<<"examen du noeud "<<x<<" "<<y<<endl;
		//display(graphe);
		if(x!=0)
		{
			if(graphe[x-1][y]==1)
				graphe=arbre(graphe,x-1,y,n,m);
		}
		if(x!=m-1)
		{
			if(graphe[x+1][y]==1)
				graphe=arbre(graphe,x+1,y,n,m);
		}
		if(y!=0)
		{
			if(graphe[x][y-1]==1)
				graphe=arbre(graphe,x,y-1,n,m);
		}
		if(y!=n-1)
		{
			//cout<<x<<" "<<y<<endl<<"graphe[x][y+1]="<<graphe[x][y+1]<<endl;
			if(graphe[x][y+1]==1)
				graphe=arbre(graphe,x,y+1,n,m);
		}
	}
	return graphe;
}
bool connexite(zone sol,int n,int m)
{
	//on cree le graphe correspondant à la sol et on teste si ce graphe est connexe ou non
	int xDeb=0,yDeb=0;
	bool deb=true;
	zone graphe;
	vector<int> colonne;
	bool connexe=true;
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<n;j++)
		{
			colonne.push_back(sol[i][j]);
			if(sol[i][j]==1&&deb){
				xDeb=i;
				yDeb=j;
				deb=false;
			}
		}
		graphe.push_back(colonne);
		colonne.clear();
	}
	//display(graphe);
	graphe=arbre(graphe,xDeb,yDeb,n,m);
	//display(graphe);
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<n;j++)
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
vector<pair<int,int> > voisins(int a,int b,int n,int m)
{
	vector<pair<int,int> > v;
	pair<int,int> p;
	p.first=a;
	p.second=b;
	if(a!=0){
		p.first--;
		v.push_back(p);
		p.first++;
	}
	if(a!=m-1){
		p.first++;
		v.push_back(p);
		p.first--;
	}
	if(b!=0){
		p.second--;
		v.push_back(p);
		p.second++;
	}
	if(b!=n-1){
		p.second++;
		v.push_back(p);
		p.second--;
	}
	return v;
}
float admissible(zone s, Instance& instance)
{
	float numa=0;
	float nump=0;
	float denoma=0;
	float denomp=0;
	for(int i=0;i<instance.m;i++)
	{
		for(int j=0;j<instance.n;j++)
		{
			numa=numa+instance.Ha[i][j]*instance.Ca[i][j]*s[i][j];
			nump=nump+instance.Hp[i][j]*instance.Cp[i][j]*s[i][j];
			denoma=denoma+instance.Ca[i][j]*s[i][j];
			denomp=denomp+instance.Cp[i][j]*s[i][j];
		}
	}
	return (numa/denoma)+(nump/denomp);
}
float score(zone s,int n,int m)
{
	float compteur=0;
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<n;j++)
		{
			compteur=compteur+s[i][j];
		}
	}
	return compteur;
}
solution voisinage1(solution s,bool* chgt,Instance& instance)
{//on tire un chiffre n au hasard entre 0 et taille interieur +taille exterieur
	//on permute la valeur la n-ieme case de la liste (interieur,exterieur)
	//on teste si la connexite est preservee et on recalcule le score et le quotiennt
	//on met à jour interieur et exterieur

   int n= instance.n;
  int m=instance.m;
  carte Ha=instance.Ha;
  carte Hp=instance.Hp;
  carte Ca=instance.Ca;
  carte Cp=instance.Cp;
	solution ss=s;
	int t_int=s.interieur.size(),t_ext=s.exterieur.size(),c;

	c=rand()%(t_int+t_ext);
	if(t_int==1&&c==0)
		c++;
	int a,b;
	if(c<t_int)
	{
		a=ss.interieur[c].first;
		b=ss.interieur[c].second;
	}
	else
	{
		c=c-t_int;
		a=ss.exterieur[c].first;
		b=ss.exterieur[c].second;
	}
	//cout<<"a et b intialises a= "<<a<<" b= "<<b<<endl;
	if(Hp[a][b]+Ha[a][b]!=0)//on permute si la case est possible
	{
		ss.z[a][b]=(ss.z[a][b]+1)%2;
		*chgt=true;
		if(ss.z[a][b]==0)//si la case est passee à zero il faut verifier la connexite et mettre a jour interieur et exterieur
		{
			bool connexe=connexite(ss.z,n,m);
			//cout<<"solution connexe : "<<connexe<<endl;
			//display(s.z,n,m);
			if(connexe)
			{
				//on met a jour l'exterieur et l'interieur
				vector<pair<int,int> > casesVoisines=voisins(a,b,n,m);
				int x,y;
				for(unsigned int i=0;i<casesVoisines.size();i++)
				{
					x=casesVoisines[i].first;
					y=casesVoisines[i].second;
					//cout<<"acces case (x,y) x= "<<x<<" y= "<<y<<endl;
					if(ss.z[x][y]==0)//la case fait partie de l'ancien exterieur
					{
						vector<pair<int,int> > v_xy=voisins(x,y,n,m);
						int compteur=0;
						for(unsigned int j=0;j<v_xy.size();j++)
						{
							//cout<<"acces case voisine (x,y) i= "<<v_xy[i].first<<" j= "<<v_xy[i].second<<endl;
							if(ss.z[v_xy[j].first][v_xy[j].second]==1)
								compteur++;
						}
						if(compteur==0)
							//la case n'a plus de voisins dans la solution donc elle ne fait plus partie de l'exterieur
						{
							for(unsigned int j=0;j<ss.exterieur.size();j++)
							{
								if(ss.exterieur[j].first==x&&ss.exterieur[j].second==y)
								{
									ss.exterieur.erase(ss.exterieur.begin()+j);
									break;
								}
							}
						}

					}
				}
				//on passe la case dont on a change la valeur de interieur vers exterieur
				for(unsigned int j=0;j<ss.interieur.size();j++)
				{
					if(ss.interieur[j].first==a&&ss.interieur[j].second==b)
					{
						ss.interieur.erase(ss.interieur.begin()+j);
						break;
					}
				}
				pair<int,int> p;
				p.first=a;
				p.second=b;
				ss.exterieur.push_back(p);
				ss.taille--;
			}
			else
			{
				ss.z[a][b]=1;
				*chgt=false;

			}

		}
		else//ss.z[a][b]==1
		{
			vector<pair<int,int> > v=voisins(a,b,n,m);
			for(unsigned int i=0;i<v.size();i++)
			{
				//cout<<"acces case voisin (a,b) x= "<<v[i].first<<" y= "<<v[i].second<<endl;
				if(ss.z[v[i].first][v[i].second]==0)//on verifie que la case est sur l'exterieur sinon on l'ajoute
				{
					bool find=false;
					for(unsigned int j=0;j<ss.exterieur.size();j++)
					{
						if(ss.exterieur[j].first==v[i].first&&ss.exterieur[j].second==v[i].second)
						{
							find=true;
							break;
						}
					}
					if(!find)
						ss.exterieur.push_back(v[i]);
				}
			}
			for(unsigned int j=0;j<ss.exterieur.size();j++)
				{
					if(ss.exterieur[j].first==a&&ss.exterieur[j].second==b)
					{
						ss.exterieur.erase(ss.exterieur.begin()+j);
						break;
					}
				}
				pair<int,int> p;
				p.first=a;
				p.second=b;
				ss.interieur.push_back(p);
				ss.taille++;
		}
	}
	else
		*chgt=false;
	ss.quotient=admissible(ss.z,instance);
	ss.taille=score(ss.z,n,m);

return ss;
}

