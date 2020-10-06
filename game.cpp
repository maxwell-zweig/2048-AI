
#include "game.h"
using namespace std;
random_device rd;
default_random_engine generator(rd());
Game::Game() {
    for (int i = 0; i != 2; i++) {
        insert_random();
    }
}
//Used in initializing the board. The first pieces can either by 2 or 4, most likely a 2.
int Game::val_random() {
    uniform_int_distribution<int> int_distribution(0, 9);
    return(int_distribution(generator)%10);
}
//Picks a random index between 0 and the number of spaces. Used to randomly place new pieces.
int Game::index_random(int x) {
    uniform_int_distribution<int>int_distribution(0, x);
    return(int_distribution(generator));
}
void Game::insert_random() {
    int count = 0;
    int indices[2][16] = { {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1} };
    for (int i = 0; i != 4; i++) {
        for (int j = 0; j != 4; j++) {
            if (board[i][j] == 0) {
                indices[0][count]=i;
                indices[1][count]=j;
                count++;
            }
        }
    }
    if (count==0)
        return;
    int val = val_random();
    int index = index_random(count - 1);
    int row = indices[0][index];
    int col = indices[1][index];
    board[row][col] = 2;
    if (val==0)
        board[row][col] = 4;
}
//Moves a slice of the board. A board slice and whether or not the slice of the board is being moved left or right.
void Game::move_arr(int arr[4], int x) {
    int filtered[4] = { 0,0,0,0 };
    if (x == 0) {
        bool is_zero = true;
        for (int i = 0; i != 4; i++) {
            if (arr[i] != 0) 
                is_zero = false;
        }
        if (is_zero == true)
            return;
        int count = 0;
        for (int i = 0; i != 4; i++) {
            int x = arr[i];
            if (x != 0) {
                filtered[count] = x;
                count++;
            }
        }
        for (int i = 0; i != 3; i++) {
            int x = filtered[i];
            if (x == filtered[i + 1]) {
                filtered[i] = x * 2;
                filtered[i + 1] = 0;
                score += x * 2;
            }
        }
        count = 0;
        for (int i = 0; i != 4; i++) {
            arr[i] = 0;
            int x = filtered[i];
            if (x != 0) {
                arr[count] = x;
                count++;
            }
        }
    }
    if (x == 1) {
        bool is_zero = true;
        for (int i = 0; i != 4; i++) {
            if (arr[i] != 0)
                is_zero = false;
        }
        if (is_zero == true)
            return;
        int count = 0;
        for (int i = 3; i != -1; i--) {
            int x = arr[i];
            if (x != 0) {
                filtered[3-count] = x;
                count++;
            }
        }
        for (int i = 3; i != -1; i--) {
            int x = filtered[i];
            if (x == filtered[i -1]) {
                filtered[i] = x * 2;
                filtered[i - 1] = 0;
                score += x * 2;
            }
        }
        count = 0;
        for (int i = 3; i != -1; i--) {
            arr[i] = 0;
            int x = filtered[i];
            if (x != 0) {
                arr[3-count] = x;
                count++;
            }
        }
    }
}
void* Game::clean_transpose(int board[4][4]) {
    int(*a)[4] = new int[4][4];
    for (int i = 0; i != 4; i++) {
        for (int j = 0; j != 4; j++)
            a[i][j] = board[j][i];
    }
    return a;
}
//Performs the transformation elicited by a given movement.
bool Game::move_board(int action) {
    int previous[4][4];
    for (int i = 0; i != 4; i++) {
        for (int j = 0; j != 4; j++) {
            previous[i][j] = board[i][j];
        }
    }
    if (action == 0) {
        int column[4];
        for (int i = 0; i != 4; i++) {
            for (int j = 0; j != 4; j++) {
                column[j] = board[j][i];
            }
            move_arr(column, 0);
            for (int k = 0; k != 4; k++)
                board[k][i] = column[k];
        }
    }
    if (action == 1) {
        int column[4];
        for (int i = 0; i != 4; i++) {
            for (int j = 0; j != 4; j++) {
                column[j] = board[j][i];
            }
            move_arr(column, 1);
            for (int k = 0; k != 4; k++)
                board[k][i] = column[k];
        }
    }
    
    if (action == 2) {
        for (int i = 0; i != 4; i++)
            move_arr(board[i],0);
    }
    if (action == 3) {
        for (int i = 0; i != 4; i++) {
            move_arr(board[i],1);
        }
    }
    if (!is_identical(previous)) {
        insert_random();
        return true;
    }
    return false;

}
//Checks if a slice of the board is done.
bool Game::is_done_arr(int arr[4]) {
    for (int i = 0; i != 4; i++) {
        if (arr[i] == 0)
            return false;
    }
    for (int i = 0; i != 3; i++) {
        if (arr[i] == arr[i + 1])
            return false;
    }
    return true;
}
//Given a board state, it sets the board state of the current game to that board state. Used to create a game with a given board position.
void Game::set_board(int arr[4][4]) {
    for (int i = 0; i != 4; i++) {
        for (int j = 0; j != 4; j++) {
            board[i][j] = arr[i][j];
        }
    }
}
//Checks if the current game is done.
bool Game::is_done() {
    for (int i = 0; i != 4; i++) {
        if (is_done_arr(board[i]) == false)
            return false;
    }

    //On review of my code, clean transpose seems redundant. Could just create an array fill it vertically instead of horizontally, and then pass it to the is_done_arr function. More elegant solution.
    int(*a)[4];
    a = (int(*)[4]) clean_transpose(board);
    for (int i = 0; i != 4; i++)
        if (is_done_arr(a[i]) == false) {
            delete a;
            return false;
        }
    delete a;
    return true;
}
void Game::print_board() {
    for (int i = 0; i != 4; i++) {
        for (int j = 0; j != 4; j++) {
            double d;
            if (board[i][j] != 0) {
                d = log2(board[i][j]);
                cout << d;
            }
            else
                cout << 0;
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}
//Checks if the board position is identical to a given board position. Used in determining whether or not to insert a new piece.
bool Game::is_identical(int arr[4][4]) {
    for (int i = 0; i != 4; i++) {
        for (int j = 0; j != 4; j++) {
            if (arr[i][j] != board[i][j])
                return false;
        }
    }
    return true;
}
