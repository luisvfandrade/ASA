#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

typedef struct sequence {
    vector<int> values;
    int size = 0;
} sequence;

sequence readSequence() {
    int value, c;
    sequence x;

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
    while ((c = getchar()) != '\n' && c != EOF);

    return x;
}

vector<int> problem1(sequence x) {
    vector<int> maxLis{1, x.size};
    return maxLis;
}

int problem2(sequence x1, sequence x2) {
    int maxLcis = 0;
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
        printf("here %d\n", problem);       
        sequence x = readSequence();
        vector<int> results = problem1(x);
        return to_string(results[0]) + " " + to_string(results[1]);
    }
    else {
        sequence x1 = readSequence(), x2 = readSequence();
        return to_string(problem2(x1, x2));
    }
}

int main() {
    printf("%s\n", resolve().c_str());
    return 0;
}