#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>

using namespace std;

void findCommonWords(const string& file1, const string& file2, const string& outputFile) {
    ifstream inFile1(file1);
    ifstream inFile2(file2);
    ofstream outFile(outputFile);

    if (!inFile1.is_open() || !inFile2.is_open()) {
        cerr << "Could not open one of the input files." << endl;
        return;
    }

    if (!outFile.is_open()) {
        cerr << "Could not open the output file." << endl;
        return;
    }

    unordered_set<string> wordsFile1;
    string word;

    while (inFile1 >> word) {
        wordsFile1.insert(word);
    }
    inFile1.close();

    while (inFile2 >> word) {
        if (wordsFile1.find(word) != wordsFile1.end()) {
            outFile << word << endl;
        }
    }
    inFile2.close();
    outFile.close();
}

int main() {
    findCommonWords("output/permutations.txt", "../src/words/wordle-ta.txt", "output/guesses.txt");
    return 0;
}
