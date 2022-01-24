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
#include <map>
#include <string>
#include <iostream>

using namespace std;

/*
 * Constantes:
*/

#define CYCLE 0
#define DFS_V1 1
#define DFS_V2 2

/*
 * Estruturas:
*/

typedef struct nodeInfo {
    int inDegree = 0;
    int outDegree = 0;
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

        bool addEdge(int v1, int v2) {
            adjList[v1 - 1].push_back(v2 - 1);
            revList[v2 - 1].push_back(v1 - 1);
            nodesInfo[v2 - 1].inDegree++;
            nodesInfo[v1 - 1].outDegree++;

            if (nodesInfo[v2 - 1].inDegree > 2) {
                return false;
            }
            else if (nodesInfo[v1 - 1].outDegree > 2) {
                return false;
            }
            return true;
        }

        bool cycleUtil(int node, int *visited, int *path);

        void revUtil(int flag, int node, int *visited, map<int, int> *attainables1, int *count, map<int, int> *attainables2);

        bool dfs(int flag, int *node, map<int, int> *attainables1, map<int, int> *attainables2);

        vector<int> lca(int v1, int v2);
};

bool Graph::cycleUtil(int node, int *visited, int *path) {
    visited[node] = 1;
    path[node] = 1;

    for(vector<int>::iterator i = adjList[node].begin(); i != adjList[node].end(); ++i) {
        if (visited[*i] == 0 && cycleUtil(*i, visited, path))
            return true;
        else if (path[*i] == 1) {
            return true;
        }
    }
    path[node] = 0;
    return false;
}

void Graph::revUtil(int flag, int node, int *visited, map<int, int> *attainables1, int *count = NULL, map<int, int> *attainables2 = NULL) {
    int initialCount = 0;

    visited[node] = 1;
    if (flag == DFS_V2) initialCount = *count;

    for (vector<int>::iterator i = revList[node].begin(); i != revList[node].end(); ++i) {
        if (visited[*i] == 1)
            continue;
        else if (flag == DFS_V1) {
            (*attainables1)[*i] = 1;
            revUtil(flag, *i, visited, attainables1);
        }
        else {
            if ((*attainables1)[*i] == 1) {
                (*attainables2)[*i] = *count;
                (*count)++;
            }
            revUtil(flag, *i, visited, attainables1, count, attainables2);
        }
        if (flag == DFS_V2) (*count) = initialCount;
    }
}

bool Graph::dfs(int flag, int *node = NULL, map<int, int> *attainables1 = NULL, map<int, int> *attainables2 = NULL) {
    int *visited = new int[nodes](), *path;
    
    if (flag == CYCLE) {
        path = new int[nodes]();
        for (int i = 0; i < nodes; i++) {
            if (visited[i] == 0)
                if (cycleUtil(i, visited, path))
                    return false;
        }
    }
    else if (flag == DFS_V1)
        revUtil(flag, *node, visited, attainables1);
    else {
        int count = 0;
        revUtil(flag, *node, visited, attainables1, &count, attainables2);
    }
    return true;
}

vector<int> Graph::lca(int v1, int v2) {
    vector<int> results;
    map<int, int> attainablesV1, attainablesCommon;

    attainablesV1[v1] = 1;
    dfs(DFS_V1, &v1, &attainablesV1);

    if (attainablesV1[v2] == 1)
        results.push_back(v2);
    else {
        dfs(DFS_V2, &v2, &attainablesV1, &attainablesCommon);
        for (map<int, int>::iterator i = attainablesCommon.begin(); i != attainablesCommon.end(); ++i) {
            if (i->second == 0)
                results.push_back(i->first);
        }
    }
    return results;
}

/*
 * Funções:
*/
   
bool buildGraph(Graph &g) {
    char c;

    for (int i = 0; i < g.edges; i++) {
        int x, y;
        if (scanf("%d %d", &x, &y) != 2 || x <= 0 || y <= 0) {
            fprintf(stderr, "error: Edge not valid.\n");
            exit(1);
        }
        else if ((c = getchar()) != '\n') {
            fprintf(stderr, "error: The edge lines must only contain two nodes.\n");
            exit(1);
        }

        if (!g.addEdge(x, y))
            return false;
    }
    return true;
}

string resolve() {
    int v1, v2, nodes, edges, solutionSize;
    char c;

    if (scanf("%d %d", &v1, &v2) != 2 || v1 <= 0 || v2 <= 0) {
        fprintf(stderr, "error: v1 and/or v2 not valid.\n");
        exit(1);
    }
    else if ((c = getchar()) != '\n') {
        fprintf(stderr, "error: The first line must only contain v1 and v2.\n");
        exit(1);
    }
    else if (scanf("%d %d", &nodes, &edges) != 2 || nodes <= 0 || edges <= 0) {
        fprintf(stderr, "error: V and/or E of the graph not valid.\n");
        exit(1);
    }
    else if ((c = getchar()) != '\n') {
        fprintf(stderr, "error: The second line must only contain V and E of the graph.\n");
        exit(1);
    }

    vector<int> results;
    string solution = "";
    Graph g(nodes, edges);

    if (!buildGraph(g) || !g.dfs(CYCLE))
        return "0";
        
    results = g.lca(v1 - 1, v2 - 1);
    if ((solutionSize = results.size()) == 0)
        return "-";

    vector<int>::iterator i = results.begin();
    solution += to_string(*(i++) + 1);
    for(; i != results.end(); ++i)
        solution += " " + to_string(*i + 1);
        
    return solution;
}

/*
 * Função Main:
*/

int main() {
    printf("%s\n", resolve().c_str());
    return 0;
}