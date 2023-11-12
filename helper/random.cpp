#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    vector<std::string> words;
    string word;
    
    //ifstream file("output/guesses.txt");
    ifstream file("helper/output/guesses.txt");
    if (!file) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }
    
    while (file >> word) {
        words.push_back(word);
    }
    
    file.close();
    
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    if (words.empty()) {
        std::cerr << "No words found in the file." << std::endl;
        return 1;
    }
    
    int random_index = std::rand() % words.size();
    
    std::cout << "Random word: " << words[random_index] << std::endl;
    
    return 0;
}
