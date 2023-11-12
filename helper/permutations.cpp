#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void generatePermutations(const string &letters, string currentPerm, int level, ofstream &outputFile) {
    if (level == 5) { 
        outputFile << currentPerm << endl;
        return;
    }
    for (char c : letters) {
        generatePermutations(letters, currentPerm + c, level + 1, outputFile);
    }
}

int main() {
    ifstream inputFile("output/common.txt");
    ofstream outputFile("output/permutations.txt");
    string letters;

    if (!inputFile.is_open()) {
        cerr << "Could not open the input file!" << endl;
        return EXIT_FAILURE;
    }

    if (!outputFile.is_open()) {
        cerr << "Could not open the output file!" << endl;
        return EXIT_FAILURE;
    }

    inputFile >> letters;
    inputFile.close();

    letters = letters.substr(0, 7);

    generatePermutations(letters, "", 0, outputFile);

    outputFile.close();
    return EXIT_SUCCESS;
}
