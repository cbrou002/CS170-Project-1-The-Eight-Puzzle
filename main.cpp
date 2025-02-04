#include <iostream>
#include <sstream>
#include <vector>
#include <queue>

using namespace std;

vector<vector<int>> trivial_puzzle = {{ 1, 2, 3 },
                                      { 4, 5, 6 },
                                      { 7, 8, 0 }};
vector<vector<int>> very_easy_puzzle = {{ 1, 2, 3 },
                                        { 4, 5, 6 },
                                        { 7, 0, 8 }};
vector<vector<int>> easy_puzzle = {{ 1, 2, 0 },
                                   { 4, 5, 3 },
                                   { 7, 8, 6 }};
vector<vector<int>> doable_puzzle = {{ 0, 1, 2 },
                                     { 4, 5, 3 },
                                     { 7, 8, 6 }};
vector<vector<int>> hard_puzzle = {{ 8, 7, 1 },
                                   { 6, 0, 2 },
                                   { 5, 4, 3 }};
vector<vector<int>> impossible_puzzle = {{ 8, 6, 7 },
                                         { 2, 5, 4 },
                                         { 3, 0, 1 }};

struct Node{ //node structure
    vector<vector<int>> state; //current state of the puzzle
    vector<vector<int>>* parent;//previous state of the puzzle
    Node(const vector<vector<int>>& s){//Node Constructor
        state = s;
        parent = nullptr;
    }
};
bool goalTest(vector<vector<int>> state){
    if(state == trivial_puzzle){
        return true;
    }
    else{
        return false;
    }
}

void printPuzzle(vector<vector<int>> puzzle){//Prints the puzzle
    for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                cout << puzzle[i][j] << " ";
            }
            cout << endl;
        }
}

//Function uniform cost search (same for all 3 types of searches)
Node* SearchAlgorithm(vector<vector<int>> puzzle, int type){
    queue<Node*> nodes;
    Node* root = new Node(puzzle);
    nodes.push(root);
    Node* goal_node = nullptr;
    while(true){
        if(nodes.empty()){
            return nullptr;
        }
        if(goalTest(nodes.front()->state)){
            goal_node = nodes.front();
            return goal_node;
        }
        nodes.pop();

    }
    return nullptr;
}

int main(){
    //input 1 for default puzzle and 2 for custom puzzle
    int puzzle_mode = 0;
    int temp;
    vector<vector<int>> user_puzzle(3, vector<int>(3));

    cout << "Welcome to my 8-puzzle solver: Type (1) to use a default 8-puzzle, or type (2) to make a custom 8-puzzle. \n";
    while(puzzle_mode != 1 && puzzle_mode != 2) //checks for valid input
    {
        cin >> puzzle_mode;
    }
    int difficulty = -1;
    if(puzzle_mode == 1){ /////////////////////////////////////////////// CREATE DEFAULT PUZZLE
        cout << "You wish to use a default puzzle. "; 
        while (difficulty < 0 || difficulty > 5){//checks for valid input
            cout << "Please enter a desired difficulty on a scale from 0 to 5.\n";
            cin >> difficulty;
        }
        if (difficulty == 0){
            cout << "Trivial difficulty selected. \n";
            user_puzzle = trivial_puzzle;
        }
        if (difficulty == 1){
            cout << "Very easy difficulty selected. \n";
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
        //printPuzzle(user_puzzle);
    }
    
    else if(puzzle_mode == 2){ /////////////////////////////////////////// CREATE CUSTOM PUZZLE
        string row_input;
        cout << "Enter your puzzle, using a zero to represent the blank. Please only enter valid 8-puzzles.\n";
        cout << "Enter the first row (separated by spaces and use 0 to represent the blank space), then hit enter\n";
        for(int i = 0; i < 3; i++){
            cin >> temp;
            user_puzzle[0][i] = temp;
        }
        cout << "Enter the second row (separated by spaces and use 0 to represent the blank space), then hit enter\n";
        for(int i = 0; i < 3; i++){
            cin >> temp;
            user_puzzle[1][i] = temp;
        }
        cout << "Enter the third row (separated by spaces and use 0 to represent the blank space), then hit enter\n";
        for(int i = 0; i < 3; i++){
            cin >> temp;
            user_puzzle[2][i] = temp;
        }
        cout << endl;
        //printPuzzle(user_puzzle);
    }
    ///////////////////////////////////////////////////PICKING ALGORITHM
    temp = -1;
    while(temp != 1 && temp != 2 && temp != 3){//checks for valid input
        cout << "Select algorithm. (1) for Uniform Cost Search, (2) for the Misplaced Tile Heuristic, or (3) the Manhattan Distance Heuristic.\n";
        cin >> temp;
    }
    if(temp == 1){ //Uniform Cost Search

    }
    else if(temp == 2){ //Misplaced Tile Heuristic
        
    }
    else if(temp == 3){ //Manhattan Distance Heuristic

    }
    return 0;
}