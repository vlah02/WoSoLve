// #include <iostream>
// #include <fstream>
// #include <string>
// #include <algorithm>
// #include <vector>
// #include <unordered_set>
// #include <set>
// #include <cstdlib>
// #include <ctime>
// #include <map>

// using namespace std;

// struct WordleState {
//     string availableChars; 
//     vector<char> greenChars; 
//     vector<set<char>> yellowChars;
//     set<char> requiredChars; 
//     map<char, int> letterCounts;

//     WordleState() : availableChars("abcdefghijklmnopqrstuvwxyz"), greenChars(5, ' '), yellowChars(5) {}

//     void updateState(const string& guess) {
//         map<char, int> minusCounts;
//         for (int i = 0; i < guess.length(); i += 2) {
//             char letter = guess[i];
//             char symbol = guess[i + 1];
//             int position = i / 2;

//             if (symbol == '+') {
//                 greenChars[position] = letter;
//                 letterCounts[letter]++;
//             } else if (symbol == '*') {
//                 yellowChars[position].insert(letter);
//                 requiredChars.insert(letter); 
//                 letterCounts[letter]++;
//             } else if (symbol == '-') {
//                 minusCounts[letter]++;
//             }
//         }

//         for (const auto& lc : minusCounts) {
//             if (letterCounts.find(lc.first) == letterCounts.end() || letterCounts[lc.first] < lc.second) {
//                 availableChars.erase(remove(availableChars.begin(), availableChars.end(), lc.first), availableChars.end());
//             }
//         }
//     }

//     bool isValidWord(const string& word) {
//         for (char c : requiredChars) {
//             if (word.find(c) == string::npos) {
//                 return false;
//             }
//         }
//         return true;
//     }
// };

// void generateWords(const string &characters, int currentLength, string currentWord, WordleState& state, set<string> &outputSet) {
//     if (currentLength == 5) {
//         if (state.isValidWord(currentWord)) {
//             outputSet.insert(currentWord);
//         }
//         return;
//     }

//     for (char c : characters) {
//         if (state.greenChars[currentLength] != ' ' && state.greenChars[currentLength] != c) continue;
//         if (state.yellowChars[currentLength].count(c)) continue;

//         generateWords(characters, currentLength + 1, currentWord + c, state, outputSet);
//     }
// }

// int main() {
//     WordleState state;
//     string guess;
//     set<string> outputSet;

//     while (true) {
//         cout << "Enter guess (format: a+b-c*d+e-), or 'exit' to quit: ";
//         cin >> guess;
//         cout << endl;

//         if (guess == "exit") break;

//         if (guess.length() != 10) {
//             cout << "ERROR: Incorrect number of characters!" << endl;
//             continue;
//         }

//         state.updateState(guess);
//         outputSet.clear();
//         generateWords(state.availableChars, 0, "", state, outputSet);

//         ofstream outputFile("src/output/generated.txt", ofstream::out);
//         for (const auto &word : outputSet) {
//             outputFile << word << endl;
//         }
//         outputFile.close();

//         ifstream generatedFile("src/output/generated.txt");
//         if (!generatedFile.is_open()) {
//             cerr << "ERROR: Could not open the generated file!" << endl;
//             return -1;
//         }

//         unordered_set<string> generatedWords;
//         string generatedWord;
//         while (generatedFile >> generatedWord) {
//             generatedWords.insert(generatedWord);
//         }
//         generatedFile.close();

//         ifstream compareFile("src/words/wordle-ta.txt");
//         if (!compareFile.is_open()) {
//             cerr << "ERROR: Could not open comparison txt file!" << endl;
//             return -2;
//         }

//         ofstream matchingFile("src/output/output.txt");
//         if (!matchingFile.is_open()) {
//             cerr << "ERROR: Could not open the output file!" << endl;
//             return -3;
//         }

//         string compareWord;
//         while (compareFile >> compareWord) {
//             if (generatedWords.find(compareWord) != generatedWords.end()) {
//                 matchingFile << compareWord << endl;
//             }
//         }

//         compareFile.close();
//         matchingFile.close();

//         vector<string> words;
//         string word;
        
//         ifstream randomFile("src/output/output.txt");
//         if (!randomFile) {
//             std::cerr << "Unable to open file" << endl;
//             return 1;
//         }
        
//         while (randomFile >> word) {
//             words.push_back(word);
//         }
        
//         randomFile.close();
        
//         srand(static_cast<unsigned int>(time(nullptr)));
        
