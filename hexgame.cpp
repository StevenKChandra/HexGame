// HexGame Code
// by Steven Kurniawan Chandra
// 27 09 23

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <random>
#include <queue>
#include <tuple>

using namespace std;

// Hex Board Class
typedef class HexBoard{
    public:
    random_device rd = std::random_device (); 
    default_random_engine rng = std::default_random_engine (rd());
    // constructor, it ask for the Board Size when called
    HexBoard(void){
        int n;
        while (n!=5 && n!=7 && n!=9 && n!=11) {
            cout<<"Please enter Board Size (5,7,9, or 11): ";
            cin>>n;
            if (n!=5 && n!=7 && n!=9 && n!=11) {
                cout<<"Invalid Board Size"<<endl;
            }
        }
        
        string Ai;
        while (Ai != "y" && Ai != "n") {
            cout<<"Play againts the AI(y/n): ";
            cin>>Ai;
            if (Ai != "y" && Ai != "n") {
                cout<<"Invalid answer"<<endl;
            }
        }
        
        // Set the class attributes
        this->TurnNumber = 0;
        this->BoardSize = n;
        this->Board = vector<vector<int>> (n, vector<int> (n));
        this->PlayAgaintsAi = (Ai=="y")?true:false;
        PrintBoard();
    }

    // function to make a move, takes x and y integer as input
    bool MakeMove(int x, int y) {
        int n = this->BoardSize;
        
        // check wheter the move is valid or not, x, y have to be between [1, n] and node x,y must be empty
        if (x>=1 && x<=n && y>=1 && y<=n && this->Board[(y-1)][(x-1)] == 0) {

            // mark the x,y node with respective player's mark
            this->Board[(y-1)][(x-1)] = this->TurnNumber++%2 + 1;
            
            // after a move, print the board
            PrintBoard();

            if (Win(this->Board, (this->TurnNumber-1)%2+1) == 1){
                    cout<<"Player 1 wins"<<endl;
                    return true;
                }

            // if playing againts AI
            if (this->PlayAgaintsAi){

                // let the AI move
                this -> AIMove();

                // after the AI move, print the board
                PrintBoard();

                // check wheter there is a winner or not, if there is, print the winner
                if (Win(this->Board, (this->TurnNumber-1)%2+1) == 2){
                    cout<<"The AI wins"<<endl;
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                // check wheter there is a winner or not, if there is, print the winner
                if (Win(this->Board, (this->TurnNumber-1)%2+1) == 2){
                    cout<<"Player the 2 wins"<<endl;
                    return true;
                }
                else{
                    return false;
                }
            }
        }

        // if move is not valid, tell the player
        else{
            cout<<"Invalid Move"<<endl;
            return false;
        }
    }

    // function that return 1 if this is first player turn and 2 otherwise
    int PlayerTurn(void){
        return (this->TurnNumber)%2+1;
    }

    private:
    // Board attributes
    int TurnNumber;
    int BoardSize;
    vector<vector <int>> Board;
    bool PlayAgaintsAi;

    // routine to print the board
    void PrintBoard(void){
        int n = this->BoardSize;
        vector<vector <int>> Board = this->Board;
        string WhiteSpace;

        // print the header of the board
        for (int i=0; i < n; i++){
            WhiteSpace.assign("   ",0 , (3-(i+2)/10));
            cout<<(i+1)<<WhiteSpace;
        }
        cout<<endl;

        // print the rest of the board
        string Slashes;
        Slashes.assign("\\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ ", 0, 4*(n-1)+2);
        for (int i=0; i < n; i++){
            WhiteSpace.assign("                     ", 0 , 2*i);
            cout<<WhiteSpace;
            for (int j=0; j < n; j++){
                if (Board[i][j] == 0){
                    cout<<".";
                }
                else if (Board[i][j] == 1){
                    cout<<"X";
                }
                else{
                    cout<<"O";
                }
                if (j!=(n-1)){
                    cout<<" - ";
                }
            }
            cout<<" "<<(i+1);
            if (i!=(n-1)){
                WhiteSpace.assign("                     ", 0 , 2*i+1);
                cout<<endl<<WhiteSpace<<Slashes<<endl;
            }
        }
        cout<<endl;
    }

        // the win function using dijkstra algorithm with slight modifivation
        // it there is a winner, it returns the player winning 1/2, if there is no winner it returns 0
        int Win(const vector<vector <int>> Board, int Player){
            int n = this ->BoardSize;

            // create a priority queue that accepts tuple of (distance, vertex) with distance as the key
            // this priority queue will act as our Open Set
            auto comparator_function = [] (tuple<int,tuple<int,int>> left, tuple<int,tuple<int,int>> right) {return get<0>(left) > get<0>(right);}; // comparator function to determine the priority of the key
            priority_queue<tuple<int,tuple<int,int>>, vector<tuple<int,tuple<int,int>>>, decltype(comparator_function)> OpenSet(comparator_function); // the call priority queue from the library template

            // create a vector that keeps track of "visited" vertex, aka vertex that the shortest distance has been found
            // this will act as our Closed Set
            vector <tuple<int, int>> ClosedSet;

            // current is the placeholder for current intermediate vertex
            tuple<int, int> current;
            tuple<int, int> next = {0,0};

            // to initialize the algorithm, push the wall (denoted as node -1,-1) into the queue
            // the distance here is equal -i, this way the queue will prioritize node closer to the destination
            OpenSet.push({1, {-1,-1}});

            // if we are checking if player 2 is winning, then perform transpose operation on the board
            vector<vector <int>> EvaluatedBoard (n, vector<int> (n));
            if (Player == 2){
                for (int i = 0; i<n; i++){
                    for (int j = 0; j<n; j++){
                        EvaluatedBoard[j][i] = Board[i][j];
                    }
                }
            }
            else{
                EvaluatedBoard = Board;
            }

            // keep running this until the priority queue is empty or destination is reached
            // destination is denoted as node (n, n)
            while (!OpenSet.empty() && next != tuple<int,int> {n,n}){

                // extract the closest vertex from the queue, set the vertex found as our intermediate vertex
                current = get<1>(OpenSet.top());
                OpenSet.pop();
                
                // check if the current intermediate vertex has been visited before
                if (find(ClosedSet.begin(), ClosedSet.end(), current) == ClosedSet.end()){

                    // set current intermediate vertex as a visited vertex
                    ClosedSet.push_back(current);

                    vector <tuple<int, int>> Neighbors = FindNeighbors(current, n);

                    // this is the relaxation routine
                    for (int i = 0; i < Neighbors.size(); i++){
                        
                        next = Neighbors[i];
                        // if the new distance is shorter compared to old shortest distance
                        if (next != tuple<int,int> {n,n} && EvaluatedBoard[get<0>(next)][get<1>(next)] == Player){

                            // push the neighbor to the queue, with distance equal to -i
                            OpenSet.push({-get<0>(next), next});
                        }
                    }
                }
            }
            
            // if destination is found
            if (next == tuple<int,int> {n,n}){
                return Player;
            }
            // if not
            else{
                return 0;
            }
        }

    // function to call the AI and play a move
    void AIMove(void){
        
        cout<<"AI is thinking..."<<endl;
        // the idea is evaluating every possible next move
        // we do this by doing monte carlo experiment 1000 times on each possible move,
        // recording the number of win when we fill the rest of the board randomly

        // max_i and max_j are placeholder for the move (i, j) that wins the game for the AI
        // win_count are the win_count of the current candidate move
        // max_wincount is the maximum win_count from all the candidate move
        int max_i, max_j, win_count, max_wincount = -1, n = this -> BoardSize;

        vector <vector<int>> CurrentBoard = this->Board, MoveTestBoard, FilledBoard;

        // for all possible move
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){

                // if the node is still empty
                if (CurrentBoard[i][j] == 0){

                    // copy the current board
                    MoveTestBoard = Board;

                    // make a move by marking the node
                    MoveTestBoard[i][j] = 2;

                    // evaluate the move
                    win_count = 0;
                    for (int k = 0; k<1000; k++){
                        
                        // fill the rest of the board randomly
                        FilledBoard = FillBoard(MoveTestBoard);

                        // check if the AI win in the final position
                        if (Win(FilledBoard, 2) == 2){

                            // tally the win
                            win_count++;
                        }
                    }

                    // if this move candidate scores higher win then previous best move
                    if (win_count > max_wincount){

                        // use this this candidate move as the current best move
                        max_wincount = win_count;
                        max_i = i;
                        max_j = j;
                    }
                }
            }
        }
        
        // play the best move and increase the turn counter
        this->Board[max_i][max_j] = this->TurnNumber++%2 + 1;
    }

    // function that takes a non empty board and fill it with random node and return full board
    vector <vector<int>> FillBoard(const vector <vector<int>> InputBoard){
        int n = this-> BoardSize;

        // count the empty nodes
        int EmptyNodeCount = n*n - 1 - this->TurnNumber;

        // board filler is a placeholder of player one's mark and player two's mark used to fill the empty nodes
        // since the number of empty nodes must be even on board evaluation, the number of player one's mark will be one more than player's two mark
        vector <int> BoardFiller(EmptyNodeCount, 2);
        for (int i = EmptyNodeCount/2; i < EmptyNodeCount; i++){
            BoardFiller[i] = 1;
        }
        
        // shuffle the marks
        shuffle(BoardFiller.begin(), BoardFiller.end(), rng);

        // put the marks inside the board
        vector <vector<int>> FilledBoard = InputBoard;
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                
                // if node is empty
                if (FilledBoard [i][j] == 0){

                    // pop the first mark in the board filler and put it on the node
                    FilledBoard [i][j] = *BoardFiller.begin();
                    BoardFiller.erase(BoardFiller.begin());
                }
            }
        }

        // return the full board
        return FilledBoard;
    }
    vector<tuple<int,int>> FindNeighbors(tuple<int,int> node, int n){
        int i = get<0>(node), j = get<1>(node);
        vector<tuple<int,int>> Neighbors;
        if (i == -1){
            for (int y = 0; y < n; y++){
                Neighbors.push_back({0, y});
            }
        }
        else if (i == (n-1)){
            Neighbors.push_back({n, n});
        }
        else if (i == 0){
            if (j == 0){
                Neighbors.push_back({i+1, j}); Neighbors.push_back({i, j+1});
            }
            else if (j == (n-1)){
                Neighbors.push_back({i, j-1}); Neighbors.push_back({i+1, j-1}); Neighbors.push_back({i+1, j});
            }
            else{
                Neighbors.push_back({i, j-1}); Neighbors.push_back({i, j+1}); Neighbors.push_back({i+1, j-1}); Neighbors.push_back({i+1, j});
            }
        }
        else {
            if (j == 0){
                Neighbors.push_back({i-1, j}); Neighbors.push_back({i-1, j+1}); Neighbors.push_back({i, j+1}); Neighbors.push_back({i+1, j});
            }
            else if (j == (n-1)){
                Neighbors.push_back({i-1, j}); Neighbors.push_back({i, j-1}); Neighbors.push_back({i+1, j-1}); Neighbors.push_back({i+1, j});
            }
            else{
                Neighbors.push_back({i-1, j}); Neighbors.push_back({i-1, j+1}); Neighbors.push_back({i, j-1}); Neighbors.push_back({i, j+1}); Neighbors.push_back({i+1, j-1}); Neighbors.push_back({i+1, j});
            }
        }
        return Neighbors;
    }
} HexBoard;

int main(void){
    // create the boardgame
    HexBoard Board = HexBoard();

    // create string stream to receive input
    stringstream ss;
    string s;
    char ch;
    
    // GameOver is placeholder to deterimine if there is a winner or not
    int GameOver = 0;
    
    // x and y are placeholders for the input
    int x, y;

    // while there is no winner
    while (!GameOver){

        // ask for input from the user
        if (Board.PlayerTurn()==1){
            cout<<"Player 1 turn: "<<endl;
        }
        else{
            cout<<"Player 2 turn: "<<endl;
        }
        cout<<"enter x, y: ";
        cin>>s;

        // process the input into integer
        ss= stringstream(s);
        ss>>x;
        ss>>ch;
        ss>>y;

        // feed the integer as move
        GameOver = Board.MakeMove(x,y);
    }
    cout<<"Thanks for playing. Press any key to exit."<<endl;
    system("pause");
    return 0;
}