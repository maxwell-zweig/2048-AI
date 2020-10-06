#pragma once
#include <iostream>
#include <vector>
#include <random>
#include<algorithm>
#include<iterator>
#include <conio.h>
using namespace std;
class Game {
public:
    int board[4][4] = { {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0} };
    int new_board[4][4]= { {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0} };
    int score = 0;
    Game();
    static int val_random();
    static int index_random(int x);
    void insert_random();
    void move_arr(int arr[4], int x);
    void transpose_board();
    void* clean_transpose(int board[4][4]);
    bool move_board(int action);
    void print_board();
    bool is_done_arr(int arr[4]);
    void set_board(int arr[4][4]);
    bool is_done();
    bool is_identical(int arr[4][4]);
};