//         if (words.empty()) {
//             std::cerr << "No words found in the file." << endl;
//             return 1;
//         }
        
//         int random_index = std::rand() % words.size();
        
//         cout << "Random word to try (or look at src/output/output.txt for a list of possible solutions): " << words[random_index] << endl;
//     }

//     return 0;
// }


#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <set>
#include <cstdlib>
#include <ctime>
#include <map>

using namespace std;

struct WordleState {
    string availableChars; 
    vector<char> greenChars; 
    vector<set<char>> yellowChars;
    set<char> requiredChars; 
    map<char, int> letterCounts;

    WordleState() : availableChars("abcdefghijklmnopqrstuvwxyz"), greenChars(5, ' '), yellowChars(5) {}

    void updateState(const string& guess) {
        map<char, int> minusCounts, plusAndStarCounts;

        for (int i = 0; i < guess.length(); i += 2) {
            char letter = guess[i];
            char symbol = guess[i + 1];
            int position = i / 2;

            if (symbol == '+') {
                greenChars[position] = letter;
                plusAndStarCounts[letter]++;
            } else if (symbol == '*') {
                yellowChars[position].insert(letter);
                requiredChars.insert(letter);
                plusAndStarCounts[letter]++;
            } else if (symbol == '-') {
                minusCounts[letter]++;
            }
        }

        for (const auto& [letter, count] : minusCounts) {
            if (plusAndStarCounts[letter] < count) {
                availableChars.erase(remove(availableChars.begin(), availableChars.end(), letter), availableChars.end());
            }
        }

        for (const auto& [letter, count] : plusAndStarCounts) {
            letterCounts[letter] = count;
        }
    }

    bool isValidWord(const string& word) {
        for (char c : requiredChars) {
            if (word.find(c) == string::npos) {
                return false;
            }
        }

        for (const auto& [letter, count] : letterCounts) {
            if (count != std::count(word.begin(), word.end(), letter)) {
                return false;
            }
        }
        
        return true;
    }

};

void generateWords(const string &characters, int currentLength, string currentWord, WordleState& state, set<string> &outputSet) {
    if (currentLength == 5) {
        if (state.isValidWord(currentWord)) {
            outputSet.insert(currentWord);
        }
        return;
    }

    for (char c : characters) {
        if (state.greenChars[currentLength] != ' ' && state.greenChars[currentLength] != c) continue;
        if (state.yellowChars[currentLength].count(c)) continue;

        generateWords(characters, currentLength + 1, currentWord + c, state, outputSet);
    }
}

int main() {
    WordleState state;
    string guess;
    set<string> outputSet;

    while (true) {
        cout << "Enter guess (format: a+b-c*d+e-), or 'exit' to quit: ";
        cin >> guess;
        cout << endl;

        if (guess == "exit") break;

        if (guess.length() != 10) {
            cout << "ERROR: Incorrect number of characters!" << endl;
            continue;
        }

        state.updateState(guess);
        outputSet.clear();
        generateWords(state.availableChars, 0, "", state, outputSet);

        ofstream outputFile("src/output/generated.txt", ofstream::out);
        for (const auto &word : outputSet) {
            outputFile << word << endl;
        }
        outputFile.close();

        ifstream generatedFile("src/output/generated.txt");
        if (!generatedFile.is_open()) {
            cerr << "ERROR: Could not open the generated file!" << endl;
            return -1;
        }

        unordered_set<string> generatedWords;
        string generatedWord;
        while (generatedFile >> generatedWord) {
            generatedWords.insert(generatedWord);
        }
        generatedFile.close();

        ifstream compareFile("src/words/wordle-ta.txt");
        if (!compareFile.is_open()) {
            cerr << "ERROR: Could not open comparison txt file!" << endl;
            return -2;
        }

        ofstream matchingFile("src/output/output.txt");
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

        vector<string> words;
        string word;
        
        ifstream randomFile("src/output/output.txt");
        if (!randomFile) {
            std::cerr << "Unable to open file" << endl;
            return 1;
        }
        
        while (randomFile >> word) {
            words.push_back(word);
        }
        
        randomFile.close();
        
        srand(static_cast<unsigned int>(time(nullptr)));
        
        if (words.empty()) {
            std::cerr << "No words found in the file." << endl;
            return 1;
        }
        
        int random_index = std::rand() % words.size();
        
        cout << "Random word to try (or look at src/output/output.txt for a list of possible solutions): " << words[random_index] << endl;
    }

    return 0;
}
