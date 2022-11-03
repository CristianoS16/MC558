#include <iostream>
#include <vector>
#include <iomanip>
#include <queue>
#include <cmath>
using namespace std;

#define NIL -1
#define PROB_MIN 0

// ATENÇÃO!
// Você é livre para incluir outras bibliotecas, implementar funções auxiliares, acrecentar definições, tipos e estruturas.

typedef pair<int, double> iPair;

// Esta função deve retornar o valor obtido pela sua solução //
/* Parâmetros: 
    n, m e k são os números de terras, pontes e ingredientes, respecitivamente.
    pontes[m][2] é uma matriz m X 2 indicando as pontes existentes.
    probPontes[j] é a probabilidade da ponte j não cair, para cada ponte j de de 0 a m-1.
    mapaIngredientes[i] é o ingrediente na terra i, para toda terra i de 0 a n-1.
*/
double melhorRota(int n, int m, vector<vector<int> > &pontes, vector<double> &probPontes, int k, vector<int> &mapaIngredientes)
{
	double resultado = NIL;
	
	// MODIFIQUE AQUI NO MEIO
	cout << "n=" << n;
	cout << "\nm=" << m;
	cout << "\nk=" << k;

	cout << "\n----------\nINGREDIENTES\n";
	for (int i=0; i < n; i++){
		cout << mapaIngredientes[i] << " ";
	}

	cout << "\n----------\nPROB:\n";
	for (int i=0; i<m; i++){
		cout << probPontes[i] << " ";
	}

	cout << "\n----------\nPONTES\n";
	for (int i = 0; i < m; i++){
		cout << pontes[i][0] << ',' << pontes[i][1];
		cout << '\n';
	}
	
	cout << "\n ---------- imprimiu pontes ---------- \n";
	//Estrutura para armazenar o grado em lista de adjacência
	vector<vector<pair<int,double> > > Graph(n);

	for(int i=0; i<m; i++){
		Graph[pontes[i][0]].push_back( make_pair(pontes[i][1], probPontes[i]) );
		Graph[pontes[i][1]].push_back( make_pair(pontes[i][0], probPontes[i]) );
	}

	//Imprime lista de adjacência 
	for(int i=0; i<n; i++){
		cout << "VERTICE " << i << '\n';
		int listSize = Graph[i].size();
		for(int j=0; j<listSize; j++){
			cout << "i: " << i << " j: " << j << "\n";
			cout << Graph[i][j].first << ", " << Graph[i][j].second << '\n';
		}
	}

	cout << "imprimeu lista de adj";

	//Dijkstra 
	//typedef pair<int, double> iPair;
	//Fila de prioridades
	priority_queue<iPair> Q;
	vector<double> distance(n, PROB_MIN);

	//Inicia o primeiro vertice com prob 1
	distance[0] = 1;
	//Primeiro vertice e empilhado para startar o algoritmo
	Q.push( make_pair(0,1) );

	while(Q.size()>0){
		cout << "\n --------------- Entra While  --------------- \n";
		int v = Q.top().first;
		double prob = Q.top().second;
		Q.pop();

		cout << "\nCurrent v: " << v;
		cout << "\nCurrent prob: " << prob << "\n";

		//Relax
		// if(distance[v]!=prob) continue;

		//Verifica o tamanho da lista de adj
		int listSize = Graph[v].size();
		//Faz o relax para vertice adj
		for(int j=0; j<listSize; j++){
			int u = Graph[v][j].first;
			double cost = Graph[v][j].second;
			cout << "i: " << v << " j: " << j << "\n";
			cout << "DENTRO LALAL: " << Graph[0][0].second;
			cout << "\ncost: " << cost << "\n";
			cout << "\nFATHER : " << v << "\n";
			cout << "distance[v] : " << distance[v] << "\n";
			cout << "RELAX : " << u << "\n";
			cout << "distance[u] : " << distance[u] << "\n";
			cout << "distance[v]*cost : " << distance[v]*cost << "\n";

			if( distance[u]<distance[v]*cost ){
				cout << "ENTRA IF \n";
				distance[u] = distance[v]*cost;
				Q.push( make_pair(u, distance[u]));
			}
		}
	}

	cout << "\n DISTANCE \n";
	for(int i=0; i<n; i++){
		cout << distance[i] << " - ";
	}

	return resultado;
}

/////////////////////////////////////////////////////
// NÃO MOFIFICAR A PARTIR DAQUI
/* Função que lida com leitura e escrita de valores */
int main()
{
	int n, m, k;
	cin >> n >> m;

	vector<vector<int> > pontes(m, vector<int>(2, NIL));
	vector<int> mapaIngredientes(n, NIL);
	vector<double> probPontes(m, NIL);

	int u, v;
	double p;
	for (int i = 0; i < m; i++)
	{
		cin >> u >> v >> p;
		pontes[i][0] = u;
		pontes[i][1] = v;
		probPontes[i] = p;
	}

	cin >> k;

	int ing;
	for (int i = 0; i < n; i++)
	{
		cin >> ing;
		mapaIngredientes[i] = ing;
	}

	double resultado = NIL;
	resultado = melhorRota(n, m, pontes, probPontes, k, mapaIngredientes);

	cout << "\n RESULTADO: " << fixed << setprecision(5) << resultado << endl;

	return 0;
}
