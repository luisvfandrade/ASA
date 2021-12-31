/*
 * Ficheiro: p1.cpp
 * Autor: Luis Freire D'Andrade (N94179), Joana Maria de Brito (N96037)
 * Descricao: Projeto 1 de ASA: desenvolvimento, em linguagem C++, de dois problemas, LIS e LCIS.
*/

/*
 * Bibliotecas:
*/

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

/*
 * Estruturas:
*/

typedef struct sequence {
    vector<int> values;
    int size = 0;
} sequence;

/*
 * Funcoes:
*/

sequence readSequence() {
    int value, c;
    sequence x;

    //Empty line
    if ((c = getchar()) == '\n')
        return x;
    else
        ungetc(c, stdin);

    do {
        //Possible Integer Sequence Error
        if (scanf("%d", &value) == 0) {
            fprintf(stderr, "error: Invalid integer sequence.\n");
            exit(1);
        }
        else {
            x.values.push_back(value);
            x.size++;
        }
    }
    while ((c = getchar()) != '\n' && c != '\t' && c != EOF);

    return x;
}

vector<int> problem1(sequence x) {
    vector<int> maxLis{0, x.size};
    if (x.size == 0)
        return maxLis;

    vector<int> lis(x.size, 1), numLis(x.size, 1);
    maxLis[0] = 1;
    
    for (int i = 1; i < x.size; i++) {
        for (int j = i - 1; j >= 0; j--) {
            if (x.values[j] < x.values[i]) {
                if (lis[i] < lis[j] + 1) {
                    lis[i] = lis[j] + 1;
                    numLis[i] = numLis[j];
                    if (lis[i] > maxLis[0]) {
                        maxLis[0] = lis[i];
                        maxLis[1] = numLis[i];
                    }
                    else if (lis[i] == maxLis[0])
                        maxLis[1] += numLis[i];
                }
                else if (lis[i] == lis[j] + 1) {
                    numLis[i] += numLis[j];
                    if (lis[i] == maxLis[0])
                        maxLis[1] += numLis[j];
                }
            }
        }
    }

    return maxLis;
}

int problem2(sequence x1, sequence x2) {
    int maxLcis = 0;
    if (x1.size == 0 || x2.size == 0)
        return maxLcis;

    vector<int> lcis(x2.size, 0);
    for (int i = 0; i < x1.size; i++) {
        int currentLcis = 0;
        for (int j = 0; j < x2.size; j++) {
            if (x1.values[i] == x2.values[j]) {
                lcis[j] = max(lcis[j], currentLcis + 1);
                if (lcis[j] > maxLcis)
                    maxLcis = lcis[j];
            }
            else if (x1.values[i] > x2.values[j])
                currentLcis = max(lcis[j], currentLcis);
        }
    }

    return maxLcis;
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
        sequence x = readSequence();
        vector<int> results = problem1(x);
        return to_string(results[0]) + " " + to_string(results[1]);
    }
    else {
        sequence x1 = readSequence(), x2 = readSequence();
        if (x1.size > x2.size)
            return to_string(problem2(x1, x2));
        else
            return to_string(problem2(x2, x1));
    }
}

/*
 * Funcao Main:
*/

int main() {
    printf("%s\n", resolve().c_str());
    return 0;
}