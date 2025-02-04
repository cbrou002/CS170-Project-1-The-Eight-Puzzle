#include <iostream>
#include <vector>
#include <queue>

using namespace std;

vector<vector<int>> trivial_puzzle = {{ 1, 2, 3 }
                                      { 4, 5, 6 }
                                      { 7, 8, 0 }};
vector<vector<int>> very_easy_puzzle = {{ 1, 2, 3 }
                                        { 4, 5, 6 }
                                        { 7, 0, 8 }};
vector<vector<int>> easy_puzzle = {{ 1, 2, 0 }
                                   { 4, 5, 3 }
                                   { 7, 8, 6 }};
vector<vector<int>> doable_puzzle = {{ 0, 1, 2 }
                                     { 4, 5, 3 }
                                     { 7, 8, 6 }};
vector<vector<int>> hard_puzzle = {{ 8, 7, 1 }
                                   { 6, 0, 2 }
                                   { 5, 4, 3 }};
vector<vector<int>> impossible_puzzle = {{ 8, 6, 7 }
                                         { 2, 5, 4 }
                                         { 3, 0, 1 }};
//Function output puzzle

//node structure

//Function uniform cost search (same for all 3 ptypes of searches)

int main(){
    //input 1 for default puzzle and 2 for custom puzzle
    char puzzle_mode = 0;
    int difficulty = -1;
    vector<vector<int>> user_puzzle;
    cout << "Welcome to my 8-puzzle solver: Type '1' to use a default 8-puzzle,
             or type '2' to make a custom 8-puzzle.\n";
    while(puzzle_mode != '1' && puzzle_mode != '2') //checks for valid input
    {
        cin >> puzzle_mode;
    }
    if(puzzle_mode == '1'){ // sets user puzzle to default puzzle
        cout << "You wish to use a default puzzle. 
                Please enter a desired difficulty on a scale from 0 to 5.\n";
        while (difficulty < 0 || difficulty > 5)
        cin >> difficulty;
        if (difficulty == 0){
            cout << "Trival difficulty selected.\n";
            user_puzzle = trivial_puzzle;
        }
        if (difficulty == 1){
            cout << "Very easy difficulty selected.\n";
            user_puzzle = very_easy_puzzle;
        }   
        if (difficulty == 2){
            cout << "Easy difficulty selected.\n";
            user_puzzle = easy_puzzle;
        }
        if (difficulty == 3){
            cout << "Doable difficulty selected.\n";
            user_puzzle = doable_puzzle;
        }
        if (difficulty == 4){
            cout << "Hard difficulty selected.\n";
            user_puzzle = hard_puzzle;
        }  
        if(difficulty == 5){
            cout << "Trival difficulty selected.\n";
            user_puzzle = impossible_puzzle;
        }
    }
    else if(puzzle_mode == '2'){ // sets user puzzle to custom puzzle
        //prompt to get first row
        //get input for first row
        //prompt to get second row
        //get input for second row
        //prompt to get third row
        //get input for third row
    }

    //if input 1
        // cout << select algorithm 1. Uniform Cost Search, 2. Misplaced Tile Heuristic, 3. Manhattan Distance Heuristic
        //call functions respectively
    //if input 2
        // cout << select algorithm 1. Uniform Cost Search, 2. Misplaced Tile Heuristic, 3. Manhattan Distance Heuristic
        //read input for puzzle by rows
    return 0;
}