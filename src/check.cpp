#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
using namespace std;

int main() {
    ifstream generatedFile("output/generated.txt");
    if (!generatedFile.is_open()) {
        cerr << "ERROR: Could not open generated.txt!" << endl;
        return -1;
    }

    unordered_set<string> generatedWords;
    string word;
    while (generatedFile >> word) {
        generatedWords.insert(word);
    }
    generatedFile.close();

    ifstream compareFile("words/wordle-ta.txt");
    if (!compareFile.is_open()) {
        cerr << "ERROR: Could not open comparison txt file!" << endl;
        return -2;
    }

    ofstream matchingFile("output/output.txt");
    if (!matchingFile.is_open()) {
        cerr << "ERROR: Could not open the output file!" << endl;
        return -3;
    }

    string compareWord;
    while (compareFile >> compareWord) {
        if (generatedWords.find(compareWord) != generatedWords.end()) {
            matchingFile << compareWord << endl;
        }
    }

    compareFile.close();
    matchingFile.close();

    return 0;
}