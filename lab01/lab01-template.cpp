#include <iostream>
#include <vector>
#include <iomanip>
#include <queue>
#include <cmath>
#include <math.h>
#include <bitset>

using namespace std;

#define NIL -1
#define PROB_MIN 0

// ATENÇÃO!
// Você é livre para incluir outras bibliotecas, implementar funções auxiliares, acrecentar definições, tipos e estruturas.

//Tipo para armazenar os pares de vertice x probabilidade e vertice x combinação de ingredientes
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

	//Estrutura para armazenar o grafo em lista de adjacência
	vector<vector<pair<int,double> > > Graph(n);

	//Gera representação do grafo em lista de adjacência
	for(int i=0; i<m; i++){
		Graph[pontes[i][0]].push_back( make_pair(pontes[i][1], probPontes[i]) );
		Graph[pontes[i][1]].push_back( make_pair(pontes[i][0], probPontes[i]) );
	}

	//Estrutura de dados para guardar os estados
	int combinations = pow(2,k);
	double mat[64][100];
  std::fill(*mat, *mat + n*combinations, PROB_MIN);
	mat[0][0] = 1;

	//Bitset para guardar as possiveis combinações dos ingredientes
	bitset<6> bits;
	
	//Dijkstra 

	//Fila de prioridades para extrair maximo
	priority_queue<iPair> Q;

	//Primeiro vertice e adicionado a fila de prioridades para startar o algoritmo
	Q.push( make_pair(0,0) );


	while(Q.size()>0){

		//Recupera vertice, conjunto de ingerdientes e probabilidade
		int v = Q.top().first;
		int comb = int(Q.top().second);
		
		//Retira o vertice corrente da fila de prioridades
		Q.pop();

		//Relax
		
		//Verifica o tamanho da lista de adj do vertice corrente
		int listSize = Graph[v].size();
		
		//Faz o relax para vertice adj
		for(int j=0; j<listSize; j++){

			//Recupera um vertice adjascente e calcula a nova probabilidade para alcança-lo
			int u = Graph[v][j].first;
			double cost = Graph[v][j].second;
			double newProb = mat[comb][v]*cost;
			
			// Vetor de bits para simbolizar a combinação de ingredientes
			bitset<6> newComb( comb );

			//Se alcança um ingrediente novo atualiza combinação de ingredientes encontrados
			if(mapaIngredientes[u]>0 ){
				bitset<6> aux1( comb );
				bitset<6> aux2;
				aux2[mapaIngredientes[u]-1] = 1;
				newComb = aux1 | aux2;
			}
				
			//Realiza o Relax de fato, verificando se a nova probabilidade para esse estado e melhor que a anterior
			if(mat[newComb.to_ulong()][u]<newProb){
				mat[newComb.to_ulong()][u] = newProb;
				Q.push( make_pair(u, newComb.to_ulong()) );
			}
		}
	}

	//Atualiza o resultado
	resultado = mat[combinations-1][n-1];

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

	cout << fixed << setprecision(5) << resultado << endl;

	return 0;
}
