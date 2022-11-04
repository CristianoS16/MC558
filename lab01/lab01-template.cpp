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

typedef pair<int, double> iPair;

int bitToDec(bool bitVector[6]){
	int result = 0;
	for(int i =1; i<7; i++){
		if(bitVector[i-1]){
			result += pow(2,6-i); 
		}
	}
	return result;
}
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

	//Estrutura de dados para guardar os estados
	int combinations = pow(2,k);
	double mat[64][100];
  std::fill(*mat, *mat + n*combinations, PROB_MIN);
	mat[0][0] = 1;
	//Imprime a matriz de estados
	cout << "\n ---------- imprime matriz de estados ---------- \n";
	for(int i=0; i<combinations; i++){
		for(int j=0; j<n; j++){
			cout << mat[i][j] << " ";
		}
		cout << "\n";
	}

	//bitarray
	bool bitVector[] = {false, false, false, false, false, false};
	bitset<6> bits;
	cout << bits << '\n';
	cout << bits.to_ulong() << '\n';
	

	cout << "\n AQUI MAN AQUI MAN: " << bitToDec( bitVector ) << "\n";

	// bitVector[4] = true;

	// mat[bitToDec(bitVector)][2] = 1;

	// 	//Imprime a matriz de estados
	// cout << "\n ---------- imprime matriz de estados ---------- \n";
	// for(int i=0; i<combinations; i++){
	// 	for(int j=0; j<n; j++){
	// 		cout << mat[i][j] << " ";
	// 	}
	// 	cout << "\n";
	// }

	// return 1;

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
	Q.push( make_pair(0,0) );

	while(Q.size()>0){
		cout << "\n --------------- Entra While  --------------- \n";
		int v = Q.top().first;
		int comb = int(Q.top().second);
		double prob = mat[int(Q.top().second)][v];
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
			
			// cout << "i: " << v << " j: " << j << "\n";
			// cout << "DENTRO LALAL: " << Graph[0][0].second;
			// cout << "\ncost: " << cost << "\n";
			cout << "\nFATHER : " << v << "\n";
			// cout << "distance[v] : " << distance[v] << "\n";
			cout << "RELAX : " << u << "\n";
			// cout << "distance[u] : " << distance[u] << "\n";
			// cout << "distance[v]*cost : " << distance[v]*cost << "\n";

			// double newProb = distance[v]*cost;
			double newProb = mat[comb][v]*cost;
			bitset<6> newComb( comb ); 
			//Se tem um ingrediente no novo vertice atualiza combinação para a tabela
			if(mapaIngredientes[u]>0 ){
				bitset<6> aux1( comb );
				// cout << "aux1: " << aux1;
				// cout << " \n";
				bitset<6> aux2;
				aux2[mapaIngredientes[u]-1] = 1;
				// cout << "aux2: " << aux2;
				// cout << " \n";
				newComb = aux1 | aux2;}
				// cout << "Decimal: " << mapaIngredientes[u];
				// cout << " \n";
				// cout << "ALOOUUUU: " << newComb;
				cout << "CURRENT_PROB: " << mat[newComb.to_ulong()][u] << " NEW_PROB: " << newProb << "\n";
				if(mat[newComb.to_ulong()][u]<newProb){
					mat[newComb.to_ulong()][u] = newProb;
					Q.push( make_pair(u, newComb.to_ulong()) );
					cout << "mat[" << newComb.to_ulong() << "][" << u << "] = " << newProb << "\n";

				
				// return 1;
			}
			// cout << "	TESTE LEGAL : " << newProb;
			// if( distance[u]<distance[v]*cost ){
			// 	cout << "ENTRA IF \n";
			// 	distance[u] = distance[v]*cost;
			// 	Q.push( make_pair(u, distance[u]));
			// }
			//Atualiza ingredientes pegos
			// if( mapaIngredientes[u]>0 ){
			// 	cout << "INGREDIENTE ENCONTRADO: " << mapaIngredientes[u] << "\n";
			// 	bitVector[6-mapaIngredientes[u]] = true;	
			// 	bits[mapaIngredientes[u]-1] = 1;
			// 	cout << "COMPARA BITS" << bitToDec(bitVector) << "|||" << bits.to_ulong() << '\n';
			// }
			//Atualiza estado no tabela
			// if(mat[bitToDec(bitVector)][u]<newProb){
			// 	cout << "ENTRE NO IF MAN HUHUHUHUHUHU";
			// 	mat[bitToDec(bitVector)][u] = newProb;
			// 	cout << "\nTESTESTESTETESTE: " << mat[bitToDec(bitVector)][u];
			// 	cout << "\n u: " << u << "bittodec: " << bitToDec(bitVector);
			// }
			// if(mat[bits.to_ulong()][u]<newProb){
			// 	cout << "ENTRE NO IF MAN HUHUHUHUHUHU";
			// 	mat[bits.to_ulong()][u] = newProb;
			// 	cout << "\nTESTESTESTETESTE: " << mat[bits.to_ulong()][u];
			// 	cout << "\n u: " << u << "bittodec: " << bits.to_ulong();
			// }
			cout << "\n\n";
		}

		// break;
	}

	cout << "\n DISTANCE \n";
	for(int i=0; i<n; i++){
		cout << distance[i] << " - ";
	}

	//Imprime a matriz de estados
	cout << "\n ---------- imprime matriz de estados final da interação ---------- \n";
	for(int i=0; i<combinations; i++){
		for(int j=0; j<n; j++){
			cout << mat[i][j] << "     ";
		}
		cout << "\n";
	}

	// if(mat[combinations-1][n-1]!=0){
	resultado = mat[combinations-1][n-1];
	// }

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
