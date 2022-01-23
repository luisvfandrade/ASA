/*
 * Ficheiro: p2.cpp
 * Autor: Luis Freire D'Andrade (N94179), Joana Maria de Brito (N96037)
 * Descricao: Projeto 2 de ASA: desenvolvimento, em linguagem C++, de dois problemas, se um grafo G forma uma árvore geneologica, e o conjunto dos ancestrais comuns mais proximos entre dois vértices v1 e v2.
*/

/*
 * Bibliotecas:
*/

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

/*
 * Constantes:
*/

#define DFS 0
#define CYCLE 1

/*
 * Estruturas:
*/

typedef struct nodeInfo {
    int inDegree = 0;
    int outDegree = 0;
    int source = 1;
} nodeInfo;

/*
 * Classes:
*/

class Graph {
    public:
        int nodes, edges;
        vector<vector<int>> adjList, revList;
        vector<nodeInfo> nodesInfo;

    public:
        Graph(int n, int e) {
            nodes = n;
            edges = e;
            adjList.resize(nodes);
            revList.resize(nodes);
            nodesInfo.resize(nodes);
        }

        void addEdge(int v1, int v2) {
            adjList[v1 - 1].push_back(v2 - 1);
            revList[v2 - 1].push_back(v1 - 1);
            nodesInfo[v2 - 1].inDegree++;
            nodesInfo[v1 - 1].outDegree++;
            nodesInfo[v2 - 1].source = 0;

            if (nodesInfo[v2 - 1].inDegree > 2) {
                // Para Submeter: Tirar Prints!
                fprintf(stderr, "error: The graph must be a genealogical tree.\n");
                exit(1);
            }
        }

        void cycleUtil(int node, int *visited, int *path);

        void dfsUtil(int node, int *visited);

        void dfs(int flag);
};

void Graph::cycleUtil(int node, int *visited, int *path) {
    visited[node] = 1;
    path[node] = 1;

    for(vector<int>::iterator i = adjList[node].begin(); i != adjList[node].end(); ++i) {
        if (visited[*i] == 0)
            cycleUtil(*i, visited, path);
        else if (path[*i] == 1) {
            fprintf(stderr, "error: The graph must be a genealogical tree.\n");
            exit(1);
        }
    }
    path[node] = 0;
}

void Graph::dfsUtil(int node, int *visited) {
    visited[node] = 1;
 
    for (vector<int>::iterator i = adjList[node].begin(); i != adjList[node].end(); ++i)
        if (visited[*i] == 0)
            dfsUtil(*i, visited);
}

void Graph::dfs(int flag) {
    int *visited = new int[nodes](), *path;
    if (flag == CYCLE) path = new int[nodes]();

    for (int i = 0; i < nodes; i++) {
        if (nodesInfo[i].source == 0)
            continue;
        if (flag == CYCLE) {
            if (visited[i] == 1)
                path[i] = 0;
            else 
                cycleUtil(i, visited, path);
        }
        else
            if (visited[i] == 0)
                dfsUtil(i, visited);
    }
}

/*
 * Funções:
*/
   
Graph buildGraph(int nodes, int edges) {
    char c;
    Graph g(nodes, edges);

    for (int i = 0; i < edges; i++) {
        int x, y;
        if (scanf("%d %d", &x, &y) != 2 || x <= 0 || y <= 0) {
            fprintf(stderr, "error: Edge not valid.\n");
            exit(1);
        }
        else if ((c = getchar()) != '\n') {
            fprintf(stderr, "error: The edge lines must only contain two nodes.\n");
            exit(1);
        }

        g.addEdge(x, y);
    }
    return g;
}

string resolve() {
    int v1, v2, nodes, edges;
    char c;

    if (scanf("%d %d", &v1, &v2) != 2 || v1 <= 0 || v2 <= 0) {
        fprintf(stderr, "error: v1 and/or v2 not valid.\n");
        exit(1);
    }
    else if ((c = getchar()) != '\n') {
        fprintf(stderr, "error: The first line must only contain v1 and v2.\n");
        exit(1);
    }

    if (scanf("%d %d", &nodes, &edges) != 2 || nodes <= 0 || edges <= 0) {
        fprintf(stderr, "error: V and/or E of the graph not valid.\n");
        exit(1);
    }
    else if ((c = getchar()) != '\n') {
        fprintf(stderr, "error: The second line must only contain V and E of the graph.\n");
        exit(1);
    }

    Graph g = buildGraph(nodes, edges);
    g.dfs(CYCLE);
    
    return "WE DID IT";
    //return to_string() + " " + to_string();
}

/*
 * Função Main:
*/

int main() {
    printf("%s\n", resolve().c_str());
    return 0;
}