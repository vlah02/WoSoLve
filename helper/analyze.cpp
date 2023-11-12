#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

bool compare(const pair<char, int>& a, const pair<char, int>& b) {
    return a.second > b.second;
}

int main() {
    ifstream inputFile("../src/words/wordle-ta.txt");
    ofstream outputFile("output/common.txt");

    if (!inputFile.is_open()) {
        cerr << "Could not open the input file!" << endl;
        return EXIT_FAILURE;
    }
    if (!outputFile.is_open()) {
        cerr << "Could not open the output file!" << endl;
        return EXIT_FAILURE;
    }

    map<char, int> charCount;
    int totalChars = 0;
    char c;

    while (inputFile.get(c)) {
        if (isalpha(c)) { 
            c = tolower(c);
            charCount[c]++;
            totalChars++;
        }
    }

    inputFile.close();

    vector<pair<char, int>> charCountVector(charCount.begin(), charCount.end());

    sort(charCountVector.begin(), charCountVector.end(), compare);

    cout << "Character Percentages in Descending Order:" << endl;
    for (const auto &pair : charCountVector) {
        double percentage = 100.0 * pair.second / totalChars;
        cout << pair.first << ": " << percentage << "%" << endl;
    }

    string topLetters;
    for (int i = 0; i < 7 && i < charCountVector.size(); ++i) {
        topLetters += charCountVector[i].first;
    }

    outputFile << topLetters << endl;

    outputFile.close();

    return EXIT_SUCCESS;
}
