#include <iostream>
#include <vector>
#include <queue>


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
vector<vector<int>> depth_20_puzzle = {{ 7, 1, 2 },//depth 20 puzzle 
                                        { 4, 8, 5 },
                                        { 6, 3, 0 }};
vector<vector<int>> depth_24_puzzle = {{ 0, 7, 2 },//depth 24 puzzle 
                                        { 4, 6, 1 },
                                        { 3, 5, 8 }};                                                                                                        
vector<vector<int>> depth_31_puzzle_1 = {{ 8, 6, 7 },//depth 31 puzzle
                                         { 2, 5, 4 },
                                         { 3, 0, 1 }};
vector<vector<int>> depth_31_puzzle_2 = {{ 6, 4, 7 },//depth 31 puzzle added
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
        return (a->depth) > (b->depth);
    }
};
struct CompareNodesMisplacedTiles {
    bool operator()(Node* a, Node* b) {
        return (a->depth + getMisplacedTiles(a->state)) > (b->depth + getMisplacedTiles(b->state));
    }
};
struct CompareNodesManhattanDistance {
    bool operator()(Node* a, Node* b) {
        return (a->depth + getManhattanDistance(a->state)) > (b->depth + getManhattanDistance(b->state)); 
    }
};

template <typename Compare>
void expandQueueMisplaced(priority_queue<Node*, vector<Node*>, Compare>& nodes, int algorithmType){
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
    priority_queue<Node*, vector<Node*>, CompareNodes> uniform_cost_nodes;
    priority_queue<Node*, vector<Node*>, CompareNodesMisplacedTiles> misplaced_nodes;
    priority_queue<Node*, vector<Node*>, CompareNodesManhattanDistance> manhattan_nodes;
    Node* goal_node = nullptr;
    Node* root = new Node(puzzle);
    int max_queue_size = 0;
    long long nodes_expanded = 0;

    if(algorithmType == 0){
        uniform_cost_nodes.push(root);
        while(!uniform_cost_nodes.empty()){
            if(goalTest(uniform_cost_nodes.top()->state)){
                goal_node = uniform_cost_nodes.top();
                printPuzzle(goal_node->state);
                cout << "Solution depth was " << goal_node->depth << "\n";
                cout << "Number of nodes expanded: " << nodes_expanded << "\n";
                cout << "Max queue size: " << max_queue_size << "\n";
                return goal_node;
            }
            expandQueueMisplaced(uniform_cost_nodes, algorithmType);
            cout << "The best state to expand with a g(n) = " << uniform_cost_nodes.top()->depth << " and h(n) = " << getMisplacedTiles(uniform_cost_nodes.top()->state) << " is...\n";
            if(uniform_cost_nodes.size() >= max_queue_size){
                max_queue_size = uniform_cost_nodes.size();
            }
            uniform_cost_nodes.pop();
            nodes_expanded++;
            printPuzzle(uniform_cost_nodes.top()->state);
            cout << endl;
        }
    }
    else if(algorithmType == 1){
        misplaced_nodes.push(root);
        while(!misplaced_nodes.empty()){
            if(goalTest(misplaced_nodes.top()->state)){
                goal_node = misplaced_nodes.top();
                printPuzzle(goal_node->state);
                cout << "Solution depth was " << goal_node->depth << "\n";
                cout << "Number of nodes expanded: " << nodes_expanded << "\n";
                cout << "Max queue size: " << max_queue_size << "\n";
                return goal_node;
            }
            expandQueueMisplaced(misplaced_nodes, algorithmType);
            cout << "The best state to expand with a g(n) = " << misplaced_nodes.top()->depth << " and h(n) = " << getMisplacedTiles(misplaced_nodes.top()->state) << " is...\n";
            if(misplaced_nodes.size() >= max_queue_size){
                max_queue_size = misplaced_nodes.size();
            }
            misplaced_nodes.pop();
            nodes_expanded++;
            printPuzzle(misplaced_nodes.top()->state);
            cout << endl;
        }
    }
    else if(algorithmType == 2){
        manhattan_nodes.push(root);
        while(!manhattan_nodes.empty()){
            if(goalTest(manhattan_nodes.top()->state)){
                goal_node = manhattan_nodes.top();
                printPuzzle(goal_node->state);
                cout << "Solution Depth: " << goal_node->depth << "\n";
                cout << "Nodes Expanded: " << nodes_expanded << "\n";
                cout << "Max queue size: " << max_queue_size << "\n";
                return goal_node;
            }
            expandQueueMisplaced(manhattan_nodes, algorithmType);
            cout << "The best state to expand with a g(n) = " << manhattan_nodes.top()->depth << " and h(n) = " << getMisplacedTiles(manhattan_nodes.top()->state) << " is...\n";
            if(manhattan_nodes.size() >= max_queue_size){
                max_queue_size = manhattan_nodes.size();
            }
            manhattan_nodes.pop();
            nodes_expanded++;
            
            printPuzzle(manhattan_nodes.top()->state);
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
        while (difficulty < 0 || difficulty > 9){//checks for valid input
            cout << "Please enter a desired difficulty on a scale from 0 to 9.\n";
            cin >> difficulty;
        }
        if (difficulty == 0){
            cout << "Depth 0 puzzle selected. \n";
            user_puzzle = depth_0_puzzle;
        }
        else if (difficulty == 1){
            cout << "Depth 2 puzzle selected. \n";
            user_puzzle = depth_2_puzzle;
        }   
        else if (difficulty == 2){
            cout << "Depth 4 puzzle selected.\n";
            user_puzzle = depth_4_puzzle;
        }
        else if (difficulty == 3){
            cout << "Depth 8 puzzle selected.\n";
            user_puzzle = depth_8_puzzle;
        }
        else if (difficulty == 4){
            cout << "Depth 12 puzzle selected.\n";
            user_puzzle = depth_12_puzzle;
        }  
        else if (difficulty == 5){
            cout << "Depth 16 puzzle selected.\n";
            user_puzzle = depth_16_puzzle;
        }  
        else if (difficulty == 6){
            cout << "Depth 20 puzzle selected.\n";
            user_puzzle = depth_20_puzzle;
        }
        else if (difficulty == 7){
            cout << "Depth 24 puzzle selected.\n";
            user_puzzle = depth_24_puzzle;
        }
        else if(difficulty == 8){
            cout << "Depth 31 option 1 selected.\n";
            user_puzzle = depth_31_puzzle_1;
        }
        else if(difficulty == 9){
            cout << "Depth 31 puzzle option 2 selected.\n";
            user_puzzle = depth_31_puzzle_2;
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
        cout << endl;
        cout << "Enter the second row (separated by spaces and use 0 to represent the blank space), then hit enter\n";
        for(int i = 0; i < 3; i++){
            cin >> temp;
            user_puzzle[1][i] = temp;
        }
        cout << endl;
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