// GameCode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <numeric>
#include <windows.h>
#include <fstream>
#include "game.h"
using namespace std;
int main() {

    string observations_file_name;
    int num_games;
    ofstream observations_file;
    cin >> observations_file_name;
    cin >> num_games;
    observations_file.open(observations_file_name);

    /* Plays num_games number of games. Stores each board state and selected action in one line 257 numbers long in the output file.
     For each board position, the "correct" action is selected by playing 25 virtual games for each action. 
     After the initial action, the games are played out randomly. After all the games are played, the action with the greatest average score is selected.
    */

    for (int i = 0; i !=num_games; i++) {
        Game game;
        vector<vector<int>> positions;
        vector<int> actions;
        while (!game.is_done()) {

            int up_reward[25] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
            int down_reward[25] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
            int right_reward[25] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
            int left_reward[25] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

            //Stores the current board position.
            vector<int> temp_position;
            for (int i = 0; i != 4; i++) {
                for (int j = 0; j != 4; j++) {
                    temp_position.push_back(game.board[i][j]);
                }
            }
            positions.push_back(temp_position);

            //Selects the best move.
            for (int j = 0; j != 25; j++) {
                Game background_run;
                background_run.set_board(game.board);
                background_run.score = game.score;
                if (!background_run.move_board(0))
                    break;
                while (!background_run.is_done()) {
                    int action = background_run.index_random(3);
                    background_run.move_board(action);
                }
                up_reward[j] = background_run.score;
            }
            for (int j = 0; j != 25; j++) {
                Game background_run;
                background_run.set_board(game.board);
                background_run.score = game.score;
                if (!background_run.move_board(1))
                    break;
                while (!background_run.is_done()) {
                    int action = background_run.index_random(3);
                    background_run.move_board(action);
                }
                down_reward[j] = background_run.score;
            }
            for (int j = 0; j != 25; j++) {
                Game background_run;
                background_run.set_board(game.board);
                background_run.score = game.score;
                if (!background_run.move_board(2))
                    break;
                while (!background_run.is_done()) {
                    int action = background_run.index_random(3);
                    background_run.move_board(action);
                }
                left_reward[j] = background_run.score;
            }
            for (int j = 0; j != 25; j++) {
                Game background_run;
                background_run.set_board(game.board);
                background_run.score = game.score;
                if (!background_run.move_board(3))
                    break;
                while (!background_run.is_done()) {
                    int action = background_run.index_random(3);
                    background_run.move_board(action);
                }
                right_reward[j] = background_run.score;
            }
            double maximum = accumulate(begin(up_reward), end(up_reward), 0.0);
            int action = 0;
            double average_list[4] = { accumulate(begin(up_reward), end(up_reward), 0.0), accumulate(begin(down_reward), end(down_reward), 0.0), accumulate(begin(left_reward), end(left_reward), 0.0), accumulate(begin(right_reward), end(right_reward), 0.0) };
            for (int i = 0; i != 3; i++) {
                double curr = average_list[i + 1];
                if (curr > maximum) {
                    maximum = curr;
                    action = i + 1;
                }
            }
            //Takes and stores the action.
            game.move_board(action);
            actions.push_back(action);
        }

        /*Once the game is finished, converts each 4 by 4 board state into a 1 by 256 array, and stores it in the file. Then appends the action taken to the end of the line
        Does this after each game so as to not store a massive multi-dimensional array in memory.*/
        for (int count = 0; count != positions.size(); count++) {
            int transformed_observations[256];
            for (int k = 0; k != 256; k++)
                transformed_observations[k] = 0;
            for (int x = 0; x != 16; x++) {
                if (positions[count][x] != 0) {
                    int f = log2(positions[count][x]);
                    transformed_observations[(16 * (f - 1)) + x] = 1;
               }
            }
            for (int z = 0; z != 256; z++) {
                observations_file << transformed_observations[z] << ",";
            }
            observations_file << actions[count] << endl;
        }
        actions.clear();
        for (int count = 0; count != positions.size(); count++) {
            positions[count].clear();
        }
        cout << i + 1 << endl;;
        game.print_board();

    }
    observations_file.close();
    return 0;
}
