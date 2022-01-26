#include <iostream>
#include <list>
#include <vector>
#include <set>
#include <map>

using namespace std;

class Graph {
    int N;
    map<int,int> visitStatus;
    list<int> *adj_vertices;
    bool isCyclic(int vertex, bool marked[], bool *rs);
public:
    Graph(int N);
    bool Cyclic();
    void addEdge(int v1, int v2);
    void mark(int vertex, set<int> &common_nums, set<int> &first_BFS);
    void BFS(int vertex, int value, set<int> &first_BFS, set<int> &common_nums);
};

Graph::Graph(int N) {
    this->N = N;
    adj_vertices = new list<int>[N];
}

void Graph::addEdge(int v1, int v2) {
    adj_vertices[v1-1].push_back(v2-1); 
}

void Graph::mark(int vertex, set<int> &common_nums, set<int> &first_BFS) {
    list<int>::iterator it;
    for (it = adj_vertices[vertex].begin(); it != adj_vertices[vertex].end(); ++it) {
        if (visitStatus[*it] != -1) {
            visitStatus[*it] = -1;
            if (common_nums.find(*it) != common_nums.end())
                common_nums.erase(*it);
            if (first_BFS.find(*it) != first_BFS.end())
                first_BFS.erase(*it);
            mark(*it, common_nums, first_BFS);
        }
    }
}

void Graph::BFS(int vertex, int value, set<int> &first_BFS, set<int> &common_nums) {
    bool *marked = new bool[N];
    for (int i = 0; i < N; i++)
        marked[i] = false;

    list<int> list1;

    marked[vertex] = true;
    list1.push_back(vertex);

    list<int>::iterator it;

    while(!list1.empty()) {
        vertex = list1.front();
        if (value == 0) {
            first_BFS.insert(vertex);
        }
        else {
            if (first_BFS.find(vertex) != first_BFS.end()) {
                if(visitStatus[vertex] != -1) {
                    common_nums.insert(vertex);
                    mark(vertex, common_nums, first_BFS);
                }
            }
        }
        list1.pop_front();

        for (it = adj_vertices[vertex].begin(); it != adj_vertices[vertex].end(); ++it) {
            if (!marked[*it]) {
                marked[*it] = true;
                list1.push_back(*it);
            }
        }
    }
}

void getInput(vector<int> &input, int &n, int &m) {
    int v1, v2;
    scanf("%d %d", &v1, &v2);
    input.push_back(v1);
    input.push_back(v2);
    scanf("%d %d", &n, &m);
}

int getGraph(Graph &graph, vector<int> &count) {
    int v1, v2;
    int n_edges = 0;
    while (scanf("%d %d", &v1, &v2) != EOF) {
        n_edges++;
        graph.addEdge(v2, v1);
        if (count[v2-1]++ == 3)
            return -1;
    }
    return n_edges;
}

bool Graph::isCyclic(int vertex, bool marked[], bool *recursionStack) {
    if(marked[vertex] == false) {
        marked[vertex] = true;
        recursionStack[vertex] = true;

        list<int>::iterator it;
        for (it = adj_vertices[vertex].begin(); it != adj_vertices[vertex].end(); it++){
            if (!marked[*it] && isCyclic(*it, marked, recursionStack))
                return true;
            else if (recursionStack[*it])
                return true;
        }
    }
    recursionStack[vertex] = false;
    return false;
}

bool Graph::Cyclic() {

    bool *recursionStack = new bool[N];
    bool *marked = new bool[N];
    for (int i = 0; i < N; i++) {
        recursionStack[i] = false;
         marked[i] = false;
    }
    for(int i = 0; i < N; i++)
        if (isCyclic(i, marked, recursionStack))
            return true;
    return false;
}

int getValidity(Graph &graph, int m, vector<int> &count) {
    int validity = getGraph(graph, count);
    if (validity != m || graph.Cyclic())
        return -1;
    return 0;
}

int main() {
    vector<int> input;
    set<int> first_BFS;
    set<int> common_nums;
    int n, m;

    getInput(input, n, m);
    Graph graph(n);
    vector<int> count(n, 0);
    int valid = getValidity(graph, m, count);
    
    if (valid == -1)
        cout << 0;
    else {
        graph.BFS(input[0]-1, 0, first_BFS, common_nums);
        graph.BFS(input[1]-1, 1, first_BFS, common_nums);
        
        if (common_nums.empty())
            cout << "-";
        else {
            for (int i: common_nums)
                cout << i+1 << ' ';
        }
    }
    cout << '\n';
    return 0;
}
