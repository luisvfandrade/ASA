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
        vector<vector<int>> adjList;
        int **revList;

    public:
        Graph(int n, int e) {
            nodes = n;
            edges = e;
            adjList.resize(nodes);
            revList = new int*[n];
            for (int i = 0; i < n; i++) {
                revList[i] = new int[3];
                revList[i][0] = -1;
                revList[i][1] = -1;
                revList[i][2] = 0;
            }
        }

        bool addEdge(int v1, int v2) {
            int pos;

            adjList[v1 - 1].push_back(v2 - 1);
            if ((pos = ++revList[v2 - 1][2]) > 2)
                return false;
            revList[v2 - 1][pos - 1] = v1 - 1;
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
        for (int j = 0; j < revList[i][2]; j++) {
            if (visited[revList[i][j]] == 0) {
                visited[revList[i][j]] = 2;
                (*parents).erase(revList[i][j]);
                queue.push(revList[i][j]);
            }
            else if (visited[revList[i][j]] == 1) {
                visited[revList[i][j]] = 2;
                (*parents).erase(revList[i][j]);
                (*lca).erase(revList[i][j]);
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
        if (visited[i] == 2)
            continue;
        for (int j = 0; j < revList[i][2]; j++) {
            if (visited[revList[i][j]] != 0)
                continue;
            visited[revList[i][j]] = 1;
            if (flag == BFS_V1) {
                (*parents).insert(revList[i][j]);
                queue.push(revList[i][j]);
            }
            else {
                if ((*parents).find(revList[i][j]) != (*parents).end()) {
                    (*lca).insert(revList[i][j]);
                    remLca(revList[i][j], visited, parents, lca);
                }
                else
                    queue.push(revList[i][j]);
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
    for (int i = 0; i < g.edges; i++) {
        int x, y;
        cin >> x >> y;
        if (x <= 0 || y <= 0) {
            fprintf(stderr, "error: Edge not valid.\n");
            exit(1);
        }

        if (!g.addEdge(x, y))
            return false;
    }
    return true;
}

string resolve() {
    int v1, v2, nodes, edges;

    cin >> v1 >> v2;
    if (v1 <= 0 || v2 <= 0) {
        fprintf(stderr, "error: v1 and/or v2 not valid.\n");
        exit(1);
    }
    cin >> nodes >> edges;
    if (nodes <= 0 || edges < 0) {
        fprintf(stderr, "error: V and/or E of the graph not valid.\n");
        exit(1);
    }

    set<int> results;
    string solution = "";
    Graph g(nodes, edges);

    if (!buildGraph(g) || !g.genTree())
        return "0\n";  
    results = g.lca(v1 - 1, v2 - 1);
    if (results.empty())
        return "-\n";
    for (int i: results)
        solution += to_string(i + 1) + " ";
    solution += "\n";
    return solution;
}

/*
 * Função Main:
*/

int main() {
    ios_base::sync_with_stdio(false);
    cout << resolve();
    return 0;
}
