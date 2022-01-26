/*
 * Ficheiro: p2.cpp
 * Autor: Luis Freire D'Andrade (No 94179), Joana Maria de Brito (No 96037)
 * Descricao: Projeto 2 de ASA: desenvolvimento, em linguagem C++, de dois problemas, se um grafo G forma uma árvore geneologica, e o conjunto dos ancestrais comuns mais proximos entre dois vértices v1 e v2.
*/

/*
 * Bibliotecas:
*/

#include <stdio.h>
#include <vector>
#include <set>
#include <queue>
#include <string>
#include <iostream>

using namespace std;

/*
 * Constantes:
*/

#define BFS_V1 1
#define BFS_V2 2

/*
 * Classes:
*/

class Graph {
    public:
        int nodes, edges;
        vector<vector<int>> adjList, revList;
        vector<int> inDegrees;

    public:
        Graph(int n, int e) {
            nodes = n;
            edges = e;
            adjList.resize(nodes);
            revList.resize(nodes);
            inDegrees.resize(nodes);
        }

        bool addEdge(int v1, int v2) {
            adjList[v1 - 1].push_back(v2 - 1);
            revList[v2 - 1].push_back(v1 - 1);
            inDegrees[v2 - 1]++;
            if (inDegrees[v2 - 1] > 2)
                return false;
            return true;
        }

        bool dfsCycle(int node, int *visited, int *path);

        bool genTree();

        void remLca(int node, int *visited, set<int> *parents, set<int> *lca);

        void bfsLca(int flag, int node, set<int> *parents, set<int> *lca);

        set<int> lca(int v1, int v2);
};

bool Graph::dfsCycle(int node, int *visited, int *path) {
    if (visited[node] == 0) {
        visited[node] = 1;
        path[node] = 1;

        for(vector<int>::iterator i = adjList[node].begin(); i != adjList[node].end(); ++i) {
            if (visited[*i] == 0 && dfsCycle(*i, visited, path))
                return true;
            else if (path[*i] == 1) {
                return true;
            }
        }
    }
    path[node] = 0;
    return false;
}

bool Graph::genTree() {
    int *visited = new int[nodes](), *path = new int[nodes]();

    for (int i = 0; i < nodes; i++)
        if (dfsCycle(i, visited, path))
            return false;
    return true;
}

void Graph::remLca(int node, int *visited, set<int> *parents, set<int> *lca) {
    queue<int> queue;
    queue.push(node);
    while (!queue.empty()) {
        int i = queue.front();
        queue.pop();
        for (vector<int>::iterator j = revList[i].begin(); j != revList[i].end(); ++j) {
            if (visited[*j] == 0) {
                visited[*j] = 1;
                (*parents).erase(*j);
                queue.push(*j);
            }
            else {
                if ((*parents).find(*j) != (*parents).end()) {
                    (*parents).erase(*j);
                    (*lca).erase(*j);
                }
            }
        }
    }
}

void Graph::bfsLca(int flag, int node, set<int> *parents, set<int> *lca = NULL) {
    int *visited = new int[nodes]();
    queue<int> queue;

    queue.push(node);
    visited[node] = 1;
    while (!queue.empty()) {
        int i = queue.front();
        queue.pop();
        for (vector<int>::iterator j = revList[i].begin(); j != revList[i].end(); ++j) {
            if (visited[*j] != 0)
                continue;
            visited[*j] = 1;
            if (flag == BFS_V1) {
                (*parents).insert(*j);
                queue.push(*j);
            }
            else {
                if ((*parents).find(*j) != (*parents).end()) {
                    (*lca).insert(*j);
                    remLca(*j, visited, parents, lca);
                }
                else
                    queue.push(*j);
            }
        }
    }
}

set<int> Graph::lca(int v1, int v2) {
    set<int> parentsV1, lca;

    parentsV1.insert(v1);
    bfsLca(BFS_V1, v1, &parentsV1);
    
    if (parentsV1.find(v2) != parentsV1.end())
        lca.insert(v2);
    else {
        bfsLca(BFS_V2, v2, &parentsV1, &lca);
    }
    return lca;
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
    else if (scanf("%d %d", &nodes, &edges) != 2 || nodes <= 0 || edges < 0) {
        fprintf(stderr, "error: V and/or E of the graph not valid.\n");
        exit(1);
    }
    else if ((c = getchar()) != '\n') {
        fprintf(stderr, "error: The second line must only contain V and E of the graph.\n");
        exit(1);
    }

    set<int> results;
    string solution = "";
    Graph g(nodes, edges);

    if (!buildGraph(g) || !g.genTree())
        return "0";  
    results = g.lca(v1 - 1, v2 - 1);
    if (results.empty())
        return "-";
    for (int i: results)
        solution += to_string(i + 1) + " ";
    return solution;
}

/*
 * Função Main:
*/

int main() {
    printf("%s\n", resolve().c_str());
    return 0;
}
