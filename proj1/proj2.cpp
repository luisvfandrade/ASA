/*
 * Ficheiro: p2.cpp
 * Autor: Luis Freire D'Andrade (N94179), Joana Maria de Brito (N96037)
 * Descricao: Projeto 2 de ASA: desenvolvimento, em linguagem C++, de dois problemas, se o grafo G forma uma árvore geneologica valida
 e o conjunto dos ancestrais comuns mais proximos entre v1 e v2.
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
map <int,int> read_list(int arches) {
    int x, y, c;
    map<int, int> mapa;
    map<int, int> number_of_in_degrees;
    map<int, int> error;


    for(int i=0; i < arches; i++){
        if(scanf("%d %d", &x, &y)==0 ||(x <= 0 && y <= 0)){
            fprintf(stderr, "error: v1 and v2 have to be positive numbers.\n");
            exit(1);
        }
        else if ((c = getchar()) != '\n') {
        fprintf(stderr, "error: Every line must contain only the problem number.\n");
        exit(1);
        }

        //se o vertice x ja e chave no mapa
        if(number_of_in_degrees.find(x)!=number_of_in_degrees.end()){
            number_of_in_degrees[x]++;
            if(number_of_in_degrees[x]>2){
                return error;
            }
            mapa.insert(pair<int, int>(x, y));
        }
        //se o vertice x ainda nao e chave no mapa
        else{
            number_of_in_degrees.insert(pair<int, int>(x, 1));
            mapa.insert(pair<int, int>(x, y));
        }
    }
    return mapa;
}


vector<int> problem(int v1, int v2, map<int, int> values) {
    vector<int> res{0, 2};
    return res;
}

string resolve() {
    int v1, v2;
    int c;
    int n_v, n_a;

    //Possible Problem Number Related Errors
    if (scanf("%d %d", &v1, &v2) == 0 || v1 <= 0 || v2 <= 0) {
        fprintf(stderr, "error: v1 and v2 have to be positive numbers.\n");
        exit(1);
    }

    else if ((c = getchar()) != '\n') {
        fprintf(stderr, "error: The first line must contain only the problem number.\n");
        exit(1);
    }
    
    if (scanf("%d %d", &n_v, &n_a) == 0 || n_v <= 0 || n_a <= 0) {
        fprintf(stderr, "error: Invalid.\n");
        exit(1);
    }
    else if ((c = getchar()) != '\n') {
        fprintf(stderr, "error: The second line must contain only the problem number.\n");
        exit(1);
    }

    map<int, int> values = read_list(n_a);
    //caso em que o grafo nao e uma arvore valida
    if(values.empty()){
        return "";
    }
    //caso em que o grafo e uma arvore valida
    else{
        vector<int> results = problem(v1, v2, values);
        return to_string(results[0]) + " " + to_string(results[1]);
    }

}

/*
 * Funcao Main:
*/

int main() {
    printf("%s\n", resolve().c_str());
    return 0;
}