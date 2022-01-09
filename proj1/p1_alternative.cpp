/*
 * Ficheiro: p1.cpp
 * Autor: Luis Freire D'Andrade (No 94179), Joana Maria de Brito (No 96037)
 * Descricao: Projeto 1 de ASA: desenvolvimento, em linguagem C++, de dois problemas, LIS e LCIS.
*/

/*
 * Bibliotecas:
*/

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#define P1 0
#define P2_FIRST 1
#define P2_SECOND 2

using namespace std;

/*
 * Estruturas:
*/

typedef struct sequence {
    vector<int> values;
    long int size = 0;
} sequence;

/*
 * Funcoes:
*/

sequence readSequence(int flag, map<int, int> *values1 = NULL, map<int, int> *values2 = NULL) {
    sequence x;
    string line;
    int value;

    getline(cin, line);
    istringstream iss(line);
    while (iss >> value) {
        if (flag == P2_SECOND)
            if ((*values1)[value] == 0)
                continue;
            else
                values2->insert(pair<int, int>(value, 1));
        else if (flag == P2_FIRST)
            values1->insert(pair<int, int>(value, 1));
        x.values.push_back(value);
        x.size++;
    }
    return x;
}

vector<long int> problem1(sequence x) {
    vector<long int> maxLis{0, x.size};
    
    if (x.size == 0)
        return maxLis;

    maxLis[0] = 1;
    vector<long int> lis(x.size, 1), numLis(x.size, 1);
    for (long int i = 1; i < x.size; i++) {
        for (long int j = i - 1; j >= 0; j--) {
            if (x.values[j] < x.values[i]) {
                if (lis[i] > lis[j] + 1)
                    continue;
                else if (lis[i] < lis[j] + 1) {
                    lis[i] = lis[j] + 1;
                    numLis[i] = numLis[j];
                    if (lis[i] > maxLis[0]) {
                        maxLis[0] = lis[i];
                        maxLis[1] = numLis[i];
                    }
                    else if (lis[i] == maxLis[0])
                        maxLis[1] += numLis[i];
                }
                else {
                    numLis[i] += numLis[j];
                    if (lis[i] == maxLis[0])
                        maxLis[1] += numLis[j];
                }
            }
        }
    }

    return maxLis;
}

int problem2(sequence x1, sequence x2, map<int, int> common) {
    int **current, **previous, **aux, solution;
    sequence x3;
    for (map<int,int>::iterator it = common.begin(); it != common.end(); ++it) {
        x3.values.push_back(it->first);
        x3.size++;
    }

    if (x1.size == 0 || x2.size == 0 || x3.size == 0)
        return 0;

    current = new int*[x2.size + 1];
    previous = new int*[x2.size + 1];
    for (int i = 0; i < x2.size + 1; i++) {
        current[i] = new int[x1.size + 1];
        previous[i] = new int[x1.size + 1];
    }

    for (int i = 0; i < x3.size + 1; i++) {
        aux = current;
        current = previous;
        previous = aux;
        for (int j = 0; j < x2.size + 1; j++) {
            for (int k = 0; k < x1.size + 1; k++) {
                if (i == 0 || j == 0 || k == 0)
                    current[j][k] = 0;
                else if (x3.values[i - 1] == x2.values[j - 1] && x2.values[j - 1] == x1.values[k - 1])
                    current[j][k] = previous[j - 1][k - 1] + 1;
                else
                    current[j][k] = max(max(current[j - 1][k], current[j][k - 1]), previous[j][k]);
            }
        }
    }
    solution = current[x2.size][x1.size];

    for (int i = 0; i < x2.size + 1; i++) {
        delete [] current[i];
        delete [] previous[i];
    }
    delete [] current;
    delete [] previous;

    return solution;
}

string resolve() {
    int problem, c;

    //Possible Problem Number Related Errors
    if (scanf("%d", &problem) == 0 || (problem != 1 && problem != 2)) {
        fprintf(stderr, "error: Invalid problem number.\n");
        exit(1);
    }
    else if ((c = getchar()) != '\n') {
        fprintf(stderr, "error: The first line must contain only the problem number.\n");
        exit(1);
    }
    
    if (problem == 1) {     
        sequence x = readSequence(P1);
        vector<long int> results = problem1(x);
        return to_string(results[0]) + " " + to_string(results[1]);
    }
    else {
        map<int, int> auxValues, commonValues;
        sequence aux = readSequence(P2_FIRST, &auxValues), x1, x2 = readSequence(P2_SECOND, &auxValues, &commonValues);
        for (int i = 0; i < aux.size; i++) {
            if (commonValues[aux.values[i]] == 1) {
                x1.values.push_back(aux.values[i]);
                x1.size++;
            }
        }
        return to_string(problem2(x1, x2, commonValues));
    }
}

/*
 * Funcao Main:
*/

int main() {
    printf("%s\n", resolve().c_str());
    return 0;
}
