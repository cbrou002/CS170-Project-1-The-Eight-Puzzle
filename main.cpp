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
vector<vector<int>> easy_puzzle = {{ 1, 2, 0 },
                                   { 5, 0, 6 },
                                   { 4, 7, 8 }};
vector<vector<int>> doable_puzzle = {{ 1, 3, 6 },
                                     {5, 0, 2 },
                                     { 4, 7, 8 }};
vector<vector<int>> hard_puzzle = {{ 1, 3, 6 },
                                   { 5, 0, 7 },
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
    Node(const vector<vector<int>>& s, Node* p, int depth_){//Node Constructor
        state = s;
        parent = p;
        depth = depth_;
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

void expandQueue(queue<Node*>& nodes, int algorithmType){
    int temp;
    vector<vector<int>> state = nodes.front()->state;
    if(state[0][0] == 0){   //blank in upper left corner
        //moving blank space down
        temp = state[1][0]; 
        vector<vector<int>> next_state_1 = state;
        next_state_1[0][0] = temp;
        next_state_1[1][0] = 0;
        Node* child_1 = new Node(next_state_1, nodes.front(), nodes.front()->depth + 1);
        //moving blank space right
        temp = state[0][1]; 
        vector<vector<int>> next_state_2 = state;
        next_state_2[0][0] = temp;
        next_state_2[0][1] = 0;
        Node* child_2 = new Node(next_state_2, nodes.front(), nodes.front()->depth + 1); 
        switch (algorithmType){
            case 0: // Uniform Cost Search (Uninformed)
                nodes.push(child_1);
                nodes.push(child_2);
                break;
            case 1: // Misplaced Tile Heuristic
                if(getMisplacedTiles(child_1->state) < getMisplacedTiles(child_2->state)){
                    nodes.push(child_1);
                    nodes.push(child_2);
                }
                else{
                    nodes.push(child_2);
                    nodes.push(child_1);
                }
                break;
            case 2: // Manhattan Distance Heuristic
                break;
        }
    }
    else if(state[0][1] == 0){ //blank in upper middle edge
        //moving blank space left
        temp = state[0][0]; 
        vector<vector<int>> next_state_1 = state;
        next_state_1[0][1] = temp;
        next_state_1[0][0] = 0;
        Node* child_1 = new Node(next_state_1, nodes.front(), nodes.front()->depth + 1);
        //moving blank space down
        temp = state[1][1]; 
        vector<vector<int>> next_state_2 = state;
        next_state_2[0][1] = temp;
        next_state_2[1][1] = 0;
        Node* child_2 = new Node(next_state_2, nodes.front(), nodes.front()->depth + 1);
        //moving blank space right
        temp = state[0][2]; 
        vector<vector<int>> next_state_3 = state;
        next_state_3[0][1] = temp;
        next_state_3[0][2] = 0;
        Node* child_3 = new Node(next_state_3, nodes.front(), nodes.front()->depth + 1);
        switch (algorithmType){
            case 0: // Uniform Cost Search (Uninformed)
                nodes.push(child_1);
                nodes.push(child_2);
                nodes.push(child_3);
                break;
            case 1: // Misplaced Tile Heuristic
                //Node* best_node = nullptr;
                int heuristics[3];
                heuristics[0] = getMisplacedTiles(child_1->state);
                heuristics[1] = getMisplacedTiles(child_2->state);
                heuristics[2] = getMisplacedTiles(child_3->state);
                sort(heuristics, heuristics + 3);
                for(int i = 0; i < 3; i++){
                    if(heuristics[i] == getMisplacedTiles(child_1->state)){
                        nodes.push(child_1);
                    }
                    else if(heuristics[i] == getMisplacedTiles(child_2->state)){
                        nodes.push(child_2);
                    }
                    else if(heuristics[i] == getMisplacedTiles(child_3->state)){
                        nodes.push(child_3);
                    }
                }    
                break;
            case 2: // Manhattan Distance Heuristic
                break;
        }
    }
    else if(state[0][2] == 0){ //blank in upper right corner
        //moving blank space left
        temp = state[0][1]; 
        vector<vector<int>> next_state_1 = state;
        next_state_1[0][2] = temp;
        next_state_1[0][1] = 0;
        Node* child_1 = new Node(next_state_1, nodes.front(), nodes.front()->depth + 1);
        //moving blank space down
        temp = state[1][2]; 
        vector<vector<int>> next_state_2 = state;
        next_state_2[0][2] = temp;
        next_state_2[1][2] = 0;
        Node* child_2 = new Node(next_state_2, nodes.front(), nodes.front()->depth + 1);
        switch (algorithmType){
            case 0: // Uniform Cost Search (Uninformed)
                nodes.push(child_1);
                nodes.push(child_2);
                break;
            case 1: // Misplaced Tile Heuristic
                //Node* best_node = nullptr;
                int heuristics[2];
                heuristics[0] = getMisplacedTiles(child_1->state);
                heuristics[1] = getMisplacedTiles(child_2->state);
                sort(heuristics, heuristics + 2);
                for(int i = 0; i < 2; i++){
                    if(heuristics[i] == getMisplacedTiles(child_1->state)){
                        nodes.push(child_1);
                    }
                    else if(heuristics[i] == getMisplacedTiles(child_2->state)){
                        nodes.push(child_2);
                    }
                }    
                break;
            case 2: // Manhattan Distance Heuristic
                break;
        }
    }
    else if(state[1][0] == 0){ //blank in middle left edge
        //moving blank space up
        temp = state[0][0]; 
        vector<vector<int>> next_state_1 = state;
        next_state_1[1][0] = temp;
        next_state_1[0][0] = 0;
        Node* child_1 = new Node(next_state_1, nodes.front(), nodes.front()->depth + 1);
        //moving blank space down
        temp = state[2][0]; 
        vector<vector<int>> next_state_2 = state;
        next_state_2[1][0] = temp;
        next_state_2[2][0] = 0;
        Node* child_2 = new Node(next_state_2, nodes.front(), nodes.front()->depth + 1);
        //moving blank space right
        temp = state[1][1]; 
        vector<vector<int>> next_state_3 = state;
        next_state_3[1][0] = temp;
        next_state_3[1][1] = 0;
        Node* child_3 = new Node(next_state_3, nodes.front(), nodes.front()->depth + 1);
        switch (algorithmType){
            case 0: // Uniform Cost Search (Uninformed)
                nodes.push(child_1);
                nodes.push(child_2);
                nodes.push(child_3);
                break;
            case 1: // Misplaced Tile Heuristic
                int heuristics[3];
                heuristics[0] = getMisplacedTiles(child_1->state);
                heuristics[1] = getMisplacedTiles(child_2->state);
                heuristics[2] = getMisplacedTiles(child_3->state);
                sort(heuristics, heuristics + 3);
                for(int i = 0; i < 3; i++){
                    if(heuristics[i] == getMisplacedTiles(child_1->state)){
                        nodes.push(child_1);
                    }
                    else if(heuristics[i] == getMisplacedTiles(child_2->state)){
                        nodes.push(child_2);
                    }
                    else if(heuristics[i] == getMisplacedTiles(child_3->state)){
                        nodes.push(child_3);
                    }
                }    
                break;
            case 2: // Manhattan Distance Heuristic
                break;
        }
    }
    else if(state[1][1] == 0){ //blank in center
        //moving blank space up
        temp = state[0][1]; 
        vector<vector<int>> next_state_1 = state;
        next_state_1[1][1] = temp;
        next_state_1[0][1] = 0;
        Node* child_1 = new Node(next_state_1, nodes.front(), nodes.front()->depth + 1);
        //moving blank space down
        temp = state[2][1]; 
        vector<vector<int>> next_state_2 = state;
        next_state_2[1][1] = temp;
        next_state_2[2][1] = 0;
        Node* child_2 = new Node(next_state_2, nodes.front(), nodes.front()->depth + 1);
        //moving blank space right
        temp = state[1][2]; 
        vector<vector<int>> next_state_3 = state;
        next_state_3[1][1] = temp;
        next_state_3[1][2] = 0;
        Node* child_3 = new Node(next_state_3, nodes.front(), nodes.front()->depth + 1);
        //moving blank space left
        temp = state[1][0]; 
        vector<vector<int>> next_state_4 = state;
        next_state_4[1][1] = temp;
        next_state_4[1][0] = 0;
        Node* child_4 = new Node(next_state_4, nodes.front(), nodes.front()->depth + 1);
        switch (algorithmType){
            case 0: // Uniform Cost Search (Uninformed)
                nodes.push(child_1);
                nodes.push(child_2);
                nodes.push(child_3);
                nodes.push(child_4);
                break;
            case 1: // Misplaced Tile Heuristic
                //Node* best_node = nullptr;
                int heuristics[4];
                heuristics[0] = getMisplacedTiles(child_1->state);
                heuristics[1] = getMisplacedTiles(child_2->state);
                heuristics[2] = getMisplacedTiles(child_3->state);
                heuristics[3] = getMisplacedTiles(child_4->state);
                sort(heuristics, heuristics + 4);
                for(int i = 0; i < 4; i++){
                    if(heuristics[i] == getMisplacedTiles(child_1->state)){
                        nodes.push(child_1);
                    }
                    else if(heuristics[i] == getMisplacedTiles(child_2->state)){
                        nodes.push(child_2);
                    }
                    else if(heuristics[i] == getMisplacedTiles(child_3->state)){
                        nodes.push(child_3);
                    }
                    else if(heuristics[i] == getMisplacedTiles(child_4->state)){
                        nodes.push(child_4);
                    }
                }    
                break;
            case 2: // Manhattan Distance Heuristic
                break;
        }
    }
    else if(state[1][2] == 0){ // blank in middle right edge
        //moving blank space up
        temp = state[0][2]; 
        vector<vector<int>> next_state_1 = state;
        next_state_1[1][2] = temp;
        next_state_1[0][2] = 0;
        Node* child_1 = new Node(next_state_1, nodes.front(), nodes.front()->depth + 1);
        //moving blank space down
        temp = state[2][2]; 
        vector<vector<int>> next_state_2 = state;
        next_state_2[1][2] = temp;
        next_state_2[2][2] = 0;
        Node* child_2 = new Node(next_state_2, nodes.front(), nodes.front()->depth + 1);
        //moving blank space left
        temp = state[1][1]; 
        vector<vector<int>> next_state_3 = state;
        next_state_3[1][2] = temp;
        next_state_3[1][1] = 0;
        Node* child_3 = new Node(next_state_3, nodes.front(), nodes.front()->depth + 1);
        switch (algorithmType){
            case 0: // Uniform Cost Search (Uninformed)
                nodes.push(child_1);
                nodes.push(child_2);
                nodes.push(child_3);
                break;
            case 1: // Misplaced Tile Heuristic
                //Node* best_node = nullptr;
                int heuristics[3];
                heuristics[0] = getMisplacedTiles(child_1->state);
                heuristics[1] = getMisplacedTiles(child_2->state);
                heuristics[2] = getMisplacedTiles(child_3->state);
                sort(heuristics, heuristics + 3);
                for(int i = 0; i < 3; i++){
                    if(heuristics[i] == getMisplacedTiles(child_1->state)){
                        nodes.push(child_1);
                    }
                    else if(heuristics[i] == getMisplacedTiles(child_2->state)){
                        nodes.push(child_2);
                    }
                    else if(heuristics[i] == getMisplacedTiles(child_3->state)){
                        nodes.push(child_3);
                    }
                }    
                break;
            case 2: // Manhattan Distance Heuristic
                break;
        }
    }
    else if(state[2][0] == 0){ //blank in bottom left edge
        //moving blank space up
        temp = state[1][0]; 
        vector<vector<int>> next_state_1 = state;
        next_state_1[2][0] = temp;
        next_state_1[1][0] = 0;
        Node* child_1 = new Node(next_state_1, nodes.front(), nodes.front()->depth + 1);
        //moving blank space right
        temp = state[2][1]; 
        vector<vector<int>> next_state_2 = state;
        next_state_2[2][0] = temp;
        next_state_2[2][1] = 0;
        Node* child_2 = new Node(next_state_2, nodes.front(), nodes.front()->depth + 1);
        switch (algorithmType){
            case 0: // Uniform Cost Search (Uninformed)
                nodes.push(child_1);
                nodes.push(child_2);
                break;
            case 1: // Misplaced Tile Heuristic
                //Node* best_node = nullptr;
                int heuristics[2];
                heuristics[0] = getMisplacedTiles(child_1->state);
                heuristics[1] = getMisplacedTiles(child_2->state);
                sort(heuristics, heuristics + 2);
                for(int i = 0; i < 2; i++){
                    if(heuristics[i] == getMisplacedTiles(child_1->state)){
                        nodes.push(child_1);
                    }
                    else if(heuristics[i] == getMisplacedTiles(child_2->state)){
                        nodes.push(child_2);
                    }
                }    
                break;
            case 2: // Manhattan Distance Heuristic
                break;
        }
    }
    else if(state[2][1] == 0){ //blank in bottom center edge
        //moving blank space up
        temp = state[1][1]; 
        vector<vector<int>> next_state_1 = state;
        next_state_1[2][1] = temp;
        next_state_1[1][1] = 0;
        Node* child_1 = new Node(next_state_1, nodes.front(), nodes.front()->depth + 1);
        //moving blank space left
        temp = state[2][0]; 
        vector<vector<int>> next_state_2 = state;
        next_state_2[2][1] = temp;
        next_state_2[2][0] = 0;
        Node* child_2 = new Node(next_state_2, nodes.front(), nodes.front()->depth + 1);
        //moving blank space right
        temp = state[2][2]; 
        vector<vector<int>> next_state_3 = state;
        next_state_3[2][1] = temp;
        next_state_3[2][2] = 0;
        Node* child_3 = new Node(next_state_3, nodes.front(), nodes.front()->depth + 1);
        switch (algorithmType){
            case 0: // Uniform Cost Search (Uninformed)
                nodes.push(child_1);
                nodes.push(child_2);
                nodes.push(child_3);
                break;
            case 1: // Misplaced Tile Heuristic
                //Node* best_node = nullptr;
                int heuristics[3];
                heuristics[0] = getMisplacedTiles(child_1->state);
                heuristics[1] = getMisplacedTiles(child_2->state);
                heuristics[2] = getMisplacedTiles(child_3->state);
                sort(heuristics, heuristics + 3);
                for(int i = 0; i < 3; i++){
                    if(heuristics[i] == getMisplacedTiles(child_1->state)){
                        nodes.push(child_1);
                    }
                    else if(heuristics[i] == getMisplacedTiles(child_2->state)){
                        nodes.push(child_2);
                    }
                    else if(heuristics[i] == getMisplacedTiles(child_3->state)){
                        nodes.push(child_3);
                    }
                }    
                break;
            case 2: // Manhattan Distance Heuristic
                break;
        }
    }
    else if(state[2][2] == 0){ //blank in bottom right edge
        //moving blank space left
        temp = state[2][1]; 
        vector<vector<int>> next_state_1 = state;
        next_state_1[2][2] = temp;
        next_state_1[2][1] = 0;
        Node* child_1 = new Node(next_state_1, nodes.front(), nodes.front()->depth + 1);
        //moving blank space up
        temp = state[1][2]; 
        vector<vector<int>> next_state_2 = state;
        next_state_2[2][2] = temp;
        next_state_2[1][2] = 0;
        Node* child_2 = new Node(next_state_2, nodes.front(), nodes.front()->depth + 1);
        switch (algorithmType){
            case 0: // Uniform Cost Search (Uninformed)
                nodes.push(child_1);
                nodes.push(child_2);
                break;
            case 1: // Misplaced Tile Heuristic
                //Node* best_node = nullptr;
                int heuristics[2];
                heuristics[0] = getMisplacedTiles(child_1->state);
                heuristics[1] = getMisplacedTiles(child_2->state);
                sort(heuristics, heuristics + 2);
                for(int i = 0; i < 2; i++){
                    if(heuristics[i] == getMisplacedTiles(child_1->state)){
                        nodes.push(child_1);
                    }
                    else if(heuristics[i] == getMisplacedTiles(child_2->state)){
                        nodes.push(child_2);
                    }
                }    
                break;
            case 2: // Manhattan Distance Heuristic
                break;
        }
    }
    
}

//Function uniform cost search (same for all 3 types of searches)
Node* SearchAlgorithm(vector<vector<int>> puzzle, int algorithmType){
    queue<Node*> nodes;
    Node* root = new Node(puzzle);
    nodes.push(root);
    Node* goal_node = nullptr;
    
    while(!nodes.empty()){
        Node* front = nodes.front();
        if(goalTest(nodes.front()->state)){
            goal_node = nodes.front();
            printPuzzle(goal_node->state);
            cout << "Solution Depth: " << goal_node->depth;
            return goal_node;
        }
        expandQueue(nodes, algorithmType);
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
            cout << "Impossible difficulty selected.\n";
            user_puzzle = impossible_puzzle;
        }
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