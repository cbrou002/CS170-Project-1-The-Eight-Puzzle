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
    Node* parent;//previous state of the puzzle
    Node(const vector<vector<int>>& s, Node* p = nullptr){//Node Constructor
        state = s;
        parent = p;
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

void expandQueue(queue<Node*>& nodes){
    int temp;
    vector<vector<int>> state = nodes.front()->state;
    if(state[0][0] == 0){   //upper right corner
        temp = state[1][0]; //move blank spacedown
        vector<vector<int>> next_state_1 = state;
        next_state_1[0][0] = temp;
        next_state_1[1][0] = 0;
        Node* child_1 = new Node(next_state_1, nodes.front());
        nodes.push(child_1);
        
        temp = state[0][1]; //move blank space right
        vector<vector<int>> next_state_2 = state;
        next_state_2[0][0] = temp;
        next_state_2[0][1] = 0;
        Node* child_2 = new Node(next_state_2, nodes.front());
        nodes.push(child_2);
    }
    else if(state[0][1] == 0){ //upper middle edge
        //blank can move left, down, or right
    }
    else if(state[0][2] == 0){ //upper right corner
        //blank can move left or down
    }
    else if(state[1][0] == 0){ //middle left edge
        //blank can move up, down, or right
    }
    else if(state[1][1] == 0){ //center
        //blank can move in any of the four directions
    }
    else if(state[1][2] == 0){ // middle right edge
        //blank can move up, down, or left
    }
    else if(state[2][0] == 0){ //bottom left edge
        //blank can move up or right
    }
    else if(state[2][1] == 0){ //bottom center edge
        //blank can move left, up, or right
    }
    else if(state[2][2] == 0){ //bottom right edge
        //blank can move left or up
    }
    
}

//Function uniform cost search (same for all 3 types of searches)
Node* SearchAlgorithm(vector<vector<int>> puzzle, int type){
    queue<Node*> nodes;
    Node* root = new Node(puzzle, nullptr);
    nodes.push(root);
    Node* goal_node = nullptr;
    
    while(goal_node == nullptr){
        if(nodes.empty()){
            return nullptr;
        }
        Node* front = nodes.front();
        if(goalTest(nodes.front()->state)){
            goal_node = nodes.front();
            return goal_node;
        }
        expandQueue(nodes);
        nodes.pop();
        
    }
    return goal_node;
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