#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

vector<vector<int>> trivial_puzzle = {{ 1, 2, 3 },
                                      { 4, 5, 6 },
                                      { 7, 8, 0 }};
vector<vector<int>> very_easy_puzzle = {{ 1, 2, 3 },
                                        { 4, 5, 6 },
                                        { 0, 7, 8 }};
vector<vector<int>> easy_puzzle = {{ 1, 2, 3 },
                                   { 5, 0, 6 },
                                   { 4, 7, 8 }};
vector<vector<int>> doable_puzzle = {{ 1, 3, 6 },
                                     {5, 0, 2 },
                                     { 4, 7, 8 }};
vector<vector<int>> hard_puzzle = {{ 1, 3, 6 },
                                   { 5, 0, 7 },
                                   { 4, 8, 2 }};
vector<vector<int>> very_hard_puzzle = {{ 1, 6, 7 },
                                        { 5, 0, 3 },
                                        { 4, 8, 2 }};                                   
vector<vector<int>> impossible_puzzle = {{ 8, 6, 7 },
                                         { 2, 5, 4 },
                                         { 3, 0, 1 }};

struct Node{ //node structure
    vector<vector<int>> state; //current state of the puzzle
    Node* parent;//previous state of the puzzle
    int depth;
    Node(const vector<vector<int>>& s){//Node Constructor
        state = s;
        parent = nullptr;
        depth = 0;
    }
    Node(const vector<vector<int>>& s, Node* p){//Node Constructor
        state = s;
        parent = p;
        depth = 0;
    }
    Node(const vector<vector<int>>& s, Node* p, int d){//Node Constructor
        state = s;
        parent = p;
        depth = d;
    }
};

int getMisplacedTiles(vector<vector<int>> state){
    int num_misplaced_tiles = 0;
    for(int i = 0; i < 3; i++){
        if(state[0][i] != i+1){
            num_misplaced_tiles++;
        }
    }
    for(int i = 0; i < 3; i++){
        if(state[1][i] != i+4){
            num_misplaced_tiles++;
        }
    }
    if(state[2][0] != 7){
        num_misplaced_tiles++;
    }
    if(state[2][1] != 8){
        num_misplaced_tiles++;
    }
    return num_misplaced_tiles;
}

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

struct CompareNodes {
    bool operator()(Node* a, Node* b) {
        return (a->depth + getMisplacedTiles(a->state)) > (b->depth + getMisplacedTiles(b->state)); // Min-heap
    }
};

void pushChildren(priority_queue<Node*, vector<Node*>, CompareNodes>& nodes, Node* child_1, Node* child_2, Node* child_3, Node* child_4){
    nodes.push(child_1);
    nodes.push(child_2);
    if(child_3){
        nodes.push(child_3);
    }
    if(child_4){
        nodes.push(child_4);
    }
}
void pushChildren(priority_queue<Node*, vector<Node*>, CompareNodes>& nodes, vector<Node*> children){
    for (Node* child : children) {
        nodes.push(child);
    }
}

void expandQueue(priority_queue<Node*, vector<Node*>, CompareNodes>& nodes, int algorithmType){
    vector<vector<int>> state = nodes.top()->state;
    int row = -1, col = -1;

    // Find where blank space (0) is
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (state[i][j] == 0) {
                row = i;
                col = j;
                break;
            }
        }
    }

    // Possible moves: {row_change, col_change}
    vector<pair<int, int>> moves = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    vector<Node*> children;

    for (const auto& move : moves) {
        int newRow = row + move.first;
        int newCol = col + move.second;

        if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3) {// Check if the move is within bounds of vector
            vector<vector<int>> next_state = state;
            swap(next_state[row][col], next_state[newRow][newCol]); // Move blank
            Node* child = new Node(next_state, nodes.top(), nodes.top()->depth + 1);
            children.push_back(child);
        }
    }

    // Push nodes based on the algorithm type
    if (algorithmType == 0) { // Uniform Cost Search
        for (Node* child : children) {
            nodes.push(child);
        }
    } 
    else if (algorithmType == 1) { // Misplaced Tile Heuristic
        pushChildren(nodes, children);
    } 
    else if (algorithmType == 2) { // Manhattan Distance Heuristic
        // Implement heuristic-based priority logic here
    }
}

//Function uniform cost search (same for all 3 types of searches)
Node* SearchAlgorithm(vector<vector<int>> puzzle, int algorithmType){
    //queue<Node*> nodes;
    priority_queue<Node*, vector<Node*>, CompareNodes> nodes;
    Node* root = new Node(puzzle);
    nodes.push(root);
    Node* goal_node = nullptr;
    
    while(!nodes.empty()){
        if(goalTest(nodes.top()->state)){
            goal_node = nodes.top();
            printPuzzle(goal_node->state);
            cout << "Solution Depth: " << goal_node->depth << "\n";
            return goal_node;
        }
        expandQueue(nodes, algorithmType);
        nodes.pop();
        cout << "The best state to expand with a g(n) = " << nodes.top()->depth << " and h(n) = " << getMisplacedTiles(nodes.top()->state) << " is...\n";
        printPuzzle(nodes.top()->state);
        cout << endl;
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
        while (difficulty < 0 || difficulty > 6){//checks for valid input
            cout << "Please enter a desired difficulty on a scale from 0 to 6.\n";
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
        if (difficulty == 5){
            cout << "Very hard difficulty selected.\n";
            user_puzzle = very_hard_puzzle;
        }  
        if(difficulty == 6){
            cout << "Impossible difficulty selected.\n";
            user_puzzle = impossible_puzzle;
        }
        cout << "Your default puzzle:\n";
        printPuzzle(user_puzzle);
        cout << endl;
    }
    else if(puzzle_mode == 2){ /////////////////////////////////////////// CREATE CUSTOM PUZZLE
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
    }
    ///////////////////////////////////////////////////PICKING ALGORITHM
    temp = -1;
    while(temp != 1 && temp != 2 && temp != 3){//checks for valid input
        cout << "Select algorithm. (1) for Uniform Cost Search, (2) for the Misplaced Tile Heuristic, or (3) the Manhattan Distance Heuristic.\n";
        cin >> temp;
    }
    Node* goal_state_node = nullptr;
    if(temp == 1){ //Uniform Cost Search
        goal_state_node = SearchAlgorithm(user_puzzle, 0);
    }
    else if(temp == 2){ //Misplaced Tile Heuristic
        goal_state_node = SearchAlgorithm(user_puzzle, 1);
    }
    else if(temp == 3){ //Manhattan Distance Heuristic
        goal_state_node = SearchAlgorithm(user_puzzle, 2);
    }
    return 0;
}