//Cristiano Sampaio RA:256352
#include <vector>
#include <cstdio>
#include <sstream>
#include <set>

#include "gurobi_c++.h"

#define NIL 0
#define V_START 0
#define V_END 1
#define COST 2
#define CAPACITY 3
#define HIGH_CAPACITY 100000

using namespace std;

int prizedKpaths(int n, int m, vector<vector<int>> &passages, vector<int> &prizes, int P, int k, int target)
{
	int result = NIL;

	try
	{
		// Set gurobi environment
		GRBEnv env = GRBEnv(true); // creates empty environment
		// env.set(GRB_IntParam_OutputFlag, 0); // comment this line to show optimization data in the std output
		env.start();

		//------------------------------------------------------------------
		//Modelagem dos dados de entrada
		//------------------------------------------------------------------
		//Total de arestas = nº de arestas originais + n-1 arestas partindo da fonte s + 2 novas arestas por vertice
		int edges_total = m+3*n-1;
		//Total de vértices = 2 vezes o número inicial + vértice fonte s
		int vertices_total = 2*n+1;

		//Estrutura para armazenar o grafo em matriz de adjacência
		vector<vector<vector<pair<int,pair<int,int>>>>> graph_matrix(
			vertices_total,
			vector<vector<pair<int,pair<int,int>>>>(vertices_total)
		);
		
		//Gera representação do grafo em matriz de adjacência já realizando todas as manipulações detalhadas no relatório
		int edge_id = 0;		//ID utilizado como indice da lista de variáveis de decisão do Gurobi
		set<int> duplicate_vertices;		//Conjunto para fazer o controle dos vértices que já foram duplicados
		for(int i=0; i<m; i++){
			//Verifica se o vértice "fonte" da aresta atual já foi duplicado
			if(!duplicate_vertices.count(passages[i][V_START])){
				/*
				Duplica vértices já os conectanto com duas arestas: 
						Uma com capacidade 1 e o valor do tesouro
						Outra com capacidade infinito e valor 0
				*/
				duplicate_vertices.insert(passages[i][V_START]);
				graph_matrix[passages[i][V_START]][n+passages[i][V_START]].push_back(
					make_pair(prizes[passages[i][V_START]-1], make_pair(1, edge_id))
				);
				edge_id++;
				graph_matrix[passages[i][V_START]][n+passages[i][V_START]].push_back(
					make_pair(0, make_pair(HIGH_CAPACITY, edge_id))
				);
				edge_id++;
			}
			//Adicionar aresta atual/original na matriz de adjacência
			graph_matrix[n+passages[i][V_START]][passages[i][V_END]].push_back(
				make_pair(-passages[i][COST], make_pair(1, edge_id))
			);
			edge_id++;
		}

		//Adiciona vértice fonte s e o conecta a todos os demais vértices, com exceção do terminal t
		for (int i = 1; i < n; i++){
			if(i!=target){
				graph_matrix[0][i].push_back(
					make_pair(0, make_pair(1, edge_id))
				);
			}
			edge_id++;
		}

		//------------------------------------------------------------------
		//Setup inicial Gurobi
		//------------------------------------------------------------------
		GRBModel model = GRBModel(env); //cria modelo
    model.set(GRB_StringAttr_ModelName, "lab02_mc558");
		model.set(GRB_DoubleParam_TimeLimit, 5); //Limita o tempo de execução a 5 segundos


		//Inicializa vetor com as variáveis de decisão
		GRBVar* x = 0;		//Cria vetor para armazenas as variáveis de decisão
		x = model.addVars(edges_total);
		for (int i = 0; i < edges_total; i++){
			ostringstream vname;
			vname << "x" << i;
			x[i].set(GRB_DoubleAttr_UB, 1);
			x[i].set(GRB_CharAttr_VType, GRB_INTEGER);
			x[i].set(GRB_StringAttr_VarName, vname.str());
		}
		model.update();

		//------------------------------------------------------------------
		//Define função objetivo
		//------------------------------------------------------------------
		GRBLinExpr obj = 0;
		for (int i = 0; i < vertices_total; i++){
			for (int j = 0; j < vertices_total; j++){
				int listSize = graph_matrix[i][j].size();
				for(int l=0; l < listSize; l++){
					obj += x[graph_matrix[i][j][l].second.second] * graph_matrix[i][j][l].first;
				}
			}
		}
		model.setObjective(obj, GRB_MAXIMIZE);
		model.update();

		//------------------------------------------------------------------
		// Define Restrições
		//------------------------------------------------------------------
		// Restrição da capacidade das arestas
		for (int i = 0; i < vertices_total; i++){
			for (int j = 0; j < vertices_total; j++){
				int listSize = graph_matrix[i][j].size();
				for(int l=0; l < listSize; l++){
					GRBLinExpr capacity = 0;
					capacity = x[graph_matrix[i][j][l].second.second];
					model.addConstr(capacity <= graph_matrix[i][j][l].second.first, "capacity");
				}
			}
		}

		//Restrição para a conservação de fluxo
		for (int j = 1; j < vertices_total; j++){
			GRBLinExpr flow = 0;
			for (int i = 0; i < vertices_total; i++){
				if(j!=target ){
					int listSize = graph_matrix[i][j].size();
					for(int l=0;l<listSize;l++){
						flow += x[graph_matrix[i][j][l].second.second];
					}
					listSize = graph_matrix[j][i].size();
					for(int l=0;l<listSize;l++){
						flow -= x[graph_matrix[j][i][l].second.second];
					}
				}	
			}
			model.addConstr(flow == 0, "flow");
		}

		//Restrição para limitar o número de caçadores k na fonte
		GRBLinExpr start_hunter = 0;
		for (int i = 1; i < vertices_total; i++){
			int listSize = graph_matrix[0][i].size();
			for(int l=0;l<listSize;l++){
				start_hunter += x[graph_matrix[0][i][l].second.second];
			}
		}
		model.addConstr(start_hunter == k, "start_hunter");

		//Restrição para limitar o número de caçadores k no terminal
		GRBLinExpr end_hunter = 0;
		for (int i = 1; i < vertices_total; i++){
			int listSize = graph_matrix[i][target].size();
			for(int l=0;l<listSize;l++){
				end_hunter += x[graph_matrix[i][target][l].second.second];
			}
		}
		model.addConstr(end_hunter == k, "end_hunter");

		//------------------------------------------------------------------
		// Resolução do modelo
		//------------------------------------------------------------------
		model.update();
		model.optimize();
		if(model.get(GRB_DoubleAttr_ObjVal) + prizes[target-1]>=P){
			result = model.get(GRB_DoubleAttr_ObjVal) + prizes[target-1];
		}

	}
	catch (GRBException e)
	{
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (...)
	{
		cout << "Exception during optimization." << endl;
	}

	return result;
}

//////////////////////////////////////////////////////
// NÃO MOFIFICAR A PARTIR DAQUI
/* Função que lida com leitura e escrita de valores */
int main()
{
	int n, m, t, k, P;
	cin >> n >> m;

	vector<vector<int>> passages(m, vector<int>(3, NIL));
	vector<int> prizes(n, NIL);

	int u, v, w;
	for (int i = 0; i < m; i++)
	{
		cin >> u >> v >> w;
		passages[i][0] = u;
		passages[i][1] = v;
		passages[i][2] = w;
	}

	int p;
	for (int i = 0; i < n; i++)
	{
		cin >> p;
		prizes[i] = p;
	}
	cin >> P >> k >> t;

	cout << prizedKpaths(n, m, passages, prizes, P, k, t) << endl;

	return 0;
}
