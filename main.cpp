#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

vector<vector<int>> depth_0_puzzle = {{ 1, 2, 3 },//Depth 0 puzzle
                                      { 4, 5, 6 },
                                      { 7, 8, 0 }};
vector<vector<int>> depth_2_puzzle = {{ 1, 2, 3 },//depth 2 puzzle
                                        { 4, 5, 6 },
                                        { 0, 7, 8 }};
vector<vector<int>> depth_4_puzzle = {{ 1, 2, 3 },//depth 4 puzzle
                                   { 5, 0, 6 },
                                   { 4, 7, 8 }};
vector<vector<int>> depth_8_puzzle = {{ 1, 3, 6 },//depth 8 puzzle
                                     {5, 0, 2 },
                                     { 4, 7, 8 }};
vector<vector<int>> depth_12_puzzle = {{ 1, 3, 6 },//depth 12 puzzle
                                   { 5, 0, 7 },
                                   { 4, 8, 2 }};
vector<vector<int>> depth_16_puzzle = {{ 1, 6, 7 },//depth 16 puzzle
                                        { 5, 0, 3 },
                                        { 4, 8, 2 }};
vector<vector<int>> depth_20_puzzle = {{ 7, 1, 2 },//depth 20 puzzle added
                                        { 4, 8, 5 },
                                        { 6, 3, 0 }};                                                                
vector<vector<int>> depth_31_puzzle_1 = {{ 8, 6, 7 },//depth 31
                                         { 2, 5, 4 },
                                         { 3, 0, 1 }};
vector<vector<int>> depth_31_puzzle_2 = {{ 6, 4, 7 },//depth 31 added
                                         { 8, 5, 0 },
                                         { 3, 2, 1 }};

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

int getManhattanDistance(vector<vector<int>> state){
    int distance = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            int value = state[i][j];
            if(value != 0){
                int correctRow = (value - 1) / 3;
                int correctCol = (value - 1) % 3;
                distance += abs(i - correctRow) + abs(j - correctCol);
            }
        }
    }
    return distance;
}

bool goalTest(vector<vector<int>> state){
    if(state == depth_0_puzzle){
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
        return (a->depth + getMisplacedTiles(a->state)) > (b->depth + getMisplacedTiles(b->state));
    }
};

struct CompareNodesManhattanDistance {
    bool operator()(Node* a, Node* b) {
        return (a->depth + getManhattanDistance(a->state)) > (b->depth + getManhattanDistance(b->state)); 
    }
};

void expandQueueMisplaced(priority_queue<Node*, vector<Node*>, CompareNodes>& nodes, int algorithmType){
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
    for (Node* child : children) {
            nodes.push(child);
        }
}
void expandQueueManhattan(priority_queue<Node*, vector<Node*>, CompareNodesManhattanDistance>& nodes, int algorithmType){
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
    for (Node* child : children) {
        nodes.push(child);
    }
}

//Function uniform cost search (same for all 3 types of searches)
Node* SearchAlgorithm(vector<vector<int>> puzzle, int algorithmType){
    priority_queue<Node*, vector<Node*>, CompareNodes> nodes;
    priority_queue<Node*, vector<Node*>, CompareNodesManhattanDistance> nodes2;
    Node* goal_node = nullptr;
    Node* root = new Node(puzzle);
    if(algorithmType == 0 || algorithmType == 1){
        nodes.push(root);
        while(!nodes.empty()){
            if(goalTest(nodes.top()->state)){
                goal_node = nodes.top();
                printPuzzle(goal_node->state);
                cout << "Solution Depth: " << goal_node->depth << "\n";
                return goal_node;
            }
            expandQueueMisplaced(nodes, algorithmType);
            //check for algorithm type to use the right expandQueueMisplaced()?
            nodes.pop();
            cout << "The best state to expand with a g(n) = " << nodes.top()->depth << " and h(n) = " << getMisplacedTiles(nodes.top()->state) << " is...\n";
            printPuzzle(nodes.top()->state);
            cout << endl;
        }
    }
    else if(algorithmType == 2){
        nodes2.push(root);
        while(!nodes2.empty()){
        if(goalTest(nodes2.top()->state)){
            goal_node = nodes2.top();
            printPuzzle(goal_node->state);
            cout << "Solution Depth: " << goal_node->depth << "\n";
            return goal_node;
        }
        expandQueueManhattan(nodes2, algorithmType);
        //check for algorithm type to use the right expandQueueMisplaced()?
        nodes2.pop();
        cout << "The best state to expand with a g(n) = " << nodes2.top()->depth << " and h(n) = " << getMisplacedTiles(nodes2.top()->state) << " is...\n";
        printPuzzle(nodes2.top()->state);
        cout << endl;
        }
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
            cout << "Depth 0 puzzle selected. \n";
            user_puzzle = depth_0_puzzle;
        }
        if (difficulty == 1){
            cout << "Depth 2 puzzle selected. \n";
            user_puzzle = depth_2_puzzle;
        }   
        if (difficulty == 2){
            cout << "Depth 4 puzzleselected.\n";
            user_puzzle = depth_4_puzzle;
        }
        if (difficulty == 3){
            cout << "Depth 8 puzzle selected.\n";
            user_puzzle = depth_8_puzzle;
        }
        if (difficulty == 4){
            cout << "Depth 12 puzzle selected.\n";
            user_puzzle = depth_12_puzzle;
        }  
        if (difficulty == 5){
            cout << "Depth 16 puzzle selected.\n";
            user_puzzle = depth_16_puzzle;
        }  
        if(difficulty == 6){
            cout << "Impossible difficulty selected.\n";
            user_puzzle = depth_31_puzzle_1;
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